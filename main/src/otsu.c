#include "header.h"

float		*make_histogram(BinaryImage *img)
{
	int		i;
	int		max;
	float		*res;

	i = 0;
	max = (img -> h * img -> w );
	if(!(res = (float*)malloc(256 * sizeof(float))))
		return (NULL);
	while (i < 256)
	{
		res[i] = 0;
		i++;
	}
	i = 0;
	while (i < max)
	{
		res[(img -> matrix[i])]++;
		i++;
	}
	i = 0;
	return (res);
}

float		*normalize_histogram(float *histogram,int nbPixel)
{
	int		i;

	i = 0;
	while (i < 256)
	{
		histogram[i] /= nbPixel;
		i++;
	}
	return (histogram);
}

float		zero_order_cumultative_moment(float *histogram, int k)
{
	float		res;
	int		i;

	res = 0;
	i = 0;
	while (i < k)
	{
		res += histogram[i];
		i++;
	}
	return (res);
}

float		first_order_cumultative_moment(float *histogram, int k)
{
	float		res;
	int		i;

	res = 0;
	i = 0;
	while (i < k)
	{
		res += i * histogram[i];
		i++;
	}
	return (res);
}

float		calc_variance(float uT, float wk, float uk)
{
	return ((uT * wk - uk) * (uT *wk - uk)) / (wk * (1 - wk));
}

BinaryImage		*otsu(BinaryImage *img)
{
	int				i;
	int				nbPixel;
	float				*histogram;
	float				w[256];
	float				u[256];
	float				uT;
	float				variance;
	float				best_threshold;
	float				vk;
	BinaryImage		*res = NULL;

	res = (BinaryImage *)malloc(sizeof(BinaryImage));
	if(!res)
		return 0;

	i = 0;
	variance = -1;
	best_threshold = 0;
	nbPixel = (img -> w) * (img -> h);
	histogram = make_histogram(img);
	histogram = normalize_histogram(histogram, nbPixel);
	while (i < 256)
	{
		w[i] = zero_order_cumultative_moment(histogram, i);
		u[i] = first_order_cumultative_moment(histogram, i);
		++i;
	}
	uT = first_order_cumultative_moment(histogram, 256);
	i = 0;
	while (i < 256)
	{
		vk = calc_variance(uT, w[i], u[i]);
		if (vk > variance)
		{
			variance = vk;
			best_threshold = i;
		}
		++i;
	}
	i = 0;
	res->h = img -> h;
	res->w = img -> w;
	res->matrix =(Uint8*)malloc(nbPixel*sizeof(Uint8));
	while (i < nbPixel)
	{
		if (img -> matrix[i] < best_threshold)
			res -> matrix[i] = 255;
		else
			res -> matrix[i] = 0;
		i++;
	}
	return (res);
}
