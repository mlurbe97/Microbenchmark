/*
## Microbenchmark: benchmark to saturate memory bandwidth.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022
*/

#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

cpu_set_t mask;

//Only for single testing
void assignToCore(int core_id){
  CPU_ZERO(&mask);
  CPU_SET(core_id,&mask);
  sched_setaffinity(0, sizeof(mask),&mask);
}

int main (int argc, char *argv[]) {
  printf("\nMicrobenchmark: benchmark to saturate memory bandwidth.\nAuthor: Manel Lurbe Sempere\ne-mail: malursem@gap.upv.es\nYear: 2022\n");
  int i, j, k, mem_accesses = 0;
  int *A;
  int nop_ops, mem_ops, stride, core, iter;
  int size = 1024*1024*40; //160MB
  if (argc < 4) {
    printf("Error: Missing arguments. (Number of arguments = %d). Usage: ./microbench iter nop_ops stride core\n", argc);
    return 1;
  }
  if (argc == 5) {
    core = atoi(argv[4]);
    assignToCore(core);
  }
  iter = atoi (argv[1]);
  nop_ops = atoi(argv[2]);
  stride = atoi(argv[3]);
  A = (int *) malloc (sizeof (int *) * size);
  //int A[N][256], B[N][256];
  //B = (int *) malloc (sizeof (int *) * N * M);
  //for (i=0; i<N; i++) {
  //  A [i] = (int *) malloc (sizeof (int) * M);
  //  B [i] = (int *) malloc (sizeof (int) * M);
  // }
  //sleep	(1);
  while (1) {
    for (k=0; k<size; k+=stride) {
      A[k]++;
      mem_accesses += 1;
    }
    for (i=0; i<nop_ops; i++) {
      asm("nop");
    }
  }
  sleep(5);
  printf("The theoretical number of memory accesses is: %d\n", mem_accesses);
	return 0;
}
