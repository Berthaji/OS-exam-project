default:
	gcc main.c Scene.c task.c -o gioco -lncurses
threaded:
	gcc main.c Scene.c task.c -o gioco -lncurses -DTHREADED

build_debug:
	gcc main.c Sprites.c Scene.c task.c  -o gioco -lncurses -Wall

run:
	./gioco

all:
	gcc main.c Scene.c -o gioco -lncurses
	./gioco


project:
	gcc main.c Scene.c -o gioco -lncurses & ./gioco

clean:
	rm ./gioco

fix:
	stty sane