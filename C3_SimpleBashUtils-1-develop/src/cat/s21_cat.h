#ifndef SRC_CAT_S21_CAT_H
#define SRC_CAT_S21_CAT_H
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
struct opt {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int error;
};
struct option long_option[] = {{"number-nonblank", 0, NULL, 1},
                               {"number", 0, NULL, 2},
                               {"squeeze-blank", 0, NULL, 3}};
int main();
void Funct(char** arr, struct opt all_opt, int count);
void Function_S(struct opt all_opt, char* txt1, char* txt2, int* check,
                FILE* f);
void Function_B(struct opt all_opt, char* txt1, char* txt2, int* number,
                FILE* f);
void Function_E(struct opt all_opt, char* txt1);
void Function_N(struct opt all_opt, char* txt1, char* txt2, int* number);
void Function_T(struct opt all_opt, char* txt1);
void Function_V(struct opt all_opt, char* txt1);

#endif  // SRC_CAT_S21_CAT_H
