// myftpserve.c

#include "myftp.h"


int main(){
	int control;
	int sock = get_server_socket(PORT);
	struct sockaddr_in clientAddr;
	int length = sizeof(struct sockaddr_in);

	if(listen(sock, BACKLOG)) error(0, "Listen failed");

	printf("Ready for connection...\n");
	fflush(stdout);

	while(1){
		control = accept(sock, (struct sockaddr *) &clientAddr, &length);
		if(control == -1) error(0, "Connection failed"); 
		
		printf("%d CONNECTED\n", clientAddr.sin_port);

		int child;
		if(!(child = fork())){
			if(!fork()){
				server(control, clientAddr.sin_port);		
				exit(0);
			}
			exit(0);
		}
		waitpid(child, NULL, 0);
	}

	return(0);
}


void server(int control, int childPort){
	int dataSock;
	char* buff = calloc(BUFF_SIZE, sizeof(char));

	//while(read(control, buff, BUFF_SIZE) > 0){
	while(1){
		int x = read(control, buff, BUFF_SIZE);
		printf("%d\n", x);
		printf("%s\n", buff);
		fflush(stdout);
		int len = strlen(buff);
		if(!(buff[len-1] == '\n')){
			char c;
			while(1){
				read(control, &c, 1);
				buff[len++] = c;
				if(c == '\n') break;
			}
		}

		printf("%d >> ", childPort);
		fflush(stdout);

		switch(buff[0]){
			case 'Q':
				Q(buff, control);	
				return;
			case 'C':
				C(buff, control);
				break;
			case 'D':
				dataSock = server_data_socket(control);
				break;
			case 'L':
				L(dataSock, control);
				break;
			case 'G':
				G(buff, dataSock, control);
				break;
			case 'P':
				P(buff, dataSock, control);
				break;
		}

		free(buff);
		buff = calloc(BUFF_SIZE, sizeof(char));
	}

	if(buff) free(buff);
	error(0, "Read failed");
}



/////////////////////////////////////////////////////////////////////////////////
////////////////// Commands /////////////////////////////////////////////////////

void Q(char* buff, int control){
	printf("Quitting...\n");
	write(control, "A\n", 2);
	close(control);
	free(buff);
}


void C(char* path, int control){
	path[strlen(path)-1] = '\0';
	if(!chdir(path+1)){
		printf("Changed Working Directory: %s\n", path+1);
		write(control, "A\n", 2);
		return;
	}
	write_error(control, "Error: Failed to change directory.\n");
}


void L(int dataSock, int control){
	printf("Server LS\n");
	write(control, "A\n", 2);
	int child;
	if(!(child = fork())){
		dup2(dataSock, 1);
		execlp("ls", "ls", "-l", NULL);
	}
	waitpid(child, NULL, 0);
	close(dataSock);
}


void G(char* path, int dataSock, int control){
	path[strlen(path)-1] = '\0';

	if(access(path+1, R_OK)){
		write_error(control, "Error: File cannot be opened\n");
		close(dataSock);
		return;
	}

	int fd = open(path+1, O_RDONLY);
	if(fd == -1){
		write_error(control, "Error: File cannot be opened\n");
		close(dataSock);
		return;
	}

	struct stat fileStat;
	fstat(fd, &fileStat);
	if(!S_ISREG(fileStat.st_mode)){
		write_error(control, "Error: File is not regular\n");
		close(fd);
		close(dataSock);
		return;
	}

	printf("Transfering %s\n", path+1);
	write(control, "A\n", 2);

	int size = fileStat.st_size;
	char fileBuff[size];
	int i = 0;
	char c;
	while(i < size){
		if(read(fd, &c, 1) < 0) error(0, "Read failed");
		fileBuff[i++] = c;
	}

	if(write(dataSock, fileBuff, size) == -1) error(0, "Write failed");
	close(fd);
	close(dataSock);
}


void P(char* path, int dataSock, int control){
	path[strlen(path)-1] = '\0';
	int fd = open(path+1, O_CREAT | O_EXCL |  O_RDWR | O_TRUNC, S_IRWXU);
	if(fd == -1){
		write_error(control, "Error: File cannot be created\n");
		return;
	}
	printf("Getting %s\n", path+1);
	write(control, "A\n", 2);

	char c;
	while(read(dataSock, &c, 1) > 0){
		write(fd, &c, 1);
		if(c == EOF) break;
	}
	close(fd);
	close(dataSock);
}



/////////////////////////////////////////////////////////////////////////////////
////////////////// Creations ////////////////////////////////////////////////////

// from assignment 8
struct sockaddr_in get_socket_addr(int port){
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	return(addr);
}

// from assignment 8
int get_server_socket(int port){
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if(s == -1) error(0, "Socket creation failed");
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)))
		error(0, "Set socket options failed");
	struct sockaddr_in servAddr = get_socket_addr(port);
	if (bind(s, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
		error(0, "Bind failed");
	return(s);
}


int server_data_socket(int control){
	printf("Created data socket\n");
	int  s = get_server_socket(0);
	
	struct sockaddr_in addr;
	memset((void *)&addr, 0, sizeof(addr));
	int len = sizeof(addr);
	getsockname(s, (struct sockaddr*) &addr, &len);
	int dataPort = ntohs(addr.sin_port);
	
	char portString[BUFF_SIZE] = {0};
	portString[0] = 'A';
	sprintf(portString+1, "%d", dataPort);

	len = strlen(portString);
	portString[len] = '\n';
	if(write(control, portString, len+1) < 0) error(0, "Write failed");

	if(listen(s, 1)) error(0, "Listen failed");
	int	dataSock = accept(s, (struct sockaddr *) &addr, &len);

	return(dataSock);
}


/////////////////////////////////////////////////////////////////////////////////
////////////////// Write Error //////////////////////////////////////////////////

void write_error(int control, char* e){
	int len = strlen(e);
	write(control, e, len);
	fprintf(stderr, "%s", e);
	if(errno) fprintf(stderr, "\t%s\n", strerror(errno));
	errno = 0;
}
