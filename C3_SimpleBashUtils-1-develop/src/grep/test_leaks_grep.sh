#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
RESULT=0
DIFF_RES=""

declare -a tests=(
"\055e s test_0_grep.txt VAR"
"\055e for s21_grep.c s21_grep.h Makefile VAR"
"\055e for s21_grep.c VAR"
"\055e for -e ^int s21_grep.c s21_grep.h Makefile VAR"
"\055e for -e ^int s21_grep.c VAR"
"\055e regex -e ^print s21_grep.c VAR  test_ptrn_grep.txt"
"\055e while -e void s21_grep.c Makefile VAR  test_ptrn_grep.txt"
)

declare -a extra=(
"-n for test_1_grep.txt test_2_grep.txt"
"-n for test_1_grep.txt"
"-n -e ^\} test_1_grep.txt"
"-c -e \/ test_1_grep.txt"
"-c -e ^int test_1_grep.txt test_2_grep.txt"
"\055e ^int test_1_grep.txt"
"-n -i -v = test_1_grep.txt test_2_grep.txt"
"\055e"
"-i -e INT test_5_grep.txt"
"-e -c -a test_1_grep.txt test_2_grep.txt"
"-n -e = -e out test_5_grep.txt"
"-i -v int test_5_grep.txt"
"-i -n int test_5_grep.txt"
"-c -l aboba test_1_grep.txt test_5_grep.txt"
"-v test_1_grep.txt -e ank"
"-l for test_1_grep.txt test_2_grep.txt"
"-e = -e out test_5_grep.txt"
"-e ing -e as -e the -e not -e is test_6_grep.txt"
"-l for no_file.txt test_2_grep.txt"
)

testing()
{
    t=$(echo -e $@ | sed "s/VAR/$var/")
    leaks -quiet -atExit -- ./s21_grep $t > test_s21_grep.log
    leak=$(grep -A100000 leaks test_s21_grep.log)
    (( COUNTER++ ))
    if [[ $leak == *"0 leaks for 0 total leaked bytes"* ]]
    then
      (( SUCCESS++ ))
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
#        echo -e "$leak"
    fi
    rm test_s21_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in v c l n
do
    for var2 in v c l n
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in v c l n
do
    for var2 in v c l n
    do
        for var3 in v c l n
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done




echo -e "\033[31mFAIL: $FAIL\033[0m"
echo -e "\033[32mSUCCESS: $SUCCESS\033[0m"
echo -e "ALL: $COUNTER"
