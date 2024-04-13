// client.cpp


#include "client.h"

uint64_t id;
uint64_t gameId;

Flag* flags;
int sock = get_client_socket();
int ticksock = get_client_socket();
int tickProcessId;


void sigint_handler(int signal){
	std::cout << getpid() << " killed by SIGINT.\n";
	close(sock);
	close(ticksock);
	exit(SIGINT);
}


// int main(){
// 	std::signal(SIGINT, sigint_handler);

// 	struct sockaddr_in addr = get_socket_addr(PORT);
// 	while(connect(sock, (struct sockaddr*) &addr, sizeof(struct sockaddr_in))< 0){
// 		std::cout << "Trying to connect...\n";
// 		sleep(1);
// 	}
	
// 	struct sockaddr_in tickaddr = get_socket_addr(TICK_PORT);
// 	while(connect(ticksock, (struct sockaddr*) &tickaddr, sizeof(struct sockaddr_in))< 0){
// 		std::cout << "Trying to connect to Tick Server...\n";
// 		sleep(1);
// 	}

// 	char c = 'n';
// 	write(sock, &c, 1);
// 	read(sock, &id, sizeof(uint64_t));
// 	//printf("client ids: %llu\n", id);
	
// 	std::cout << "Waiting for opponents to join...\n";
// 	fflush(stdout);

// 	int color;
// 	while(c != 'a'){
// 		read(sock, &color, sizeof(int));
// 		read(sock, &gameId, sizeof(uint64_t));
// 		read(sock, &c, 1);
// 		if(c != 'a') sec_sleep(0.1);
// 	}

// 	//game(color);
// }

int join_game(SDL_Renderer* renderer){
	std::signal(SIGINT, sigint_handler);

	struct sockaddr_in addr = get_socket_addr(PORT);
	while(connect(sock, (struct sockaddr*) &addr, sizeof(struct sockaddr_in))< 0){
		std::cout << "Trying to connect...\n";
		sleep(1);
	}
	
	struct sockaddr_in tickaddr = get_socket_addr(TICK_PORT);
	while(connect(ticksock, (struct sockaddr*) &tickaddr, sizeof(struct sockaddr_in))< 0){
		std::cout << "Trying to connect to Tick Server...\n";
		sleep(1);
	}

	char c = 'n';
	write(sock, &c, 1);
	read(sock, &id, sizeof(uint64_t));
	//printf("client ids: %llu\n", id);
	
	std::cout << "Waiting for opponents to join...\n";
	fflush(stdout);

	int color;
	while(c != 'a'){
		read(sock, &color, sizeof(int));
		read(sock, &gameId, sizeof(uint64_t));
		read(sock, &c, 1);
		if(c != 'a') sec_sleep(0.1);
	}
	game(color, renderer);
	printf("its done\n");
	return 0;
}



void game(int color, SDL_Renderer* renderer){
	std::cout << "Starting game...\n"; 
	std::cout << "id: " << id << " Color: " << color << "\n";
	start_game(color, renderer);
	std::cout << "Game started.\n";
	flags = init_flags();
	std::cout << "Flags initialized.\n";
	tick();
	std::cout << "Tick started.\n";



	//placement_phase();
	//print_board();

	// while(!check_flag(flags->shutdown)){
	// 	if(check_flag(flags->disconnect)){
	// 		sec_sleep(0.1);
	// 		continue;
	// 	}
	// 	sec_sleep(1.0);
	// }
	
	// end_game();
	// std::cout << "Client process shutdown\n";
}


/////////////////////////// PHASES /////////////////////////////////

void placement_phase(){
	//default_placement();

	int* m = create_initial_placement_message();

	write_message(m);

	int* msg = read_message();
	process_message(msg);
}


/////////////////////////// PROCESS MESSAGE ////////////////////////

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
	int color = msg[C_COLOR];
	int size = C_INITIAL_PLACEMENT_SIZE;
	int startpos = C_INITIAL_PLACEMENT_BODY;
	int i = 0;
	
	while(1){
		int pos = startpos + (i*C_IPP_SIZE);
		if(pos >= size-1) break;
		Piece* p = create_piece(msg[pos+C_IPP_TYPE], !player->color);
		place_piece(p, msg[pos+C_IPP_X], msg[pos+C_IPP_Y]);
		++i;
	}
}


/////////////////////////// CREATE MESSAGE /////////////////////////

