CC=gcc
CFLAGS=-Wall

all: serveur client

serveur: serveur.c var.h
	$(CC) $(CFLAGS) serveur.c -o serveur

client: client.c var.h
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -f serveur client

