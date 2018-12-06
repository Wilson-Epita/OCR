#include "network.h"

network	*init_xor()
{
	int	nb_layers		= 3;
	int	size_layers[3]	= {2, 5, 1};
	return(init_network(nb_layers, size_layers));
}

void	train_xor(network *xor, float precision)
{
	float	cost = 1;
	int		nb_training = 4;

	float input_00[2] = {0, 0};
	float input_01[2] = {0, 1};
	float input_10[2] = {1, 0};
	float input_11[2] = {1, 1};
	float output_1[1] = {1};
	float output_0[1] = {0};

	while(cost > precision)
	{
		clear_nabla_network(xor);
		cost = 0;

		cost += get_cost_network(xor, input_00, output_0, nb_training);
		backprop(xor, input_00, output_0);


		cost += get_cost_network(xor, input_01, output_1, nb_training);
		backprop(xor, input_01, output_1);


		cost += get_cost_network(xor, input_10, output_1, nb_training);
		backprop(xor, input_10, output_1);


		cost += get_cost_network(xor, input_11, output_0, nb_training);
		backprop(xor, input_11, output_0);

		apply_nabla_network(xor, 4, 7);
		printf("Generation Done : cost = %f\n", cost);
	}
}

void	print_xor(network *ntw)
{
	printf("Inupt : <%f> <%f>\n",
			ntw->layers[0]->neurons[0]->activation,
			ntw->layers[0]->neurons[1]->activation);


	printf("Output : \n");
	printf("	output = %f\n", ntw->layers[2]->neurons[0]->activation);
	printf("	error : %f\n", ntw->layers[2]->neurons[0]->error);
/*	printf("	weights : <%f> <%f>\n",
			ntw->layers[2]->neurons[0]->weights[0],
			ntw->layers[2]->neurons[0]->weights[1]);
	printf("	bias : %f\n", ntw->layers[2]->neurons[0]->bias);
*/
}

void	test_xor(network *ntw)
{
	float input_00[2] = {0, 0};
	float input_01[2] = {0, 1};
	float input_10[2] = {1, 0};
	float input_11[2] = {1, 1};
	float output_1[1] = {1};
	float output_0[1] = {0};


	printf("\n-----------------------\n");
	printf("Case 1 : Input = {0, 0}\n");
	clear_network(ntw);
	set_network_activation(ntw, input_00);
	set_error_network(ntw, output_0);
	print_xor(ntw);

	printf("\n-----------------------\n");
	printf("Case 2 : Input = {1, 0}\n");
	clear_network(ntw);
	set_network_activation(ntw, input_10);
	set_error_network(ntw, output_1);
	print_xor(ntw);

	printf("\n-----------------------\n");
	printf("Case 3 : Input = {0, 1}\n");
	clear_network(ntw);
	set_network_activation(ntw, input_01);
	set_error_network(ntw, output_1);
	print_xor(ntw);

	printf("\n-----------------------\n");
	printf("Case 4 : Input = {1, 1}\n");
	clear_network(ntw);
	set_network_activation(ntw, input_11);
	set_error_network(ntw, output_0);
	print_xor(ntw);
}
