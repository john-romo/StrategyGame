// server.cpp


#include "server.h"


//std::atomic<bool>* killFlag;

bool* killFlagb;
std::binary_semaphore sem_killFlag{1};


void init_killFlag(){
	int prot = PROT_READ | PROT_WRITE;
	int opts = MAP_SHARED | MAP_ANONYMOUS;
	//killFlag = (std::atomic<bool>*) mmap(NULL, sizeof(std::atomic<bool>), prot, opts, -1, 0);
	//*killFlag = { false };
	killFlagb = (bool*) mmap(NULL, sizeof(bool), prot, opts, -1, 0);
	*killFlagb = false;
}

bool check_killFlag(){
	sem_killFlag.acquire();
	bool r = *killFlagb; 
 	sem_killFlag.release();
	return r;
}

bool set_killFlag(bool b){
	sem_killFlag.acquire();
	*killFlagb = b;
	sem_killFlag.release();
	return b;
}

typedef struct _counter{
	uint64_t value = 0;
	std::binary_semaphore sem{0};
} Counter;

void inc(Counter* c){
	c->sem.acquire();
	++(c->value);
	c->sem.release();
}

void dec(Counter* c){
	c->sem.acquire();
	--(c->value);
	c->sem.release();
}

Counter* create_counter(){
	int prot = PROT_READ | PROT_WRITE;
	int opts = MAP_SHARED | MAP_ANONYMOUS;
	Counter* c = (Counter*) mmap(NULL, sizeof(Counter), prot, opts, -1, 0);
	c->sem.release();
	return c;
}


int masterProcessId;
uint64_t nextClientId;
uint64_t nextGameId;

int serverSocket;
int tickSocket;

std::unordered_map<uint64_t, Game*>* ongoingGames;
std::binary_semaphore sem_ongoingGames{1};
Counter* runningGamesCounter;

std::unordered_map<uint64_t, Client*>* connectedClients;
std::binary_semaphore sem_connectedClients{1};

std::queue<Client*> waitingClients;
std::binary_semaphore sem_waitingClients{1};

std::thread cleanGames(clean_games);
std::counting_semaphore<SEM_VALUE_MAX> signal_cleanGames{0};

std::thread cleanClients(clean_clients);
std::counting_semaphore<SEM_VALUE_MAX> signal_cleanClients{0};


auto rng = std::default_random_engine {};
Game* game;
Flag* flag;


void signal_handler(int signal){
	if(getpid() == masterProcessId){
		std::cout << "Master process " << masterProcessId << " shuting down...\n";

		//set_flag(*killFlag, true);
		set_killFlag(true);
					
		std::cout << "Shuting down clean_games thread...\n";
		signal_cleanGames.release();
		sem_ongoingGames.release();
		cleanGames.join();

		std::cout << "Shuting down clean_clients thread...\n";
		signal_cleanClients.release();
		sem_connectedClients.release();
		cleanClients.join();

		close(serverSocket);
		close(tickSocket);

		//set_killFlag(true);

		while(runningGamesCounter->value) sec_sleep(0.1);
		//set_flag(*killFlag, false);
		set_killFlag(false);
		sec_sleep(0.1);
		munmap(killFlagb, sizeof(bool));
		munmap(runningGamesCounter, sizeof(Counter));
		std::cout << "killFlag deleted.\n";

	
		//munmap(killFlag, sizeof(std::atomic<bool>));



		std::cout << "Master process shutdown.\n";
		exit(signal);	
	}
}


void alloc(){
	init_killFlag();
	runningGamesCounter = create_counter(); 
	ongoingGames = new std::unordered_map<uint64_t, Game*>;
	connectedClients = new std::unordered_map<uint64_t, Client*>;
	start_listen(&serverSocket, PORT);
	start_listen(&tickSocket, TICK_PORT);
}


int main(){
	std::signal(SIGINT, signal_handler);
	std::signal(SIGTERM, signal_handler);
	std::signal(SIGHUP, signal_handler);
	masterProcessId = getpid();

	alloc();

	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);


	while(1){
		Client* client = get_connection(&addr, &len);
		get_client_request(client);
	}
	return 0;
}


Client* get_connection(sockaddr_in* addr, socklen_t* len){
	std::cout << "Ready for connections...\n";
	int clientSocket = accept(serverSocket, (struct sockaddr*) addr, len);
	if(clientSocket == -1){
		std::cout << "ERROR: Connection failed\n"; 
		return NULL;
	}
	int clientTickSocket = accept(tickSocket, (struct sockaddr*) addr, len);
	if(clientSocket == -1){
		std::cout << "ERROR: Connection failed\n"; 
		return NULL;
	}
	return create_client(clientSocket, clientTickSocket);
}


