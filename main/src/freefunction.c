#include "header.h"

void    FreeImage(Image *img)
{
    if (img)
	    free(img -> matrix);
	free(img);
}

void 	FreeBinary(BinaryImage *img)
{
    if (img)
	    free(img -> matrix);
	free(img);
}

void    FreeHisto(Histo *histo)
{
    if (histo)
	    free(histo->arr);
	free(histo);
}

void	FreeWordRegion(WordRegion *region)
{
    if(region)
	    free(region->chars);
	free(region);
}

void    FreeLineRegion(LineRegion *region)
{
    if (region)
    {
        for (int i = 0; i < region->len ; ++i)
        {
            FreeWordRegion(region->words[i]);
        }
    }

    free(region);
}

void    FreeTextRegion(TextRegion *region)
{
    if (region)
    {
        for (int i = 0; i < region->len ; ++i)
        {
            FreeLineRegion(region->lines[i]);
        }
    }

    free(region);
}

void	FreeWordImage(WordImage *region)
{
    if (region)
    {
        for (int i = 0; i < region->len ; ++i)
        {
            FreeBinary(region->chars[i]);
        }
    }
	free(region);
}

void    FreeLineImage(LineImage *region)
{
    if (region)
    {
        for (int i = 0; i < region->len ; ++i)
        {
            FreeWordImage(region->words[i]);
        }
    }

    free(region);
}

void    FreeTextImage(TextImage *region)
{
    if (region )
    {
        for (int i = 0; i < region->len ; ++i)
        {
            FreeLineImage(region->lines[i]);
        }
    }

    free(region);
}
