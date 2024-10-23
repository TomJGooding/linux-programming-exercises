CFLAGS = -Wall -Wextra

copy: copy.c
	$(CC) $(CFLAGS) -o copy copy.c
