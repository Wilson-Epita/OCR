#ifndef IMAGE_H
#define IMAGE_H

typedef struct
{
	Uint8	r;
	Uint8	g;
	Uint8	b;
}Pixel;

typedef struct
{
	int		h;
	int		w;
	Pixel	*matrix;
}Image;

typedef struct
{
	int		h;
	int		w;
	Uint8	*matrix;
}BinaryImage;

#endif
