#ifndef DECOUPAGE_H
#define DECOUPAGE_H

#include "header.h"
typedef struct
{
	int		x0;
	int		x1;
	int		y0;
	int		y1;
}textRegion;

typedef struct
{
	int		len;
	int		*arr;
}Histo;

typedef struct
{
	int	len;
	textRegion		*chars;
}WordRegion;

typedef struct
{
	int	len;
	WordRegion		**words;
}LineRegion;

typedef struct
{
	int	len;
	LineRegion		**lines;
}TextRegion;

typedef struct
{
	int	len;
	BinaryImage		**chars;
}WordImage;

typedef struct
{
		int	len;
    WordImage		**words;
}LineImage;

typedef struct
{
	int	len;
  LineImage		**lines;
}TextImage;

#endif
