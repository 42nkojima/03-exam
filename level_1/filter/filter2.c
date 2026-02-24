#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 2

int main(int ac, char** av) {
	if (ac != 2 || !av[1][0])
		return (1);

	char* buffer = NULL;
	ssize_t loaded = 0, bytes_read, capacity = BUFFER_SIZE;
	// readで読み取るようにする
	while (1) {
		// 次読み取る分のreadの最大許容量を計算していまのバッファで足りるかを考える
		if (loaded + BUFFER_SIZE >= capacity) {
			// reallocでバッファを拡張する
			capacity *= 2;
			char* tmp = realloc(buffer, capacity);
			if (!tmp) return (perror("Error"), free(buffer), 1);

			buffer = tmp;
		}

		bytes_read = read(0, buffer + loaded, BUFFER_SIZE);
		if (bytes_read == 0) break;
		if (bytes_read == -1) return (perror("Error"), free(buffer), 1);

		loaded += bytes_read;
	}
	buffer[loaded] = '\0';

	// bufferからmemmemを使って、targetを見つける
	// NULLじゃないまでを繰り返す
	// 2回目の検索対象の長さは、検索対象(原文)の長さから前回の見つけた位置(bufferとの距離)
	char* search = buffer;
	size_t buffer_len = strlen(buffer);
	char* target = av[1];
	size_t target_len = strlen(target);

	char* found;
	while ((found = memmem(search, buffer_len - (search - buffer), target, target_len)) != NULL) {
		for (size_t i = 0; i < target_len; i++) {
			found[i] = '*';
		}

		// 次の検索対象を移動させる
		// どう移動させるか？ -> 見つけたfoundの位置とその時の検索開始位置の距離 と、検索対象の長さ分 右にずらす
		search += (found - search) + target_len;
	}

	write(1, buffer, loaded);
	free(buffer);
	return (0);
}
