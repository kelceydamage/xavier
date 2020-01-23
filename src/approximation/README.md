# lm_series_stream_processor
A levenberg marquardt based stream processor that calculates no-linear coefficients for an arbitrary series of data.

## Dependencies 
* DLIB http://dlib.net/ (As a header only library. Only needed on compiling-development server).
* G++7 or greater if compiling.

## TODO
* Models other than cubic, quartic, and biquartic, are not working.

## Usage

download and execute either `approximate-centos7-x86_64` or `approximate-ubuntu-x86_64` depending on your platform.

### Help
```bash
cat values.txt | approximate cubic
```

```bash
Usage:
	<values> | approximate <model> [-s | -t]

Pipe a series of values to the approximate function, while specifying
which model to run. Output flags are optional and change the formatting

Models:
	linear   	f(x) = A(x) + B
	quadratic	f(x) = A(x^2) + B(x) + C
	cubic    	f(x) = A(x^3) + B(x^2) + C(x) + D
	quartic  	f(x) = A(x^4) + B(x^3) + C(x^2) + D(x) + E
	biquartic	f(x) = A(x^8) + B(x^6) + C(x^4) + D(x^2) + E

Options:
	-s	Stream result for processing raw
	-p	Stream result as python dictionary
	-t	Test model (verbose)
```

### Verbose Output
```bash
Processing: values.txt
derivative error: -nan
Use Levenberg-Marquardt, approximate derivatives
iteration: 0   objective: 5.41451e+09
iteration: 1   objective: 5.70225e+06
iteration: 2   objective: 28.1047
iteration: 3   objective: 27.5007
iteration: 4   objective: 27.5007
Average Error: 0.89423
inferred parameters:  0.0014417 -0.0526462   0.717927   -1.22645
Function: A(x^3) + B(x^2) + C(x) + D
```

### Python Output
```bash
{'a': 0.0014417, 'b': -0.0526462, 'c': 0.717927, 'd': -1.22645, }
```

### Raw Output
```bash
 0.0014417
-0.0526462
  0.717927
  -1.22645
```

### Rough Performance
```bash
real    0m0.003s
user    0m0.002s
sys     0m0.000s
```