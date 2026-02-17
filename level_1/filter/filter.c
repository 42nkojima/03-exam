

/*
大容量malloc -> readで全データappend
-> memmemで検索しながら置換 -> writeで出力
*/

#define _GNU_SOURCE
#define BUFFER_SIZE 4096
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main(int ac, char **av) {
	if (ac != 2)
		return (1);

	char *search_start = malloc(BUFFER_SIZE);
	if (!search_start)
		return (perror("Error: "), free(search_start), 1);
	char *result = search_start;

	ssize_t bytes_read = read(0, search_start, BUFFER_SIZE);
	if (bytes_read == -1)
		return (perror("Error: "), free(search_start), 1);
	search_start[bytes_read] = '\0';
	size_t search_len = strlen(search_start);

	char *pattern = av[1];
	size_t pattern_len = strlen(pattern);

	// memmemで検索しながら置換を繰り返す
	// search_startは最初はstrの先頭にしといて、foundがNULLじゃなかったら pattern_len分進める
	char *found;
	while ((found = memmem(search_start, search_len, pattern, pattern_len)) != NULL) {
		for (size_t i = 0; i < pattern_len; i++) {
			found[i] = '*';
		}
		search_start += pattern_len;
	}

	for (size_t i = 0; i < strlen(result); i++) {
		write(1, &result[i], 1);
	}
	return (0);
}
