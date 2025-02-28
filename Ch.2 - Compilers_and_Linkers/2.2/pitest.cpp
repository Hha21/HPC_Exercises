#include <iostream>
#include <cmath>
#include <math.h>
#include <boost/timer/timer.hpp>

using namespace std;

double func1(int n1)
{
    double sum = 0.0;

    for (int k = 0; k <= n1; k++)
    {
        sum += pow(-1, k) / (2 * k + 1);
    }
    return (4.0 * sum);
}

double func2(int n2)
{
    double product = 1.0;

    for (int k = 1; k <= n2; k++)
    {
        product *= (4.0 * k * k) / (4.0 * (k * k) - 1.0);
    }
    return (2.0 * product);
}

int main()
{

    boost::timer::auto_cpu_timer t;

    int n1 = 1000000;
    int n2 = 10000;
    cout << "Accurate pi value: " << M_PI << endl;
    cout << "pi estimation using func1 : " << func1(n1) << endl;
    cout << "pi estimation using func2 : " << func2(n2) << endl;

    // function 2 converges faster.
    return 0;
}