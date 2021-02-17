#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h>

#define MAX_URL 2500
#define START_MAX_TEXT 2000
#define VAL_FOR_INC 1500

void download_file(char *url, char *path) {
    int pid = fork();
    if (pid == 0) {
        execlp("wget", "wget", "-P", path, url, NULL);
    }
    if (pid == -1) {
        perror("bad fork");
    }
    wait(NULL);
}

void increase_size_text(char ** text) {
    int len_text = strlen(*text);
    (*text) = (char *) realloc((*text), len_text + VAL_FOR_INC);
}

void read_file(int fd1, char **text) {
    int i = read(fd1, *text, VAL_FOR_INC);
    if (i == -1) {
        perror("Can't read file");
    }
    int k = VAL_FOR_INC;
    while (i > 0) {
        i = read(fd1, *text + k, VAL_FOR_INC);
        if (i > 0) {
            k += VAL_FOR_INC;
            increase_size_text(text);
        }
    }
    int cls1 = close(fd1);
    if (cls1 < 0) {
        perror("File close failed");
    }
}

char *name_of_file(char *url) {
    char *arr = (char *) calloc(MAX_URL, 1);
    int i = 0;
    int len_url = strlen(url);
    int k = 0;

    for (i = 0; i < len_url; i++) {
        if ((url[i] == '/') && (i != (len_url - 1))) {
            k = i;
        }
        arr[i - k] = url[i];
    }
    arr[len_url - k] = '/';

    char *res = (char *) calloc(MAX_URL, 1);
    for (i = 1; i < strlen(arr); i++) {
        if(arr[i] == '/') {
            break;
        }
        res[i - 1] = arr[i];
    }

    free(arr);
    return res;
}

void read_file_from_url(char* url, char **text) {
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);

    download_file(url, buf);
    char *file = name_of_file(url);
    int fd1 = open(file, O_RDONLY);
    if (fd1 >= 0) {
        read_file(fd1, text);
    }
}

int is_url(char *url) {
    char *istr = NULL;

    istr = strcasestr(url, ".ru");
    if (istr != NULL) {
        return 1;
    }

    istr = strcasestr(url, ".com");
    if (istr != NULL) {
        return 1;
    }

    istr = strcasestr(url, ".org");
    if (istr != NULL) {
        return 1;
    }

    return 0;
}

int size_of_argv(int ac, char *av[]) {
    int i = 0;
    int sum = 0;
    for (i = 1; i < ac; i++) {
        sum += strlen(av[i]);
    }
    return sum;
}

void increase_text_for_argv(char **text, int count) {
    int size_of_text = START_MAX_TEXT;
    while(size_of_text <= count) {
        increase_size_text(text);
        size_of_text += VAL_FOR_INC;
    }
}

void read_argv(int ac, char *av[], char **text) {
    int i = 0;
    int chars_in_argv = size_of_argv(ac, av);
    increase_text_for_argv(text, chars_in_argv);

    strcat((*text), av[1]);
    for(i = 2; i < ac; i++) {
        strcat((*text), " ");
        strcat((*text), av[i]);
    }
}

void text_reader(int ac, char *av[], char **text) {
    if (ac == 1) {
        printf("Ошибка ввода.\n");
        exit(0);
    }
    if (ac == 2) {
        // Попробуем прочитать файл, если это он, тогда все получится
        int fd1 = open(av[1], O_RDONLY);
        if (fd1 >= 0) {
            read_file(fd1, text);
        }

        // Здесь проверка того, что это url, нормальных способов на С не нашел,
        // поэтому буду просто искать .ru, .ru/, .com, .com/, .org, .org/
        if (is_url(av[1]) == 1) {
            read_file_from_url(av[1], text);
        } else {
            read_argv(ac, av, text);
        }
    } else {
        read_argv(ac, av, text);
    }
}
