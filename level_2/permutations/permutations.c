#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static size_t ft_strlen(char *s) {
	size_t len = 0;
	while (s[len]) len++;

	return (len);
}

static void swap(char *a, char *b) {
	char tmp = *a;
	*a = *b;
	*b = tmp;
}

static void sort(char *s, size_t len) {
	for (size_t i = 0; i < len; i++) {
		for (size_t j = 0; j < len - 1 - i; j++) {
			if (s[j] > s[j+1]) {
				swap(&s[j], &s[j+1]);
			}
		}
	}
}

static void dfs(char *result, char *remaining, size_t rem_len, size_t depth) {
	// base case
	if (rem_len == 0) {
		result[depth] = '\0';
		puts(result);
		return;
	}

	for (size_t pick = 0; pick < rem_len; pick++) {
		result[depth] = remaining[pick];

		char *new_remaining = malloc(sizeof(*new_remaining) * (rem_len - 1));
		if (!new_remaining) return;
		for (size_t i = 0; i < pick; i++) {
			new_remaining[i] = remaining[i];
		}
		for (size_t i = pick; i < rem_len - 1; i++) {
			new_remaining[i] = remaining[i + 1];
		}

		dfs(result, new_remaining, rem_len - 1, depth + 1);
		free(new_remaining);
	}
}

int main(int ac, char **av) {
	if (ac != 2 || !av[1][0]) {
		return (1);
	}

	size_t t_len = ft_strlen(av[1]);

	char *result = malloc(sizeof(*result) * (t_len + 1));
	if (!result) return (1);

	// 昇順で出力するために入力をソートする必要がある
	sort(av[1], t_len);

	// resultと、残りの文字列と、残りの文字列の長さと、再帰の深さ
	dfs(result, av[1], t_len, 0);

	free(result);
	return (0);
}
