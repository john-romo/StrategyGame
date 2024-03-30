#include "filemap.h"

char* type;
char* path;
size_t unitSize;

int ifd;
int ofd;
int mapfd;
int logfd;
size_t unitCount;
size_t fileSize;
offsets used;
offsets unused;

void logwr(char* ch){
	write(logfd, ch, strlen(ch));
	write(logfd, "\n", 1);
}


#define ifd STDIN_FILENO
#define ofd STDOUT_FILENO

int main(int c, char** v){
	init(v);
	//print_filemap_meta();
	//if(TEST) test();

	int i = 0;	
	while(i < 8){
		char ch;
		read(ifd, &ch, 1);
		process_message(ch);
		//sleep(1);
		++i;
	}
	
	
	return 0;
}


void process_message(char c){
	write(logfd, &c, 1);
	write(logfd, "\n", 1);
	switch(c){
		case 'i' :{
			char msg[unitSize];
			read(ifd, msg, unitSize);
			off_t offset = fm_insert(msg);
			write(ofd, (char*) &offset, sizeof(off_t));
			break;
		}
		case 'd' :{
			off_t offset;
			read(ifd, (char*) &offset, sizeof(off_t));
			fm_delete(offset);
			break;
		}
		case 'l':{
			off_t offset;
			read(ifd, (char*) &offset, sizeof(off_t));
			char unit[unitSize];
			fm_load(offset, unit);
			write(ofd, unit, unitSize);
			break;
		}
		default:
	}
}


void init(char** v){
	int i = 0;
	type = v[1];
	path = v[2];
	unitSize =  atoi(v[3]);
	//dup2(STDIN_FILENO, ifd);
	//dup2(STDOUT_FILENO, ofd);
	size_t unitCount = 0;
	size_t fileSize = 0;
	init_offsets(&used);
	init_offsets(&unused);
	unitCount = 0;

	mapfd = open(path, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if(mapfd < 0){
		write(logfd, "FCERROR\n", 8);
		exit(mapfd);
	}
	logfd = open("log", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(mapfd < 0){
		write(logfd, "FCERROR\n", 8);
		exit(mapfd);
	}
}

void init_offsets(offsets* of){
	of->arr = (off_t*) malloc(DEFAULT_OFFSETS_SIZE*sizeof(off_t));
	of->size = 0;
	of->end = DEFAULT_OFFSETS_SIZE;
} 



void fm_load(off_t offset, char* ret){
	size_t i = 0;
	for(; i < used.size; ++i){
		if(offset == used.arr[i]) break;
	}
	if(i == used.size) ret = NULL;
	lseek(mapfd, offset*unitSize, SEEK_SET);
	read(mapfd, ret, unitSize);
}


off_t fm_insert(char* unit){
	off_t offset = get_insert_offset();
	lseek(mapfd, offset*unitSize, SEEK_SET);
	write(mapfd, unit, unitSize);
	++unitCount;
	return offset;
}

off_t get_insert_offset(){

	if(!used.size){
		used.arr[0] = 0;
		++(used.size);
		return 0;
	}

	resize_offsets(&used);

	int offset = used.size;
	if(unused.size){
		offset = unused.arr[(unused.size)-1];
		--(unused.size);
	}

	used.arr[used.size] = offset;
	++(used.size);

	return offset;
}





char fm_delete(off_t offset){
	logwr("in delete\0");
	if(offset >= used.size) return 0;
	logwr("1\0");
	ptrdiff_t i = 0;
	for(; (i >= 0) && (i < used.size); ++i){
		if(used.arr[i] == offset) break;
	}
	if(i == used.size) return 0;
	logwr("2\0");
	
	if(i != (used.size-1))
		for(; i < used.size; ++i) *(used.arr+i) = *(used.arr+i+1);
	--(used.size);
	logwr("3\0");

	lseek(mapfd, offset*unitSize, SEEK_SET);
	char* zeros[unitSize];
	memset(zeros, 0, unitSize);
	write(mapfd, zeros, unitSize);
	resize_offsets(&unused);
	unused.arr[unused.size] = offset;
	++(unused.size);
	--unitCount;
	logwr("4\0");

	return 1;
}




void resize_offsets(offsets* of){
	if(of->size < DEFAULT_OFFSETS_SIZE) return;
	size_t newEnd = 0;
	if(of->size == of->end) newEnd = (of->end*2);
	else if(of->size < (of->end/4)) newEnd = (of->end/2);
	if(!newEnd) return;
	newEnd = (newEnd >= DEFAULT_OFFSETS_SIZE) ? newEnd : DEFAULT_OFFSETS_SIZE;
	off_t* temp = (off_t*) malloc(newEnd*sizeof(off_t));
	memcpy(temp, of, (newEnd*sizeof(off_t)));
	free(of->arr);
	of->arr = temp;
	of->end = newEnd;
}


void free_mem(){
	if(used.arr) free(used.arr);
	if(unused.arr) free(unused.arr);
}







void print_filemap_meta(){
	printf("ifd: %d\n", ifd);
	printf("ofd: %d\n", ofd);
	printf("type: %s\n", type);
	printf("path: %s\n", path);
	printf("unitSize: %d\n", unitSize);
	printf("mapfd: %d\n", mapfd);
	printf("unitCount: %d\n", unitCount);
}

void print_used(){
	printf("\tused (size %d): ", used.size);
	for(int i = 0; i < used.size; ++i){
		printf("%d ", used.arr[i]);
	}
	printf("\n");
}

void print_unused(){
	printf("\tunused (size %d): ", unused.size);
	for(int i = 0; i < unused.size; ++i){
		printf("%d ", unused.arr[i]);
	}
	printf("\n");
}

void test(){
	print_filemap_meta();

	printf("insert at 0 ---> \n");
	char* teststr = strdup("TEST1");
	fm_insert(teststr);
	print_unused();
	print_used();

	printf("insert at 1 --->\n");
	teststr = strdup("TEST2");
	fm_insert(teststr);
	print_unused();
	print_used();
	
	printf("insert at 2 --->\n");
	teststr = strdup("TEST3");
	fm_insert(teststr);
	print_unused();
	print_used();

	printf("insert at 3 --->\n");
	teststr = strdup("TEST4");
	fm_insert(teststr);
	print_unused();
	print_used();

	printf("delete 2 --->\n");
	fm_delete(2);
	print_unused();
	print_used();
	
	printf("insert at 2 --->\n");
	teststr = strdup("TEST5");
	fm_insert(teststr);
	print_unused();
	print_used();

	char c = fm_delete(used.size);
	printf("delete used.size (worked: %d) --->\n", c);
	print_unused();
	print_used();

	printf("delete %d --->\n", used.arr[used.size-1]);
	fm_delete(used.arr[used.size-1]);
	print_unused();
	print_used();

	printf("insert at %d --->\n", unused.arr[unused.size-1]);
	teststr = strdup("TEST6");
	fm_insert(teststr);
	print_unused();
	print_used();

	printf("delete 0 --->\n");
	fm_delete(0);
	print_unused();
	print_used();

	printf("insert at 0 --->\n");
	teststr = strdup("TEST7");
	fm_insert(teststr);
	print_unused();
	print_used();
	
	char t[unitSize];
	fm_load(1, t);
	printf("load from 1: ");
	for(int i = 0; i < unitSize; ++i){
		printf("%c", t[i]);
	}
	printf("\n");


	free_mem();
}


