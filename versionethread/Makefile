default: main.o threadtasks.o scene.o 
	cc main.o threadtasks.o scene.o -o spacedefender -lncurses -lpthread

main.o: main.c threadtasks.h
	cc -c main.c threadtasks.h

threadtasks.o: threadtasks.c threadtasks.h scene.h
	cc -c threadtasks.c threadtasks.h scene.h

scene.o: scene.c scene.h
	cc -c scene.c scene.h
	
clean:
	rm *.o
	rm ./spacedefender

run:
	./spacedefender