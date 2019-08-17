main: prep main.o webHook.o
	g++ stage/main.o stage/webHook.o -lcurl -lpthread -lstdc++fs -o bin/back

main.o:
	g++ -c main.cpp -o stage/main.o

webHook.o:
	g++ -c webHook.cpp -o stage/webHook.o

prep:
	mkdir bin/ stage/ -p
	
clean:
	rm -rf bin/ stage/
