CC= gcc -Wall
LIBS = -lm
INCLUDES = -I .

MAIN= ejecutable

SRCS = hashchain.c  hashtable.c  main.c

DEPS = hashchain.h  hashtable.h

OBJS = $(SRCS:.c=.o)

$(MAIN): $(OBJS)
	$(CC) -o $(MAIN) $(OBJS) $(LIBS)
%.o: %.c $(DEPS)
	$(CC) -c $< $(INCLUDES)
cleanall: clean
	rm -f $(MAIN)
clean:
	rm -f *.o *~ $(MAIN)