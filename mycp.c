// Est' voprosy, zadam na seminare

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <libgen.h>

const int COUNT = 200;
const size_t SIZE = 100;

void copy_text(int fd1, int fd2, char *buf)
{
	int i = 0;
	i = read(fd1, buf, COUNT);
	while (i > 0) {
		write(fd2, buf, i);
		i = read(fd1, buf, COUNT);
	}
}

int main(int ac, char **av)
{
	int i = 0;
	int is_short_arg = 0;
	char *buf;
	char *curr_dir;
	curr_dir = (char *) malloc(SIZE * sizeof(char));
	buf = (char *) malloc(COUNT * sizeof(char));
	if (ac < 3) {
		write(1, "cp: bad request\n", 16);
		exit(EXIT_FAILURE);
	}
	if ((strcmp(av[1], "-i") == 0) || (strcmp(av[1], "-p") == 0)
	    || (strcmp(av[1], "-v") == 0) || (strcmp(av[1], "-f") == 0)) {
		is_short_arg = 1;
	}
	getcwd(curr_dir, SIZE);
	i = chdir(av[ac - 1]);
	if (i < 0) {
		int cls1, cls2;
		int fd1 = open(av[1 + is_short_arg], O_RDONLY);
		if (fd1 < 0) {
			perror("file 1 open failed");
		}
		int fd2 = open(av[2 + is_short_arg], O_WRONLY);
		if (fd2 >= 0) {
			if (is_short_arg == 1) {
				if (strcmp(av[1], "-i") == 0) {
					char ch;
					printf("cp : overwrite '%s'? ",
					       av[3]);
					scanf("%c", &ch);
					if (ch == 'y') {
						copy_text(fd1, fd2, buf);
					}
				}
			} else {
				copy_text(fd1, fd2, buf);
			}
		} else {
			// remove(av[2 + is_short_arg]);
			fd2 =
			    open(av[2 + is_short_arg], O_WRONLY | O_CREAT,
				 S_IREAD | S_IWRITE);
			copy_text(fd1, fd2, buf);
		}
		cls1 = close(fd1);
		if (cls1 < 0) {
			perror("file 1 close failed");
		}
		cls2 = close(fd2);
		if (cls2 < 0) {
			perror("file 2 close failed");
		}
		free(buf);
	} else {
		int j = 0;
		for (j = 1 + is_short_arg; j < (ac - 1); j++) {
			buf = (char *) malloc(COUNT * sizeof(char));
			chdir(curr_dir);
			int fd1 = open(av[j], O_RDONLY);
			if (fd1 < 0) {
				perror("file open failed");
			}
			chdir(av[ac - 1]);
			int fd2 = open(basename(av[j]), O_WRONLY);
			if (fd2 >= 0) {
				if (is_short_arg == 1) {
					if (strcmp(av[1], "-i") == 0) {
						char ch;
						printf
						    ("cp : overwrite '%s'? ",
						     basename(av[j]));
						scanf("%c", &ch);
						if (ch == 'y') {
							copy_text(fd1, fd2,
								  buf);
						}
					}
				} else {
					copy_text(fd1, fd2, buf);
				}
			} else {
				// remove(av[2 + is_short_arg]);
				fd2 =
				    open(basename(av[j]),
					 O_WRONLY | O_CREAT,
					 S_IREAD | S_IWRITE);
				copy_text(fd1, fd2, buf);
			}
			chdir(curr_dir);
			int cls1 = close(fd1);
			if (cls1 < 0) {
				perror("close failed");
			}
			int cls2 = close(fd2);
			if (cls2 < 0) {
				perror("close failed");
			}
			free(buf);
		}
	}
	// free(buf);
	free(curr_dir);
}
