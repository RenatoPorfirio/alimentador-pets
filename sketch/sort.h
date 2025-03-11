#ifndef _SORT_H_
#define _SORT_H_

template <class T>
static int hoare(T* array, int strt, int end, int (*cmp)(T a, T b)) {
  int pivot = array[strt];
  int i = strt - 1, j = end + 1;
  while(true) {
    for(j--; cmp(array[j], pivot) < 0; j--);
    for(i++; cmp(array[i], pivot) > 0; i++);
    if(j <= i) return j;
    T buffer = array[i];
    array[i] = array[j];
    array[j] = buffer;
  }
}

template <class T>
void quicksort(T* array, int strt, int end, int (*cmp)(T a, T b)) {
  if(end <= strt) return;
  int part = hoare(array, strt, end, cmp);
  quicksort(array, strt, part, cmp);
  quicksort(array, part + 1, end, cmp);
}

#endif