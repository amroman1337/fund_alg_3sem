// ./laba1x5 -ni /mnt/d/Users/roma1/Desktop/lab/in_laba1x5.txt /mnt/d/Users/roma1/Desktop/lab/out_laba1x5.txt
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

typedef enum errors {
    skip,
    file_eror,
    same_files
} errors;

typedef enum kOpts {
    OPT_D, 
    OPT_I,
    OPT_S, 
    OPT_A
} kOpts;

int letters(int asci_cod) {
    return ((asci_cod > 64) && (asci_cod < 123) && (!((asci_cod < 97) && (asci_cod > 90))));
}

int numbers(int asci_cod) {
    return ((asci_cod > 47)&&(asci_cod < 58));
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

char* insert_prefix(char* original_str, int index) {
    int original_length = 0;
    while (original_str[original_length] != '\0') {
        original_length++;
    }
    int final_length = original_length + 5;
    char* final_str = (char*)malloc(final_length * sizeof(char));

    int i = 0;
    for (; i <= index; i++) {
        final_str[i] = original_str[i];
    }
    char prefix[] = "out_";
    for (int j = 0; j < sizeof(prefix) - 1; j++, i++) {
        final_str[i] = prefix[j];
    }
    for (; original_str[i - 4] != '\0'; i++) {
        final_str[i] = original_str[i - 4];
    }
    final_str[i] = '\0';
    printf("%s\n", final_str);
    return final_str;
}

int HandlerOptD(char* input_file, char* output_file) {
    FILE* read_file = fopen(input_file, "r");
    FILE* write_file = fopen(output_file, "w");
    if (read_file == NULL) {
        fclose(write_file);
        return file_eror;
    }
    else if (is_str_equal(input_file, output_file)) {
        fclose(read_file);
        return same_files;
    }
    char t;
    int asci_cod;
    while (!(feof(read_file))) {
        t = fgetc(read_file);
        asci_cod = (int)t;
        if (!(numbers(asci_cod))) {
            fprintf(write_file, "%c", t);
        }
    }
    fclose(read_file);
    fclose(write_file);
    return 0;
}

int HandlerOptI(char* input_file, char* output_file) {
    FILE* read_file = fopen(input_file, "r");
    FILE* write_file = fopen(output_file, "w");
    if (read_file == NULL) {
        fclose(write_file);
        return file_eror;
    }
    else if (is_str_equal(input_file, output_file)) {
        fclose(read_file);
        return same_files;
    }
    char str[255];
    int amount_of_letters ;
    int asci_cod;
    while (fgets(str, sizeof(str), read_file) != NULL) {
        amount_of_letters = 0;
        for (int i = 0; (str[i] != '\0')&&(str[i] != '\n'); i++) {
            asci_cod = (int)str[i];
            if (letters(asci_cod)) {
                amount_of_letters++;
            }
        }
        fprintf(write_file, "%d\n", amount_of_letters);
    }
    fclose(read_file);
    fclose(write_file);
    return 0;
}

int HandlerOptS(char* input_file, char* output_file) {
    FILE* read_file = fopen(input_file, "r");
    FILE* write_file = fopen(output_file, "w");
    if (read_file == NULL) {
        fclose(write_file);
        return file_eror;
    }
    else if (is_str_equal(input_file, output_file)) {
        fclose(read_file);
        return same_files;
    }
    char str[255];
    int amount_of_letters;
    int asci_cod;
    while (fgets(str, sizeof(str), read_file) != NULL) {
        amount_of_letters = 0;
        for (int i = 0; (str[i] != '\0') && (str[i] != '\n'); i++) {
            asci_cod = (int)str[i];
            if ((!(letters(asci_cod)))&&(!(numbers(asci_cod)))&&(asci_cod != 32)) {
                printf("%c\n", str[i]);
                amount_of_letters++;
            }
        }
        fprintf(write_file, "%d\n", amount_of_letters);
    }
    fclose(read_file);
    fclose(write_file);
    return 0;
}

int HandlerOptA(char* input_file, char* output_file) {
    FILE* read_file = fopen(input_file, "r");
    FILE* write_file = fopen(output_file, "w");
    if (read_file == NULL) {
        fclose(write_file);
        return file_eror;
    }
    else if (is_str_equal(input_file, output_file)) {
        fclose(read_file);
        return same_files;
    }
    char t;
    int asci_cod;
    while (!(feof(read_file))) {
        t = fgetc(read_file);
        asci_cod = (int) t;
        printf("%c\n", t);
        if (numbers(asci_cod)) {
            fprintf(write_file, "%c", t);
        }
        else if (t != EOF) {
            fprintf(write_file, "%02x", asci_cod);
        }
    }
    fclose(read_file);
    fclose(write_file);
    return 0;
}

int GetOpts(int argc, char** argv, kOpts* option, int* exit_file) {
    const char* procceding_option = argv[1];
    if (procceding_option[0] == '/' || procceding_option[0] == '-') {
        int i = 1;
        if (procceding_option[1] == 'n') {
            *exit_file = 1;
            i++;
        }
        switch (procceding_option[i]) {
            case 'd':
                *option = OPT_D;
                break;
            case 'i':
                *option = OPT_I;
                break;
            case 's':
                *option = OPT_S;
                break;
            case 'a':
                *option = OPT_A;
                break;
            default:
                return 1;
        }
        }
    return 0;
}

int main(int argc, char** argv) {
    kOpts opt = 0;
    int exit_file = 0;
    int (*handlers[6])(char*, char*) = {
        HandlerOptD,
        HandlerOptI, 
        HandlerOptS, 
        HandlerOptA
    };

    if (GetOpts(argc, argv, &opt, &exit_file)) {
        printf("Incorrect options\n");
        return 1;
    }
    if (exit_file) {
        int result = handlers[opt](argv[2], argv[3]);
        printf("%s, %s\n", argv[2], argv[3]);
        FILE* read_file = fopen(argv[2], "r");
        if (result) {
            switch (result) {
            case file_eror:
                printf("mistake.No such input_file %s\n", argv[2]);
                break;
            case same_files:
                printf("input and output files must be different\n");
                break;
            }
        }
    }
    else {
        char* final_str = insert_prefix(argv[2], -1);
        handlers[opt](argv[2], final_str);
        free(final_str);
    }
    return 0;
}

