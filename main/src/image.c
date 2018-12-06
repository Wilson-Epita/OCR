#include "header.h"


/*
 * Load an image, apply filters, call cut fonctions.
 */

Image   *image_load (char *path)
{
    SDL_Surface		*img;
    Image			*load_img;

    img = NULL;
    img = IMG_Load(path);
    if (img == NULL)
    {
        printf("%s\n",SDL_GetError());
        return  0;
    }
    SDL_LockSurface(img);
    load_img = SDLToIMG(img);
    SDL_FreeSurface(img);

    return load_img;
}


BinaryImage    *applyFilters(Image *img)
{
    BinaryImage		*bin_img;

    printf("Applying filters (grey,otsu)\n");

    //niveaudegris
    bin_img = niveaudegris(img, 0);

    //otsu
    bin_img = otsu(bin_img);

    //  saveBinImage(bin_img);
	FreeImage(img);

    return bin_img;
}

TextImage    *detectionCall(BinaryImage *img)
{
    printf("Line detection start ...\n");
    WordRegion *regions = detectTextRegions(img);
    printf("Line detection end ... \n");
    printf("Character detection start ...\n");
    TextImage* text =detectwordRegions(img,regions);
    printf("Character detection end ...\n");
    return text;
}


/*
 * FILTERS FUNCTIONS
 */
BinaryImage		*niveaudegris(Image *img, int mode)
{
	int		i;
	int		j;
	int		h;
	int		w;
	Uint8	moyenne;

	h = img -> h;
	w = img -> w;
	BinaryImage *res = NULL;
	res = (BinaryImage*)malloc(sizeof(BinaryImage));
	if (!(res))
		return (0);
	res -> h = h;
	res -> w = w;
	res -> matrix = NULL;
	res -> matrix = (Uint8*)malloc(h * w * sizeof(Uint8));
	if(!(res -> matrix))
		return (0);
	j = 0;
	while (j < h)
	{
		i = 0;
		while (i < w)
		{
			if (mode)
			{
				moyenne = (0.2125 * img -> matrix[(j * w) + i].r
						+ 0.7154 * img -> matrix[(j * w) + i].g
						+ 0.0721 * img -> matrix[(j * w) + i].b) ;
			}
			else
			{
				moyenne = (img -> matrix[(j * w) + i].r +
						img -> matrix[(j * w) + i].g +
						img -> matrix[(j * w) + i].b) / 3 ;
			}
			res -> matrix[(j * w) + i] = moyenne;
			i++;
		}
		j++;
	}
	return (res);
}

void		*edgeImage(BinaryImage * img)
{
	int				i, j;
	int				w, h;
	BinaryImage		*edge;
	Uint8			upper,left,rightUpper;
	Uint8			*tmp;

	w = img -> w;
	h = img -> h;
	edge = NULL;
	edge = (BinaryImage *)malloc(sizeof(BinaryImage));
	if (!(edge))
		return (0);
	edge -> h = h;
	edge -> w = w;
	edge -> matrix = NULL;
	edge -> matrix = (Uint8 *)malloc(h*w * sizeof(Uint8));
	if(!(edge -> matrix))
		return (0);
	upper = 0;
	left = 0;
	rightUpper = 0;
	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			if ((0 < i && i < h) && (0 < j && j < w - 1))
			{
				upper = abs(((img -> matrix[i * w + j])
									-(img -> matrix[(i - 1) * w + j])));

				left = abs(((img -> matrix[i * w + j])
									-(img -> matrix[i * w + j -1])));

				rightUpper = abs(((img -> matrix[i * w + j])
									-(img -> matrix[(i - 1) * w + j + 1])));

				edge -> matrix[i * w + j] = maximum(upper, left, rightUpper);
			}
			else
				edge->matrix[i * w + j] = 0;
			j++;
		}
		i++;
	}
	tmp = edge -> matrix;
	edge -> matrix = img -> matrix;
	img -> matrix = tmp;
	FreeBinary(edge);
	return 0;
}

void	*sharpImage(BinaryImage *img)
{
	int				w;
	int				h;
	int				i;
	int				j;
	BinaryImage		*sharpen;
	Uint8			pixval;
	Uint8			temp;
	Uint8			*temp2;
	Uint8			filter[9] = {0,-1,0,-1,5,-1,0,-1,0};

	h = img -> h;
	w = img -> w;
	sharpen = NULL;
	sharpen = (BinaryImage*)malloc(sizeof(BinaryImage));
	if (!(sharpen))
		return (0);
	sharpen -> h = h;
	sharpen -> w = w;
	sharpen -> matrix = NULL;
	sharpen -> matrix = (Uint8*)malloc(h * w * sizeof(Uint8));
	if(!(sharpen -> matrix))
		return (0);
	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			pixval = 0;
			for (int k = -1; k <= 1 ; ++k)
			{
				for (int l = -1; l <=1 ; ++l)
				{
					temp = 0;
					if (0 <= (i + l) && (i + l) < h && 0 <= (j + l)
																&& (j + l) < w)
					{
						temp = filter[(k + 1) * 3 + l + 1]
											* img -> matrix[i * w + j];
					}
					pixval += temp;
				}
			}
			sharpen -> matrix[i * w + j] = pixval;
			j++;
		}
		i++;
	}
	temp2 = sharpen -> matrix;
	sharpen -> matrix = img -> matrix;
	img -> matrix = temp2;
	FreeBinary(sharpen);
	return 0;
}

BinaryImage *resize(BinaryImage *img, int width, int height){

	BinaryImage *res;
	int h;
	int w;
	float x_ratio,y_ratio;
	float x_diff, y_diff;
	int offset;
	int A,B,C,D;
	int x,y;
	int index;
	Uint8 gray;

	h = img -> h;
	w = img -> w;
	res = (BinaryImage*)malloc(sizeof(BinaryImage));
	if (!(res))
		return 0;
	res -> w = width;
	res -> h = height;
	res -> matrix = (Uint8*)malloc(sizeof(Uint8) * width * height);
	offset = 0;
	x_ratio = ((float)(w-1))/width;
	y_ratio = ((float)(h-1))/height;
	for(int i = 0;i < height; i++){
		for(int j = 0;j < width; j++){
			x = (int)(x_ratio * j);
			y = (int)(y_ratio * i);
			x_diff = (x_ratio * j) - x;
			y_diff = (y_ratio * i) - y;
			index = y*w+x;

			A = img -> matrix[index];
			B = img -> matrix[index+1];
			C = img -> matrix[index+w];
			D = img -> matrix[index+w+1];

			gray = (Uint8)(A*(1-x_diff)*(1-y_diff) + B*(x_diff)*(1-y_diff) +
				C*(y_diff)*(1-x_diff) + D*(x_diff*y_diff));
			res -> matrix[offset++] = gray;
		}
	}
	return res;
}


/*
 * MAX
 */
Uint8		maximum(Uint8 a, Uint8 b, Uint8 c)
{
	Uint8		res;

	res = c;
	if (a >= b && a >= c)
		res = a;
	else if (b >= a && b >= c)
		res = b;
	return (res);
}
