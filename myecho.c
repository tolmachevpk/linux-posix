#include <stdio.h>
#include <string.h>

int main(int ac, char* av[]) {
	int i = 0;
	int k = 1;	
	if ((ac > 1) && strcmp(av[1],"-n")) {
		k = 0;
	}
	if (ac > 1) {
		for(i = k + 1; i < ac; i++) {
			printf("%s", av[i]);
			if(i + 1 < ac) {
				putchar(' ');
			}
		}
	}
	if (k == 0) {
		putchar('\n');
	}
}
