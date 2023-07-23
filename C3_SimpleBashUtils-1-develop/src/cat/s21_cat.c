#include "s21_cat.h"

int main(int argc, char* argv[]) {
  int rez;
  struct opt allopt = {0};
  if (argc == 1) {
    fprintf(stderr, "Error: NO arguments");
    return 1;
  }
  while ((rez = getopt_long(argc, argv, "beEnstTv", long_option, NULL)) != -1) {
    switch (rez) {
      case 'b':
        allopt.b = 1;
        break;
      case 'v':
        allopt.v = 1;
        break;
      case 'e':
        allopt.e = 1;
        allopt.v = 1;
        break;
      case 'E':
        allopt.e = 1;
        break;
      case 'n':
        allopt.n = 1;
        break;
      case 's':
        allopt.s = 1;
        break;
      case 't':
        allopt.t = 1;
        allopt.v = 1;
        break;
      case 'T':
        allopt.t = 1;
        break;
      case 1:
        allopt.b = 1;
        break;
      case 2:
        allopt.n = 1;
        break;
      case 3:
        allopt.s = 1;
        break;
      case '?':
        allopt.error = 1;
        break;
    }
  }
  if (allopt.error == 0) {
    Funct(argv, allopt, argc);
  }
}

void Funct(char** arr, struct opt all_opt, int count) {
  char txt1 = '2';
  char txt2 = '2';
  int number = 1;
  int count1 = 0;
  int check = 0;
  FILE* f;
  for (int i = 1; i < count; ++i) {
    if (arr[i][0] != '-' && (f = fopen(arr[i], "r")) != NULL) {
      txt1 = getc(f);
      while (count1 != 1) {
        Function_S(all_opt, &txt1, &txt2, &check, f);
        Function_B(all_opt, &txt1, &txt2, &number, f);
        Function_N(all_opt, &txt1, &txt2, &number);
        Function_E(all_opt, &txt1);
        Function_T(all_opt, &txt1);
        Function_V(all_opt, &txt1);

        if (txt1 == EOF) {
          count1 = 1;
        }

        txt2 = txt1;
        txt1 = getc(f);
      }
      count1 = 0;

      fclose(f);
      txt1 = 1;
      txt2 = 1;
      number = 1;
    } else {
      if (arr[i][0] != '-' && f == NULL) {
        fprintf(stderr, "NO FILE\n");
      }
    }
  }
}

void Function_S(struct opt all_opt, char* txt1, char* txt2, int* check,
                FILE* f) {
  if (all_opt.s == 1) {
    if (*txt1 == '\n' && *txt2 == '\n') {
      while (*txt1 == '\n' && *txt1 != EOF) {
        *txt1 = getc(f);
      }
      if (*check == 1) {
      } else if (*txt1 == EOF) {
        fseek(f, -1, SEEK_CUR);
        *txt1 = getc(f);
      } else {
        fseek(f, -2, SEEK_CUR);
        *txt1 = getc(f);
      }
    }
    ++*check;
  }
}

void Function_B(struct opt all_opt, char* txt1, char* txt2, int* number,
                FILE* f) {
  if (all_opt.b == 1 && fseek(f, -2, SEEK_CUR) != 0) {
    if (*txt1 != '\n' && *txt1 != EOF && *number == 1) {
      printf("%6d\t", *number);
      ++(*number);
    }
  } else if (all_opt.b == 1) {
    fseek(f, 2, SEEK_CUR);
  }

  if (all_opt.b == 1) {
    if (*txt2 == '\n' && (*txt1 != '\n' && *txt1 != EOF)) {
      printf("%6d\t", *number);
      ++(*number);
    }
  }
}

void Function_E(struct opt all_opt, char* txt1) {
  if (all_opt.e == 1) {
    if (*txt1 == '\n') {
      printf("$");
    }
  }
}

void Function_N(struct opt all_opt, char* txt1, char* txt2, int* number) {
  if (all_opt.n == 1 && all_opt.b == 0 && *txt1 != EOF) {
    if (*number == 1) {
      printf("%6d\t", *number);
      ++*number;
    }
  }
  if (all_opt.n == 1 && all_opt.b == 0) {
    if (*txt2 == '\n' && *txt1 != EOF) {
      printf("%6d\t", *number);
      ++*number;
    }
  }
}

void Function_T(struct opt all_opt, char* txt1) {
  if (all_opt.t == 1) {
    if (*txt1 == 9) {
      printf("^I");
    }
  }
}

void Function_V(struct opt all_opt, char* txt1) {
  if (all_opt.v == 1) {
    if (all_opt.t == 1 && *txt1 == 9) {
    } else if (*txt1 != EOF) {
      if ((*txt1 > 31 && *txt1 < 127) || *txt1 == '\n' || *txt1 == 9) {
        printf("%c", *txt1);
      }
      if (*txt1 < 0) {
        *txt1 = 127 & *txt1;
        printf("M-%c", *txt1);
      }
      if (*txt1 == 127) {
        printf("^?");
      }
      if (*txt1 < 32 && *txt1 >= 0 && *txt1 != '\n' && *txt1 != 9) {
        *txt1 += 64;
        printf("^%c", *txt1);
      }
    }
  } else if (*txt1 != EOF) {
    if (all_opt.t == 1 && *txt1 == 9) {
    } else {
      printf("%c", *txt1);
    }
  }
}
