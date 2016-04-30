utils:
	g++ main.cpp utils.cpp -o utils.out -lssl -lcrypto
sender:
	g++ -std=c++11 sender.cpp -o sender.out -pthread
listener:
	g++ -std=c++11 listener.cpp -o listener.out -pthread
clean:
	rm -f *.out
#all: sender.out listener.out