void get_client_request(Client* client){
	char c;
	if(!smart_read(client->socket, &c, 1)){ // read client request
		std::cout << "Request cancelled -> client disconnected\n";
		set_client_connection_status(client, DEAD);
		return;
	}

	sem_connectedClients.acquire();
	connectedClients->insert({client->id, client});
	sem_connectedClients.release();

	switch(c){
		case 'n':
			process_new_game_request(client);			
			return;
		case 'r':
			return;
	}
}

void process_new_game_request(Client* client){
	std::cout << "Client #" << client->id << " request: New Game\n";
	if(!smart_write(client->socket, (char*) &(client->id), sizeof(uint64_t))){  // write client ID
		std::cout << "New Game request cancelled -> client disconnected\n";
		set_client_connection_status(client, DEAD);
		return;
	}

	waitingClients.push(client);
	if(waitingClients.size() >= NUM_COLORS) new_game();
}


///////////////////// INITIALZIE GAME ////////////////////////////////////////////////////////////////////

void new_game(){
	std::cout << "Starting new game...\n";
	Game* newgame = create_game();

	sem_ongoingGames.acquire();
	ongoingGames->insert({newgame->id, newgame});
	sem_ongoingGames.release();

	for(int i = 0; i < NUM_COLORS; ++i){
		newgame->clients[i] = waitingClients.front();
		newgame->clients[i]->gameId = newgame->id;
		waitingClients.pop();
	}
	if(!init_game(newgame)){
		set_game_status(newgame, DEAD);
		for(int i = 0; i < NUM_COLORS; ++i){
			if(check_client_connection_status(newgame->clients[i]) != CONNECTED) continue;
			waitingClients.push(newgame->clients[i]);
		}
		return;
	}
	inc(runningGamesCounter);
	if(!fork()){
		game = newgame;
		run_game();
	}
	return;
}


bool init_game(Game* newgame){
	int i = 0;
	char ch = 'a';
	for(int i = 0; i < NUM_COLORS; ++i){
		Client* client = newgame->clients[i];
		if(!smart_write(client->socket, (char*)&i, sizeof(int)) // write client color
				|| !smart_write(client->socket, (char*)&(newgame->id), sizeof(uint64_t))){ // write client game id
			set_client_connection_status(client, DEAD);
			std::cout << "Client #" << client->id << " disconnected\n";
			ch = 'q';
		}
	}
	for(int i = 0; i < NUM_COLORS; ++i){
		Client* client = newgame->clients[i];
		if(!smart_write(client->socket, &ch, 1)) // write client game go-ahead
			set_client_connection_status(client, DEAD);	
	}
	return (ch == 'a');
}

///////////////////// RUN GAME ///////////////////////////////////////////////////////////////////////////

void delete_junk(){
	delete ongoingGames;
	delete connectedClients;
	cleanGames.join();
	cleanClients.join();
}


void run_game(){	
	std::cout << "Game #" << game->id << " started\n";
	
	delete_junk();
	flag = create_flag();
	std::thread tickThread(tick);

	while(1){
		int exitCode = game_status_check();
		if(exitCode > -1){
			std::cout << "Game #" << game->id << " shuting down...\n";
			delete_flag(flag);
			tickThread.join();
			dec(runningGamesCounter);
			do{ sec_sleep(0.1); }
			while(check_killFlag());
			//while(check_flag(*killFlag));
			sec_sleep(2.0);
			std::cout << "Game #" << game->id << " shutdown.\n";
			exit(exitCode);
		}

		sec_sleep(1.001);
	}
}


int game_status_check(){
	if(check_killFlag()){
	//if(check_flag(*killFlag)){
		set_game_status(game, DEAD);
		return SIGINT;
	}
	if(check_flag(flag->shutdown)){
		set_game_status(game, DEAD);
		return 0;
	}
	if(check_flag(flag->disconnect)){
		set_game_status(game, DISCONNECTED);
		return 1;	
	}
	return -1;
}


///////////////////// TICK ///////////////////////////////////////////////////////////////////////////////

