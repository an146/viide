test: viide
	./viide

SRC = viide.c

viide: $(SRC) Makefile
	$(CC) -Werror -o $@ `pkg-config --cflags --libs gtk+-3.0 webkitgtk-3.0` $(SRC)
