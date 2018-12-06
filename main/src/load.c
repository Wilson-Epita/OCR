#define _GNU_SOURCE
#include "network.h"
#include <stdio.h>
#include <stdlib.h>
void load_network(network *ntw, FILE *fd)
{
  char	*line;
  int   param;
  size_t len = 0;
  ssize_t read;

  line = NULL;
  for(int lay = 1; lay < ntw->nb_layers; lay++)
	{
		for(int neur = 0; neur < ntw->layers[lay]->nb_neurons; neur++)
		{
      //printf("neur%d\n",neur);
      param = 0;
      while (param < ntw->layers[lay - 1]->nb_neurons + 1 &&
                                        (read = getline(&line, &len, fd) != -1))
      {
        if (param == 0)
        {
          ntw->layers[lay]->neurons[neur]->bias = atof(line);
        }
        else if (param > 0)
          ntw->layers[lay]->neurons[neur]->weights[param - 1] = atof(line);
        param++;
      }
		}
	}
  free(line);
}
