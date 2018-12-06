#include "network.h"

/* Return a random float betwen -0.5 and 0.5 : */
float	random_float(void)
{
	return ((((float)rand() / RAND_MAX) * 2) - 1);
}

/* Activation function : Sigmoid for the moment */
float	activation_f(float z)
{
	return (1 / (1 + exp(-z)));
}

/* Activation function derivative */
float	activation_f_prime(float z)
{
	z = activation_f(z);
	return(z * (1 - z));
}

/* Cost function : Cross Entropy */
float	cost_value(float activation, float expected)
{
	float res;
	if ((activation == 0 && expected == 0) || (activation == 1 && expected == 1))
		res = 0;
	else if ((activation == 1 && expected == 0) || (activation == 0 && expected == 1))
		res = 1;
	else
		res = -((expected * log(activation)) + (1 - expected) * log(1 - activation));

//	printf("expected=%f | activation=%f | res=%f\n", expected, activation, res);
	return res;
}

/* Cost function derivative of an element : */
float	cost_prime(float activation, float expected)
{
	return (activation - expected);
}

/* Cost total of the network */
float	cost_ntw(layer *output_lay, float *expected, int nb_training)
{
	float res = 0;

	for (int i = 0; i < output_lay->nb_neurons; i++)
	{
		res += cost_value(output_lay->neurons[i]->activation, expected[i]);

		res /= nb_training;
	}
	return (res);
}
