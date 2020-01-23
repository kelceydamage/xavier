#include <iostream>
#include <vector>
#include <map>

namespace parser
{
    bool test_model = false;
    bool stream_raw = false;
    bool stream_python = false;
    std::map<std::string, int> valid_args = {
        {"-", 0}, {"-s", 1}, {"-p", 2}, {"-t", 3}
    };

    // Commandline help text function. (large text exception)
    void help()
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "\t<values> | approximate <model> [-s | -t]\n" << std::endl;
        std::cout << "Pipe a series of values to the approximate function, while specifying" << std::endl;
        std::cout << "which model to run. Output flags are optional and change the formatting" << std::endl;
        std::cout << "\nModels:" << std::endl;
        std::cout << "\tlinear   \tf(x) = A(x) + B" << std::endl;
        std::cout << "\tquadratic\tf(x) = A(x^2) + B(x) + C" << std::endl;
        std::cout << "\tcubic    \tf(x) = A(x^3) + B(x^2) + C(x) + D" << std::endl;
        std::cout << "\tquartic  \tf(x) = A(x^4) + B(x^3) + C(x^2) + D(x) + E" << std::endl;
        std::cout << "\tbiquartic\tf(x) = A(x^8) + B(x^6) + C(x^4) + D(x^2) + E" << std::endl;
        std::cout << "\nOptions:" << std::endl;
        std::cout << "\t-s\tStream result for processing raw" << std::endl;
        std::cout << "\t-p\tStream result as python dictionary" << std::endl;
        std::cout << "\t-t\tTest model (verbose)" << std::endl;
    }

    // Open file containing column of vector elements.
    template<typename T>
    void open_stream (
        std::vector<T> *observed
    )
    {
        std::string line;
        double tval;

        while (std::getline(std::cin, line))
        {
            if (line.empty())
            {
                std::cout << "ERROR: No input found" << std::endl;
                std::exit(1);
            }
            tval = std::stod(line);
            observed->push_back(tval);
        }
    }

    // Validate incoming args and set flags accordingly.
    void parse_args(
        int argc,
        char **argv
    )
    {
        for (int i = 2; i < argc; i++)
        {
            switch (valid_args[argv[i]])
            {
                case 1: 
                    stream_raw = true;
                    break;
                case 2:
                    stream_python = true;
                    break;
                case 3:
                    test_model = true;
                    break;
                default:
                    std::cout << "ERROR: Discarding unknown option (";
                    std::cout << argv[i] << ")" << std::endl;
                    break;
            }
        }
        if (argc == 1)
        {
            std::cout << "ERROR: Insufficient arguments\n" << std::endl;
            help();
            exit (1);
        }
    }
}
