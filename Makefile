all: mein-dj

mein-dj: mein-dj.o
	g++ -o $@ $^

mein-dj.o: mein-dj.cpp
	g++ -c $^

clean:
	rm -f *.o mein-dj
