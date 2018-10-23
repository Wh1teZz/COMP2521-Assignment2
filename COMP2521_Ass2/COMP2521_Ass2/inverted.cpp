#include "stdafx.h"
#include <ctype.h>
#include <stdlib.h>



int main() {
	char **urls = NULL;
	FILE *fp = fopen("collection.txt", "r");
	if (!fp) { fprintf(stderr, "failed to open collections.txt\n"); return EXIT_FAILURE; }
	char* temp;
	int count = 0;
	while (fscanf(fp, "%s", &temp) != EOF) {
		urls = realloc(urls, (count) * sizeof(char*));
		urls[count++] = strdup(temp);
	}
	fclose(fp);

}


char* normalise(char* str) {

	char* newStr = calloc(strlen(str), sizeof(char));
	int i;

	// 1) remove leading whitespace
	while (*str == ' ') str++;

	// 1.5) remove trailing whitespace
	int i = strlen(str) - 1;
	while (str[i] == ' ' && i >= 0) {
		str[i] = '\0';
		i--;
	}

	// 3) remove punctuation ( . , ; ? ) 
	while (str) {
		while ((*str == '.'
			&& *str == ','
			&& *str == ';'
			&& *str == '?')) {
			str++;
		}
		newStr[i] = *str;
		i++;
		str++;
	}

	// 2) convert everything to lowercase
	for (i = 0; i < strlen(str); i += 1) {
		newStr[i] = tolower(newStr[i]);
	}

	free(str);
	return newStr;
}