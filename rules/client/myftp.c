// myftp.c

#include "myftp.h"


int main(int argc, char** argv){
	if((!argv[1]) || (!strcmp(argv[1], "-h")) || (!strcmp(argv[1], "--help"))){
		help();
		return(0);
	}
	client(argv[1]);
	return(0);
}


void client(char* address){
	printf("\n >> ");
	fflush(stdout);

	struct sockaddr_in addr = get_socket_addr(PORT);
	int sock = get_client_socket(address);
	if(connect(sock, (struct sockaddr*) &addr, sizeof(struct sockaddr_in))< 0)
		error(0, "Connection failed");
		
	char* buff = calloc(BUFF_SIZE, sizeof(char));
	
	while(read(0, buff, BUFF_SIZE) > 0){
		if(buff[0] == '\n') goto bottom;

		char* command = parse(buff);
		free(buff);
		buff = calloc(BUFF_SIZE, sizeof(char));
		
		switch(command[0]){
			case 'e':
				printf("    Invalid command\n");
			case 'h':
				help();
				break;
			case 'Q':
				quit(buff, command, sock);
				return;
			case 'c':
				cd(command);
				break;
			case 'C':
				rcd(command, sock);
				break;
			case 'l':
				ls();	
				break;
			case 'L':
				rls(address, sock);
				break;
			case 'G':
				get(command, address, sock);
				break;
			case 'P':
				put(command, address, sock);
				break;
			case 'S':  
				show(command, address, sock);
				break;
		}

		free(command);
		bottom:
		printf("\n >> ");
		fflush(stdout);
	}	
	if(buff) free(buff);
	error(0, "Read failed");
}



///////////////////////////////////////////////////////////////////////////////
///////////////////// Commands ////////////////////////////////////////////////

void quit(char* buff, char* command, int sock){
	issue_command(command, sock);
	printf("    Quitting...\n\n");
	free(command);
	free(buff);
	close(sock);
}


void cd(char* path){
	path[strlen(path)-1] = '\0';
	if(!chdir(path+1)) printf("    Changed Working Directory: %s\n", path+1);
	else fprintf(stderr, "    Failed to change working directory: %s.\n", strerror(errno));
}


void rcd(char* path, int sock){
	issue_command(path, sock);
	path[strlen(path)-1] = '\0';
	if(!read_server_reply(sock)) return;
	else printf("    Changed Server Working Directory: %s\n", path+1);
}


void ls(){
	printf("    Client LS:\n\n");
	int child, grandchild, p[2];
	if(!(child = fork())){
		pipe(p);
		if(!(grandchild = fork())){
			close(p[0]);
			dup2(p[1], 1);
			close(p[1]);
			execlp("ls", "ls", "-l", NULL);
		}
		else{
			close(p[1]);
			dup2(p[0], 0);
			close(p[0]);
			execlp("more", "more", "-n 20", NULL);
		}
	} 
	waitpid(grandchild, NULL, 0);
	waitpid(child, NULL, 0);
}


void rls(char* address, int sock){
	printf("    Server LS:\n\n");
	more("L\n", address, sock);
}


