all: main main-speed

main: asd.o main.o
	g++ -o $@ $^

main-speed: asd-speed.o main-speed.o
	g++ -o $@ $^

asd.o: asd.cc TreeMap.h
	g++ -ggdb -O0 -c -o $@ $<

asd-speed.o: asd.cc TreeMap.h
	g++ -D NDEBUG -c -O2 -o $@ $<

main.o: main.cc TreeMap.h
	g++ -D DO_TEST=1 -D DO_SPEED_TEST=0 -ggdb -O0 -c -o $@ $<

main-speed.o: main.cc TreeMap.h
	g++ -D DO_TEST=0 -D DO_SPEED_TEST=1 -D NDEBUG -O2 -c -o $@ $<

clean:
	rm -f -- main main-speed core *.o