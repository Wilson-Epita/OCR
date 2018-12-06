#include "network.h"

void save_network(network *ntw)
{
  FILE *fptr;

  fptr = fopen("save.txt", "rb+");
  if(fptr == NULL) //if file does not exist, create it
  {
        fptr = fopen("save.txt", "wb");
        if (fptr == NULL)
            errx(1,"error save file");
  }
  for(int lay = 1; lay < ntw->nb_layers; lay++)
	{
		for(int neur = 0; neur < ntw->layers[lay]->nb_neurons; neur++)
		{
      fprintf(fptr,"%f\n",ntw->layers[lay]->neurons[neur]->bias);
      for(int w = 0; w < ntw->layers[lay - 1]->nb_neurons; w++)
        fprintf(fptr,"%f\n", ntw->layers[lay]->neurons[neur]->weights[w]);
    }
	}
  fclose(fptr);
}
