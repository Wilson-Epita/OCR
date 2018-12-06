#ifndef TRAINING_H
# define TRAINING_H
# include <time.h>
# include "header.h"
typedef struct  Trainset {
	int   nb_traindata;
	float **inputs;
	float **expected;
	int 	last_pos;
} trainset;

typedef struct Batchs {
	int       nb_batchs;
	int       size;
	trainset  **mini_batchs;
} batchs;

/* Trainset */
trainset	*init_trainset(int nb_traindata);
void		add_data(trainset *trains, float *inputs, float *expected);
void		shuffle_trainset(trainset *trains);
trainset	**create_mini_batch(trainset *t_set);

/* Mini Batchs */
batchs		*init_batchs(int size, trainset *trains);
void		update_batchs(batchs *batch, trainset *trains);
/* Training */
void	train_datas(network *ntw, float *input, float *output);
void	train_batch(network *ntw, trainset *trains, float eta);
void	training(network *ntw, trainset *trains,
	 							trainset *test, int nb_epochs, float eta);
int 	testing(network *ntw, trainset *trains);

void recur(char* s, trainset *listetrain, int poschar);
#endif
