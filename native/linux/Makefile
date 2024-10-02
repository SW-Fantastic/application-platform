unsafe.o: Unsafe.cpp
	g++ -c Unsafe.cpp -o unsafe.o

libPlatform: unsafe.o
	g++ unsafe.o -fPIC -shared -o libPlatform.so

all: libPlatform