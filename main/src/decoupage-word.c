#include "header.h"

void	BlitImage(BinaryImage *source, BinaryImage *dest, textRegion   bornes)
{
	int		x0,y0,x1,y1;
	int		withd, height;
	int		srcw;

	x0 = bornes.x0;
	y0 = bornes.y0;
	x1 = bornes.x1;
	y1 = bornes.y1;
	withd = y1 - y0 +1;
	height = x1 - x0 +1;
	srcw = source -> w;
	dest -> h = height;
	dest -> w = withd;
	dest -> matrix = (Uint8*)malloc(sizeof(Uint8) * height * withd);
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
				dest -> matrix[(i - x0) * withd + (j - y0)] =
												source -> matrix[i * srcw + j];
			}
		}
	}

}

Histo	*cutHistoword(BinaryImage *img)
{
    int			i, j;
    int			val;
    Histo		*res;
    int			h, w;

    h = img -> h;
    w = img -> w;
    res = NULL;
    res = (Histo*)malloc(sizeof(Histo));
    if(res == NULL)
    {
        printf("Bug malloc\n");
        return (NULL);
    }
    res -> len = w;
    res -> arr = (int *)malloc(w * sizeof(int));
    if (!(res -> arr))
    {
        printf("Bug malloc\n");
        return (NULL);
    }
    j = 0;
    while (j < w)
    {
        i = 0;
        val = 0;
        while (i < h)
        {
            if ((img->matrix[i*w + j])>1)
                val++;
            i++;
        }
        res -> arr[j] = val;
        j++;
    }
    return (res);
}


TextImage *detectwordRegions(BinaryImage *img, WordRegion* linesregions)
{
	TextRegion * text= (TextRegion *)malloc(sizeof(TextRegion));
	text -> len = linesregions->len;
	text -> lines = (LineRegion **)malloc(sizeof(LineRegion*) * text -> len);
	detectwords(img, linesregions, text);
	TextImage *textImage = regionToImage(img, text);
	charactersave(textImage);
	FreeTextRegion(text);
	return textImage;
}

void    detectwords(BinaryImage *img, WordRegion * lines, TextRegion * text)
{
	textRegion temp;
    BinaryImage *line = NULL;
	int lg = lines->len;
	int i = 0;
	int moyenne = 0;

	while (i < lg)
	{
		temp = lines->chars[i];
        line = (BinaryImage *) malloc(sizeof(BinaryImage));
        BlitImage(img, line, lines->chars[i]);
        Histo *histo = cutHistoword(line);
		WordRegion *chars = (WordRegion *)malloc(sizeof(WordRegion));
		chars ->chars = NULL;
		chars ->len = 0;
		moyenne = determineYwordCoor(histo, chars, temp);
		//saveallchars(img, chars, i);
		text->lines[i] = (LineRegion *)malloc(sizeof(LineRegion));
		text->lines[i]->words = NULL;
		text->lines[i]->len = 0;
		detectscpaceYcoor(histo, chars, temp, text->lines[i], moyenne);
		FreeHisto(histo);
		free(chars);
		++i;
	}

	FreeBinary(line);
}

int	determineYwordCoor(Histo *h, WordRegion *words, textRegion	line)
{
	int				MinEdges, MinLineDiff, MinLines, moyenne, tailles;
	size_t			size;
	int				y;
	bool			insideTextArea;
	textRegion		rect;

	moyenne = 0;
	tailles = 0;
	MinEdges = 0;
	MinLineDiff = 2;
	MinLines = 0;
	size = (size_t) words->len;
	y = 0;
	insideTextArea = false;
	int lg = h->len;
	while (y < lg)
	{
		if ( h -> arr[y] > MinEdges ||
					(y>0 && h -> arr[y-1] - h -> arr[y]) > MinLineDiff)
		{
			if(!(insideTextArea))
			{
				rect.y0 = line.y0 +y;
				insideTextArea = true;
			}
		}
		else if (insideTextArea)
		{
			rect.y1 = line.y0 + y - 1;
			tailles += rect.y1 - rect.y0;
			rect.x0 = line.x0;
			rect.x1 = line.x1;
			size += sizeof(textRegion);
			words->chars = realloc(words -> chars, size);
			words->chars[words -> len]= rect;
			words->len = words -> len + 1;
			insideTextArea = false;
		}
		y++;
	}
	if (insideTextArea)
	{
		rect.y1 = line.y0 + y - 1;
		tailles += rect.y1 - rect.y0;
		rect.x0 = line.x0;
		rect.x1 = line.x1;
		size += sizeof(textRegion);
		words->chars = realloc(words -> chars, size);
		words->chars[words -> len]= rect;
		words->len = words -> len + 1;
		insideTextArea = false;
	}
	if (words->len)
		moyenne = tailles/words->len;
	return moyenne;
}

