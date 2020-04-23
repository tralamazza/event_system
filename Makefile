# Simple makefile

all : main

check : all
	./main

clean :
	rm -rf main *.o

main.o: main.cpp include/eventsystem/Dispatcher.hpp \
        include/eventsystem/Event.hpp BufferFullEvent.hpp \
        TestEvent.hpp Observer.hpp
	g++ -Iinclude -c main.cpp

main : main.o
	g++ -Iinclude $^ -o $@
