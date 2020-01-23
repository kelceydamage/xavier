#include <string>
#include <map>

namespace model
{
    // Inputs: X axis
    typedef dlib::matrix<double,1,1> input_vector;
    // Parameters: A, B, C, ...
    typedef dlib::matrix<double,5,1> parameter_vector;

    typedef double (*model_func) (
        const input_vector& input, 
        const parameter_vector& params
    );

    // Create a model_func object.
    model_func global_model;

    std::string function_signature = "";
    std::string derivative_signature = "";

    double compute (
        const int& terms,
        const int& spacing,
        const input_vector& input,
        const parameter_vector& params
    )
    {
        double result;

        for (int i = 1; i <= terms; i++)
        {
            result += params(i - 1) * std::pow(input(0), (terms - i) * spacing);
        }
        return result;
    }

    // f(x) = A(x) + B
    double linear (
        const input_vector& input,
        const parameter_vector& params
    )
    {
        function_signature = "f(x) = A(x) + B";
        constexpr int terms = 2;
        constexpr int spacing = 1;
        return compute(terms, spacing, input, params);
    }

    // f(x) = A(x^2) + B(x) + C
    double quadratic (
        const input_vector& input,
        const parameter_vector& params
    )
    {
        function_signature = "f(x) = A(x^2) + B(x) + C";
        constexpr int terms = 3;
        constexpr int spacing = 1;
        return compute(terms, spacing, input, params);
    }

    // f(x) = A(x^3) + B(x^2) + C(x) + D
    double cubic (
        const input_vector& input,
        const parameter_vector& params
    )
    {
        function_signature = "f(x) = A(x^3) + B(x^2) + C(x) + D";
        constexpr int terms = 4;
        constexpr int spacing = 1;
        return compute(terms, spacing, input, params);
    }

    // f(x) = A(x^4) + B(x^3) + C(x^2) + D(x) + E
    double quartic (
        const input_vector& input,
        const parameter_vector& params
    )
    {
        function_signature = "f(x) = A(x^4) + B(x^3) + C(x^2) + D(x) + E";
        constexpr int terms = 5;
        constexpr int spacing = 1;
        return compute(terms, spacing, input, params);
    }

    // f(x) = A(x^8) + B(x^6) + C(x^4) + D(x^2) + E
    double biquartic (
        const input_vector& input,
        const parameter_vector& params
    )
    {
        function_signature = "f(x) = A(x^8) + B(x^6) + C(x^4) + D(x^2) + E";
        constexpr int terms = 5;
        constexpr int spacing = 2;
        return compute(terms, spacing, input, params);
    }

    double residual (
        const std::pair<input_vector, double>& data,
        const parameter_vector& params
    )
    {
        return global_model(data.first, params) - data.second;
    }

    // In progress and not representative for calculating error.
    parameter_vector cubic_residual_derivative (
        const std::pair<input_vector, double>& data,
        const parameter_vector& coefficients
    )
    {
        parameter_vector der;
        derivative_signature = "f'(x) = A(3x^2) + B(3x) + C(3) + D(1)";

        const double p0 = coefficients(0);
        const double p1 = coefficients(1);
        const double p2 = coefficients(2);
        const double p3 = coefficients(3);

        const double i0 = data.first(0);

        der(0) = p0 * 3 * std::pow(i0, 2);
        der(1) = p1 * 2 * i0;
        der(2) = p2 * 1;
        der(3) = 1;

        return der;
    }

    // lookup map for available models
    std::map<const std::string, const model_func> models = {
        {"linear", linear},
        {"quadratic", quadratic},
        {"cubic", cubic},
        {"quartic", quartic},
        {"biquartic", biquartic}
    };

    std::map<const std::string, const int> degree = {
        {"linear", 1},
        {"quadratic", 2},
        {"cubic", 3},
        {"quartic", 4},
        {"biquartic", 4}
    };
}