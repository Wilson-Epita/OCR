#include "network.h"
#include "training.h"

trainset  *init_trainset(int nb_traindata)
{
	trainset *new_trainset;
	float **inputs;
	float **expected;

	inputs = (float **) malloc(sizeof(float *) * nb_traindata);
	expected = (float **) malloc(sizeof(float *) * nb_traindata);
	new_trainset  = (trainset*)malloc(sizeof(trainset));
	if(!new_trainset || !inputs || !expected)
		return (NULL);

	new_trainset->inputs = inputs;
	new_trainset->expected = expected;
	new_trainset->nb_traindata = nb_traindata;
	new_trainset->last_pos = 0;

	return (new_trainset);
}

void add_data(trainset *trains, float *inputs, float *expected)
{
	trains->inputs[trains->last_pos] = inputs;
	trains->expected[trains->last_pos] = expected;
	trains->last_pos = (trains->last_pos + 1) % trains->nb_traindata;
}

void swap(float **a, float **b) {
	float *temp = *a;
	*a = *b;
	*b = temp;
}

void shuffle_trainset(trainset *trains)
{
	srand(time(NULL));
	int n = trains->nb_traindata;
	int i = 0;
	int j;

	for(i = n-1; i > 0; i--)
	{
		j = rand() % (i+1);
		swap(&(trains->inputs[i]), &(trains->inputs[j]));
		swap(&(trains->expected[i]), &(trains->expected[j]));
	}
}

/* Batchs */
batchs  *init_batchs(int size, trainset *trains)
{
	batchs    *new_batchs;
	int       nb_batchs = (trains->nb_traindata / size);

	new_batchs = (batchs*) malloc(sizeof(batchs));
	if (!new_batchs)
		return (NULL);

	new_batchs->size = size;
	new_batchs->nb_batchs = nb_batchs;
	update_batchs(new_batchs, trains);

	printf("Nb Mini Batchs: %d\n", new_batchs->nb_batchs);
	return (new_batchs);
}

void update_batchs(batchs *batch, trainset *trains)
{
	trainset **mini_batchs;
	trainset *little_batch = init_trainset(batch->size);
	int pos;

	mini_batchs = (trainset **) malloc(sizeof(trainset*) * batch->nb_batchs);
	for(int i = 0; i < batch->nb_batchs; i++)
	{
		trains->last_pos = 0;
		for(int j = 0; j < batch->size; j++)
		{
			pos = i * batch->size + j;
			add_data(little_batch, trains->inputs[pos], trains->expected[pos]);
		}
	mini_batchs[i] = little_batch;
	}
	batch->mini_batchs = mini_batchs;
}

/* Training */

void train_batch(network *ntw, trainset *trains, float eta)
{
  clear_nabla_network(ntw);
	clear_network(ntw);
  float cost = 0;

  for(int data = 0; data < trains->nb_traindata; data++)
  {
    backprop(ntw, trains->inputs[data], trains->expected[data]);
    cost += get_cost_network(ntw,
			trains->inputs[data],
			trains->expected[data],
			trains->nb_traindata);
  }
  apply_nabla_network(ntw, trains->nb_traindata, eta);
}

void training(network *ntw, trainset *trains, trainset *test, int ep, float eta)
{
	int epoch = 0;
	int size = 60;
	// int stop = 0;
	// int difference = 30;

	int *ntw_grade = (int *) malloc(sizeof(int) * ep);
	batchs *batch = init_batchs(size, trains);

	while(/*!stop &&*/ epoch < ep)
	{
		shuffle_trainset(trains);
		update_batchs(batch, trains);
		for(int i = 0; i < batch->nb_batchs; i++)
		{
			train_batch(ntw, batch->mini_batchs[i], eta);
 		}
		printf("Generation Done : #%d\n", epoch);
		ntw_grade[epoch] = testing(ntw, test);
		// if (epoch >= difference)
		// 	stop = ntw_grade[epoch - difference] >= ntw_grade[epoch];

		 testing(ntw, trains);
		 printf("Arial Grade (Gen %d): %d/93\n", epoch, ntw_grade[epoch]);
		 epoch++;
	}
}

int testing(network *ntw, trainset *trains)
{
	int success = 0;

	for (int i = 0; i < trains->nb_traindata; i++)
	{
		set_network_activation(ntw, trains->inputs[i]);
		layer *output_lay = ntw->layers[ntw->nb_layers - 1];
		success += compare_expected_output(trains->expected[i], output_lay);
	}
	printf("Testing : %d/%d\n",success, trains->nb_traindata);
	return success;
}
