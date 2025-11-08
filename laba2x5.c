#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define TARGET_LENGTH 80
#define MAX_LINE_LENGTH 1024

typedef enum Errors {
    overflow,
    memory_error,
    input_error,
    calculation_error,
    file_error,
    word_too_long_error,
    same_file_error
} Errors;

int handle_error(int key) {
    switch (key) {
        case overflow:
            printf("Number overflow\n");
            return 1;
        case memory_error:
            printf("Memory allocation failed\n");
            return 1;
        case input_error:
            printf("Invalid input data\n");
            return 1;
        case calculation_error:
            printf("Calculation failed\n");
            return 1;
        case file_error:
            printf("File operation failed\n");
            return 1;
        case word_too_long_error:
            printf("Word exceeds 80 characters\n");
            return 1;
        case same_file_error:
            printf("Input and output files are the same\n");
            return 1;
        default:
            printf("Unknown error\n");
            return 1;
    }
}

size_t my_str_len(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int my_str_cmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

void my_str_copy(char *dest, const char *src) {
    while (*src != '\0') {
        *dest++ = *src++;
    }
    *dest = '\0';
}

int is_readable_char(char c) {
    return isgraph(c);
}

int check_max_word_length(const char *str) {
    int max_len = 0;
    int current_len = 0;
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (is_readable_char(str[i])) {
            current_len++;
            if (current_len > max_len) {
                max_len = current_len;
            }
        } else {
            current_len = 0;
        }
    }
    return max_len;
}

char** split_into_words(const char *line, int *word_count) {
    *word_count = 0;
    int in_word = 0;
    size_t line_len = my_str_len(line);
    
    for (size_t i = 0; i < line_len; i++) {
        if (is_readable_char(line[i])) {
            if (!in_word) {
                (*word_count)++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    if (*word_count == 0) {
        return NULL;
    }
    char **words = (char**)malloc(*word_count * sizeof(char*));
    if (!words) {
        return NULL;
    }

    int word_index = 0;
    size_t word_start = 0;
    in_word = 0;
    
    for (size_t i = 0; i <= line_len; i++) {
        char current_char = line[i];
        int is_word_char = (i < line_len) ? is_readable_char(current_char) : 0;
        
        if (is_word_char) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                words[word_index] = (char*)malloc((word_len + 1) * sizeof(char));
                if (!words[word_index]) {
                    for (int j = 0; j < word_index; j++) {
                        free(words[j]);
                    }
                    free(words);
                    return NULL;
                }
                for (size_t j = 0; j < word_len; j++) {
                    words[word_index][j] = line[word_start + j];
                }
                words[word_index][word_len] = '\0';
                word_index++;
                in_word = 0;
            }
        }
    }
    
    return words;
}

char* create_formatted_line(char **words, int start_word, int end_word, int total_spaces) {
    int word_count = end_word - start_word + 1;
    
    if (word_count == 1) {
        size_t word_len = my_str_len(words[start_word]);
        char *result = (char*)malloc((word_len + 1) * sizeof(char));
        if (!result) return NULL;
        my_str_copy(result, words[start_word]);
        return result;
    }
    
    int spaces_between = total_spaces / (word_count - 1);
    int extra_spaces = total_spaces % (word_count - 1);
    int total_length = 0;

    for (int i = start_word; i <= end_word; i++) {
        total_length += my_str_len(words[i]);
    }

    total_length += total_spaces;
    char *result = (char*)malloc((total_length + 1) * sizeof(char));
    if (!result) return NULL;
    int pos = 0;

    for (int i = start_word; i <= end_word; i++) {
        char *word = words[i];
        while (*word != '\0') {
            result[pos++] = *word++;
        }

        if (i < end_word) {
            int spaces_to_add = spaces_between + (i - start_word < extra_spaces ? 1 : 0);
            for (int j = 0; j < spaces_to_add; j++) {
                result[pos++] = ' ';
            }
        }
    }
    
    result[pos] = '\0';
    return result;
}

int format_long_line(const char *line, FILE *output_file) {
    int word_count;
    char **words = split_into_words(line, &word_count);
    
    if (!words) {
        return word_count == 0 ? 0 : memory_error;
    }
    for (int i = 0; i < word_count; i++) {
        if (my_str_len(words[i]) > TARGET_LENGTH) {
            for (int j = 0; j < word_count; j++) {
                free(words[j]);
            }
            free(words);
            return word_too_long_error;
        }
    }
    
    int error_code = 0;
    int current_word = 0;
    
    while (current_word < word_count) {
        int words_in_line = 1;
        int current_length = my_str_len(words[current_word]);
        
        for (int i = current_word + 1; i < word_count; i++) {
            int new_length = current_length + my_str_len(words[i]) + 1;
            if (new_length <= TARGET_LENGTH) {
                words_in_line++;
                current_length = new_length;
            } else {
                break;
            }
        }
        int total_chars = 0;
        for (int i = current_word; i < current_word + words_in_line; i++) {
            total_chars += my_str_len(words[i]);
        }
        int total_spaces = TARGET_LENGTH - total_chars;
        char *formatted_line=create_formatted_line(words,current_word,current_word+words_in_line - 1,total_spaces);
        
        if (!formatted_line) {
            error_code = memory_error;
            break;
        }
        fprintf(output_file, "%s\n", formatted_line);
        free(formatted_line);
        current_word += words_in_line;
    }
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    free(words);
    return error_code;
}

ssize_t my_get_line(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        *n = 128;
        *lineptr = (char*)malloc(*n * sizeof(char));
        if (*lineptr == NULL) {
            return -1;
        }
    }
    
    size_t pos = 0;
    int c;
    while ((c = fgetc(stream)) != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n * 2;
            char *new_ptr = (char*)realloc(*lineptr, new_size * sizeof(char));
            if (new_ptr == NULL) {
                return -1;
            }
            *lineptr = new_ptr;
            *n = new_size;
        }
        
        (*lineptr)[pos++] = (char)c;
        if (c == '\n') {
            break;
        }
    }
    
    if (pos == 0 && c == EOF) {
        return -1;
    }
    
    (*lineptr)[pos] = '\0';
    return (ssize_t)pos;
}

