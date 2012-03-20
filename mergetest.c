/*
Christopher Antol
*/

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <sys/time.h>
#include <unistd.h>
#include "resource.h"
#include "mergesort.h"

/*calculates the difference of two times*/
static struct timeval tv_delta(struct timeval start, struct timeval end) { 
  struct timeval delta = end;
  delta.tv_sec -= start.tv_sec;
  delta.tv_usec -= start.tv_usec;
  if (delta.tv_usec < 0) {
    delta.tv_usec += 1000000;
    delta.tv_sec--;
  }
  return delta;
}

/* compare function */
static int cmp(const void * a, const void * b) {
  int *x = (int *) a;
  int *y = (int *) b;

  if ( *x > *y) return 1;
  if (*x == *y) return 0;
  return -1;
}

int main(int argc, char *argv[]) {
  int *array, i, num_ints, seed, max, *cpy;
  struct rusage start_ru, end_ru;
  struct timeval start_wall, end_wall, diff_wall, diff_user, diff_sys; 

  /* checks for correct number of arguments */
  if (argc != 4) {
    fprintf(stderr, "Usage: mergetest <# of integers> <seed> <upper bound>\n");
    exit(EX_USAGE);
  }

  /* turns arguments into integers */
  num_ints = atoi(argv[1]);
  seed = atoi(argv[2]);
  max = atoi(argv[3]);

  /* allocate memory for array and copy of array */
  array = calloc(num_ints, sizeof(int));
  if (!array)
    err(EX_OSERR, "memory allocation error");
  cpy = calloc(num_ints, sizeof(int));
  if (!cpy)
    err(EX_OSERR, "memory allocation error");

  /* seed rng, create num_ints sized arrays of random intengers no
     greater than the size of max */
  srand(seed);
  for (i = 0; i < num_ints; i++) {
    array[i] = rand() % max;
    cpy[i] = array[i];
  }
  
  /* 0 threads start */
  gettimeofday(&start_wall, NULL);
  getrusage(RUSAGE_SELF, &start_ru);

  mergesort(cpy, num_ints, sizeof(int), cmp);

  gettimeofday(&end_wall, NULL);
  getrusage(RUSAGE_SELF, &end_ru);

  diff_wall = (tv_delta(start_wall, end_wall));
  diff_user = (tv_delta(start_ru.ru_utime, end_ru.ru_utime));
  diff_sys = (tv_delta(start_ru.ru_stime, end_ru.ru_stime));
  
  printf("0 threads: %ld.%06lds wall; ", diff_wall.tv_sec, diff_wall.tv_usec);
  printf("%ld.%06lds user; ", diff_user.tv_sec, diff_user.tv_usec);
  printf("%ld.%06lds sys\n", diff_sys.tv_sec, diff_sys.tv_usec);
  /* 0 threads end */

  /*reset array*/
  for (i = 0; i < num_ints; i++)
    cpy[i] = array[i];

  /* 1 thread start */
  gettimeofday(&start_wall, NULL);
  getrusage(RUSAGE_SELF, &start_ru);

  mt_mergesort(cpy, num_ints, sizeof(int), cmp, 1);

  gettimeofday(&end_wall, NULL);
  getrusage(RUSAGE_SELF, &end_ru);

  diff_wall = (tv_delta(start_wall, end_wall));
  diff_user = (tv_delta(start_ru.ru_utime, end_ru.ru_utime));
  diff_sys = (tv_delta(start_ru.ru_stime, end_ru.ru_stime));
  
  printf("1 threads: %ld.%06lds wall; ", diff_wall.tv_sec, diff_wall.tv_usec);
  printf("%ld.%06lds user; ", diff_user.tv_sec, diff_user.tv_usec);
  printf("%ld.%06lds sys\n", diff_sys.tv_sec, diff_sys.tv_usec);
  /* 1 thread end */

  /*reset array*/
  for (i = 0; i < num_ints; i++)
    cpy[i] = array[i];

  /* 2 thread start */
  gettimeofday(&start_wall, NULL);
  getrusage(RUSAGE_SELF, &start_ru);

  mt_mergesort(cpy, num_ints, sizeof(int), cmp, 2);

  gettimeofday(&end_wall, NULL);
  getrusage(RUSAGE_SELF, &end_ru);

  diff_wall = (tv_delta(start_wall, end_wall));
  diff_user = (tv_delta(start_ru.ru_utime, end_ru.ru_utime));
  diff_sys = (tv_delta(start_ru.ru_stime, end_ru.ru_stime));
  
  printf("2 threads: %ld.%06lds wall; ", diff_wall.tv_sec, diff_wall.tv_usec);
  printf("%ld.%06lds user; ", diff_user.tv_sec, diff_user.tv_usec);
  printf("%ld.%06lds sys\n", diff_sys.tv_sec, diff_sys.tv_usec);
  /* 2 thread end */

  /* 4 thread start */
  gettimeofday(&start_wall, NULL);
  getrusage(RUSAGE_SELF, &start_ru);

  mt_mergesort(array, num_ints, sizeof(int), cmp, 4);

  gettimeofday(&end_wall, NULL);
  getrusage(RUSAGE_SELF, &end_ru);

  diff_wall = (tv_delta(start_wall, end_wall));
  diff_user = (tv_delta(start_ru.ru_utime, end_ru.ru_utime));
  diff_sys = (tv_delta(start_ru.ru_stime, end_ru.ru_stime));
  
  printf("4 threads: %ld.%06lds wall; ", diff_wall.tv_sec, diff_wall.tv_usec);
  printf("%ld.%06lds user; ", diff_user.tv_sec, diff_user.tv_usec);
  printf("%ld.%06lds sys\n", diff_sys.tv_sec, diff_sys.tv_usec);
  /* 4 thread end */

  
  free(array);
  free(cpy);
  

  return 0;
}
