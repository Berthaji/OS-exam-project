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
	gcc main.c Scene.c task.c Utils.c -o gioco -lncurses -lpthread
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
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	stty sane
	clear

sus2: 
	gcc test2.c Scene.c -o test2 -lncurses
	./test1

sus3: 
	gcc test3.c Scene.c -o test3 -lncurses -lpthread
	./test1

sus4: 
	gcc test4.c Scene.c -o test4 -lncurses -lpthread
	./test1

sus8:
	gcc test8.c task.c Scene.c Utils.c  -o test8 -lncurses -lpthread
	./test8

sus8d:
	gcc test8.c task.c Scene.c Utils.c  -o test8 -lncurses -lpthread -Wall -Wextra -g -std=gnu90

sus10:
	gcc test10.c task.c Scene.c Utils.c  -o test10 -lncurses -lpthread
	./test10

sus11:
	gcc test11.c task.c Scene.c Utils.c  -o test11 -lncurses -lpthread
	./test11

sus11d:
	gcc test11.c task.c Scene.c Utils.c  -o test11 -lncurses -lpthread -Wall -Wextra -g


