CC = gcc



lab1:
	${CC} -o dis_time dis_time.c `pkg-config --cflags --libs gtk+-2.0` 
	${CC} -o dis_cpu dis_cpu.c `pkg-config --cflags --libs gtk+-2.0` 
	${CC} -o dis_sum dis_sum.c `pkg-config --cflags --libs gtk+-2.0` 
	${CC} -o dis_three dis_three.c

clean:
	rm -f *.o

rebuild: clean