void tick(){
	std::cout << "Game #" << game->id << " tick started.\n";
	while(1){
		//bool dead = check_flag(*killFlag) || check_flag(flag->shutdown);
		bool dead = check_killFlag() || check_flag(flag->shutdown);
		bool connected = !check_flag(flag->disconnect);
		bool check;

		char ch;
		for(int i = 0; i < NUM_COLORS; ++i){
			check = smart_read(game->clients[i]->tickSocket, &ch, 1);
			if(!check) set_client_connection_status(game->clients[i], DISCONNECTED);
			connected = connected && check;
		}

		if(dead) ch = 'x'; 
		else if(!connected) ch = 'd';
		else ch = check_flag(flag->movementPhase) ? 'u' : 'a';		
		
		for(int i = 0; i < game->clients.size(); ++i){
			check = smart_write(game->clients[i]->tickSocket, &ch, 1);
			if(!check) set_client_connection_status(game->clients[i], DISCONNECTED);
			connected = connected && check;
		}
		
		if(dead){
			break;
		}
		if(!connected){
			set_flag(flag->disconnect, true);
			break;
		}
	
		sec_sleep(0.5);
	}
	std::cout << "Game #" << game->id << " tick thread shutdown.\n";
}


void update(){
	std::cout << "Update: " << check_flag(flag->update) << "\n";
	set_flag(flag->update, false);
}




///////////////////// GAME ///////////////////////////////////////////////////////////////////////////////

Game* create_game(){
	int prot = PROT_READ | PROT_WRITE;
	int opts = MAP_SHARED | MAP_ANONYMOUS;
	Game* g = (Game*) mmap(NULL, sizeof(Game), prot, opts, -1, 0);
	g->id = ++nextGameId;
	return g;
}

void set_game_status(Game* g, int status){
	g->status.store(status, std::memory_order_release);
	if(status == DEAD) signal_cleanGames.release();
}

int check_game_status(Game* g){
	return g->status.load(std::memory_order_acquire);
}

void delete_game(Game* g){
	munmap(g, sizeof(Game));
}


///////////////////// CLIENT /////////////////////////////////////////////////////////////////////////////

Client* create_client(int sock, int ticksock){
	int prot = PROT_READ | PROT_WRITE;
	int opts = MAP_SHARED | MAP_ANONYMOUS;
	Client* c = (Client*) mmap(NULL, sizeof(Client), prot, opts, -1, 0);
	c->id = ++nextClientId;
	c->socket = create_smartsock(sock);
	c->tickSocket = create_smartsock(ticksock);
	return c;
}


void set_client_connection_status(Client* c, int status){
	c->connectionStatus.store(status, std::memory_order_release);
	if(status == DEAD) signal_cleanClients.release();
}

int check_client_connection_status(Client* c){
	return c->connectionStatus.load(std::memory_order_acquire);
}

void set_client_victory_status(Client* c, int status){
	c->victoryStatus.store(status, std::memory_order_release);
}

int check_client_victory_status(Client* c){
	return c->victoryStatus.load(std::memory_order_acquire);
}

void delete_client(Client* c){
	delete_smartsock(c->socket);
	delete_smartsock(c->tickSocket);
	munmap(c, sizeof(Client));
}


///////////////////// SMART SOCK /////////////////////////////////////////////////////////////////////////

Smartsock* create_smartsock(int sock){
	int prot = PROT_READ | PROT_WRITE;
	int opts = MAP_SHARED | MAP_ANONYMOUS;
	Smartsock* s = (Smartsock*) mmap(NULL, sizeof(Smartsock), prot, opts, -1, 0);
	int fs = fcntl(sock, F_GETFL);
	fcntl(sock, F_SETFL, fs | O_NONBLOCK);
	s->socket = sock;
	s->sockpoll[0].fd = sock;
	s->sockpoll[0].events = POLLIN | POLLOUT | POLLRDHUP;
	return s;
}

bool check_smartsock(Smartsock* s){
	int ret = poll(s->sockpoll, 1, 5000);
	if(!ret) return false;
	if(s->sockpoll[0].revents & (POLLHUP | POLLRDHUP | POLLERR | POLLNVAL))
		return false;
	return true;
}

void delete_smartsock(Smartsock* s){
	close(s->socket);
	munmap(s, sizeof(Smartsock));	
}


bool smart_read(Smartsock* sock, char* buff, int len){
	sock->sockpoll[0].events = POLLIN | POLLRDHUP | POLLNVAL;
	ssize_t bytes;
	if(!check_smartsock(sock)) return false;
	while(len > 0){
		bytes = read(sock->socket, buff, len);
		if(bytes < 0) return false;
		buff += bytes;
		len -= bytes;
	}
	return true;
}

