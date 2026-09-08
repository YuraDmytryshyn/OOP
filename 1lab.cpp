#include <windows.h>
#include <iostream>
#include <cmath>
using namespace std;

class Functions {
public:
    double x, y, z;

    void setValues(double a, double b, double c) {
        x = a;
        y = b;
        z = c;
    }

    double calcB() {
        double b = 1 + pow(fabs(y - x), 2) / pow(fabs(z - 1), 1.34) + pow(z - x, 2) / pow(sin(z), 2) + pow(fabs(y - z), 3) / pow(cos(y), 2);
        return b;
    }

    double calcA(double b) {
        double a = pow(x + y, 2) + pow(z, 3) / pow(b + y, 2) + 1 / (1 + exp(-(x - y))) + pow(fabs(z), 0.34);
        return a;
    }
};

int main() {

    double x = 0.48 * 3;
    double y = 0.47 * 3;
    double z = -1.32 * 3;

    Functions f;
    f.setValues(x, y, z);

    double b = f.calcB();
    double a = f.calcA(b);

    cout << "b = " << b << endl;
    cout << "a = " << a << endl;

    return 0;
}