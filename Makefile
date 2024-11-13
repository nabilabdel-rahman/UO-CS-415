CC=gcc
CFLAGS=-W -Wall -g
OBJS=main.o command.o string_parser.o
TARGET=pseudo-shell

$(TARGET): main.o command.o string_parser.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c command.h string_parser.h
	$(CC) $(CFLAGS) -c main.c

string_parser.o: string_parser.c string_parser.h
	$(CC) $(CFLAGS) -c string_parser.c

command.o: command.c command.h
	$(CC) $(CFLAGS) -c command.c

clean:
	rm -f $(TARGET) $(OBJS)