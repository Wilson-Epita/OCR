#include "network.h"

/* Neuron */
	/* Initialisation */
neuron	*init_neuron(int nb_inputs)
{
	neuron *new_neuron;
	float	*weights;
	float	*nabla_w;
	float	bias;

	new_neuron	= (neuron*)	malloc(sizeof(neuron));
	weights		= (float*)	malloc(sizeof(float) * nb_inputs);
	nabla_w		= (float*)	malloc(sizeof(float) * nb_inputs);
	if(!new_neuron || !weights || !nabla_w)
		return (NULL);
	bias = random_float();
	for(int i = 0; i < nb_inputs; i++)
		weights[i] = random_float();

	new_neuron->nb_inputs	= nb_inputs;
	new_neuron->bias		= bias;
	new_neuron->weights		= weights;
	new_neuron->nabla_b		= 0;
	new_neuron->nabla_w		= nabla_w;
	clear_neuron(new_neuron);
	return (new_neuron);
}

	/* Clear */
void	clear_neuron(neuron *neur)
{
	neur->activation	= 0;
	neur->z				= 0;
	neur->error			= 0;
}
void	clear_nabla_neuron(neuron *neur)
{
	neur->nabla_b	= 0;
	for(int i = 0; i < neur->nb_inputs; i++)
		neur->nabla_w[i] = 0;
}

	/* Set activation */
float	set_neuron_activation(neuron *neur, float *input)
{
	float z;

	z = neur->bias;
	for(int i = 0; i < neur->nb_inputs; i++)
		z += input[i] * neur->weights[i];

	neur->z						= z;
	neur->activation	= activation_f(z);
	return(neur->activation);
}

	/* Set error neuron from output layer */
void	error_neuron_output(neuron *neur, float expected)
{
	float e;

	e = cost_prime(neur->activation, expected);
	neur->error = e;
}

	/* Set error neuron from hidden layer */
void	error_neuron_hidden(neuron *neur, layer *next_lay, int pos)
{
	float	s = 0;

	for(int i = 0; i < next_lay->nb_neurons; i++)
		s += next_lay->neurons[i]->weights[pos] * next_lay->neurons[i]->error;
	neur->error = (s * activation_f_prime(neur->z));
}
	/* Add the value of the nabla */
void	add_nabla_neuron(neuron *neur, layer *prev_lay)
{
	neur->nabla_b += neur->error;
	for(int i = 0; i < prev_lay->nb_neurons; i++)
		neur->nabla_w[i] += prev_lay->neurons[i]->activation * neur->error;
}

void	apply_nabla_neuron(neuron *neur, int nb_training, float eta)
{
	neur->bias -= (neur->nabla_b / nb_training);
	for(int i = 0; i < neur->nb_inputs; i++)
		neur->weights[i] -= (neur->nabla_w[i] / nb_training) * eta;
}

/* Layer */
	/*	Initialisation */
layer	*init_layer(int nb_neurons, int nb_inputs)
{
	layer	*new_layer;
	neuron	**neurons;

	new_layer	= (layer*)	malloc(sizeof(layer));
	neurons		= (neuron**)malloc(sizeof(neuron*) * nb_neurons);
	if(!new_layer || !neurons)
		return (NULL);
	for(int i = 0; i < nb_neurons; i++)
		neurons[i] = init_neuron(nb_inputs);

	new_layer->nb_neurons	= nb_neurons;
	new_layer->neurons		= neurons;
	return (new_layer);
}

	/* Clear layer */
void	clear_layer(layer *lay)
{
	for(int i = 0; i < lay->nb_neurons; i++)
		clear_neuron(lay->neurons[i]);
}
	/* Clear nabla layer */
void	clear_nabla_layer(layer *lay)
{
	for(int i = 0; i < lay->nb_neurons; i++)
		clear_nabla_neuron(lay->neurons[i]);
}

	/* Set activation */
float	*set_layer_activation(layer *lay, float *input)
{
	float *res;

	res = (float*)malloc(sizeof(float) * lay->nb_neurons);
	if(!res)
		return (NULL);
	for(int i = 0; i < lay->nb_neurons; i++)
		res[i] = set_neuron_activation(lay->neurons[i], input);
	return (res);
}

	/* Set error of hidden layer */
void	set_hidden_error(layer *lay, layer *next_lay)
{
	for(int i = 0; i < lay->nb_neurons; i++)
		error_neuron_hidden(lay->neurons[i], next_lay, i);
}

	/* Set error of output layer */
void	set_output_error(layer *lay, float *expected)
{
	for(int i = 0; i < lay->nb_neurons; i++)
		error_neuron_output(lay->neurons[i], expected[i]);
}
	/* Add gradien descent for each neuron */
