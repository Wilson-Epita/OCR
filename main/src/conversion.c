#include "header.h"

Image			*SDLToIMG(SDL_Surface* img)
{
	int			w,h;
	Uint32		pixSDL;
	Image		*res;
	int			i,j;

	i = 0;
	j = 0;
	h = img -> h;
	w = img -> w;
	res = NULL;
	res = (Image*)malloc(sizeof(Image));
	if (!(res))
		return (0);
	res -> h = h;
	res -> w = w;
	res -> matrix = (Pixel*)malloc(h * w * sizeof(Pixel));
	if (!(res -> matrix))
		return (0);
	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			Pixel currentPix;
			pixSDL = getpixel(img, i, j);
			SDL_GetRGB(pixSDL, img -> format, &(currentPix.r), &(currentPix.g),
					&(currentPix.b));
			res -> matrix[i * w + j] = currentPix;
			j++;
		}
		i++;
	}
	return (res);
}

SDL_Surface			*BinaryToSDL(BinaryImage *img)
{
	int				i,j;
	int				h,w;
	Uint8			current;
	Uint32			pixel;
	SDL_Surface		*sdl;

	sdl = NULL;
	i = 0;
	j = 0;
	h = img -> h;
	w = img -> w;
	sdl = SDL_CreateRGBSurfaceWithFormat(0, w, h, 24, SDL_PIXELFORMAT_BGR24);
	if (sdl == NULL)
	{
		printf("%s\n",SDL_GetError());
		errx(1,"ERROR");
	}
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			current = img -> matrix[i * w + j];
			pixel = SDL_MapRGB(sdl -> format, current, current, current);
			putpixel(sdl, i, j, pixel);
			j++;
		}
		i++;
	}
	return (sdl);
}

SDL_Surface		*IMGtoSDL(Image *img)
{
	int				i,j;
	int				h,w;
	Pixel			current;
	Uint32			pixel;
	SDL_Surface		*sdl;

	i = 0;
	j = 0;
	h = img -> h;
	w = img -> w;
	sdl = NULL;
	sdl = SDL_CreateRGBSurfaceWithFormat(0, w, h, 24 , SDL_PIXELFORMAT_BGR24);
	if (sdl == NULL)
	{
		printf("%s\n",SDL_GetError());
		errx(1,"ERROR");
	}
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			current = img -> matrix[i * w + j];
			pixel = SDL_MapRGB(sdl -> format, current.r, current.g, current.b);
			putpixel(sdl, i, j, pixel);
			j++;
		}
		i++;
	}
	return (sdl);
}


SDL_Surface	*BinaryRegionToSDL(BinaryImage *img, textRegion rc)
{
	int				i,j;
	int				h,w;
	int				width,height;
	Uint8			current;
	Uint32			pixel;
	SDL_Surface		*sdl;

	sdl = NULL;
	i = rc.x0;
	j = rc.y0;
	h = rc.x1;
	w = rc.y1;
	width = w - j + 1;
	height = h - i + 1;
	sdl = SDL_CreateRGBSurfaceWithFormat(0, width,
											height, 24, SDL_PIXELFORMAT_BGR24);
	if (sdl == NULL)
	{
		printf("%s\n",SDL_GetError());
		errx(1,"ERROR");
	}
	while (i < h)
	{
		j = rc.x0;
		while (j < w)
		{
			current = img -> matrix[i * (img -> w) + j];
			pixel = SDL_MapRGB(sdl -> format, current, current, current);
			putpixel(sdl, i, j, pixel);
			j++;
		}
		i++;
	}
	return (sdl);
}

void	BlitPadImage(BinaryImage *source, BinaryImage *dest, textRegion   bornes)
{
    int		x0,y0,x1,y1;
    int     diffx, diffy, side;
    int		width, height;
    int		srcw;

    x0 = bornes.x0;
    y0 = bornes.y0;
    x1 = bornes.x1;
    y1 = bornes.y1;
    height = x1 - x0 +1;
    width  = y1 - y0 +1;
    if (height>width)
    {
        diffx = (height - width)/2 ;
        diffy = 0;
        side = height;
    } else
    {
        diffy = (width - height)/2;
        diffx = 0;
        side = width;
    }
    srcw = source->w;
    dest -> h = side;
    dest -> w = side;
    dest -> matrix = (Uint8*)calloc( side * side ,sizeof(Uint8));
    if (!(dest -> matrix))
    {
        FreeBinary(dest);
        dest = NULL;
    }
    else
    {
        for (int i = x0; i <= x1; ++i)
        {
            for (int j = y0; j <=  y1; ++j)
            {
                dest -> matrix[(i - x0 + diffy) * side + (j - y0 + diffx)] =
                        source -> matrix[i * srcw + j];
            }
        }
    }

}

TextImage *regionToImage(BinaryImage *source, TextRegion *text)
{
    if (!text)
        return NULL;

    LineImage *currLine;
    WordImage *currWord;
    TextImage *res = (TextImage *)malloc(sizeof(TextImage));
    res->len = text->len;
    res->lines = (LineImage **)malloc(sizeof(LineImage *)* res->len);
    int i  = 0;
    int j  = 0;
    int k = 0;
    while(i<text->len)
    {
        currLine = res->lines[i] = (LineImage *)malloc(sizeof(LineImage));
        currLine->len = text->lines[i]->len;
        currLine->words = (WordImage **)malloc(sizeof(WordImage *) * currLine->len);
        j = 0;
        while(j<text->lines[i]->len)
        {
            currWord = currLine->words[j] = (WordImage *)malloc(sizeof(WordImage));
            currWord->len = text->lines[i]->words[j]->len;
            currWord->chars = (BinaryImage **)malloc(sizeof(BinaryImage*) * currWord->len);
            k = 0;
            while (k<text->lines[i]->words[j]->len)
            {
                currWord->chars[k] = (BinaryImage *)malloc(sizeof(BinaryImage));
                BlitPadImage(source, currWord->chars[k], text->lines[i]->words[j]->chars[k]);
                k++;
            }
            j++;
        }
        ++i;
    }
    return res;
}
