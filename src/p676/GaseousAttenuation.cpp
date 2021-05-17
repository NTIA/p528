#include "..\..\include\p676.h"

double GaseousAttenuation(int num_of_layers, double a_i[], double gamma_i[])
{
    double A_gas = 0;

    // Equation 13
    for (int i = 0; i < num_of_layers; i++)
        A_gas += a_i[i] * gamma_i[i];

    return A_gas;
}