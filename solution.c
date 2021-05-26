#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/sysinfo.h>

static long sum = 0;
static int seed = 0;
static int total = 0;
static long end = 0;
static long start = 0;
static long portion =0;
static long primeCounter = 0;
pthread_mutex_t lock;


int isPrime(int num) {
	int i=0;
    if (num <= 3 && num > 1) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;
	for (i = 5; i*i <= num; i += 6) {
		if (num % i == 0 || num % (i+2) == 0) return 0; 
	}
	return 1;
}
void* calcPrimes() {
	long ts = start;
	long te = end;
	int random = 0;
	end += portion;
	start += portion;
	for(int i = ts ;i < te && i< total; i++){
		random = rand();
		if (isPrime(random)){
			pthread_mutex_lock(&lock);
			sum += random;
			primeCounter++;
			pthread_mutex_unlock(&lock);
		}
    }
	return NULL;
}
int main(int argc, char *argv[]) {
    int err;
	int i = 0;
	int cores = get_nprocs_conf();
	pthread_t tid[cores];
	seed = atoi(argv[1]);
	total = atoi(argv[2]);
 	portion = total / cores;
	srand(seed);
	if(argc != 3) {
	    printf("Too few arguments ");
	    printf("USAGE: ./primeCalc <prime pivot> <num of random numbers>");
		exit(0);
	}
	if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }
	
	end += portion;
	while(i < cores) {
        err = pthread_create(&(tid[i]), NULL, &calcPrimes,NULL);
        if (err != 0){
			printf("\ncan't create thread :[%s]", strerror(err));
		}
		i++;
    }
	i--;
	while(i >= 0){
		pthread_join(tid[i], NULL);
		i--;
	}

	printf("%ld,%ld \n", sum, primeCounter);
    exit(0);
}
