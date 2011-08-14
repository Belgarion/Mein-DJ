all: mein-dj

mein-dj: mein-dj.o
	g++ -o $@ $^

mein-dj.o: mein-dj.cpp
	g++ -c $^

clean:
	rm -f *.o mein-dj

install: all
	install -m 0755 mein-dj /usr/local/bin
	install -m 0755 mixer/mein-dj-start /usr/local/bin
	install -m 0755 mixer/mein-dj-dasmixer /usr/local/bin
	install -m 0755 mixer/playback-capture /usr/local/bin
	install -m 0755 mixer/hercules-hdj_mod/add /usr/share/hercules-hdj_mod
	install -d /usr/local/mixer
	cp -ax mixer/mixer /usr/local/
