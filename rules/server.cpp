// server.cpp

#include "server.h"

std::queue<Client*> waitingClients;
std::mutex mutex_waitingClients;


void server(){
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
	//printf("\n");
	create_players();
	create_headings();
	create_board();

}

void end_game(){
	delete_pieces();
	delete_board();
	delete_players();
	//printf("\n");
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
void init_clients(){
	int i = 0;
	while(i < NUM_COLORS){
		Client* c = new Client();
		c->player = players[i++];
		c->socket = accept(sock, (struct sockaddr *) &clientAddr, &length);
		if(c->socket == -1) std::cerr << "Connection failed\n";
		printf("Player %d CONNECTED\n", c->player->color);
		write(c->socket, &c->player->color, sizeof(int));
		event.data.fd = c->socket; 
    	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, c->socket, &event) < 0){
    		std::cerr << "epoll_ctl() failed";
		}
	}
	close(sock);
	auto rng = std::default_random_engine {};
	std::shuffle(std::begin(clients), std::end(clients), rng);
}


void delete_clients(){
	while(!clients.empty()){
		Client* c = clients.back();
		clients.pop_back();
		close(c->socket);
		delete(c);
	}
}


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






///////////////////// FROM CLIENT ///////////////////////////////


/*
char process_client_message(Client* client, int* m){
	try{
		int type = m[C_TYPE];

		switch(type){
			case MSG_TYPE_MOVE:
				return process_client_move(client, m);
				break;
			case MSG_TYPE_PLACE:
				return process_client_placement(client, m);
				break;
			default:
				return ERROR_CODE;
		}	
	}
	catch(...){
		std::cerr << "ERROR: translate_client_message() FAILED\n";
		return ERROR_CODE;
	}
	return ERROR_CODE;
}


char process_client_move(Client* client, int* m){
	try{
		int x = m[C_MOVE_X];
		int y = m[C_MOVE_Y];

		Square* s = get_square(x, y);
		if(!s) return ERROR_CODE;
		Task* t;

		if(m[C_CANCEL] == MSG_TYPE_CANCEL){
			t = new Task(MSG_TYPE_CANCEL, 0, 0);
		}
		else{
			int headingID = m[C_MOVE_H];
			t = new Task(MSG_TYPE_MOVE, s->piece, headingID);	
		}
		
		s->piece->mutex.lock();
		s->piece->tasks.push(t);
		s->piece->mutex.unlock();
		return ACCEPT_CODE;
	}
	catch(...){
		std::cerr << "ERROR: process_client_move() FAILED\n";
		return ERROR_CODE;
	}
	return ERROR_CODE;
}


char process_client_placement(Client* client, int* m){
	try{
		int type = m[C_PLACE_TYPE];
		int color = m[C_PLACE_COLOR];
		int x = m[C_PLACE_X];
		int y = m[C_PLACE_Y];
		Piece* p = create_piece(type, color);
		if(!place_piece(p, x, y)){
			delete(p);
			return ERROR_CODE;
		}
		return ACCEPT_CODE;
	}
	catch(...){
		std::cerr << "ERROR: process_client_placement() FAILED\n";
		return ERROR_CODE;
	}
}


///////////////////// TO CLIENT /////////////////////////////////

void create_message_header(int* m, int type, int msgLength, int unitLength){
	m[S_TYPE] = type;
	m[S_SIZE] = msgLength;
	m[S_UNIT] = unitLength;
}

//-------------------- MOVE MESSAGE ---------------------------//

void send_move_message(std::stack<BinMove*>* moves){
	int* msg = create_move_message(moves);
	delete(msg);
}

int* create_move_message(std::stack<BinMove*>* moves){
	int size = moves->size();
	int length = sizeof(BinMove);
	int* m = new int[S_HEADER_SIZE + (size*length)];
	create_message_header(m, MSG_TYPE_MOVE, size, length);
 
	int i = S_HEADER_SIZE;
	while(!moves->empty()){
		BinMove* b = moves->top();
		moves->pop();
		m[i++] = b->startX;
		m[i++] = b->startY;
		m[i++] = b->endX;
		m[i++] = b->endY;
		delete(b);
	}

	return m;
}


//-------------------- PLACEMENT MESSAGE ----------------------//
*/




/////////////// STRUCT CONSTRUCTION /////////////////////////////


Error* create_error(int type){
	Error* error = new Error;
	error->type = type;
	return error;
}





BinMove* create_binmove(int _sx, int _sy, int _ex, int _ey){
	BinMove* b = new BinMove;
	b->startX = _sx;
	b->startY = _sy;
	b->endX = _ex;
	b->endY = _ey;
	return b;
}




//////////////// PRINT //////////////////////////////////////////
/*
void print_message(int* m){
	for(int i = 0; i < S_HEADER_SIZE; ++i){
		//printf("%s %d\n", HEADER_SECTIONS[i], m[i]);
	}

	int unitSize = m[S_UNIT];
	int size = m[S_HEADER_SIZE]*unitSize;
	for(int i = S_HEADER_SIZE; i < size;){
		for(int j = 0; j < unitSize; ++j, ++i){
			printf("%d ", m[i]);
		}
		printf("\n");
	}
}
*/

void print_move_message(MoveMsg* m){
	printf("Move Message: \n");
	for(int i = 0; i < m->size; ++i){
		print_move(&((m->msg)[i]));
	}
}


void print_move(BinMove* b){
	printf("%d ", b->startX);
	printf("%d ", b->startY);
	printf("%d ", b->endX);
	printf("%d ", b->endY);
	printf("\n");
}

void print_message(int* m, int size){
	for(int i = 0; i < size; ++i){
		printf("%d ", m[i]);
	}
	printf("\n");
}




/////////////// TEST CONVERSIONS ////////////////////////////////

/*
MoveMsg* convert_move_message(int* m){
	MoveMsg* msg = new MoveMsg();
	int size = m[S_HEADER_SIZE];
	msg->size = size;
	msg->msg = new BinMove[size];
	for(int i = 0; i < size; ++i){
		convert_bin_move((msg->msg)+i, m+S_HEADER_SIZE+(i*sizeof(BinMove)));
	}
	delete(m);
	return msg;
}



void convert_bin_move(BinMove* b, int* m){
	int i = 0;
	b->startX = m[i++];
	b->startY = m[i++];
	b->endX = m[i++];
	b->endY = m[i++];
}
*/


