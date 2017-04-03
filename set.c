#include "mergesort.h"
#include "set.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////
// INTEGRITY INSTRUCTIONS

// Explicitly state the level of collaboration on this question
// Examples:
//   * I discussed ideas with classmate(s) [include name(s)]
//   * I worked together with classmate(s) in the lab [include name(s)]
//   * Classmate [include name] helped me debug my code
//   * I consulted website [include url]
//   * None
// A "None" indicates you completed this question entirely by yourself
// (or with assistance from course staff)
/////////////////////////////////////////////////////////////////////////////
// INTEGRITY STATEMENT:
// I received help from the following sources:

// NONE

// Name: Shuyi Lu
// login ID: sy6lu
/////////////////////////////////////////////////////////////////////////////

struct set{
  int len;
  int maxlen;
  int *values;
};

// set_create() returns a new empty set
// effects: allocates memory (caller must call set_destroy)
// time: O(1)
struct set *set_create(void){
  struct set *s = malloc(sizeof(struct set));
  s->len = 0;
  s->maxlen = 1;
  s->values = malloc(s->maxlen * sizeof(int));
  return s;
}

// set_destroy(s) frees all dynamically allocated memory 
// effects: the memory at s is invalid (freed)
// time: O(1)
void set_destroy(struct set *s){
  assert(s);
  free(s->values);
  free(s);
}

//  binsearch(item, a, len) finds the index of item in a, 
//    or returns -1 if it does not exist
//  requires: a is in sorted (ascending) order with no duplicates
//  time: O(logn)
int binsearch(const int item, const int a[], const int len) {
  int low = 0;
  int high = len-1;
  while (low <= high) {
    int mid = low + (high - low) / 2;
    if (a[mid] == item) {
      return mid;
    } else if (a[mid] < item) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }
  return -1;
}

// set_size(s) returns the number of elements in s
// time: O(1)
int set_size(const struct set *s){
  return s->len;
}

// set_member(s, i) determines if i is in set s
// time: O(logn)
bool set_member(const struct set *s, int i){
  assert(s);
  int index = binsearch(i, s->values, s->len);
  if (index == -1) {
    return 0;
  }
  return 1;
}

// set_add(s, i) adds i to the set s, if it does not already contain i
// effects: s may be modified
// time: O(n) where n is set_size(s)
void set_add(struct set *s, int i){
  if (set_member(s, i) == 1) return;
  else if (s->len >= s->maxlen) {
    s->maxlen *= 2;
    s->values = realloc(s->values, s->maxlen * sizeof(int));
  } else {
    s->values[s->len] = i;
    ++ (s->len);
  }
}

// set_remove(s, i) removes i from s. If i is not in s, s isn't changed
// effects: s may be modified
// time: O(n) where n is set_size(s)
void set_remove(struct set *s, int i){
  if (! set_member(s, i)) {
    return;
  }
  int index = 0;
  for (int k = 0; k < s->len; ++k){
    if (s->values[k] == i){
      index = k;
    }
  }
  for (int i = index; i < s-> len; ++i){
    s->values[i] = s->values[i+1];
  }
  s->len--;
}

// set_create_specify_maxlen(maxlen)create a set that could contain
// at most maxlen element
// effects: allocates memory for the new set (caller must call set_destroy)
// time: O(1)
struct set *set_create_specify_maxlen(int maxlen) {
  struct set *s = malloc(sizeof(struct set));
  s->maxlen = maxlen;
  s->values = malloc(maxlen *sizeof(int));
  return s;
}

// set_insert_value(s, value) inserts a value into s
// time: O(1)
void set_insert_value(struct set *s, int value){
  s->values[s->len++] = value;
}

// set_union(s1, s2) returns a new set that is the union of s1 and s2
// effects: allocates memory for the new set (caller must call set_destroy)
// time: O(n) where n is set_size(s1) + set_size(s2)
struct set *set_union(const struct set *s1, const struct set *s2){
  int maxlen = s1->len + s2->len;
  struct set *s = set_create_specify_maxlen(maxlen);
  s->len = 0;
  int p1 = 0, p2 = 0;
  while (p1 < s1->len && p2 < s2->len) {
    if (s1->values[p1] < s2->values[p2]) {
      s->values[s->len] = s1->values[p1];
      p1++;
    } else if (s1->values[p1] > s2->values[p2]) {
      s->values[s->len] = s2->values[p2];
      p2++;
    } else {
      s->values[s->len] = s1->values[p1];
      p1++;
      p2++;
    }
    s->len++;
  } 
  if (s1->len > p1) {
    for (int i = p1; i < s1->len; i++) {
      s->values[s->len] = s1->values[p1];
      s->len++;
    }
  }
  if (s2->len > p2) {
    for (int i = p2; i < s2->len; i++) {
      s->values[i] = s2->values[p1];
      s->len++;
    }
  }
  return s;
}

// set_intersect(s1, s2) returns a new set that is the intersection of s1 and s2
// effects: allocates memory for the new set (caller must call set_destroy)
// time: O(n) where n is set_size(s1) + set_size(s2)
struct set *set_intersect(const struct set *s1, const struct set *s2){
  int size;
  if (s1->len < s2->len){
    size = s1->len;
  } else {
    size = s2->len;
  }
  struct set *s = set_create_specify_maxlen(size);
  int p1 = 0, p2 = 0;
  s->len = 0;
  while (p1 < s1->len && p2 < s2->len) {
    if (s1->values[p1] == s2->values[p2]) {
      s->values[s->len] = s1->values[p1];
      p1++;
      p2++;
      s->len++;
    } else if (s1->values[p1] < s2->values[p2]) {
      p1++;
    } else {
      p2++;
    }
  }
  return s;
}

// array_to_set(a, len) returns a new set that is all the unique elements of a
// requires: len > 0
// effects: allocates memory for the new set (caller must call set_destroy)
// time: O(nlogn)
struct set *array_to_set(const int a[], int len){
  // copy array
  int* arr = malloc(sizeof(int) * len);
  for (int i = 0; i < len; i++) {
    arr[i] = a[i];
  }
  // sort
  merge_sort(arr, len);
  // create struct
  struct set* s = malloc(sizeof(struct set));
  s->values = arr;
  s->maxlen = len;
  s->len = 0;
  for (int i = 0; i < len; i++) {
    if (! set_member(s, arr[i])) {
      s->values[s->len] = arr[i];
      s->len++;
    }
  }
  return s;
}

// set_to_array(s) returns a new array that is the elements of s
//   in ascending order, or NULL if s is empty
// effects: may allocate memory for a new array (caller must call free)
// time: O(n)
int *set_to_array(const struct set *s){
  int *arr = malloc(sizeof(int) * s->len);
  for (int i = 0; i < s->len; ++i){
    arr[i] = s->values[i];
  }
  return arr;
}

// set_print(s) prints the elements of set s to the screen in ascending order.
//   using the format: "[element_smallest,element_next,...,element_largest]\n"
//   or "[empty]\n"
// effects: prints out a message
// time: O(n)
void set_print(const struct set *s){
  assert(s);
  if (s->len == 0){
    printf("[empty]\n");
  } else {
    printf ("[");
    for (int i = 0; i <s->len; ++i) {    
      if (i == s->len - 1) {
        printf("%d]\n", s->values[i]);
      } else {
        printf ("%d,", s->values[i]);
      }
    }
  }
}
