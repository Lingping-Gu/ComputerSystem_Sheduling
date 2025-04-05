CC = gcc
CFLAGS = -Wall -g
OBJS = scheduler.o job.o queue.o utils.o schedulers.o

simulator: $(OBJS)
	$(CC) $(CFLAGS) -o simulator $(OBJS)

scheduler.o: scheduler.c job.h queue.h utils.h schedulers.h
job.o: job.c job.h
queue.o: queue.c queue.h job.h
utils.o: utils.c utils.h
schedulers.o: schedulers.c schedulers.h job.h queue.h utils.h

clean:
	rm -f *.o simulator
