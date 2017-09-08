#include <iostream>
#include "printer.h"
#include "expression.h"
#include "utils.h"
#include "biginteger.h"

using namespace std;

int main() {
    string expr;
    getline(cin, expr);
    expr = replaceAll(expr, "c", "C");
    try {
        Expression expression(expr);
        BigInteger v = expression.calc();
        Printer printer;
        string ascii = expr + " = " + v.value();
        printer.putString(ascii);
        printer.saveToBMP("asciiart.bmp");
    } catch (overflow_error e1) {;
        cout << e1.what() << endl;
    } catch (logic_error e2) {
        cout << e2.what() << endl;
    }
    return 0;
}