bool smart_write(Smartsock* sock, char* buff, int len){
	sock->sockpoll[0].events = POLLOUT | POLLRDHUP | POLLNVAL;
	ssize_t bytes;
	if(!check_smartsock(sock)) return false;
	while(len > 0){
		bytes = write(sock->socket, buff, len);
		if(bytes < 0) return false;
		buff += bytes;
		len -= bytes;
	}
	return true;
}
///////////////////// FLAG ///////////////////////////////////////////////////////////////////////////////

Flag* create_flag(){
	int prot = PROT_READ | PROT_WRITE;
	int opts = MAP_SHARED | MAP_ANONYMOUS;
	Flag* f = (Flag*) mmap(NULL, sizeof(Flag), prot, opts, -1, 0);
	return f;
}

bool check_flag(std::atomic<bool>& flag){
	return flag.load(std::memory_order_acquire);
}

void set_flag(std::atomic<bool>& flag, bool setval){
	flag.store(setval, std::memory_order_release);
}

void delete_flag(Flag* f){
	munmap(f, sizeof(Flag));
}


///////////////////// SOCKETS ////////////////////////////////////////////////////////////////////////////

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
		std::cerr <<  "Bind failed " << errno << " " << strerror(errno) << "\n";
	return(s);
}

void start_listen(int* sock, int port){
	*sock = get_server_socket(port);
	if(listen(*sock, 128)) std::cerr << "ERROR: Listen failed\n";
}


///////////////////// IO /////////////////////////////////////////////////////////////////////////////////




///////////////////// CONTAINERS /////////////////////////////////////////////////////////////////////////


Game* find_game(uint64_t id){
	Game* g;
	sem_ongoingGames.acquire();
	try{
		g = ongoingGames->at(id);
	}
	catch(...){
		sem_ongoingGames.release();
		return NULL;
	}
	sem_ongoingGames.release();
	return g;
}


Client* find_client(uint64_t id){
	Client* c;
	sem_connectedClients.acquire();
	try{
		c = connectedClients->at(id);	
	}
	catch(...){
		sem_connectedClients.release();
		return NULL;
	}
	sem_connectedClients.release();
	return c;
}




///////////////////// GARBAGE ////////////////////////////////////////////////////////////////////////////

void clean_games(){
	while(1){
		signal_cleanGames.acquire();
		//if(check_flag(*killFlag)) break;
		if(check_killFlag()) break;
		std::cout << "Deleting dead games...\n";
		sem_ongoingGames.acquire();
		std::unordered_map<uint64_t, Game*>* temp = new std::unordered_map<uint64_t, Game*>;
		for (const auto& [id, g] : *ongoingGames) {
			if(check_game_status(g) == DEAD) delete_game(g);
			else temp->insert({id, g});
		}
		delete ongoingGames;
		ongoingGames = temp;
		sem_ongoingGames.release();
	}
	sem_ongoingGames.acquire();
	for (const auto& [id, g] : *ongoingGames) delete_game(g);
	delete(ongoingGames);
	std::cout << "Clean games thread shutdown\n";
}

void clean_clients(){
	while(1){
		signal_cleanClients.acquire();	
		//if(check_flag(*killFlag)) break;
		if(check_killFlag()) break;
		std::cout << "Deleting dead clients...\n";
		sem_connectedClients.acquire();
		std::unordered_map<uint64_t, Client*>* temp = new std::unordered_map<uint64_t, Client*>;
		for (const auto& [id, c] : *connectedClients) {
			if(check_client_connection_status(c) == DEAD) delete_client(c);
			else temp->insert({id, c});
		}
		delete connectedClients;
		connectedClients = temp;
		sem_connectedClients.release();
	}
	sem_connectedClients.acquire();
	for (const auto& [id, c] : *connectedClients) delete_client(c);
	delete(connectedClients);
	std::cout << "Clean clients thread shutdown.\n";
}



///////////////////// UTILS //////////////////////////////////////////////////////////////////////////////

