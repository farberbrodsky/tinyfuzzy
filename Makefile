CC = cc
CCFLAGS = -Wall

default: build/main.o build/libtinyfuzzy.o
	$(CC) $(CCFLAGS) build/main.o build/libtinyfuzzy.o -o build/tinyfuzzy

build/main.o: src/main.c src/tinyfuzzy.h
	$(CC) $(CCFLAGS) -c src/main.c -o build/main.o

build/libtinyfuzzy.o: src/tinyfuzzy.c src/tinyfuzzy.h
	$(CC) $(CCFLAGS) -c src/tinyfuzzy.c -o build/libtinyfuzzy.o

clean:
	rm -f build/tinyfuzzy build/*.o
