CC = gcc
CFLAGS = -O2 -Wall -I .

# This flag includes the Pthreads library on a Linux box.
# Others systems will probably require something different.
LIB = -lpthread

all: tiny cgi

tiny: csapp.o tiny_main.o clienterror.o doit.o parse_uri.o read_requesthdrs.o serve_dynamic.o serve_static.o
	$(CC) $(CFLAGS) -o tiny tiny_main.o clienterror.o doit.o parse_uri.o read_requesthdrs.o serve_dynamic.o serve_static.o csapp.o $(LIB)
	rm *.o

csapp.o: csapp.c
tiny_main.o: tiny_main.c
clienterror.o: clienterror.c
doit.o: doit.c
parse_uri.o: parse_uri.c
read_requesthdrs.o: read_requesthdrs.c
serve_dynamic.o: serve_dynamic.c
serve_static.o: serve_static.c

cgi:
	(cd cgi-bin; make)

clean:
	rm -f *.o tiny *~
	(cd cgi-bin; make clean)

