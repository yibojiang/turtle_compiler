#include <iostream>
#include <sstream>
#include <string>

int computation(std::string& s, int& i);
int expression(std::string& s, int& i);
int term(std::string& s, int& i);
int factor(std::string& s, int& i);
int number(std::string& s, int& i);

int computation(std::string& s, int& i) {
    int res = expression(s, i);
    return res;
}

int expression(std::string& s, int& i) {
    int res = term(s, i);
    /* Repeat as long as there are + or - operators. */
    while (s[i] == '+' || s[i] == '-') {
        char op = s[i++];
        int val = term(s, i);
        if (op == '+') {
            res += val;
        }
        else {
            res -= val;
        }
    }
    return res;
}

int term(std::string& s, int& i) {
    int res = factor(s, i);
    /* Repeat as long as there are * or / operators. */
    while (s[i] == '*' || s[i] == '/') {
        char op = s[i++];
        int val = factor(s, i);
        if (op == '*') {
            res *= val;
        }
        else {
            res /= val;
        }
    }
    return res;
}

int factor(std::string& s, int& i) {
    int res = 0;
    if (s[i] == '(') {
        i++;
        res = expression(s, i);
        if (s[i] != ')') {
            std::cerr << "Error: Missing close parenthesis." << std::endl;
            return 0;
        }
        i++;
    }
    else {
        res = number(s, i);
    }
    return res;
}

int number(std::string& s, int& i) {
    int res = 0;
    while (isdigit(s[i])) {
        res = res * 10 + (s[i] - '0');
        i++;
    }
    return res;
}
bool is_valid_expression(const std::string& str)
{
    for (char c : str)
    {
        if (!isdigit(c) && c != '*' && c != '+' && c != '-' && c != '/')
        {
            return false;
        }
    }
    return true;
}

double Compute(const std::string& expression) {

    /* Create a string stream from the expression string */
    std::istringstream iss(expression);
    double result;
    if (iss >> result) {
        return result;
    }
    return 0;
}

int main() {
    std::string input;
    std::cout << "Enter two computations separated by a space: ";
    std::getline(std::cin, input);

    /* Split the input string into two expressions. */

    int period_index = input.find(".");
    if (period_index == std::string::npos) {
        std::cerr << "Error: Missing period at the end of the expression." << std::endl;
        return 0;
    }
    std::string first_expression = input.substr(0, period_index + 1);
    std::string second_expression = input.substr(period_index + 2);

    /* Compute the results of the two expressions. */
    double result1 = Compute(first_expression);
    double result2 = Compute(second_expression);
    std::cout << result1 << std::endl;
    std::cout << result2 << std::endl;

    return 0;
}
