#include "header.h"


void PrintIMG(Image *img)
{
	int i = 0;
	int j = 0;
	int h = img->h;
	int w = img-> w;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			printf("IMG in [%d] [%d]. R = %d  G = %d  B  = %d\n", i, j,
			img->matrix[i*w+j].r, img->matrix[i*w+j].b, img->matrix[i*w+j].b);
			j++;
		}
		i++;
	}
}

void PrintSurface(SDL_Surface *img)
{
	int i = 0;
	int j = 0;
	int h = img->h;
	int w = img-> w;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			Uint32 pixSDL = getpixel(img, i, j);
			printf("Surface in [%d] [%d]. value = %#010x\n", i, j, pixSDL);
			++j;
		}
		i++;
	}
}

int countDigit(long long n)
{
	int count = 0;
	while (n != 0)
	{
		n = n / 10;
		++count;
	}
	return count;
}

void printHisto(Histo *h)
{
	int len = h->len;
	int i = 0;
	while (i < len)
	{
		printf("Histo en %d = %d\n", i, h->arr[i]);
		++i;
	}
}

void	savetxtregions(BinaryImage *img, WordRegion	*regions, char	*path)
{
	int					i, len;
	int					x, y, w, h;
	int					width, height;
	SDL_Surface			*surface;
	SDL_Surface			*sdl;
	textRegion			rc;

	SDL_ClearError();
	i = 0;
	sdl = NULL;
	surface = BinaryToSDL(img);
	len = regions -> len;
	while (i < len)
	{
		rc = regions -> chars[i];
		x = rc.x0;
		y = rc.y0;
		h = rc.x1;
		w = rc.y1;
		width = w - y + 1;
		height = h - x + 1;
		sdl = SDL_CreateRGBSurfaceWithFormat(0, width, height, 24,
											 SDL_PIXELFORMAT_BGR24);
		SDL_Rect rect= {y,x,width,height};
		SDL_BlitSurface(surface, &rect, sdl, NULL);
		char buff[strlen(path)+10+countDigit(i)];
		sprintf(buff, "%s/block%d.bmp",path, i);
		SDL_SaveBMP(sdl, buff);
		SDL_FreeSurface(sdl);
		i++;
	}
}

void  saveallchars(BinaryImage	*img, WordRegion	*chars, int index)
{
	struct stat sb;
	char *wordpath = "wordfiles";
	if(stat(wordpath, &sb) == -1)
	{
		mkdir(wordpath, 0700);
	}
	char *path = "wordfiles/line";
	char res[strlen(path)+countDigit(index)];
	sprintf(res, "wordfiles/line%d",index);

	if(stat(res, &sb) == -1)
	{
		mkdir(res, 0700);
	}
	savetxtregions(img, chars, res);
}


void saveBinImage(BinaryImage *img)
{
	SDL_Surface *sdlimg = BinaryToSDL(img);
	struct stat sb;
	char *binoutput="filtersOutput";
	if(stat(binoutput, &sb) == -1)
	{
		mkdir(binoutput, 0700);
	}
	SDL_SaveBMP(sdlimg,"filtersOutput/bin.bmp");
	SDL_FreeSurface(sdlimg);
}
