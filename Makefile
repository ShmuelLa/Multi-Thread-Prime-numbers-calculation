CC = gcc
CFLAG = -Wall -Wextra -g -o

slow:
	$(CC) solution.c $(CFLAG) solution -lpthread

git:
	git add -A
	git commit -m "$m"
	git push

clean:
	rm -f solution *.o 