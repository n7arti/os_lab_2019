#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;


struct FactArgs {
  int begin;
  int end;
  int mod;
};


int thread_factorial(struct FactArgs *args)
{
    int fact = 1;
  for (int i = args->begin; i < args->end+1; i++){
      fact *= i%args->mod;
      printf("%d func\n",i );
  }
  return fact;
}
int main(int argc, char **argv) {
  int k = -1;
  int mod = -1;
  int pnum = -1;
  while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {{"k", required_argument, 0, 0},
                                        {"mod", required_argument, 0, 0},
                                        {"pnum", required_argument, 0, 0},
                                        {0, 0, 0, 0}};

        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

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
        args[i].mod=mod; 
        if(i == 0)
        args[i].begin = i+1;
        else {
        args[i].begin = (i*k/pnum)+1;
        }
      if(i == (pnum-1))
        args[i].end = k;
      else 
        args[i].end = (k/pnum)*(i+1);
        printf("%d begin \n",args[i].begin);
        printf("%d end \n",args[i].end);
    }
    for(int i = 0; i < pnum; i++) {
        
        if (pthread_create(&thread[i], NULL, (void *)thread_factorial, (void *)&args[i]) != 0){
            perror("pthread_create");
            exit(1);
        }
        
    }
    int total_fact = 1;
    int fact = 1;
  for (int i = 0; i < pnum; i++) {
    pthread_join(thread[i], (void *)&fact);
    printf("%d fact\n",fact);
    total_fact *= fact%mod;
  }
  printf("%d\n", total_fact);

}
