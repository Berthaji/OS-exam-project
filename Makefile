default:
	gcc main.c Scene.c task.c Utils.c -o gioco -lncurses
	
threaded:
	gcc main.c Scene.c task.c Utils.c -o gioco -lncurses -DTHREADED

build_debug:
	gcc main.c Scene.c task.c Utils.c -o gioco -lncurses -Wall

bugs:
	gcc main.c Scene.c task.c Utils.c -o gioco -lncurses -Wall -Wextra -g

run:
	./gioco
	stty sane

all:
	rm ./gioco 2> /dev/null
	gcc main.c Scene.c task.c Utils.c -o gioco -lncurses
	./gioco
	stty sane


project:
	gcc main.c Scene.c -o gioco -lncurses & ./gioco

clean:
	rm ./gioco

fix:
	stty sane
	stty sane
	stty sane
	stty sane
	clear

sus: 
	gcc test1.c Scene.c -o test1 -lncurses
	./test1