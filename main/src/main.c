#include "header.h"

int		main(int argc, char *argv[])
{
	/*if (argc == 2)
		tempfunction(argv[1]);
	//else
		errx(1,"Arg error");
	return (0);*/
	FILE 		*fp;

	ocr = init_ocr();
	fp = fopen("save.txt", "r");
	if (fp == NULL)
	errx(1,"error file");
	printf("...Strat Loading...\n");
	load_network(ocr, fp);
	printf("...Finish...\n");
	if (argc == 1)
	{
			return interfaceCall(argc, argv);
	}
	else
	{
		int nb_generation = 100;

		char *train_path = "train";
		char *arial_path = "test_arial";

		trainset *listetrain;
		trainset *arial;

		printf("...Start Initialisation of trainset...\n");
		listetrain = init_trainset(1028);
		arial = init_trainset(93);

		recur(train_path, listetrain, 0);
		recur(arial_path, arial, 0);

		printf("...Finish...\n");
		training(ocr, listetrain, arial, nb_generation, 0.1);
		printf("Finish...\n");

		printf ("Arial test : \n"),
		testing(ocr, arial);
		save_network(ocr);
	}
	free_network(ocr);
	fclose(fp);
	return 0;
}
