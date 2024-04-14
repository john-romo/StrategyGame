#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stddef.h>

#define DEFAULT_OFFSETS_SIZE 10
#define TEST 1


typedef struct _offsets{
	off_t* arr;
	size_t size;
	size_t end;
} offsets;



void process_message(char c);


void init_offsets(offsets* of);
void fm_load(off_t offset, char* ret);
off_t fm_insert(char* unit);
char fm_delete(off_t offset);
off_t get_insert_offset();
void resize_offsets(offsets* of);
void free_mem();
void test();
void init(char** v);
void print_filemap_meta();
void print_used();
void print_unused();






