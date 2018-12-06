#ifndef NETWORK_H
# define NETWORK_H

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include <err.h>
# include <unistd.h>

#define BUFF_SIZE 32
#define FD  4090

typedef struct	Neurons {
	int		nb_inputs;
	float	activation;
	float	z;
	float	bias;
	float	*weights;
	float	error;
	float	nabla_b;
	float	*nabla_w;
}			neuron;

typedef struct	Layers {
	int		nb_neurons;
	neuron	**neurons;
}			layer;

typedef	struct	Network	{
	int		nb_layers;
	int		*size_layers;
	layer	**layers;
}			network;

#endif

/* Neuron */
neuron	*init_neuron(int nb_inputs);
void	clear_neuron(neuron *neur);
void	clear_nabla_neuron(neuron *neur);
float	set_neuron_actication(neuron *neur, float *input);
void	error_neuron_output(neuron *neur, float expected);
void	error_neuron_hidden(neuron *neur, layer *next_lay, int pos);
void	add_nabla_neuron(neuron *neu, layer *prev_lay);
void	apply_nabla_neuron(neuron *neur, int nb_training, float eta);

/* Layer */
layer	*init_layer(int nb_neurons, int nb_inputs);
void	clear_layer(layer *lay);
void	clear_nabla_layer(layer *lay);
float	*set_layer_activation(layer *lay, float *input);
void	set_hidden_error(layer *lay, layer *next_lay);
void	set_output_error(layer *lay, float *expected);
void	add_nabla_layer(layer *lay, layer *prev_layer);
void	apply_nabla_layer(layer *lay, int nb_training, float eta);

/* Network */
network	*init_network(int nb_layers, int *size_layers);
void	clear_network(network *ntw);
void	clear_nabla_network(network *ntw);
void	set_network_activation(network *ntw, float *input);
void	set_error_network(network *ntw, float *expected);
float	get_cost_network(network *ntw, float *input, float *ecpected, int n);
void	add_nabla_network(network *ntw);
void	apply_nabla_network(network *ntw, int nb_training, float eta);
void	backprop(network *ntw, float *input, float *expected);
void 	free_network(network *net);

/* Network */
network		*init_xor();
void		train_xor(network *xor, float precision);
void		print_xor(network *ntw);
void		test_xor(network *ntw);
network		*init_ocr();
char		output_to_char(layer *output_lay);
int	   		compare_expected_output(float *expected, layer *output);
float 	*char_to_output(char c);

/* Utils */
float	random_float(void);
float	activation_f(float z);
float	activation_f_prime(float z);
float	cost_value(float activation, float expected);
float	cost_prime(float activation, float expected);
float	cost_ntw(layer *output_lay, float *expected, int nb_training);

/* gnl */
int			get_next_line(const int fd, char **line);
char		*ft_strfjoin(const char *s1, const char *s2);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strsub(char const *s, unsigned int start, size_t len);
char		*ft_strchr(const char *str, int c);
char		*ft_strcpy(char *dst, const char *src);
char		*ft_strdup(const char *s1);
char		*ft_strnew(size_t size);
size_t		ft_strlen(const char *s);
void		ft_strdel(char **as);
void			load_network(network *ntw, FILE *fd);
void    	save_network(network *ntw);
