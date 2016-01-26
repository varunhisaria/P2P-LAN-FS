#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <string>
#include <cerrno>
#define LOCALHOST "127.0.0.1"
#define LISTENINGPORT 9090
#define DEBUG true
#define BLOCK_SIZE 4096

using namespace std;


/*listening_port : if server, this is the port to listen for number_of_connections
				   if client, this is the port which is used to send data
*/
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
		
	public:
		// Constructor
		ConnectionManager(bool is_tcp=true, unsigned int listening_port=LISTENINGPORT, string ip_address=LOCALHOST){
			// Initialise the variables
			this->is_tcp = is_tcp;
			this->listening_port = listening_port;
			this->self_ip_address = ip_address;

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
				if((this->socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)==-1){
					if(DEBUG){
						cout<<"Error while creating UDP socket\n";
						cout<<strerror(errno);
					}
					exit(1);
				}
			}

			if(setsockopt(this->socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1){
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
						cout<<"Error while listening for connections.\n"
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
		struct sockaddr_in* acceptConnection(){
			struct sockaddr_in client;
			socklen_t client_size = sizeof(client);
			accept(this->socket_descriptor, (struct sockaddr*)&client, &client_size);
			return &client;
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

		void sendDataToServer(string data){
			this->data = data.c_str();
			send(this->socket_descriptor, this->data, strlen(this->data),0);
			if(recv(this->socket_descriptor, buffer, sizeof(buffer),0)==0){
				if(DEBUG)
					cout<<"Connection terminated by server.\n";
			}
			if(DEBUG){
				cout<<"Server Responded : "<<buffer<<endl;
			}
		}

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
