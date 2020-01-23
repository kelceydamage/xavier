#include <vector>
#include <numeric>
#include "../dlib/dlib/optimization.h"
#include "parser.hpp"
#include "model.hpp"

namespace lmp
{
    std::vector<double> *observed = new std::vector<double>;
    std::vector<double> *calculated = new std::vector<double>;
    std::vector<double> *error = new std::vector<double>;
    std::vector<double> *error_percent = new std::vector<double>;
    model::parameter_vector coefficients;
    model::parameter_vector params;
    int degree;
    std::vector coefficient_names = {
        "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k"
    };
}

// Initial configuration for the processor. Set the global model, load the 
// data, configure verbosity.
void configure_processor(
    int argc, 
    char **argv
)
{
    parser::parse_args(argc, argv);
    std::string modelname = argv[1];
    if (parser::stream_raw or parser::stream_python)
    {
        std::cout.setstate(std::ios_base::failbit);
    }
    parser::open_stream(lmp::observed);
    model::global_model = model::models[modelname];
    lmp::degree = model::degree[modelname];
}

// Handle printing of the results.
void print_results() 
{
    std::cout.clear();
    if (parser::stream_raw)
    {
        for (int i = 0; i <= lmp::degree; i++)
        {
            std::cout << lmp::coefficients(0, i) << std::endl;
        }
    } 
    else if (parser::stream_python)
    {
        std::cout << "{";
        for (int i = 0; i <= lmp::degree; i++)
        {
            std::cout << "'" << lmp::coefficient_names[i] << "': ";
            std::cout << lmp::coefficients(0, i) << ", ";
        }
        std::cout << "}" << std::endl;
    }
}

void print_parameters()
{
    for (int i = 0; i <= lmp::degree; i++)
        {
            std::cout << lmp::coefficients(0, i) << "  ";
        }
        std::cout << std::endl;
}

double test_residual_error(std::vector<std::pair<model::input_vector, double>> *data_samples) {
    // Before we do anything, let's make sure that our derivative function 
    // defined above matches the approximate derivative computed using 
    // central differences (via derivative()). If this value is big then it 
    // means we probably typed the derivative function incorrectly. double 
    // _residual = residual_derivative(data_samples[0], params);
    //
    // Automatic derivative function is used. This is more explanatory.

    double t = dlib::length(
        model::cubic_residual_derivative((*data_samples)[3], lmp::coefficients) - 
        dlib::derivative(model::residual)((*data_samples)[3], lmp::coefficients)
    );
    return t;
}

void run_model() 
{
    try
    {
        // Add x, y pairs to the data_samples vector.
        std::vector<std::pair<model::input_vector, double>> data_samples;
        model::input_vector input;
        for (int i = 0; i < lmp::observed->size(); ++i)
        {
            input = i;
            data_samples.push_back(std::make_pair(input, (*lmp::observed)[i]));
        }
        //parameter_vector coefficients;
        lmp::coefficients(0) = 1;
        lmp::coefficients(1) = 1;
        lmp::coefficients(2) = 1;
        lmp::coefficients(3) = 1;
        lmp::coefficients(4) = 1;

        double t = test_residual_error(&data_samples);
        std::cout << "derivative error: " << t << std::endl;
        
        std::cout << "Use Levenberg-Marquardt, approximate derivatives\n";

        // If we didn't create the residual_derivative function then we could
        // have used this method which numerically approximates the derivatives 
        // for you.
        dlib::solve_least_squares_lm(
            dlib::objective_delta_stop_strategy(1e-7).be_verbose(), 
            model::residual,
            dlib::derivative(model::residual),
            data_samples,
            lmp::coefficients
        );
        /*
        dlib::solve_least_squares_lm(
            dlib::objective_delta_stop_strategy(1e-7).be_verbose(), 
            model::residual,
            model::cubic_residual_derivative,
            data_samples,
            lmp::coefficients
        );
        */

        // Solve to check accuracy
        double _error;
        double output;
        for (int i = 0; i < lmp::observed->size(); ++i)
        {
            input = i;
            output = model::global_model(input, lmp::coefficients);
            lmp::calculated->push_back(output);
            _error = std::abs(lmp::observed->at(i) - lmp::calculated->at(i));
            lmp::error->push_back(_error);
            lmp::error_percent->push_back(_error / lmp::observed->at(i));
        }

        float total_error = std::accumulate(
            lmp::error->begin(),
            lmp::error->end(),
            0.0
        );

        float total_error_percent = std::accumulate(
            lmp::error_percent->begin(),
            lmp::error_percent->end(),
            0.0
        );

        int mid = std::ceil(lmp::error->size() / 2);
        float mean_error = lmp::error->at(mid);
        float mean_error_percent = lmp::error_percent->at(mid);
        float average_error = total_error / lmp::error->size();
        float average_error_percent = total_error_percent / lmp::error_percent->size();

        std::cout << std::endl;
        std::cout << "Average Error: " << average_error << ", " << average_error_percent * 100 << "%" <<  std::endl;
        std::cout << "Mean Error: " << mean_error << ", " << mean_error_percent * 100 << "%" <<  std::endl;

        // Now coefficients contains the solution.  If everything worked it 
        // will be equal to params.

        std::cout << std::endl;
        std::cout << "inferred parameters: ";
        print_parameters();
        std::cout << std::endl;
        std::cout << "Function: " << model::function_signature << std::endl;
        if (parser::test_model) 
        {
            std::cout << "solution error: ";
            std::cout << dlib::length(lmp::coefficients - lmp::params);
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void cleanup()
{
    delete lmp::observed;
    delete lmp::calculated;
    delete lmp::error;
}


int main(
    int argc,
    char **argv
)
{
    configure_processor(argc, argv);

    run_model();

    print_results();

    cleanup();
}