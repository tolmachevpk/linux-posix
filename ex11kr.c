/*
 * ex11
 * Будем считать, что пришел массив unsigned int'ов, удобно для генерации рандомных чисел
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

const int SIZE = sizeof(unsigned long long);

int main(int ac, char* av[]) {
    int length = atoi(av[1]);
    int processes = atoi(av[2]);
    int arr[length];
    int i = 0;
    srand(time(0));
    unsigned long long sum = 0;
    for (i = 0; i < length; i++) {
        arr[i] = (rand() % 20);
        sum += arr[i];
    }
    printf("С помощью цикла for получилось sum = %llu\n", sum);
    int mypipefd[2];
    int checkpipe = pipe(mypipefd);
    if (checkpipe != 0) {
        perror("problems with pipe");
    }
    sum = 0;
    int pid;
    int count_of_elem = (int) length / processes;
    for (i = 0; i < processes; i++) {
        pid = fork();
        if (pid == 0) {

            int j = 0;
            close(mypipefd[0]);
            long long unsigned sum_fork = 0;
            int hightest_val = 0;
            if (i == processes - 1) {
                hightest_val = length;
            } else {
                hightest_val = count_of_elem * (i + 1);
            }
            for (j = (count_of_elem * i); j < hightest_val; j++) {
                sum_fork += arr[j];
            }
            printf("тут еще все работает %d\n", i);
            int return_write = 0;
            return_write = write(mypipefd[1], &sum_fork, SIZE);
            printf("write для fork #%d возвращает %d\n", i, return_write);
            //char ch = '\n';
            //write(mypipefd[1], &ch, 1);
            close(mypipefd[1]);
            return 0;
        }
    }
    for (i = 0; i < processes; i++) {
        wait(NULL);
    }
    long long unsigned sum_of_part = 0;
    close(mypipefd[1]);
    while ( (i = read(mypipefd[0], &sum_of_part, SIZE)) > 0) {
        printf("%llu", sum_of_part);
        sum += sum_of_part;
    }
    printf("read возвращает %d\n", i);
    close(mypipefd[0]);
    printf("С помощью fork() и pipe() sum = %llu\n", sum);

}
