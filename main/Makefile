NAME	=	ocr

SDIR    =   src
ODIR    =   obj

SRCS	=	$(wildcard $(SDIR)/*.c)


_OBJS    =   $(SRCS:.c=.o)

OBJS    =   $(patsubst  $(SDIR)/%.c,$(ODIR)/%.o,$(SRCS))

INCLUDE = include

CC		=	gcc

CFLAGS	+= -Wextra -Wall -std=c99

LIB	=	`sdl2-config --cflags --libs` -lm
GTK 	=	`pkg-config --cflags --libs gtk+-3.0`
GMOD 	= `pkg-config --libs gmodule-2.0`
RM	=	rm -f
CLEAN	=	clean


all	:
		@make $(NAME)
$(NAME)	:   $(OBJS)
		@echo $(_OBJS)
		$(CC) -g -o $(NAME) $^ $(LIB) $(GTK) $(GMOD) -lSDL2_image
		# -fsanitize=address
clean	:
		$(RM) $(OBJS)

obj/%.o: src/%.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIB) $(GTK) $(GMOD) -I$(INCLUDE)

fclean	:	clean
		$(RM) $(NAME)
re	:	fclean all
