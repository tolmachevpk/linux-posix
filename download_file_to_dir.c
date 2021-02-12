#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// https://mirror.yandex.ru/debian/doc/mailing-lists.txt

#define MAX_URL 2500

void make_line_for_wget(char **res, char *start, char *path, char *url) {
    int len_start = strlen(start);
    int len_path = strlen(path);
    int len_url = strlen(url);
    (*res) = (char *) calloc(len_path + len_start + len_url + 1, sizeof(char));

    for (int i = 0; i < len_start; i++) {
        (*res)[i] = start[i];
    }

    for (int i = 0; i < len_path; i++) {
        (*res)[i + len_start] = path[i];
    }

    (*res)[len_path + len_start] = ' ';

    for (int i = 0; i < len_url; i++) {
        (*res)[len_path + len_start + 1 + i] = url[i];
    }
}

int main(int ac, char *av[]) {
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    char *url = "https://mirror.yandex.ru/debian/doc/mailing-lists.txt";
    char *start = "wget -P ";

    char *res = NULL;

    make_line_for_wget(&res, start, buf, url);

    // printf("%s\n", res);

    system(res);
}