int process_line(const char *line, FILE *output_file) {
    size_t len = my_str_len(line);
    
    if (len == 0) {
        fputc('\n', output_file);
        return 0;
    }
    int max_word_len = check_max_word_length(line);
    if (max_word_len > TARGET_LENGTH) {
        return word_too_long_error;
    }
    
    if (len <= TARGET_LENGTH) {
        fprintf(output_file, "%s\n", line);
        return 0;
    } else {
        return format_long_line(line, output_file);
    }
}

int files_same_check(const char *file1, const char *file2) {
    return my_str_cmp(file1, file2) == 0;
}

int process_file(const char *input_path, const char *output_path) {
    if (files_same_check(input_path, output_path)) {
        return same_file_error;
    }
    
    FILE *input_file = fopen(input_path, "r");
    if (!input_file) {
        return file_error;
    }
    FILE *output_file = fopen(output_path, "w");
    if (!output_file) {
        fclose(input_file);
        return file_error;
    }
    char *line = NULL;
    size_t len = 0;
    int error_code = 0;
    
    while (my_get_line(&line, &len, input_file) != -1) {
        size_t line_len = my_str_len(line);
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0';
        }
        
        error_code = process_line(line, output_file);
        if (error_code != 0) {
            break;
        }
    }
    
    if (error_code == 0 && ferror(input_file)) {
        error_code = file_error;
    }
    
    if (line) free(line);
    fclose(input_file);
    fclose(output_file);
    return error_code;
}

int main() {
    char input_path[256];
    char output_path[256];
    
    printf("Enter input file path: ");
    if (scanf("%255s", input_path) != 1) {
        return handle_error(input_error);
    }
    printf("Enter output file path: ");
    if (scanf("%255s", output_path) != 1) {
        return handle_error(input_error);
    }
    
    int result = process_file(input_path, output_path);
    if (result != 0) {
        return handle_error(result);
    }
    printf("Successfully!\n");
    return 0;
}