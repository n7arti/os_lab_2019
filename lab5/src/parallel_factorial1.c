#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
uint32_t total_fact = 1;

struct FactArgs {
  u_int32_t* fact;
  u_int32_t begin, end, mod;
};


int thread_factorial(void *args)
{
    struct FactArgs *d = (struct FactArgs *)args;
    for (int i = d->begin; i < d->end; i++)
    {
        pthread_mutex_lock(&mut);
        *(d->fact) *= i;
        *(d->fact) %= d->mod;
        pthread_mutex_unlock(&mut);
        
    }
}

int main(int argc, char **argv) {
  u_int32_t k = -1;
  u_int32_t mod = -1;
  u_int32_t pnum = -1;
  while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {{"k", required_argument, 0, 0},
                                        {"mod", required_argument, 0, 0},
                                        {"pnum", required_argument, 0, 0},
                                        {0, 0, 0, 0}};

        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);

        if (c == -1) break;

        switch (c) {
        case 0:
            switch (option_index) {
            case 0:
                k = atoi(optarg);
                if (k < 1) {
                    printf("Error: k must be positive number\n");
                    return(1);
                }
                break;
            case 1:
                mod = atoi(optarg);
                if (mod < 2) {
                    printf("Error: mod must be more than one");
                    return(1);
                }
                break;
            case 2:
                pnum = atoi(optarg);
                if (pnum < 1) {
                    printf("Error: pnum must be positive number\n");
                    return(1);
                }
                break;

            defalut:
                printf("Index %d is out of options\n", option_index);
            }
            break;

        case '?':
            break;

        default:
            printf("getopt returned character code 0%o?\n", c);
        }
    }

    if (optind < argc) {
        printf("Has at least one no option argument\n");
            return 1;
    }

    if (k == -1 || mod == -1 || pnum == -1) {
        printf("Usage: %s -k \"num\" --pnum \"num\" --mod \"num\" \n",
            argv[0]);
        return 1;
    }
    struct FactArgs args[pnum];
    pthread_t thread[pnum];

    for (int i=0;i<pnum;i++){  
      args[i].fact = &total_fact;
      args[i].begin = i * k / pnum + 1;
      args[i].end = (i == (pnum - 1) ) ? k + 1 : (i + 1) * k / pnum + 1;
      args[i].mod = mod;
    }
    for(int i = 0; i < pnum; i++) {
        
        if (pthread_create(&thread[i], NULL, (void *)thread_factorial, (void *)&args[i])){
            perror("pthread_create");
            exit(1);
        }
        
    }
     for (int i = 0; i < pnum; i++)
      if (pthread_join(thread[i], NULL) != 0) {
        perror("pthread_join");
        exit(1);
      }
  printf("Total: %d \n", total_fact);

}
