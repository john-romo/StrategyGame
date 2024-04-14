
#include "server.h"

std::queue<Client*> waitingClients;
std::mutex mutex_waitingClients;


int main(){
	int serverSocket = get_server_socket(PORT);
	if(listen(serverSocket, 128)) std::cerr << "ERROR: Listen failed\n";
	std::cout << "Ready for connection...\n";
	fflush(stdout);

	struct sockaddr_in clientAddr;
	socklen_t length = sizeof(struct sockaddr_in);

	int clientSocket;
	while(1){
		clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &length);
		if(clientSocket == -1){
			std::cout << "ERROR: Connection failed\n"; 
			continue;
		}
		std::cout << clientAddr.sin_port << " CONNECTED\n";

		Client*	client = create_client(NULL, clientSocket, clientAddr);

		mutex_waitingClients.lock();
		waitingClients.push(client);

		if(waitingClients.size() >= 2){
			Client* white = waitingClients.front();
			waitingClients.pop();
			Client* black = waitingClients.front();
			waitingClients.pop();
			mutex_waitingClients.unlock();

			Game* game = new Game();
			game->clients.push_back(white);
			game->clients.push_back(black);

			int child;
			if(!(child = fork())){
				if(!fork()){
					new_game(game);
				}
				exit(0);
			}
			waitpid(child, NULL, 0);
		}
		else mutex_waitingClients.unlock();
	}
	return 0;
}


void new_game(Game* game){
	start_game();
	for(int i = 0; ; ++i){
		game->clients.at(i)->player = players[i];
	}
	run_game(game);
	end_game();
	return;
}


void run_game(Game* game){
	
}


void start_game(){
	create_players();
	create_headings();
	create_board();

}

void end_game(){
	delete_pieces();
	delete_board();
	delete_players();
}


Game* create_game(){
	Game* game = new Game;
	game->joinable = true;
	return game;
}


char* read_message(int socket){
	int charHSize = C_HEADER_SIZE*sizeof(int);
	int header[C_HEADER_SIZE] = {0};
	read(socket, header, charHSize); 

	int size = header[C_SIZE]; 
	int charBSize = size*sizeof(int);
	int* body[size] = {0};
	read(socket, body, charBSize-charHSize);

	char* msg = new char[size*sizeof(int)];
	memcpy(msg, header, charHSize);
	memcpy(msg+charHSize, body, charBSize-charHSize);

	return(msg);
}


void write_response(int socket, int errorType){		
	int r[S_RESPONSE_SIZE];
	r[S_TYPE] = MSG_TYPE_RESPONSE;
	r[S_SIZE] = S_RESPONSE_SIZE;
	r[S_ERROR_TYPE] = errorType;
	write(socket, r, S_RESPONSE_SIZE*sizeof(int));
}


Client* create_client(Player* player, int socket, sockaddr_in addr){
	Client* client = new Client;
	client->player = player;
	client->socket = socket;
	client->addr = addr;
	return client;
}




///////////////////// SOCKET STRUCTS ////////////////////////////

struct sockaddr_in get_socket_addr(int port){
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	return(addr);
}

int get_server_socket(int port){
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if(s == -1) std::cerr << "Socket creation failed\n";
	int one = 1;
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)))
		std::cerr <<  "Set socket options failed\n";
	struct sockaddr_in servAddr = get_socket_addr(port);
	if (bind(s, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
		std::cerr <<  "Bind failed\n";
	return(s);
}


/*
int server(){
	std::vector<Client*> clients;

	int epollfd;
	int readyfds;
	struct epoll_event event;
	struct epoll_event eventlist[NUM_COLORS];


	epollfd = epoll_create1(0);
    if (0 > epollfd){
		std::cerr << "epoll_create1() failed\n";	
		exit(1);
	}
	event.events = EPOLLIN | EPOLLRDHUP;

	init_clients();	


	while(!gameOver){
		readyfds = epoll_wait(epollfd, eventlist, NUM_COLORS, -1);
		if(readyfds < 0) continue;

		bool crashed = false;
		if((event.events & EPOLLERR) != 0) crashed = true;
		if((event.events & EPOLLHUP) != 0) crashed = true;
		if((event.events & EPOLLRDHUP) != 0) crashed = true;
		
		for(int i = 0; i < readyfds; ++i){
			printf("%d\n", i);
			for(Client* c : clients){
				if(eventlist[i].data.fd == c->socket){
					if(crashed){
						printf("%d CRASHED\n", c->player->color);
						break;
					}	
				}
				read_client_message(c);
				if(crashed) break;
			}
		}
		if(crashed) break;

	}

	delete_clients();
	return(0);
}

*/



void print_message(int* m, int size){
	for(int i = 0; i < size; ++i){
		printf("%d ", m[i]);
	}
	printf("\n");
