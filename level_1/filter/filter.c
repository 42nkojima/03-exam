#define _GNU_SOURCE
#define BUFFER_SIZE 2
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main(int ac, char **av) {
	if (ac != 2 | strlen(av[1]) == 0)
		return (1);

	char *buffer_start = malloc(BUFFER_SIZE);
	if (!buffer_start)
		return (perror("Error"), free(buffer_start), 1);

	ssize_t bytes_read;
	size_t used_size = 0;
	char *new_buffer;

	while (1) {
		// readで読み取る前に、今まで読み込んだbufferの量がBUFFER_SIZEを超えていたら、reallocでバッファサイズを増やす
		if (used_size + BUFFER_SIZE > BUFFER_SIZE) {
			new_buffer = realloc(buffer_start, used_size + BUFFER_SIZE);
			if (!new_buffer)
				return (perror("Error"), free(buffer_start), 1);
			buffer_start = new_buffer;
		}

		bytes_read = read(0,  buffer_start + used_size, BUFFER_SIZE);
		if (bytes_read == -1)
			return (perror("Error"), free(buffer_start), 1);
		if (bytes_read == 0) break;

		used_size += bytes_read;
	}

	buffer_start[used_size] = '\0';
	size_t buffer_len = strlen(buffer_start);

	char *pattern = av[1];
	size_t pattern_len = strlen(pattern);

	// memmemで検索しながら置換を繰り返す
	char *match_pos, *search_pos = buffer_start;
	while((match_pos = memmem(search_pos, buffer_len - (search_pos - buffer_start), pattern, pattern_len)) != NULL) {
		for (size_t i = 0; i < pattern_len; i++) {
			match_pos[i] = '*';
		}
		search_pos = match_pos + pattern_len;
	}

	for (size_t i = 0; i < buffer_len; i++) {
		write(1, &buffer_start[i], 1);
	}
	free(buffer_start);
	return (0);
}
