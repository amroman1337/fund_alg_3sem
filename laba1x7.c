// ./laba1x7 /mnt/d/Users/roma1/Desktop/lab/in_laba7.txt /mnt/d/Users/roma1/Desktop/lab/out_laba7.txt
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable : 4996)

typedef enum errors {
    zero,
    file_error,
    same_files_error,
    amount_of_arguments_error,
    sign_error,
    input_error
} errors;

int error(int key) {
    if (!(key)) {
        return 0;
    }
    switch (key) {
    case file_error:
        printf("No such file or Directory");
        break;
    case same_files_error:
        printf("All files must be different");
        break;
    case amount_of_arguments_error:
        printf("You have mistake with amount of arguments");
        break;
    case input_error:
        printf("Unknown symbol");
        break;
    }
    return 1;
}

int is_str_equal(char* str1, char* str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) return 0;
        str1++;
        str2++;
    }
    if (!(*str1) && !(*str2)) return 1;
    return 0;
}

int GetOpts(int argc, char** argv) {
    if (argc != 3) {
        return amount_of_arguments_error;
    }
    if (is_str_equal(argv[1], argv[2])) {
        return same_files_error;
    }
    return 0;
}

int is_lower_case(char symbol) {
    return (symbol <= 'z') && (symbol >= 'a');
}

char upper_case(char symbol) {
    if (is_lower_case(symbol)) {
        return symbol - ' ';
    }
    return symbol;
}

int correct_input(char symbol) {
    return ((symbol >= '0') && (symbol <= 'Z') && (!((symbol >= ':') && (symbol <= '@'))));
}

int min_base(char max_element) {
    char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i = 0;
    while (alphabet[i] != max_element) {
        i++;
    }
    return i + 1;
}

int max_element(char* str, int* base) {
    char max_symbol = '0';
    char current_symbol;
    for (int i = 0; str[i] != '\0'; i++) {
        current_symbol = upper_case(str[i]);
        if (!(correct_input(current_symbol))) {
            return input_error;
        }
        if (current_symbol > max_symbol) {
            max_symbol = current_symbol;
        }
    }
    *base = min_base(max_symbol);
    return 0;
}

void delete_zeros(char* str, char* answer) {
    int i = 0;
    for (; str[i] != '\0'; i++) {
        if (str[i] != '0') {
            break;
        }
    }
    if (str[i] == '\0') {
        int j = 0;
        for (; str[j] != '\0'; j++) {
            answer[j] = '0';
        }
        answer[j] = '\0';
        return;
    }
    int j = i;
    for (; str[j] != '\0'; j++) {
        answer[j - i] = str[j];
    }
    answer[j - i] = '\0';
}

int find_index(char symbol, char* alphabet) {
    for (int i = 0; i < sizeof(alphabet); i++) {
        if (alphabet[i] == symbol) {
            return i;
        }
    }
}

int to_dec(char* str, int start_base) {
    int k = 0;
    while (str[k] != '\0') {
        k++;
    }
    if (start_base == 1) {
        return k;
    }
    k--;
    double result = 0;
    char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; str[i] != '\0'; i++, k--) {
        result += find_index(str[i], alphabet) * pow(start_base, k);
    }
    return result;
}

int solution(char** argv) {
    FILE* file1 = fopen(argv[1], "r");
    FILE* file2 = fopen(argv[2], "w");
    char str[255]; 
    char current_str[255];
    while (fscanf(file1, "%s", str) != EOF) {
        int base;
        if (max_element(str, &base)) {
            fprintf(file2, "Unknown symbol\n");
        }
        else if (base == 1) {
            int base10 = to_dec(str, base);
            fprintf(file2, "%s %d %d\n", str, base, base10);
        }
        else {
            delete_zeros(str, current_str);
            int base10 = to_dec(current_str, base);
            fprintf(file2, "%s %d %d\n", current_str, base, base10);
        }
    }
    fclose(file1);
    fclose(file2);
    return 0;
}

int main(int argc, char** argv) {
    int arguments = GetOpts(argc, argv);
    if (arguments) {
        return error(arguments);
    }
    int result = solution(argv);
	return error(result);
}