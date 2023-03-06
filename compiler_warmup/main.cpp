#include <iostream>
#include <string>

#include "Parser.h"
#include "FileReader.h";

using std::cout;
using std::endl;
using std::string;

int main()
{
    // string inputStr = R"(
    //     main
    //     var a, b, c, d, e; {
    //     let a < -call InputNum();
    //     let b < -a;
    //     let c < -b;
    //     let d < -b + c;
    //     let e < -a + b;
    //     if a < 0 then let d < -d + e; let a < -d else let d < -e fi;
    //     call OutputNum(a)
    // }.)";
    string inputStr = R"(
        main
        var a, b, c, d, e; {
        let a < -call InputNum();
        let b < -a;
        let c < -b;
        let d < -b + c;
        let e < -a + b;
    }.)";
    Parser parser(inputStr);
    parser.Computation();
    Parser pasrser(inputStr);

    return 1;
}
