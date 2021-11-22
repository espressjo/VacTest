PROG = vacTest
lib:
	cd ../lib && make all
all: lib
	g++ -std=c++11 -c *.cpp -I../lib
	g++ -std=c++11 -I../lib main.o ../lib/gauge972.o ../lib/esp_32_serial_read.o -o $(PROG)
	g++ -std=c++11 -I../lib ../lib/gauge972.o ../lib/gauge972cmd.o -o gauge972cmd

clean:
	cd ../lib && make clean
	rm -f *.o
	rm -f gauge972cmd
	rm -f $(PROG)
