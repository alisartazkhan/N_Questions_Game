game: game.c mq.o
	gcc -Wall -Werror -std=c11 -o game game.c mq.o
mq.o: mq.c mq.h
	gcc -Wall -Werror -std=c11 -c mq.c
clean:
	rm -f game mq.o
