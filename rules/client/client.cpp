// client.cpp


#include "client.h"

int main(int argc, char** argv){
	client(ADDRESS);
	return(0);
}

struct sockaddr_in get_socket_addr(int port){
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ADDRESS);
	return(addr);
}


int get_client_socket(const char* address){
	struct addrinfo info, *data;
	memset(&info, 0, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;
	
	int err = getaddrinfo(address, NULL, &info, &data);
	
	return(socket(data->ai_family, data->ai_socktype, 0));
}

int playerID;

void client(const char* address){
	struct sockaddr_in addr = get_socket_addr(PORT);
	int sock = get_client_socket(address);
	if(connect(sock, (struct sockaddr*) &addr, sizeof(struct sockaddr_in))< 0)
		std::cout << "Connection failed\n";
		
	
	while(1){
		int* m = create_newgame_message(0, 15, 0, 2);
		write(sock, m, C_PLACE_SIZE*sizeof(int));

		char* msg = read_message(sock);
		print_message((int*) msg, (int) msg[S_SIZE*sizeof(int)]);
		break;
	}


}

char* read_message(int socket){
	int charHSize = S_HEADER_SIZE*sizeof(int);
	int header[S_HEADER_SIZE] = {0};
	read(socket, header, charHSize); 

	int size = header[S_SIZE]; 
	int charBSize = size*sizeof(int);
	int* body[size] = {0};
	read(socket, body, charBSize-charHSize);

	char* msg = new char[size*sizeof(int)];
	memcpy(msg, header, charHSize);
	memcpy(msg+charHSize, body, charBSize-charHSize);

	return(msg);
}




////////////////////////////////////////////////////////////////////

int* create_newgame_message(int color, int width, int pieces, int numplayers){
	int* m = new int[C_NEWGAME_SIZE];
	m[C_COLOR] = color;
	m[C_TYPE] = MSG_TYPE_NEWGAME;
	m[C_SIZE] = C_NEWGAME_SIZE;
	m[C_CANCEL] = 0;
	m[C_NEWGAME_WIDTH] = width;
	m[C_NEWGAME_PIECES] = 0;
	m[C_NEWGAME_NUMPLAYERS] = numplayers;
	return m;
}

int* create_placement_message(int type, int color, int x, int y){
	int* m = new int[C_PLACE_SIZE];

	m[C_COLOR] = color;
	m[C_TYPE] = MSG_TYPE_PLACE;
	m[C_SIZE] = C_PLACE_SIZE;
	m[C_CANCEL] = 0;
	m[C_PLACE_TYPE] = type;
	m[C_PLACE_COLOR] = color;
	m[C_PLACE_X] = x;
	m[C_PLACE_Y] = y;

	return m;
}

int* create_move_message(int color, int x, int y, int headingID){
	int* m = new int[C_MOVE_SIZE];

	m[C_COLOR] = color;
	m[C_TYPE] = MSG_TYPE_MOVE;
	m[C_SIZE] = C_MOVE_SIZE;
	m[C_CANCEL] = 0;
	m[C_MOVE_X] = x;
	m[C_MOVE_Y] = y;
	m[C_MOVE_H] = headingID;

	return m;
}




////////////////////////////////////////////////////////////////////

void print_message(int* m, int size){
	for(int i = 0; i < size; ++i){
		printf("%d ", m[i]);
	}
	printf("\n");
}


////////////////////////////////////////////////////////////////////









