#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

const int COUNT = 5;

void print_text(int fd, char *buf)
{
	int i = 0;
	i = read(fd, buf, COUNT);
	while (i > 0) {
		write(1, buf, i);
		i = read(fd, buf, COUNT);
	}
}

int main(int ac, char **av)
{
	char *buf;
	int j = 0;;
	buf = (char *) malloc(COUNT * sizeof(char));
	if (ac == 1) {
		print_text(0, buf);
	} else {
		for (j = 1; j < ac; j++) {
			int cls = 0;
			int fd = open(av[j], O_RDONLY);
			if (fd < 0) {
				perror("open failed");
			}
			print_text(fd, buf);
			cls = close(fd);
			if (cls < 0) {
				perror("close failed");
			}
		}
	}
	free(buf);
}
