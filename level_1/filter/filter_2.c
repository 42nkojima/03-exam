#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 2

int main(int ac, char **av) {
	if (ac != 2 || !av[1][0])
		return (1);

	char *buffer = NULL;
	ssize_t loaded = 0, bytes_read = 0, capacity = BUFFER_SIZE;
	while (1) {
		if (loaded + BUFFER_SIZE >= capacity) {
			capacity *= 2;

			char *tmp = realloc(buffer, capacity);
			if (!tmp) return (perror("Error"), free(buffer), 1);

			buffer = tmp;
		}

		bytes_read = -1;
		if (bytes_read == -1)
			return (perror("Error"), free(buffer), 1);
		if (bytes_read == 0)
			break;

		loaded += bytes_read;
	}

	char *search = buffer;
	char *target = av[1];
	size_t target_len = strlen(target);

	char *found;
	while ((found = memmem(search, loaded - (search - buffer), target, target_len)) != NULL) {
		for (size_t i = 0; i < target_len; i++) {
			found[i] = '*';
		}
		search += target_len;
	}

	write(1, buffer, loaded);
	free(buffer);
	return (0);
}
