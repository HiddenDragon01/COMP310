#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void parse(char *string, char delimiter, char *tokens[]) {
	char buffer[1000];
	int s=0, b=0, t=0; // counters for string, buffer and tokens

	while(*(string+s) != '\0') {
		// skip leading spaces
		for(;*(string+s)==' ';s++);  // super cool no?!

		// exit if end of string
		if (*(string+s) == '\0') break;

		// otherwise, add word to buffer
		b=0;
		while(*(string+s)!=' ' && *(string+s)!='\0') {
			buffer[b] = *(string+s);
			s++;
			b++;
		}
		buffer[b]='\0'; // make it a string

		// create a token
		tokens[t] = strdup(buffer);
		t++;
	}
	tokens[t] = NULL;
}

int main() {
	char *tokens[100];
	int i;

	parse("  my  name is bob  ",' ',tokens);

	i = 0;
	while(tokens[i]!=NULL) { 
		printf(">>%s<<\n",tokens[i]);
		i++;
	}
	return 0;
}
