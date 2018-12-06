#define _DEFAULT_SOURCE
#include "network.h"
#include "training.h"
#include "header.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>

int ft_substring(char *s)
{
    int	i;
    int	lenstr;
    int	lensearch;
    int	j;
    char *search = ".bmp";
    i = 0;
    lenstr = strlen(s);
    lensearch = strlen(search);
    while (i < lenstr)
    {
      if (s[i] == search[0])
      {
        j = 0;
        while (j < lensearch && s[i] == search[j])
        {
          j++;
          i++;
        }
        if (j == lensearch)
          return (1);
      }
      i++;
    }
    return (0);
}


float *convert(Uint8 *matrix, int height, int width)
{
  float *convert;
  int i = 0;
  int w = 0;
  convert = (float*)malloc(height*width*sizeof(float));
  while (i < height)
  {
    w = 0;
    while (w < width)
    {
      if (matrix[i * width + w] != 0)
        convert[i*width + w] = 1;
      else
        convert[i*width+w] = 0;
      w++;
    }
    i++;
  }
  return(convert);
}

float *convert_inverse(Uint8 *matrix, int height, int width)
{
  float *convert;
  int i = 0;
  int w = 0;
  convert = (float*)malloc(height*width*sizeof(float));
  while (i < height)
  {
    w = 0;
    while (w < width)
    {
      if (matrix[i * width + w] != 0)
        convert[i*width + w] = 0;
      else
        convert[i*width+w] = 1;
      w++;
    }
    i++;
  }
  return(convert);
}
void recur(char* s, trainset *listetrain, int poschar)
{
    char chemin[4096];
    char tmp[4096];
    struct dirent *lecture;
    DIR *rep;

    rep = opendir(s);
    if (rep == NULL)
    {
       perror(s);
       return;
    }
    //printf("%s:\n", s);
    while ((lecture = readdir(rep))!=NULL)
    {
        if (lecture->d_type == DT_DIR)
        {
          if (strcmp(lecture->d_name, ".")!=0 && strcmp(lecture->d_name, "..")!=0)
          {
            strcpy(chemin, s);
            strcat(chemin,"/");
            strcat(chemin, lecture->d_name);
            poschar = atoi(lecture->d_name) + ' ';
            //printf("paths %s char %d \n",(lecture->d_name), poschar);
            recur(chemin, listetrain, poschar);
          }
        }
        else
        {
          strcpy(tmp,s);
          char *path = strcat(tmp, "/");
          path = strcat(path,lecture->d_name);
          if (ft_substring(lecture->d_name))
          {
            //printf("paths %s \n",path);
            Image *charac = image_load(path);
            BinaryImage *char2 = applyFilters(charac);
            add_data(listetrain, convert(char2->matrix,char2->h,char2->w),
                            char_to_output(poschar));
            //printf("here %c \n", poschar);
          //  FreeImage(charac);
            //FreeBinary(char2);
          }
        }
      }
    closedir(rep);
}
