CC=gcc
CFLAGS=-Wall
LDFLAGS= -lSDL2 -lSDL2_image -lm -lSDL2_ttf -lSDL2_mixer
EXEC=exec

all: $(EXEC)
	@echo "Compilation complete\nLauching Game ./exec"
	./exec

exec: main.o menu.o voiture.o SDL_Interface.o game.o score.o decors.o
	$(CC) -o exec main.o menu.o voiture.o SDL_Interface.o game.o score.o decors.o $(LDFLAGS)

main.o: main.c define.h
	$(CC) -o main.o -c main.c $(CFLAGS)

game.o: game.c game.h define.h voiture.h decors.o
	$(CC) -o game.o -c game.c $(CFLAGS)

menu.o: menu.c menu.h define.h
	$(CC) -o menu.o -c menu.c $(CFLAGS)

SDL_Interface.o: SDL_Interface.c SDL_Interface.h
	$(CC) -o SDL_Interface.o -c SDL_Interface.c $(CFLAGS)

voiture.o: voiture.c voiture.h define.h
	$(CC) -o voiture.o -c voiture.c $(CFLAGS)

score.o: score.c score.h define.h
	$(CC) -o score.o -c score.c $(CFLAGS)

decors.o: decors.c decors.h define.h
	$(CC) -o decors.o -c decors.c $(CFLAGS)

clean:
	rm -rf *.o 

mrproper: clean
	rm -rf $(EXEC)

edit:
	subl game.h game.c
	subl voiture.h voiture.c
	subl decors.h decors.c
	subl menu.c menu.h
	subl main.c define.h
	subl score.c score.h
	subl SDL_Interface.c SDL_Interface.h
