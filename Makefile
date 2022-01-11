run:
	gcc main.c scene.c scene.h processtasks.c  processtasks.h threadtasks.c  threadtasks.h -o procc -lncurses -pthread
	./procc


