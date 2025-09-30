CC=gcc
CFLAGS=-pthread -I. -Wall -Wno-int-conversion -Wl,--no-as-needed -lrt

binaries=syncarray 

all: $(binaries)

syncarray: syncarray.c worker.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) -f $(binaries) *.o


 
