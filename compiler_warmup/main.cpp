#include <iostream>
#include <string>

#include "Parser.h"
#include "FileReader.h";

using std::cout;
using std::endl;
using std::string;

int main()
{
    string inputStr = "computation var i <- 2 * (-3) + 1; var abracadabra <- 7;(((abracadabra * i))) + 10;i - 5 - 1.";
    Parser parser(inputStr);
    parser.Computation();
    Parser pasrser(inputStr);

    return 1;
}
