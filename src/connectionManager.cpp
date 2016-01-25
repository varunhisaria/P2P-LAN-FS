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
			if(this->is_tcp)
				this->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
			else
				this->socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
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

			// Error handling to be done.
			if(bind(this->socket_descriptor, (struct sockaddr*)&server, sizeof(server))!=0){
				if(DEBUG){
					cout<<"Error while binding socket to local address."<<endl;
					cout<<"Origin : ConnectionManager::startServer()"<<endl;
					cout<<errno;
					cout<<strerror(errno);
				}
			}

			if(listen(this->socket_descriptor, this->number_of_connections)!=0){
					if(DEBUG){
						cout<<"Error while listening for connections."<<endl;
						cout<<"Origin : ConnectionManager::startServer()"<<endl;
						cout<<strerror(errno);
					}
				}

			// Debugging information
			if(DEBUG){
				cout<<"Server started. Waiting for connections.";
				cout<<"Listening on port :: "<<this->listening_port;
				cout<<"Number of connections allowed :: "<<this->number_of_connections;
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
				cout<<"Attempting to connect to "<<this->sending_ip_address<<" at port "<<this->sending_port;
				cout<<"Connection going from "<<this->self_ip_address<<" and port "<<this->listening_port;
			}
			fflush(stdout);
			// Connect to the server. Error handling to be done.
			connect(socket_descriptor, (struct sockaddr*)&server, sizeof(server));
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
