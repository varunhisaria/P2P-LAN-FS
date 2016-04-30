#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <string>
#include <cerrno>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <atomic>



#define LOCALHOST "0.0.0.0"
#define LISTENINGPORT 9090
#define DEBUG true
#define BLOCK_SIZE 10
#define NO_OF_THREADS 10
#define min(a,b) a<b ? a : b

//extern "C" void* processConnection(void *args);
using namespace std;


class ConnectionManager{
	private:
		unsigned int socket_descriptor;
		bool is_tcp;
		unsigned int listening_port;
		unsigned int sending_port;
		string self_ip_address;
		string sending_ip_address;
		unsigned int number_of_connections;
		char* buffer;
		char* data;

		// Vector to handle connection in each thread.
		vector<thread> connectionThreadVector;
		// Counter to keep track of thread number
		unsigned int thread_no;

	public:

		ConnectionManager(bool is_tcp=true, unsigned int listening_port=LISTENINGPORT, string ip_address=LOCALHOST){
			// Initialise the variables
			this->is_tcp = is_tcp;
			this->listening_port = listening_port;
			this->self_ip_address = ip_address;
			this->thread_no = 0;
			// Initialise the object
			this->init();

			if(DEBUG){
				cout<<"Socket initialised.\n";
			}
		}

		// Initialise a TCP or UDP socket based on the boolean is_tcp
		void init(){
			if(this->is_tcp){
				if((this->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1){
					if(DEBUG){
						cout<<"Error while creating TCP socket\n";
						cout<<strerror(errno);
					}
					exit(1);
				}
			}
			else{
				if(this->socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)==-1){
					if(DEBUG){
						cout<<"Error while creating UDP socket\n";
						cout<<strerror(errno);
					}
					exit(1);
				}
			}
            int yes =1;
            int setsockreturn = setsockopt(this->socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
            cout<<"Set sock returns "<<setsockreturn<<endl;
			if(setsockreturn==-1){
				if(DEBUG){
					cout<<"Error while setting socket options.\n";
					cout<<strerror(errno);
				}
				exit(1);
			}
		}

		// Start a server. TCP or UDP dependent on the socket type.
		void startServer(unsigned int number_of_connections=SOMAXCONN){
			// Initialise the number of connections for the server
			this->number_of_connections = number_of_connections;
			struct sockaddr_in server;

			memset(&server,0,sizeof(server));
			server.sin_family=AF_INET;
			server.sin_addr.s_addr=inet_addr(this->self_ip_address.c_str());
			server.sin_port = htons(this->listening_port);

			// Bind socket to address
			if(bind(this->socket_descriptor, (struct sockaddr*)&server, sizeof(server))!=0){
				if(DEBUG){
					cout<<"Error while binding socket to local address.\n";
					cout<<"Origin : ConnectionManager::startServer()\n";
					cout<<strerror(errno);
				}
				exit(1);
			}

			// Listen for connections
			if(listen(this->socket_descriptor, this->number_of_connections)!=0){
					if(DEBUG){
						cout<<"Error while listening for connections.\n";
						cout<<"Origin : ConnectionManager::startServer()\n";
						cout<<strerror(errno);
					}
					exit(1);
				}

			// Debugging information
			if(DEBUG){
				cout<<"Server started. Waiting for connections.\n";
				cout<<"Listening on port :: "<<this->listening_port<<endl;
				cout<<"Number of connections allowed :: "<<this->number_of_connections<<endl;
			}

			fflush(stdout);
		}

		// Accept an incoming connection. Return the client socket information to the calling function.
		int acceptConnection(){
			struct sockaddr_in client;
			int new_fd;
			socklen_t client_size = sizeof(client);
			while (thread_no < NO_OF_THREADS){
				new_fd = accept(this->socket_descriptor, (struct sockaddr*)&client, &client_size);
				if(new_fd < 0){
					cerr << "Cannot accept connection" << endl;
					exit(1);
				}
				else{
					cout<<"Connection successful" << endl;
				}

				connectionThreadVector.push_back(thread(&ConnectionManager::processConnection,this,new_fd));

				thread_no++;
			}


			for (auto& th : connectionThreadVector) th.join();

		}

		void processConnection(int fd){
			//cout<<"Thread No : "<<pthread_self()<<endl;
			cout<<"Processing connection "<<endl;
			char buffer[BLOCK_SIZE];
			bzero(buffer,BLOCK_SIZE+1);
			//int fd = *((int*)args_ptr);
			//bool loop = false;
			int sizeLeft = 0;
			int totalSize = 0;
			int counter = 0;
			bzero(buffer, BLOCK_SIZE+1);
			recv(fd, buffer, 16, 0);
			cout<<"Header received : "<<buffer<<endl;
			totalSize = stoi(buffer);
			cout<<"File size to be downloaded : "<<totalSize;
			sizeLeft = totalSize;
			while(sizeLeft > 0){
				bzero(buffer, BLOCK_SIZE+1);
				recv(fd, buffer, min(BLOCK_SIZE,sizeLeft),0);
				//if(counter > 10)
				//	break;
				string buffer_str(buffer);
				cout<<buffer_str<<endl;
				ofstream writer;
				writer.open("recv_"+to_string(counter));
				cout<<"Writing data to filename :: "<<"recv_"+to_string(counter)<<endl;
				cout<<"Data to be written :: "<<buffer_str<<endl;
				writer<<buffer_str;
				writer.close();
				//printf("%d", buffer[0]);
				counter++;
				sizeLeft -= min(BLOCK_SIZE,sizeLeft);
			}
			this->mergeParts(totalSize, "recv", counter);
			cout<<"Closing connection and thread"<<endl;
			close(fd);
		}
		void mergeParts(int totalSize, string filename, int counter){
			ofstream writer;
			ifstream reader;
			char* buffer = new char[10];
			writer.open(filename);
			for(int i=0; i<counter; i++){
				cout<<"Appending block :: "<<i<<endl;
				bzero(buffer,11);
				reader.open(filename+"_"+to_string(i));
				reader.read(buffer,10);
				cout<<"Data :: "<<buffer<<endl;
				writer << buffer;
				reader.close();
			}
			writer.close();
		}
		void connectToServer(string sending_ip_address=LOCALHOST, unsigned int sending_port=LISTENINGPORT){

			// Initialise variables
			this->sending_ip_address = sending_ip_address;
			this->sending_port = sending_port;

			// Initialise struct to hold information about server to which to connect to
			struct sockaddr_in server;
			memset(&server,0, sizeof(server));
			server.sin_family = AF_INET;
			server.sin_addr.s_addr=inet_addr(this->sending_ip_address.c_str());
			server.sin_port = htons(this->sending_port);

			if(DEBUG){
				cout<<"Attempting to connect to "<<this->sending_ip_address<<" at port "<<this->sending_port<<endl;
				cout<<"Connection going from "<<this->self_ip_address<<" and port "<<this->listening_port<<endl;
			}
			fflush(stdout);
			// Connect to the server. Error handling to be done.
			connect(this->socket_descriptor, (struct sockaddr*)&server, sizeof(server));
		}

		/*void sendDataToServer(string data){
			this->data = data.c_str();
			send(this->socket_descriptor, this->data, strlen(this->data),0);
			if(recv(this->socket_descriptor, buffer, sizeof(buffer),0)==0){
				if(DEBUG)
					cout<<"Connection terminated by server.\n";
			}
			if(DEBUG){
				cout<<"Server Responded : "<<buffer<<endl;
			}
		}*/

		void getServerDetails(){
			cout<<"IP Address :: "<<this->self_ip_address;
			cout<<"Port :: "<<this->listening_port;
			cout<<"Number of connections :: "<<this->number_of_connections;
		}

		void getClientDetails(){
			cout<<"IP Address :: "<<this->self_ip_address;
			cout<<"Port :: "<<this->listening_port;
			cout<<"Server IP Address :: "<<this->sending_ip_address;
			cout<<"Server Port :: "<<this->sending_port;
		}
};

int main()
{
    ConnectionManager listner;
    listner.startServer();
    int listening_fd = listner.acceptConnection();
    cout<<listening_fd;
    //char rec[100];
    //recv(listening_fd,rec,100,0);
    //scout<<rec;
}