void	add_nabla_layer(layer *lay, layer *prev_layer)
{
	for(int i = 0; i < lay->nb_neurons; i++)
		add_nabla_neuron(lay->neurons[i], prev_layer);
}

	/* Apply gradient descent for each neuron */
void	apply_nabla_layer(layer *lay, int nb_training, float eta)
{
	for(int i = 0; i < lay->nb_neurons; i++)
		apply_nabla_neuron(lay->neurons[i], nb_training, eta);
}

/* Network */
	/* Initialisation */
network	*init_network(int nb_layers, int *size_layers)
{
	network	*new_network;
	layer	**layers;

	new_network	= (network*)malloc(sizeof(network));
	layers		= (layer**) malloc(sizeof(layer*) * nb_layers);
	if(!new_network || !layers)
		return (NULL);
	layers[0] = init_layer(size_layers[0], 0);
	for(int i = 1; i < nb_layers; i++)
		layers[i] = init_layer(size_layers[i], size_layers[i - 1]);

	new_network->nb_layers		= nb_layers;
	new_network->size_layers	= size_layers;
	new_network->layers			= layers;
	return (new_network);
}

	/* Clear */
void	clear_network(network *ntw)
{
	for(int i = 0; i < ntw->nb_layers; i++)
		clear_layer(ntw->layers[i]);
}
	/* Clear nabla */
void	clear_nabla_network(network *ntw)
{
	for(int i = 0; i < ntw->nb_layers; i++)
		clear_nabla_layer(ntw->layers[i]);
}
	/* Set network activation */
void	set_network_activation(network *ntw, float *input)
{
	int i;

	for(i = 0; i < ntw->layers[0]->nb_neurons; i++)
		ntw->layers[0]->neurons[i]->activation = input[i];
	for(i = 1; i < ntw->nb_layers; i++)
		input = set_layer_activation(ntw->layers[i], input);
}
	/* Set networok error */
void	set_error_network(network *ntw, float *expected)
{
	int i = ntw->nb_layers - 1;

	set_output_error(ntw->layers[i], expected);
	i--;
	while(i > 0)
	{
		set_hidden_error(ntw->layers[i], ntw->layers[i+1]);
		i--;
	}
}

	/* Get network error */
float	get_cost_network(network *ntw, float *input, float *expected, int n)
{
	set_network_activation(ntw, input);
	set_error_network(ntw, expected);
	return (cost_ntw(ntw->layers[ntw->nb_layers - 1], expected, n));
}
	/* Add nabla_network */
void	add_nabla_network(network *ntw)
{
	for(int i = ntw->nb_layers - 1; i > 0; i--)
		add_nabla_layer(ntw->layers[i], ntw->layers[i-1]);
}
	/* Apply nabla */
void	apply_nabla_network(network *ntw, int nb_training, float eta)
{
	for(int i = 1; i < ntw->nb_layers; i++)
		apply_nabla_layer(ntw->layers[i], nb_training, eta);
}

	/* Backpropagation */
void	backprop(network *ntw, float *input, float *expected)
{
	clear_network(ntw);
	set_network_activation(ntw, input);
	set_error_network(ntw, expected);
	add_nabla_network(ntw);
}

void free_network(network *net)
{
    for(int i = 0; i < net->nb_layers; i++)
	{
		for(int j = 0; j < net->layers[i]->nb_neurons; j++)
			free(net->layers[i]->neurons[j]);
		free(net->layers[i]);
	}
	free(net);
}

/* OCR */
network *init_ocr()
{
	int	nb_layers = 3;
	int	size_layers[3] = {784, 120, 95};
	return (init_network(nb_layers, size_layers));
}

char output_to_char(layer *output_lay)
{
	char	res = 0;
	int	i;
	float					max_activation = output_lay->neurons[0]->activation;

	for(i = 1; i < output_lay->nb_neurons; i++)
	{
		if(output_lay->neurons[i]->activation > max_activation)
		{
			max_activation = output_lay->neurons[i]->activation;
			res = i;
		}
	}
	return (res + ' ');
}

char expected_to_char(float *expected)
{
	int res = 0;
	int max_activation = expected[0];

	for(int i = 1; i < 95; i++)
	{
		if(expected[i] > max_activation)
		{
			max_activation = expected[i];
			res = i;
		}
	}
	return(res + ' ');
}

int compare_expected_output(float *expected, layer *output)
{
	int res = expected[output_to_char(output) - ' '];
	if(res == 1)
	{
		printf("Success - '%c'\n",expected_to_char(expected));
	}
	else
	{
		printf("Fail    - '%c' instead of '%c'\n",
									output_to_char(output),
									expected_to_char(expected));
	}
	return (res == 1);
}

float *char_to_output(char c)
{
	float *res = malloc(sizeof(float) * 95);

	for(int i = 0; i < 95; i++)
		res[i] = 0;
	res[c - 32] = 1;
	return (res);
}
