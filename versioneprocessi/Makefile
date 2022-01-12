default: main.o processtasks.o scene.o 
	cc main.o processtasks.o scene.o -o spacedefender -lncurses

main.o: main.c processtasks.h
	cc -c main.c processtasks.h

processtasks.o: processtasks.c processtasks.h scene.h
	cc -c processtasks.c processtasks.h scene.h

scene.o: scene.c scene.h
	cc -c scene.c scene.h
	
clean:
	rm *.o
	rm ./spacedefender

run:
	./spacedefender