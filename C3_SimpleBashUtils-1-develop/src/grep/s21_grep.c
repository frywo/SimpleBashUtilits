#include "s21_grep.h"

int main(int argc, char* argv[]) {
  int rez;
  int counter = 0;
  int sum = 0;
  char E_pattern[100][100];
  struct opt allopt = {0};
  if (argc < 3) {
    fprintf(stderr, "Usage: s21_grep [OPTION]... PATTERNS [FILE]...\n");
    return 1;
  }
  while ((rez = getopt_long(argc, argv, "ivclne:", NULL, 0)) != -1) {
    switch (rez) {
      case 'e':
        allopt.e = 1;
        Copy_arr(E_pattern[counter], optarg);
        counter++;
        sum += 2;
        break;
      case 'i':
        allopt.i = 1;
        sum++;
        break;
      case 'v':
        allopt.v = 1;
        sum++;
        break;
      case 'c':
        allopt.c = 1;
        sum++;
        break;
      case 'l':
        allopt.l = 1;
        sum++;
        break;
      case 'n':
        allopt.n = 1;
        sum++;
        break;
      case '?':
        allopt.error = 1;
        sum++;
        break;
    }
  }
  helpFunction(allopt, &counter, &sum, argc, E_pattern, argv);
}

void Funct(int argc, char** argv, struct opt allopt, char E_pattern[100][100],
           int counter, int sum) {
  FILE* f;
  int stop = 0;
  int out = 0;
  for (int i = 1; i < argc; ++i) {
    if (counter == 0 &&
        argv[i][0] != '-') {  // компилю шаблон если нет флага -е
      if (stop == 1 && (f = fopen(argv[i], "r")) != NULL) {
        PrintRegEx(E_pattern, allopt, counter, argv[i], f, sum, &out);
        fclose(f);
      } else if (stop == 1) {
        fprintf(stderr, "s21_grep.c: %s No File or Directory1\n", argv[i]);
        out = 1;
      }

      stop = 1;
    }

    if (counter != 0 && argv[i][0] != '-') {  // компилю все шаблоны флага -е
      if (argv[i - 1][0] != '-' || argv[i - 1][1] != 'e') {
        if ((f = fopen(argv[i], "r")) != NULL) {
          PrintRegEx(E_pattern, allopt, counter, argv[i], f, sum, &out);
          fclose(f);
        } else {
          fprintf(stderr, "%s No File or Directory\n", argv[i]);
          out = 1;
        }
      }
    }
  }
  if (out == 0) {
    fprintf(stderr, "Nothing print\n");
  }
}

void Copy_arr(char* E_pattern, char* optarg) {
  int count = 0;
  for (int i = 0; optarg[i] != '\0'; ++i) {
    E_pattern[i] = optarg[i];
    count++;
  }
  E_pattern[count] = '\0';
}

void PatternWithoutE(char* E_pattern, char** argv) {
  for (int i = 1;; ++i) {
    if (argv[i][0] != '-') {
      Copy_arr(E_pattern, argv[i]);
      break;
    }
  }
}

void PrintRegEx(char E_pattern[100][100], struct opt allopt, int counter,
                char* file_name, FILE* f, int sum_Files, int* out) {
  char str[500];
  char Fix;
  int string_counter = 0;
  int status_counter = 0;
  int status;
  int stop = 0;
  regex_t re;
  if (counter == 0) {
    counter = 1;
  }  // если паттерн без е
  while (!feof(f) && stop != 1) {
    fgets(str, 500, f);
    string_counter++;
    for (int i = 0; i < counter; ++i) {
      flag_I(allopt, &re, E_pattern, i);  // задаю паттерн с учетом i флага
      status = regexec(&re, str, 0, NULL, 0);  // сравниваю строку с паттерном
      regfree(&re);  // освобождаю паттерн
      flag_V(allopt, &status);  // изменяю результат если флаг включен
      stop_flag(allopt, &stop, status);  // выхожу из фала если l включен
      if ((status == 0 && allopt.v == 0) || (status == 1 && allopt.v == 1)) {
        break;
      }  // когда нашлось совпадение стоп
    }
    if (status == 0) {  // счетчик совпадений
      status_counter++;
      *out = 1;
    }
    print_File_name(allopt, file_name, sum_Files, status);
    if (allopt.n == 1 && status == 0) {  // печатаю номер совпавшей строки
      printf("%d:", string_counter);
    }
    print_String(allopt, status, str);
    endOfFileCheck(&Fix, f, &stop);
  }
  sum_strings(allopt, sum_Files, file_name, status_counter);
  if (allopt.l == 1 && status_counter != 0) {
    printf("%s\n", file_name);
  }
}

void flag_I(struct opt allopt, regex_t* re, char E_pattern[100][100], int i) {
  if (allopt.i == 1) {  // компилю паттерн без учета регистров
    if (regcomp(re, E_pattern[i], REG_ICASE) != 0) {
      regfree(re);
    }
  } else {  // компилю паттерн  учитвыая регистры
    if (regcomp(re, E_pattern[i], 0) != 0) {
      regfree(re);
    }
  }
}

void flag_V(struct opt allopt, int* status) {
  if (allopt.v == 1) {  // изменяю результат сравнения
    if (*status == 0) {
      *status = 1;
    } else {
      *status = 0;
    }
  }
}

void print_File_name(struct opt allopt, char* file_name, int sum_Files,
                     int status) {
  if (sum_Files != 0 && status == 0) {  // печатаю имя файла если их несколько
    if (allopt.c == 0 && allopt.l == 0) {
      printf("%s:", file_name);
    }
  }
}

void print_String(struct opt allopt, int status, char* str) {
  if (status == 0 && allopt.l == 0 && allopt.c == 0) {  // печатаю строки
    for (int j = 0; str[j] != '\n' && str[j] != '\0'; ++j) {
      printf("%c", str[j]);
    }
    printf("\n");
  }
}

void stop_flag(struct opt allopt, int* stop, int status) {
  if (allopt.l == 1 && status == 0) {
    *stop = 1;
  }
}

void sum_strings(struct opt allopt, int sum_Files, char* file_name,
                 int status_counter) {
  if (allopt.c == 1) {  // печатаю количество совпавших строк
    if (sum_Files != 0) {
      printf("%s:", file_name);
    }
    printf("%d\n", status_counter);
  }
}

void helpFunction(struct opt allopt, int* counter, int* sum, int argc,
                  char E_pattern[100][100], char** argv) {
  if (*counter == 0) {
    *sum += 1;
  }
  if ((argc - *sum) >= 3) {
    *sum = 1;
  } else {
    *sum = 0;
  }
  if (*counter == 0) {
    PatternWithoutE(E_pattern[0], argv);
  }
  if (allopt.l == 1 || allopt.c == 1) {
    allopt.n = 0;
  }
  Funct(argc, argv, allopt, E_pattern, *counter, *sum);
}

void endOfFileCheck(char* Fix, FILE* f, int* stop) {
  if ((*Fix = getc(f)) == EOF) {
    *stop = 1;
  } else {
    fseek(f, -1, SEEK_CUR);
  }
}