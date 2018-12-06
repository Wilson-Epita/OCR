#include "header.h"


/*
 * Otsu muste be called on img before this function.
 */
Histo	*cutHisto(BinaryImage *img)
{
	int			i, j;
	int			val;
	int			h, w;
	Histo		*res;

	res = NULL;
	h = img -> h;
	w = img -> w;
	res = (Histo *)malloc(sizeof(Histo));
	if(res == NULL)
	{
		printf("Bug malloc\n");
		return (NULL);
	}
	res -> len = h;
	res -> arr = (int*)malloc(h * sizeof(int));
	if (!(res -> arr))
	{
		printf("Bug malloc\n");
		return (NULL);
	}
	i = 0;
	while (i < h)
	{
		j = 0;
		val = 0;
		while (j < w)
		{
			if ((img -> matrix[i * w + j]) > 1)
				val++;
			j++;
		}
		res -> arr[i] = val;
		i++;
	}
	return (res);
}

WordRegion	*detectTextRegions(BinaryImage *img)
{
	Histo		*H;
	WordRegion	*TC;
	H = cutHisto(img);
	TC = determineXCoordinate(H);
	determineYCoordinate(img, TC);
	struct stat sb;
	char *path="textfiles";
	if(stat(path, &sb) == -1)
	{
		mkdir(path, 0700);
	}
	savetxtregions(img, TC, path);
	return TC;
}

WordRegion	*determineXCoordinate(Histo *h)
{
	int				MinEdges, MinLines;
	size_t			size;
	int				y;
	bool			insideTextArea;
	int				current, lg;
	bool			test;
	textRegion		rect;
	WordRegion		*res;

	MinEdges = 6;
	MinLines = 6;
	size = 0;
	y = 1;
	insideTextArea = false;
	res = (WordRegion *)malloc(sizeof(WordRegion));
	if(!(res))
		return (NULL);
	res -> len = 0;
	res -> chars = NULL;
	lg = h->len;
	while (y < lg)
	{
		current = h -> arr[y] ;
		test = (current >= MinEdges) ||
			(current != 0 && ((h->arr[y-1] >= MinEdges)||
							  (current - h -> arr[y-1] >=-1)));
		if (test)
		{
			if(!(insideTextArea))
			{
				rect.x0 = y;
				insideTextArea = true;
			}
		}
		else if (insideTextArea)
		{
			rect.x1 = y - 1;
			if((rect.x1 - rect.x0) > MinLines)
			{
				size += sizeof(textRegion);
				res->chars = realloc(res -> chars, size);
				res->chars[res -> len]= rect;
				res->len = res -> len + 1;
			}
			insideTextArea = false;
		}
		y++;
	}
	return (res);
}

void	determineYCoordinate(BinaryImage *img, WordRegion *regions)
{
	int				left, right;
	int				w, j;
	int				it, len;
	int				i,h;
	textRegion		rc;

	left = img -> w;
	right = -1;
	w = left;
	it = 0;
	len = regions -> len;
	while (it < len)
	{
		rc = regions -> chars[it];
		left = img -> w;
		right = -1;
		i = rc.x0;
		h = rc.x1;
		while (i <= h)
		{
			j = 0;
			while (j < w)
			{
				if (img -> matrix[i * w + j] != 0)
				{
					if (left > j)
						left = j;
					if (right < j)
						right = j;
				}
				j++;
			}
			i++;
		}
		(regions -> chars[it]).y0 = left;
		(regions -> chars[it]).y1 = right;
		it++;
	}
}
