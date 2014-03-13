CC			= gcc
CFLAGS		= -Wall -m64 -lm -ggdb -O2

all: server client

client: Client/client.c Client/lib.c Client/client.h
	$(CC) $(CFLAGS) -o client Client/client.c Client/lib.c Client/client.h

server: Server/server.c Server/lib.c Server/server.h
	$(CC) $(CFLAGS) -o server Server/server.c Server/lib.c Server/server.h

.PHONY: clean

clean:
	rm -f client
	rm -f server 
