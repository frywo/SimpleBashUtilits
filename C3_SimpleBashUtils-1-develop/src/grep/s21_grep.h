#ifndef SRC_GREP_S21_GREP_H
#define SRC_GREP_S21_GREP_H
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct opt {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int error;
};
int main();
void Funct(int argc, char** argv, struct opt allopt, char E_pattern[100][100],
           int counter, int sum);
void Copy_arr(char* E_pattern, char* optarg);
void PatternWithoutE(char* E_pattern, char** argv);
void PrintRegEx(char E_pattern[100][100], struct opt allopt, int counter,
                char* file_name, FILE* f, int sum_Files, int* out);
void flag_I(struct opt allopt, regex_t* re, char E_pattern[100][100], int i);
void flag_V(struct opt allopt, int* status);
void print_File_name(struct opt allopt, char* file_name, int sum_Files,
                     int status);
void print_String(struct opt allopt, int status, char* str);
void stop_flag(struct opt allopt, int* stop, int status);
void sum_strings(struct opt allopt, int sum_Files, char* file_name,
                 int status_counter);
void switch_funct(struct opt allopt, int* counter, int* sum,
                  char E_pattern[100][100], char* optarg, int rez);
void helpFunction(struct opt allopt, int* counter, int* sum, int argc,
                  char E_pattern[100][100], char** argv);
void endOfFileCheck(char* Fix, FILE* f, int* stop);

#endif  // SRC_GREP_S21_GREP_H