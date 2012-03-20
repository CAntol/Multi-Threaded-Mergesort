#ifndef MERGESORT_H
#define MERGESORT_H

typedef int (*Compare_fn)(const void *, const void *);

void mergesort(void *arr, size_t num_elem, size_t elem_size, Compare_fn cmp);
void mt_mergesort(void *arr, size_t num_elem, size_t elem_size, Compare_fn cmp,
                  int num_threads);

#endif
