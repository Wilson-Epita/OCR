# ifndef HEADER_H
#define HEADER_H

# include <stdio.h>
# include <string.h>
# include <dirent.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdbool.h>
# include <math.h>
# include <SDL2/SDL.h>
# include <SDL2/SDL_surface.h>
# include "SDL_image.h"
# include "image.h"
# include "decoupage.h"
# include "network.h"
# include "training.h"
# include <err.h>
# include <gtk/gtk.h>

//Proto function
network *ocr;
char		*outputstring;
//Pixel
Uint32			getpixel(SDL_Surface *surface, int x, int y);
void			putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

//Debug
void			PrintIMG(Image *img);
void			PrintSurface(SDL_Surface *img);
int				countDigit(long long n);
void			printHisto(Histo *h);
void			savetxtregions(BinaryImage *img, WordRegion *regions,
																char *path);
void  saveBinImage(BinaryImage *img);
void  print_tab(int *tab, int h, int w);
void  saveallchars(BinaryImage	*img, WordRegion	*chars, int index);

//Image
Image   *image_load (char *path);
BinaryImage    *applyFilters(Image *img);
TextImage    *detectionCall(BinaryImage *img);
BinaryImage		*niveaudegris(Image *img, int mode);
void			*edgeImage(BinaryImage *img);
BinaryImage		*resize(BinaryImage *img, int width, int height);
Uint8			maximum(Uint8 a, Uint8 b, Uint8 c);
void			*sharpImage(BinaryImage *img);

//Convertion
Image			*SDLToIMG(SDL_Surface* img);
SDL_Surface		*IMGtoSDL( Image *img);
SDL_Surface		*BinaryToSDL(BinaryImage *img);
SDL_Surface		*BinaryRegionToSDL(BinaryImage *img, textRegion rc);
void	BlitPadImage(BinaryImage *source, BinaryImage *dest, textRegion   bornes);
TextImage *regionToImage(BinaryImage *source, TextRegion *text);
//conversion uint8 to float
float *convert(Uint8 *matrix, int height, int width);
float *convert_inverse(Uint8 *matrix, int height, int width);
//Decoupage
	//	Lines
Histo			*cutHisto(BinaryImage *img);
WordRegion		*detectTextRegions(BinaryImage *img);
WordRegion		*determineXCoordinate(Histo *h);
void			determineYCoordinate(BinaryImage *img,WordRegion *regions);


		//word
//int			reconstruction(int i, int h, int j, int w, BinaryImage *img);
Histo		*cutHistoword(BinaryImage *img);
TextImage 	*detectwordRegions(BinaryImage *img, WordRegion* linesregions);
void    	detectwords(BinaryImage *img, WordRegion * lines, TextRegion * text);
int			determineYwordCoor(Histo *h, WordRegion *words, textRegion line);
void		BlitImage(BinaryImage *source, BinaryImage *dest, textRegion   bornes);
void    charactersave(TextImage *text);
int 		addchars(WordRegion	*chars, WordRegion *word, int start, int pos);
void 		detectscpaceYcoor(Histo *h, WordRegion *chars, textRegion currLine, LineRegion * line, int moyenne);


//Otsu
BinaryImage		*otsu(BinaryImage *img);
float			*make_histogram(BinaryImage *img);
float			*normalize_histogram(float *histogram, int nbPixel);
float			zero_order_cumultative_moment(float *histogram, int k);
float			first_order_cumultative_moment(float *histogram, int k);
float			calc_variance(float uT, float wk, float uk);

//FREE
void	FreeImage(Image *img);
void	FreeBinary(BinaryImage *img);
void	FreeHisto(Histo *histo);
void	FreeWordRegion(WordRegion *region);
void	FreeLineRegion(LineRegion *region);
void	FreeTextRegion(TextRegion *region);
void	FreeWordImage(WordImage *region);
void    FreeLineImage(LineImage *region);
void    FreeTextImage(TextImage *region);

//Interface
//static void activate (GtkApplication* app, gpointer user_data);
int interfaceCall(int    argc,char **argv);
int remove_directory(const char *path);

#endif
