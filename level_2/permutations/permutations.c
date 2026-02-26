#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char *result;
	size_t t_len;
} t_perm;

static void sort(char *s, size_t len) {
	for (size_t i = 0; i < len - 1; i++) {
		for (size_t j = i + 1; j < len; j++) {
			if (s[i] > s[j]) {
				char tmp = s[i];
				s[i] = s[j];
				s[j] = tmp;
			}
		}
	}
}

static void dfs(char *result, size_t depth, char *remaining, size_t rem_len) {
	// base case
	if (rem_len == 0) {
		result[depth] = '\0';
		puts(result);
		return;
	}

	for (size_t pick = 0; pick < rem_len; pick++) {
		result[depth] = remaining[pick];

		char *new_remaining = malloc(sizeof(*new_remaining) * rem_len - 1);
		if (!new_remaining) return;
		for (size_t i = 0; i < pick; i++) {
			new_remaining[i] = remaining[i];
		}
		for (size_t i = 0; i < rem_len - 1; i++) {
			new_remaining[i] = remaining[i + 1];
		}
		dfs(result, depth + 1, new_remaining, rem_len - 1);
		free(new_remaining);
	}
}

int main(int ac, char **av) {
	if (ac != 2 || !av[1][0]) {
		return (1);
	}

	size_t t_len = strlen(av[1]);

	char *result = malloc(sizeof(*result) * (t_len + 1));
	if (!result) return (1);

	// 昇順で出力するために入力をソートする必要がある
	sort(av[1], t_len);

	// resultと、残りの文字列と、残りの文字列の長さと、再帰の深さ
	dfs(result, 0, av[1], t_len);

	free(result);
	return (0);
}
