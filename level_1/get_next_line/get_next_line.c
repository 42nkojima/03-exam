#define BUFFER_SIZE 42

char *get_next_line(int fd) {
	static char *rest = NULL;
	char buf[BUFFER_SIZE + 1];

	if (fd < 0 || BUFFER_SIZE <= 0) {
		return NULL;
	}

	char *nl;
	ssize_t n;
	while (!rest || !(nl = ft_strchr(rest, '\n'))) {
		n = read(fd, buf, BUFFER_SIZE);
		if (n == 0) break;
		// n < 0 error
		buf[n] = '\0';

		if (rest) {
			ft_strjoin(rest, buf);
		} else {
			ft_strdup(buf);
		}

		if (!rest || !*rest) {
			return (free(rest), rest = NULL);
		}

		if (nl) {
			return (line = 改行まで切り出し, rest = 残り, line);
		} else {
			return (line = ft_strdup(rest), free(rest), rest = NULL, line);
		}
	}
}