int* create_initial_placement_message(){
	int* msg = new int[C_INITIAL_PLACEMENT_SIZE];
	msg[C_SIZE] = C_INITIAL_PLACEMENT_SIZE;
	msg[C_TYPE] = MSG_TYPE_INITIAL_PLACEMENT;
	msg[C_COLOR] = player->color;
	int startpos = C_INITIAL_PLACEMENT_BODY;

	int i = 0;
	for(Piece* p : pieces[player->color]){
		if(p->type == PARATROOPER) continue;
		int pos = startpos + (i*C_IPP_SIZE);
		msg[pos+C_IPP_TYPE] = p->type;
		msg[pos+C_IPP_X] = p->x;
		msg[pos+C_IPP_Y] = p->y;
		++i;
	}
	return msg;
}


/////////////////////////// TICK ///////////////////////////////////

void tick(){
	int pid;
	if(!(pid = fork())){
		while(1){
			if(check_flag(flags->shutdown)){
				std::cout << "Tick process shutdown.\n";
				exit(0);
			}

			char ch = player->color;
			
			write(ticksock, &ch, 1);
			read(ticksock, &ch, 1);
			printf("tick: %d\n", ch);
			process_tick(ch);
		}		
	}
	tickProcessId = pid;
}


void process_tick(char tick){
	switch(tick){
		case 'x':
			std::cout << "Tick Process shutdown -> Server disconnected.\n";
			set_flag(flags->shutdown, true);
			exit(0);
		case 'a':
			set_flag(flags->disconnect, false);
			break;
		default:
			std::cout << "Opponent disconnected. " << (int)tick << " seconds to reconnect...\n" ;
			set_flag(flags->disconnect, true);
		
	}
}


/////////////////////////// IO /////////////////////////////////////

int* read_message(){
	char csize[sizeof(int)];
	split_read(csize, sizeof(int));
	int size = *((int*) csize);
	char* msg = new char[size*sizeof(int)]();
	((int*)msg)[S_SIZE] = size;
	split_read(msg+sizeof(int), (size-1)*sizeof(int));
	return (int*) msg; 
}


void write_message(int* msg){
	split_write((char*) msg, sizeof(int));
	split_write((char*) (msg+1), (msg[C_SIZE]-1)*sizeof(int));
	delete msg;
}


void split_read(char* m, int len){
	ssize_t bytes;
	while(len > 0){
		bytes = read(sock, m, len);
		if(bytes <= 0) return;
		m += bytes;
		len -= bytes;
	}
}


void split_write(char* m, int len){
	ssize_t bytes;
	while(len > 0){
		bytes = write(sock, m, len);
		if(bytes < 0) return;
		m += bytes;
		len -= bytes;
	}
}


/////////////////////////// INIT ///////////////////////////////////

void start_game(int color, SDL_Renderer* renderer){
	printf("creating players...\n");
	create_players();
	player = players[color];
	printf("creating headings...\n");
	create_headings();
	printf("creating board...\n");
	//create_board(renderer);
	create_board_filled(renderer);
	//create_pieces();
	printf("marking valid tiles...\n");
	mark_valid_tiles(renderer);
	printf("done!\n");
}


void end_game(){
	delete_pieces();
	delete_board();
	delete_players();
}


Flag* init_flags(){
	int prot = PROT_READ | PROT_WRITE;
	int opts = MAP_SHARED | MAP_ANONYMOUS;
	Flag* f = (Flag*) mmap(NULL, sizeof(Flag), prot, opts, -1, 0);
	f->disconnect = false;
	f->shutdown = false;
	f->update = false;
	return f;
}


struct sockaddr_in get_socket_addr(int port){
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ADDRESS);
	return(addr);
}


int get_client_socket(){
	struct addrinfo info, *data;
	memset(&info, 0, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;
	
	int err = getaddrinfo(ADDRESS, NULL, &info, &data);
	
	return(socket(data->ai_family, data->ai_socktype, 0));
}


/////////////////////////// UTIL ///////////////////////////////////

void set_flag(bool& flag, bool setval){
	flags->mutex.lock();
	flag = setval;
	flags->mutex.unlock();
}


bool check_flag(bool flag){
	bool r;
	flags->mutex.lock();
	r = flag;
	flags->mutex.unlock();
	return r;
}


void sec_sleep(float period){
	struct timespec t1, t2;
	t1.tv_sec = (period > 1) ? (int) period  : 0;
	t1.tv_nsec = (unsigned long long) ((period - ((int) period))*1000000000ull);
	nanosleep(&t1, &t2);
}


void print_message(int* m, int size){
	for(int i = 0; i < size; ++i){
		printf("%d ", m[i]);
		fflush(stdout);
	}
	printf("\n");
	fflush(stdout);
}


void print_flags(){
	std::cout << "falgs: " << flags->disconnect << " " 
		<< flags->shutdown << " " << flags->update << "\n";
}