void sec_sleep(float period){
	struct timespec t1, t2;
	t1.tv_sec = (period >= 1) ? (int) period  : 0;
	t1.tv_nsec = (unsigned long long) ((period - ((int) period))*1000000000ull);
	nanosleep(&t1, &t2);
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////


































/*
int* read_message(int sock){
	char csize[sizeof(int)];
	smart_read(sock, csize, sizeof(int));
	int size = *((int*) csize);
	char* msg = new char[size*sizeof(int)]();
	((int*)msg)[S_SIZE] = size;
	smart_read(sock, msg+sizeof(int), (size-1)*sizeof(int));
	return (int*) msg; 
}


void write_message(int sock, int* msg){
	smart_write(sock, (char*) msg, sizeof(int));
	smart_write(sock, (char*) (msg+1), (msg[S_SIZE]-1)*sizeof(int));
	delete msg;
}
*/



















/*
void game(){
	std::cout << "Game " << getpid() << " started.\n";
	int epollfd;
	int readyfds;
	struct epoll_event event;
	struct epoll_event eventlist[NUM_COLORS];
	init_epoll(&epollfd, &event);
	flags = init_flags();

	write_game_ready();
	start_tick();
	placement_phase();

	while(!check_flag(flags->shutdown)){
		if(check_flag(flags->kill)) game_killed();
		if(check_flag(flags->update)) update();
		if(check_flag(flags->disconnect)){
			sec_sleep(0.1);
			continue;
		}

		readyfds = epoll_wait(epollfd, eventlist, NUM_COLORS, -1);
		if(readyfds < 0) continue;

		for(int i = 0; i < readyfds; ++i){
			int* msg = read_message(eventlist[i].data.fd);
			process_message(msg);

			for(Client* c : clients){
				if(eventlist[i].data.fd == c->socket){
					int* msg = read_message(c->socket);
					process_message(msg);
				}
			}
		}
	}
	shutdown();
}


void shutdown(){
	free_alloced();
	std::cout << "Game " << getpid() << " shutdown.\n";
	exit(0);
}


void game_killed(){
	free_alloced();
	std::cout << "Game " << getpid() << " killed by SIGINT.\n";
	exit(SIGINT);
}


/////////////////////////// GAME PHASES ////////////////////////////

void placement_phase(){
	int white = clients.at(WHITE)->socket;
	int black = clients.at(BLACK)->socket;
	int* wMsg = read_message(white);
	int* bMsg = read_message(black);
	process_message(bMsg);
	process_message(wMsg);
}


/////////////////////////// PROCESS MESSAGES ///////////////////////

void process_message(int* msg){
	switch(msg[C_TYPE]){
	 	case MSG_TYPE_INITIAL_PLACEMENT:
			process_initial_placement_message(msg);
			return;
		default:
			return;
	}
}


void process_initial_placement_message(int* msg){
	write_message(clients[!(msg[C_COLOR])]->socket, msg);
}


/////////////////////////// WRITE MESSAGES /////////////////////////

void write_response(Client* c, int errorType){		
	int r[S_RESPONSE_SIZE];
	r[S_TYPE] = MSG_TYPE_RESPONSE;
	r[S_SIZE] = S_RESPONSE_SIZE;
	r[S_ERROR_TYPE] = errorType;
	write_message(c->socket, r);
}


void write_game_ready(){
	for(Client* c : clients){
		int* msg = new int[S_STARTGAME_SIZE]();
		msg[S_SIZE] = S_STARTGAME_SIZE;
		msg[S_TYPE] = MSG_TYPE_STARTGAME;
		msg[S_CLIENT_COLOR] = c->color;
		write_message(c->socket, msg);
	}
}


/////////////////////////// TICK ///////////////////////////////////





/////////////////////////// IO /////////////////////////////////////

void simple_read(int sock, int size){
	
}




/////////////////////////// INITS //////////////////////////////////




void init_game(Client* client){
	std::cout << client->addr.sin_port << " CONNECTED\n";
	waitingClients.push(client);
	if(waitingClients.size() >= 2){

		

		clients.push_back(waitingClients.front());
		waitingClients.pop();
		clients.push_back(waitingClients.front());
		waitingClients.pop();
		int pid;
		if(!(pid = fork())){
			game();
		}
		clients.clear();
	}	
}


void init_epoll(int* epollfd, struct epoll_event* event){
	*epollfd = epoll_create1(0);
    if (0 > *epollfd){
		std::cerr << "epoll_create1() failed\n";	
		exit(1);
	}
	event->events = EPOLLIN | EPOLLRDHUP;
	int i = 0;
	for(Client* c : clients){
		c->color = i++;
		event->data.fd = c->socket;
		if(epoll_ctl(*epollfd, EPOLL_CTL_ADD, c->socket, event)){
			std::cerr << "epoll_ctl() failed\n";
			exit(1);
		}
	}
}








///////////////////// UTILS /////////////////////////////////////







void print_message(int* m, int size){
	for(int i = 0; i < size; ++i){
		printf("%d ", m[i]);
	}
	printf("\n");
}





void print_flags(){
	std::cout << "falgs: " << flags->disconnect << " " 
		<< flags->shutdown << " " << flags->update << "\n";
}

*/




