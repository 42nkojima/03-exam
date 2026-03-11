#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 2

int main(int ac, char **av) {
	if (ac != 2 || !av[1][0]) {
		return (1);
	}

	// read関数を使って、入力をBUFFER_SIZE分読み取る
	// 次のread読み取る分をcapacityより大きかったらreallocしてbufferを拡張する
	// reallocする時にtmp変数に入れて~ってやる
	// readの戻り値が0だったらbreakしてヌル終端を入れる
	// -1ならperrorをする

	char *buffer = NULL;

	ssize_t bytes = 0, loaded = 0, capacity = BUFFER_SIZE;
	while (1) {
		if (loaded + BUFFER_SIZE >= capacity) {
			capacity *= 2;
			char *tmp = realloc(buffer, capacity);
			if (!tmp) {
				return (perror("Error"), free(buffer), 1);
			}
			buffer = tmp;
		}

		bytes = read(0, buffer + loaded, BUFFER_SIZE);
		if (bytes == 0) {
			buffer[loaded] = '\0';
			break;
		}
		if (bytes == -1) {
			return (perror("Error"), free(buffer), 1);
		}

		loaded += bytes;
	}

	// memmemでtargetの位置を指す
	// memmemがNULLになるまでwhileを回す
	char *search = buffer;
	size_t search_len = strlen(search);
	char* target = av[1];
	size_t target_len = strlen(target);

	char *found;
	while ((found = memmem(search, search_len - (found - search), target, target_len)) != NULL) {
		for (size_t i = 0; i < target_len; i++) {
			found[i] = '*';
		}

		search += target_len + (found - search);
	}

	printf("%s\n", buffer);
	free(buffer);
	return (0);
}
