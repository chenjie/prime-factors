#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void verify_perfect_square(int m, int n) {
    if (m * m == n) {
        printf("%d %d %d\n", n, m, m);
        exit(0);
    }
}

void verify_if_factor(int m, int n, int *prime_factor, int *ptr_num_prime_factors) {
    if (n % m == 0) {
        prime_factor[*ptr_num_prime_factors] = m;
        (*ptr_num_prime_factors)++;
        if ((*ptr_num_prime_factors) >= 2) {
            printf("%d is not the product of two primes\n", n);
            exit(0);
        }
    }
}

int main(int argc, char **argv) {
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(-1);
    }
    if (argc != 2) {
        fprintf(stderr, "Usage:\n\tpfact n\n");
        exit(-1);
    }
    int n = strtol(argv[1], NULL, 10);
    // TBD: check if it's an integer
    if (n <= 1) {
        fprintf(stderr, "Usage:\n\tpfact n\n");
        exit(-1);
    }
    if (n == 2) {
        printf("2 is prime\n");
        printf("Number of filters = 0\n");
        exit(0);
    }

    // Find all the primes that are less than n
    int all_primes[n - 2];
    for (int i = 2; i < n; i++) {
        all_primes[i - 2] = i;
    }
    int m = 2;
    int result[1024];
    int result_index = 0;
    int flag = 0;
    int next_m = 3;
    while (m < sqrt(n)) {
        flag = 0;
        for (int i = 0; i < n-2; i++) {
            if (all_primes[i] != -1) {
                if (all_primes[i] % m == 0) {
                    all_primes[i] = -1;
                } else {
                    if (flag != 1) {
                        flag = 1;
                        next_m = all_primes[i];
                    }
                }
            }
        }
        result[result_index] = m;
        result_index++;
        m = next_m;
    }
    for (int i = 0; i < n - 2; i++) {
        if (all_primes[i] != -1) {
            result[result_index] = all_primes[i];
            result_index++;
        }
    }

    m = 2;
    int num_prime_factors = 0;
    int prime_factor[2];

    verify_perfect_square(m, n);
    verify_if_factor(m, n, prime_factor, &num_prime_factors);

    int main_fd[2];
    if (pipe(main_fd) == -1) {
        perror("pipe");
        exit(-1);
    }
    pid_t r = fork();
    if (r > 0) { // parent
        printf("[pid = %d] {FILTER} m is %d\n", getpid(), m);
        if (close(main_fd[0]) == -1) {
            perror("close");
            exit(-1);
        }
        // write
        for (int i = 2; i <= n; i++) {
            if (write(main_fd[1], &i, sizeof(int)) == -1) {
                perror("write_main");
                exit(-1);
            }
        }
        if (close(main_fd[1]) == -1) {
            perror("close");
            exit(-1);
        }

        // waits for children
        int status;
        if(wait(&status) == -1) {
            perror("wait");
            exit(-1);
        } else {
            if (WIFEXITED(status)) {
                int ex_code = WEXITSTATUS(status);
                printf("Number of filters = %d\n", ex_code + 1);
                exit(0);
            } else {
                fprintf(stderr, "ERROR: Terminated with signal\n");
                exit(-1);
            }
        }

    } else if (r == 0) { // child
        if (close(main_fd[1]) == -1) {
            perror("close");
            exit(-1);
        }
        int pre_read_fd = main_fd[0];
        // read the first one
        int buf = 0;
        while (buf % m == 0) {
            if (read(pre_read_fd, &buf, sizeof(int)) == -1) {
                perror("read");
                exit(-1);
            }
        }
        int pre_m = m;
        m = buf;

        while (m <= sqrt(n)) {
            printf("[pid = %d] {FILTER} m is %d\n", getpid(), m);
            verify_perfect_square(m, n);
            verify_if_factor(m, n, prime_factor, &num_prime_factors);

            int child_fd[2];
            if (pipe(child_fd) == -1) {
                perror("pipe");
                exit(-1);
            }
            pid_t r = fork();
            if (r > 0) { // parent
                if (close(child_fd[0]) == -1) {
                    perror("close");
                    exit(-1);
                }
                do {
                    if (buf % pre_m != 0) {
                        if (write(child_fd[1], &buf, sizeof(int)) == -1) {
                            perror("write_child");
                            exit(-1);
                        }
                    }
                } while (read(pre_read_fd, &buf, sizeof(int)) != 0);
                if (close(child_fd[1]) == -1) {
                    perror("close");
                    exit(-1);
                }
                if (close(pre_read_fd) == -1) {
                    perror("close");
                    exit(-1);
                }

                // waits for children
                int status;
                if(wait(&status) == -1) {
                    perror("wait");
                    exit(-1);
                } else {
                    if (WIFEXITED(status)) {
                        exit(WEXITSTATUS(status) + 1);
                    } else {
                        fprintf(stderr, "ERROR: Terminated with signal\n");
                        exit(-1);
                    }
                }

            } else if (r == 0) { // child
                if (close(child_fd[1]) == -1) {
                    perror("close");
                    exit(-1);
                }
                pre_read_fd = child_fd[0];
                // read the first one
                buf = 0;
                while (buf % m == 0) {
                    if (read(pre_read_fd, &buf, sizeof(int)) == -1) {
                        perror("read");
                        exit(-1);
                    }
                }
                pre_m = m;
                m = buf;

            } else { // error
                perror("child's fork");
                exit(-1);
            }

        }

        // m > sqrt(n)
        if (num_prime_factors == 0) {
            printf("%d is prime\n", n);
            exit(0);
        } else if (num_prime_factors == 1) {
            for (int i = 0; i < result_index; i++) {
                int target = n / prime_factor[0];
                if (target == result[i]) {
                    printf("%d %d %d\n", n, prime_factor[0], target);
                    exit(0);
                }
            }
            printf("%d is not the product of two primes\n", n);
            exit(0);
        } else {
            fprintf(stderr, "ERROR: num_prime_factors\n");
            exit(-1);
        }

    } else { // error
        perror("fork");
        exit(-1);
    }

    return 0;
}