void detectscpaceYcoor(Histo *h, WordRegion *chars, textRegion currLine, LineRegion *line, int moyenne)
{
	size_t	size;
	int		y;
	bool	insideSpace;
	int     left ,right, pos, start;

	size = 0;
	y = left = right = pos = start = 0;
	insideSpace = false;
	int lg = h->len;
	while (y < lg)
	{
		if ( h -> arr[y] == 0 )
		{
			if(!(insideSpace))
			{
				left = currLine.y0 +y;
				insideSpace = true;
			}
		}
		else if (insideSpace)
		{
			right= currLine.y0 + y - 1;
			if(right - left >= moyenne/2)
			{
				pos = (left+right) / 2;
				size += sizeof(WordRegion *);
				line->words = realloc(line->words, size);
                line->words[line->len] = (WordRegion *)malloc(sizeof(WordRegion));
                line->words[line->len]->chars = NULL;
                line->words[line->len]-> len = 0;
                start = addchars(chars, line->words[line->len], start, pos) - 1;
				line->len = line->len + 1;
			}
			insideSpace = false;
		}
		y++;
	}
	if (!insideSpace)
	{
        size += sizeof(WordRegion *);
        line->words = realloc(line->words, size);
        line->words[line->len] = (WordRegion *)malloc(sizeof(WordRegion));
        line->words[line->len]->chars = NULL;
        line->words[line->len]-> len = 0;
		start = addchars(chars, line->words[line->len], start, currLine.y0 + lg);
        line->len = line->len + 1;
	}
}

int addchars(WordRegion	*charac, WordRegion	*word, int start, int pos)
{
	int stop, lg;
	size_t	size;
	textRegion curr;

	size = 0;
	stop  = 0;
	lg = charac->len;

	while (start<lg && !stop)
	{
		curr = charac->chars[start];
		stop = curr.y1>=pos;
		if (!stop)
		{
			size += sizeof(textRegion);
			word->chars = realloc(word->chars, size);
			word->chars[word->len]= curr;
			word->len = word->len + 1;
		}
		start++;
	}

	return start;
}

void charactersave(TextImage *text)
{
	struct stat sb;
	char *textpath = "textfiles";
	if(stat(textpath, &sb) == -1)
	{
		mkdir(textpath, 0700);
	}
	int i  = 0;
	int j  = 0;
	int k = 0;
	int lgline = 0;
	int lgword = 0;
	int lgblock = 0;
	char *linepath = "textfiles/line";
	char *wordpath = "textfiles/line/word";
	char *charpath = "textfiles/line/word/block.bmp";
	int stringlen = 0;
	while (i < text->len)
	{
		j = 0;
		stringlen++;
		while (j < text->lines[i]->len)
		{
			k = 0;
			stringlen++;
			while (k < text->lines[i]->words[j]->len)
			{
				stringlen++;
				k++;
			}
			j++;
		}
		i++;
	}
	outputstring = (char*)malloc((stringlen + 1)*sizeof(char));
	i = 0;
	j = 0;
	k = 0;
	stringlen = 0;
	while(i<text->len)
	{

		lgline = strlen(linepath)+countDigit(i);
		char linefolder[lgline];
		sprintf(linefolder, "textfiles/line%d",i);
		struct stat sb;

		if(stat(linefolder, &sb) == -1)
		{
			mkdir(linefolder, 0700);
		}
		j = 0;
		while(j<text->lines[i]->len)
		{
			lgword = strlen(wordpath) + countDigit(i) +countDigit(j);
			char wordfolder[lgword];
			sprintf(wordfolder, "textfiles/line%d/word%d",i,j);
			struct stat sb;

			if(stat(wordfolder, &sb) == -1)
			{
				mkdir(wordfolder, 0700);
			}

			k = 0;
			while (k < text->lines[i]->words[j]->len)
			{
			    SDL_Surface *charac = NULL;
			    lgblock = strlen(charpath) + countDigit(i) + countDigit(j) + countDigit(k);
          char buff[lgblock];
          sprintf(buff, "textfiles/line%d/word%d/block%d.bmp", i, j, k);
          BinaryImage *ourchar = resize(text->lines[i]->words[j]->chars[k],28,28);

					float *charfloat = convert_inverse(ourchar->matrix, ourchar->h, ourchar->w);
					for(int a = 0; a < 28; a++)
					{
						printf("debug2 : \n");
						for(int b = 0; b < 28; b++)
						{
							printf("%d ",(int)charfloat[a * 28 + b]);
						}
					}
					// printf("Debug #2\n");
					set_network_activation(ocr, charfloat);
					outputstring[stringlen++] = output_to_char(ocr->layers[ocr->nb_layers - 1]);
          charac = BinaryToSDL(ourchar);
          SDL_SaveBMP(charac, buff);
          FreeBinary(ourchar);
          SDL_FreeSurface(charac);
          k++;
			}
			outputstring[stringlen++] = ' ';
			j++;
		}
		outputstring[stringlen++] = '\n';
		++i;
	}
    printf("Debug \n");
	outputstring[stringlen] = '\0';
}