void get(char* path, char* address, int sock){
	printf("    Getting %s", path+1);
	
	int dataSock = client_data_socket(address, sock);
	
	issue_command(path, sock);
	if(!read_server_reply(sock)){
		path[strlen(path)-1] = '\0';
		remove(path+1);
		close(dataSock);
		return;
	}
	
	char c;
	int r;
	int i = 0;
	size_t size = BUFF_SIZE;
	char* filebuff = calloc(size, sizeof(char));

	while(r = read(dataSock, &c, 1)){
		if(r == -1) error(0, "Read failed");
		filebuff[i++] = c;
		if(i == size){
			size *= 2;
			filebuff = realloc(filebuff, size);
		}
	}

	int fd = create_fd(path, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	if(fd == -1){
		fprintf(stderr, "    File could not be created: %s\n", strerror(errno));
		return;
	}
	if(write(fd, filebuff, strlen(filebuff)) < 0) error(0, "Write failed");

	close(fd);
	close(dataSock);
}


void put(char* path, char* address, int sock){
	printf("    Transfering %s", path+1);
	
	int fd = create_fd(path, O_RDONLY, 0);
	if(fd == -1) return;

	struct stat fileStat;
	fstat(fd, &fileStat);
	if(!S_ISREG(fileStat.st_mode)){
		fprintf(stderr, "    Error: File is not regular.\n");
		return;
	}

	int dataSock = client_data_socket(address, sock);

	issue_command(path, sock);
	if(!read_server_reply(sock)) return;
	
	int i = 0;
	int size = fileStat.st_size;
	char c, fileBuff[size];
	while(i < size){
		if(read(fd, &c, 1) == -1) error(0, "Read failed");
		fileBuff[i++] = c;
	}

	if(write(dataSock, fileBuff, size) == -1) error(0, "Write failed");
	close(fd);
	close(dataSock);
}


void show(char* path, char* address, int sock){
	path[0] = 'G';
	int len = strlen(path)-1;
	path[len] = '\0'; 
	printf("    Showing %s:\n", path+1);
	path[len] = '\n';
	more(path, address, sock);
}


void more(char* command, char* address, int sock){
	int dataSock = client_data_socket(address, sock);

	issue_command(command, sock);
	if(!read_server_reply(sock)) return;

	int child;
	if(!(child = fork())){
		dup2(dataSock, 0);
		execlp("more", "more", "-n 20", NULL);
	}
	waitpid(child, NULL, 0);	
	close(dataSock);
}


void help(){
	printf("\nProgram Argument: server address, eg: 127.0.0.1\n");
	printf("Commands:\n");
	printf("\t'exit': exit program\n");
	printf("\t'cd <filename>': change working directory\n");
	printf("\t'rcd <filename>': change server working directory\n");
	printf("\t'ls': list working directory contents\n");
	printf("\t'rls': list server working directory contents\n");
	printf("\t'get <filename>': get filename from server\n");
	printf("\t'put <filename>': send file to server\n");
	printf("\t'show <filename>': show contents of server filename\n");
}



///////////////////////////////////////////////////////////////////////////////
///////////////////// Parce Commands //////////////////////////////////////////

char* parse(char* buff){
	int spaces = 0;
	while(1){
		if(isspace(buff[spaces])) ++spaces;
		else break;
	}
	buff = buff+spaces;

	char* cmd = strtok_r(buff, " ", &buff);
	char* command = calloc(BUFF_SIZE, sizeof(char));
    
	if(!strcmp(cmd, "cd")) set_command('c', strtok_r(buff, " ", &buff), command);
	else if(!strcmp(cmd, "rcd"))	set_command('C', strtok_r(buff, " ", &buff), command);
	else if(!strcmp(cmd, "rls\n")) set_command('L', "\n", command);
	else if(!strcmp(cmd, "ls\n")) command[0] = 'l';
	else if(!strcmp(cmd, "get"))	set_command('G', strtok_r(buff, " ", &buff), command);
	else if(!strcmp(cmd, "put"))	set_command('P', strtok_r(buff, " ", &buff), command);
	else if(!strcmp(cmd, "show"))	set_command('S', strtok_r(buff, " ", &buff), command);
	else if(!strcmp(cmd, "exit\n"))	set_command('Q', "\n", command);
	else if(!strcmp(cmd, "help\n")) command[0] = 'h';
	else command[0] = 'e';
	
	return(command);

}


void set_command(char c, char* s, char* command){
	command[0] = c;
	memcpy(command+1, s, strlen(s));
}


void issue_command(char* command, int sock){
	if(write(sock, command, strlen(command)) < 0)
		error(0, "Write failed");
}


char read_server_reply(int sock){
	char buff[BUFF_SIZE] = {0};
	char c;
	int i = 0;
	while(read(sock, &c, 1)){
		buff[i++] = c;
		if(c == '\n') break;
	}
	if(buff[0] == 'E'){
		fprintf(stderr, "    %s", buff);
		return(0);
	}
	return(1);
}



///////////////////////////////////////////////////////////////////////////////
///////////////////// Creations ///////////////////////////////////////////////

// from assignment 8
struct sockaddr_in get_socket_addr(int port){
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	return(addr);
}


// from assignment 8
int get_client_socket(char* address){
	struct addrinfo info, *data;
	memset(&info, 0, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;
	
	int err = getaddrinfo(address, NULL, &info, &data);
	if(err) error(err, NULL);
	
	return(socket(data->ai_family, data->ai_socktype, 0));
}


int client_data_socket(char* address, int sock){
	issue_command("D\n", sock);

	char buff[BUFF_SIZE] = {0};
	int i = 0;
	char c;
	int r;
	while(r = read(sock, &c, 1)){
		if(r == -1) error(0, "Read failed");
		buff[i++] = c;
		if(c == '\n') break;
	}

	if(buff[0] == 'E'){
		fprintf(stderr, "%s", buff);
		return(0);
	}

	buff[strlen(buff)-1] = '\0';
	int dataPort = (int)strtol(buff+1, NULL, 10);

	struct sockaddr_in addr = get_socket_addr(dataPort);
	int dataSock = get_client_socket(address);
	if(connect(dataSock, (struct sockaddr*) &addr, sizeof(struct sockaddr_in))< 0)
		error(0, "Connection failed");
	
	return(dataSock);
}


int create_fd(char* path, int opts, int perms){
	int len = strlen(path)-1;
	path[len] = '\0';
 	int fd = open(path+1, opts, perms);
	if(fd == -1){
		fprintf(stderr, "    Error: File failed to open.\n");
		return(-1);
	}
	path[len] = '\n';
	return(fd);
}



///////////////////////////////////////////////////////////////////////////////
///////////////////// Error ///////////////////////////////////////////////////

void error(int err, char* e){
	if(err) fprintf(stderr, "    Error: %s\n", gai_strerror(err));
	fprintf(stderr, "    %s: %s\n", e, strerror(errno));
	exit(errno);
}
