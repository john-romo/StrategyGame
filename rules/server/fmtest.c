#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

int main(){
	int ifd[2]; // write
	int ofd[2]; // read

	pipe(ifd);
	pipe(ofd);
	
	
	printf("HERE\n");

	if(!fork()){
		dup2(ifd[0], STDIN_FILENO);
		dup2(ofd[1], STDOUT_FILENO);

		close(ifd[1]);
		char* args[] = {"runtest", "client", "testfile", "5", NULL};
		execvp("/home/john/homework/cs320/rules/server/runtest", args);
	}

	close(ifd[0]);
	close(ofd[1]);
	
	off_t offset;
	char ch = 'i';
	write(ifd[1], &ch, 1);
	write(ifd[1], "test1", 5);
	read(ofd[0], (char*) &offset, sizeof(off_t));
	printf("offset: %d\n", offset);


	offset = 0;
	char ret[5];
	write(ifd[1], "l", 1);
	write(ifd[1], (char*) &offset, sizeof(off_t));
	read(ofd[0], ret, 5);
	for(int i = 0; i < 5; ++i){
		printf("%c", ret[i]);
	}
	printf("\n");

	write(ifd[1], &ch, 1);
	write(ifd[1], "test2", 5);
	read(ofd[0], (char*) &offset, sizeof(off_t));
	printf("offset: %d\n", offset);

	write(ifd[1], &ch, 1);
	write(ifd[1], "test3", 5);
	read(ofd[0], (char*) &offset, sizeof(off_t));
	printf("offset: %d\n", offset);
	
	
	write(ifd[1], &ch, 1);
	write(ifd[1], "test4", 5);
	read(ofd[0], (char*) &offset, sizeof(off_t));
	printf("offset: %d\n", offset);

	write(ifd[1], &ch, 1);
	write(ifd[1], "test5", 5);
	read(ofd[0], (char*) &offset, sizeof(off_t));
	printf("offset: %d\n", offset);

	offset = 2;
	write(ifd[1], "d", 1);
	write(ifd[1], (char*) &offset, sizeof(off_t));
	printf("delete 2\n");

	write(ifd[1], &ch, 1);
	write(ifd[1], "test6", 5);
	read(ofd[0], (char*) &offset, sizeof(off_t));
	printf("offset: %d\n", offset);

	offset = 2;
	write(ifd[1], "d", 1);
	write(ifd[1], (char*) &offset, sizeof(off_t));
	printf("delete 2\n");

	write(ifd[1], "q", 1);

	printf("\nDONE\n");
}

















