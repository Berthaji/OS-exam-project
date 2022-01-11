default:
	gcc main.c scene.c processtasks.c threadtasks.c -o spacedefender -lncurses -pthread

clean:
	rm /*.o
	rm ./spacedefender

run:
	./spacedefender