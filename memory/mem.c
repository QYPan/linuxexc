#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 5

#define MALLOC(num, type) \
	(type *)alloc((num)*sizeof(type))

typedef struct DATA{
	char *str;
	int is_continuing;
	struct DATA *next;
}Data;

void *alloc(size_t size)
{
	void *new_mem;
	new_mem = malloc(size);
	if(new_mem == NULL){
		printf("Out of memory!\n");
		exit(1);
	}
	return new_mem;
}

void append(Data **link, char *s)
{
	Data *cur;
	Data *new;
	int slen = strlen(s);
	while((cur = *link) != NULL)
		link = &cur->next;

	new = MALLOC(1, Data);
	new->str = MALLOC(slen+1, char);
	strcpy(new->str, s);
	new->is_continuing = (s[slen-1] != '\n');

	*link = new;
}

void display_data(Data *beg)
{
	Data *cur = beg;
	int new_line = 1;
	int line_count = 0;
	int struct_count = 0;
	printf("\n----------------------------------\n");
	while(cur){
		if(new_line)
			printf("line%d: ", ++line_count);
		printf("%s", cur->str);
		struct_count++;
		new_line = !cur->is_continuing;
		cur = cur->next;
	}
	printf("\nlines:   %d\n", line_count);
	printf("structs: %d\n", struct_count);
}

void free_data(Data *beg)
{
	Data *cur = beg;
	Data *next = NULL;
	while(cur){
		next = cur->next;
		free(cur->str);
		free(cur);
		cur = next;
	}
}

int main()
{
	char input[BUFSIZE];
	Data *beg = NULL;
	printf("\ninput string (a string a line) stop by <ctrl+d>:\n");
	while(fgets(input, sizeof(input), stdin)){
		append(&beg, input);
	}
	display_data(beg);
	free_data(beg);
	return 0;
}
