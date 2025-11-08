#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define STDCAP 4

typedef enum DialogFSM {
    COMMAND_WAIT,
    COMMAND_HANDLE,
    PARAMETER_WAIT
} DialogFSM;

typedef enum Command {
    HELP,
    PRINT,
    SEARCH,
    SORT,
    FIND_ID,
    ABOVE_AVERAGE,
    EXIT
} Command;

typedef enum SearchCriteria {
    ID,
    NAME,
    SURNAME,
    GROUP,
    MEAN
} SearchCriteria;

typedef struct Array {
    char *val;
    unsigned int length;
    unsigned int capacity;
} Array;

typedef union SearchParameter {
    unsigned int id;
    Array str;
    float mean;
} SearchParameter;

typedef struct Student {
    unsigned int id;
    Array name;
    Array surname;
    Array group;
    unsigned char *grades;
} Student;

typedef struct StudentArr {
    Student *val;
    unsigned int length;
    unsigned int capacity;
} StudentArr;

int throw_err(int err_code);
int is_str_equal(const char *str1, const char *str2);
int is_num(char c);
int is_alnum(char c);
int is_id_unique(StudentArr *students, unsigned int id);
int create_arr(unsigned int length, Array *arr);
void destroy(Array *arr);
void reset(Array *arr);
int copy(Array *dst, const Array *src);
int arr_compare(Array a, Array b);
int is_arr_equal(Array a, Array b);
int overfscanf(FILE *stream, const char *format, ...);
int check_paths(char *in, char *out);

int create_studarr(unsigned int length, StudentArr *arr);
int resize_stud(StudentArr *arr, int size_delta);
int extend_stud(StudentArr *arr);
int append_stud(StudentArr *arr, const Student value);
void destroy_stud(StudentArr *arr);
int create_student(Student *student);
void reset_student(Student *student);
void destroy_student(Student *student);

int read_students(FILE *in, StudentArr *students);
int write_students(FILE *out, StudentArr students, int mean);
int cmp_student_id(const void *a, const void *b);
int cmp_student_name(const void *a, const void *b);
int cmp_student_surname(const void *a, const void *b);
int cmp_student_group(const void *a, const void *b);
float student_mean(Student student);
float all_student_mean(StudentArr students);
int student_search(const StudentArr students, int criteria, SearchParameter param, StudentArr *result);

int dialog_manager(FILE* in, FILE* out);
int wait_command(Command *result);
int find_id_cmd(StudentArr students, FILE *out);
int above_average_cmd(StudentArr students, FILE *out);
int handle_command(Command cmd, StudentArr *students, FILE *out);
int wait_param(Command cmd, int *param);
int wait_search_param(SearchCriteria criteria, SearchParameter *searchParam);
void cmd_description(Command cmd);
void help_cmd();
int print_cmd(FILE* out, StudentArr students, int param);
int search_cmd(StudentArr students, SearchCriteria criteria, SearchParameter searchParam, StudentArr *found_students);
int sort_cmd(StudentArr *students, int param);

#define MEMORY_NOT_ALLOCATED 1
#define INCORRECT_INPUT_DATA 2
#define INCORRECT_OPTION 3
#define INCORRECT_ARGUMENTS 4
#define FILE_ERROR 5

int throw_err(int err_code) {
    return err_code;
}

int is_str_equal(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) return 0;
    return strcmp(str1, str2) == 0;
}

int is_num(char c) {
    return c >= '0' && c <= '9';
}

int is_alnum(char c) {
    return (c >= '0' && c <= '9');
}

int is_id_unique(StudentArr *students, unsigned int id) {
    for (int i = 0; i < students->length; ++i) {
        if (students->val[i].id == id) {
            return 0;
        }
    }
    return 1;
}

int create_arr(unsigned int length, Array *arr) {
    if (arr == NULL) return MEMORY_NOT_ALLOCATED;
    
    arr->capacity = length > 0 ? length : 1;
    arr->length = 0;
    arr->val = (char *)malloc(arr->capacity * sizeof(char));
    if (!arr->val) return MEMORY_NOT_ALLOCATED;
    arr->val[0] = '\0';
    return 0;
}

void destroy(Array *arr) {
    if (arr && arr->val) {
        free(arr->val);
        arr->val = NULL;
    }
    if (arr) {
        arr->length = 0;
        arr->capacity = 0;
    }
}

void reset(Array *arr) {
    if (arr && arr->val) {
        arr->length = 0;
        arr->val[0] = '\0';
    }
}

int copy(Array *dst, const Array *src) {
    if (dst == NULL || src == NULL || src->val == NULL) return 1;
    
    if (dst->capacity < src->length + 1) {
        char *new_val = realloc(dst->val, src->length + 1);
        if (!new_val) return 1;
        dst->val = new_val;
        dst->capacity = src->length + 1;
    }
    strcpy(dst->val, src->val);
    dst->length = src->length;
    return 0;
}

int arr_compare(Array a, Array b) {
    if (a.val == NULL || b.val == NULL) return 0;
    return strcmp(a.val, b.val);
}

int is_arr_equal(Array a, Array b) {
    if (a.val == NULL || b.val == NULL) return 0;
    return strcmp(a.val, b.val) == 0;
}

int overfscanf(FILE *stream, const char *format, ...) {
    if (stream == NULL || format == NULL) return -1;
    
    va_list args;
    va_start(args, format);
    int items_read = 0;
    const char *p = format;
    
    while (*p) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'S': {
                    Array *arr = va_arg(args, Array*);
                    if (arr == NULL) {
                        va_end(args);
                        return -1;
                    }
                    char buffer[256];
                    if (fscanf(stream, "%255s", buffer) != 1) {
                        va_end(args);
                        return -1;
                    }
                    if (arr->capacity < strlen(buffer) + 1) {
                        char *new_val = realloc(arr->val, strlen(buffer) + 1);
                        if (!new_val) {
                            va_end(args);
                            return -1;
                        }
                        arr->val = new_val;
                        arr->capacity = strlen(buffer) + 1;
                    }
                    strcpy(arr->val, buffer);
                    arr->length = strlen(buffer);
                    items_read++;
                    break;
                }
                case 'u': {
                    unsigned int *value = va_arg(args, unsigned int*);
                    if (value == NULL) {
                        va_end(args);
                        return -1;
                    }
                    if (fscanf(stream, "%u", value) != 1) {
                        va_end(args);
                        return -1;
                    }
                    items_read++;
                    break;
                }
                case 'h': {
                    if (*(p+1) == 'h' && *(p+2) == 'u') {
                        p += 2;
                        unsigned char *value = va_arg(args, unsigned char*);
                        if (value == NULL) {
                            va_end(args);
                            return -1;
                        }
                        unsigned int temp;
                        if (fscanf(stream, "%u", &temp) != 1) {
                            va_end(args);
                            return -1;
                        }
                        *value = (unsigned char)temp;
                        items_read++;
                    }
                    break;
                }
                default:
                    p--;
                    break;
            }
        }
        p++;
    }
    va_end(args);
    return items_read;
}

int check_paths(char *in, char *out) {
    if (in == NULL || out == NULL) return 1;
    return strcmp(in, out) == 0;
}

int create_studarr(unsigned int length, StudentArr *arr) {
    if (arr == NULL) return MEMORY_NOT_ALLOCATED;
    
    arr->capacity = length > 0 ? length : 1;
    arr->length = 0;
    arr->val = (Student *)malloc(arr->capacity * sizeof(Student));
    if (!arr->val) return MEMORY_NOT_ALLOCATED;
    for (unsigned int i = 0; i < arr->capacity; i++) {
        arr->val[i].id = 0;
        arr->val[i].name.val = NULL;
        arr->val[i].surname.val = NULL;
        arr->val[i].group.val = NULL;
        arr->val[i].grades = NULL;
    }
    return 0;
}

int resize_stud(StudentArr *arr, int size_delta) {
    if (arr == NULL || arr->val == NULL) return MEMORY_NOT_ALLOCATED;
    
    unsigned int new_capacity = arr->capacity + size_delta;
    if (new_capacity <= 0) return MEMORY_NOT_ALLOCATED;
    
    Student *new_addr = (Student *)realloc(arr->val, new_capacity * sizeof(Student));
    if (!new_addr) {
        return MEMORY_NOT_ALLOCATED;
    }
    arr->val = new_addr;
    for (unsigned int i = arr->capacity; i < new_capacity; i++) {
        arr->val[i].id = 0;
        arr->val[i].name.val = NULL;
        arr->val[i].surname.val = NULL;
        arr->val[i].group.val = NULL;
        arr->val[i].grades = NULL;
    }
    arr->capacity = new_capacity;
    return 0;
}

int extend_stud(StudentArr *arr) {
    if (arr == NULL) return MEMORY_NOT_ALLOCATED;
    return resize_stud(arr, arr->capacity > 0 ? arr->capacity : 1);
}

int append_stud(StudentArr *arr, const Student value) {
    if (arr == NULL) return MEMORY_NOT_ALLOCATED;
    
    if (arr->length >= arr->capacity) {
        int err = extend_stud(arr);
        if (err) return err;
    }

    Student *cur = &arr->val[arr->length];
    int err = create_student(cur);
    if (err) return err;
    
    cur->id = value.id;
    if (value.grades != NULL) {
        cur->grades = malloc(sizeof(unsigned char) * 5);
        if (!cur->grades) {
            destroy_student(cur);
            return MEMORY_NOT_ALLOCATED;
        }
        for (int i = 0; i < 5; ++i) {
            cur->grades[i] = value.grades[i];
        }
    }

    if (copy(&cur->name, &value.name) != 0 ||
        copy(&cur->surname, &value.surname) != 0 ||
        copy(&cur->group, &value.group) != 0) {
        destroy_student(cur);
        return MEMORY_NOT_ALLOCATED;
    }
    arr->length++;
    return 0;
}

void destroy_stud(StudentArr *arr) {
    if (arr == NULL) return;
    
    if (arr->val) {
        for (unsigned int i = 0; i < arr->length; ++i) {
            destroy_student(&arr->val[i]);
        }
        free(arr->val);
        arr->val = NULL;
    }
    arr->length = 0;
    arr->capacity = 0;
}

int create_student(Student *student) {
    if (student == NULL) return MEMORY_NOT_ALLOCATED;
    student->id = 0;
    student->grades = NULL;
    
    int err = create_arr(5, &student->name);
    if (err) return err;

    err = create_arr(5, &student->surname);
    if (err) {
        destroy(&student->name);
        return err;
    }

    err = create_arr(5, &student->group);
    if (err) {
        destroy(&student->name);
        destroy(&student->surname);
        return err;
    }
    return 0;
}

void reset_student(Student *student) {
    if (student == NULL) return;
    
    reset(&student->name);
    reset(&student->surname);
    reset(&student->group);
}

void destroy_student(Student *student) {
    if (student == NULL) return;
    
    destroy(&student->name);
    destroy(&student->surname);
    destroy(&student->group);
    
    if (student->grades != NULL) {
        free(student->grades);
        student->grades = NULL;
    }
}

int read_students(FILE *in, StudentArr *students) {
    if (in == NULL || students == NULL) return FILE_ERROR;
    
    Student cur;
    int err = create_student(&cur);
    if (err) return err;
    int duplicate_count = 0;
    
    while (1) {
        unsigned char first_grade = 0;
        int scan_read = overfscanf(in, "%u %S %S %S %hhu", &cur.id, &cur.name, &cur.surname, &cur.group, &first_grade);
        
        if (scan_read == -1) break;

        if (scan_read != 5 || cur.name.length == 0 || cur.surname.length == 0 || cur.group.length == 0) {
            printf("Error: Incorrect data format\n");
            destroy_student(&cur);
            create_student(&cur);
            continue;
        }

        if (!is_id_unique(students, cur.id)) {
            printf("\n\n WARNING: Duplicate ID %u found for student %s %s. Skipping.\n\n", 
                   cur.id, cur.name.val, cur.surname.val);
            duplicate_count++;
            reset_student(&cur);
            continue;
        }

        unsigned char *cur_grades = malloc(sizeof(unsigned char) * 5);
        if (!cur_grades) {
            destroy_student(&cur);
            return MEMORY_NOT_ALLOCATED;
        }
        cur_grades[0] = first_grade;
        int length = 1;
        int ch;

        for (int i = 0; i < 4; i++) {
            if (fscanf(in, "%hhu", &cur_grades[length]) == 1) {
                length++;
            }
        }

        while ((ch = fgetc(in)) != '\n' && ch != EOF);

        if (length != 5) {
            free(cur_grades);
            destroy_student(&cur);
            create_student(&cur);
            continue;
        }

        cur.grades = cur_grades;
        err = append_stud(students, cur);
        if (err) {
            free(cur_grades);
            destroy_student(&cur);
            return err;
        }

        free(cur_grades);
        cur.grades = NULL;
        reset_student(&cur);
    }

    destroy_student(&cur);
    if (duplicate_count > 0) {
        printf("Skipped %d students with duplicate IDs\n", duplicate_count);
    }
    return 0;
}

int write_students(FILE *out, StudentArr students, int mean) {
    if (out == NULL) return FILE_ERROR;
    
    for (int i = 0; i < students.length; ++i) {
        Student cur = students.val[i];
        if (cur.name.val == NULL || cur.surname.val == NULL || cur.group.val == NULL) continue;
        fprintf(out, "%.3d | %s %s | %s | ", cur.id, cur.name.val, cur.surname.val, cur.group.val);
        printf("%.3d | %s %s | %s | ", cur.id, cur.name.val, cur.surname.val, cur.group.val);
        
        if (mean) {
            float mean_val = student_mean(cur);
            fprintf(out, "%.2f", mean_val);
            printf("%.2f", mean_val);
        } else {
            if (cur.grades != NULL) {
                for (int j = 0; j < 5; ++j) {
                    fprintf(out, "%hhu ", cur.grades[j]);
                    printf("%hhu ", cur.grades[j]);
                }
            }
        }
        fprintf(out, "\n");
        printf("\n");
    }

    fprintf(out, "\n");
    printf("\n");
    return 0;
}

int cmp_student_id(const void *a, const void *b) {
    const Student *student1 = (const Student *)a;
    const Student *student2 = (const Student *)b;
    return (student1->id > student2->id) - (student1->id < student2->id);
}

int cmp_student_name(const void *a, const void *b) {
    const Student *student1 = (const Student *)a;
    const Student *student2 = (const Student *)b;
    return arr_compare(student1->name, student2->name);
}

int cmp_student_surname(const void *a, const void *b) {
    const Student *student1 = (const Student *)a;
    const Student *student2 = (const Student *)b;
    return arr_compare(student1->surname, student2->surname);
}

int cmp_student_group(const void *a, const void *b) {
    const Student *student1 = (const Student *)a;
    const Student *student2 = (const Student *)b;
    return arr_compare(student1->group, student2->group);
}

float student_mean(Student student) {
    float mean = 0;
    if (student.grades != NULL) {
        for (int i = 0; i < 5; ++i) {
            mean += (float)student.grades[i];
        }
        mean /= 5.0f;
    }
    return mean;
}

float all_student_mean(StudentArr students) {
    if (students.length == 0) return 0.0f;
    
    float mean = 0;
    for (int i = 0; i < students.length; ++i) {
        mean += student_mean(students.val[i]);
    }
    return mean / (float)students.length;
}

int student_search(const StudentArr students, int criteria, SearchParameter param, StudentArr *result) {
    if (result == NULL) return -1;
    
    double eps = 0.0001;
    int found = 0;

    for (int i = 0; i < students.length; ++i) {
        Student cur = students.val[i];
        int match = 0;
        
        switch (criteria) {
            case 0:
                if (cur.id == param.id) match = 1;
                break;
            case 1:
                if (is_arr_equal(cur.name, param.str)) match = 1;
                break;
            case 2:
                if (is_arr_equal(cur.surname, param.str)) match = 1;
                break;
            case 3:
                if (is_arr_equal(cur.group, param.str)) match = 1;
                break;
            case 4:
                if (student_mean(cur) > param.mean - eps && 
                    student_mean(cur) < param.mean + eps) match = 1;
                break;
            default:
                return -1;
        }
        if (match) {
            int err = append_stud(result, cur);
            if (err) return -1;
            found++;
        }
    }
    return found;
}

int wait_command(Command *result) {
    if (result == NULL) return 1;
    
    Array buf;
    int err = create_arr(32, &buf);
    if (err) return err;

    while (1) {
        printf("> ");
        fflush(stdout);
        int scan_result = overfscanf(stdin, "%S", &buf);

        if (scan_result <= 0) {
            destroy(&buf);
            return 1;
        }

        if (is_str_equal(buf.val, "^Z") || is_str_equal(buf.val, "exit") || is_str_equal(buf.val, "quit")) {
            *result = EXIT;
            break;
        } else if (is_str_equal(buf.val, "help")) {
            *result = HELP;
            break;
        } else if (is_str_equal(buf.val, "print")) {
            *result = PRINT;
            break;
        } else if (is_str_equal(buf.val, "search")) {
            *result = SEARCH;
            break;
        } else if (is_str_equal(buf.val, "sort")) {
            *result = SORT;
            break;
        } else if (is_str_equal(buf.val, "findid")) {
            *result = FIND_ID;
            break;
        } else if (is_str_equal(buf.val, "aboveavg")) {
            *result = ABOVE_AVERAGE;
            break;
        } else if (is_num(buf.val[0]) && buf.length == 1) {
            int cmd_num = buf.val[0] - '0' - 1;
            if (cmd_num >= HELP && cmd_num <= EXIT) {
                *result = (Command)cmd_num;
                break;
            } else {
                printf("Invalid command number. Please try again or use 'help' command.\n");
            }
        } else {
            printf("Incorrect command. Please try again or use 'help' command.\n");
        }
        reset(&buf);
    }
    destroy(&buf);
    return 0;
}

int find_id_cmd(StudentArr students, FILE *out) {
    if (out == NULL) return FILE_ERROR;
    
    unsigned int search_id;
    printf("Enter student ID to find: ");
    
    if (scanf("%u", &search_id) != 1) {
        printf("Invalid ID format.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }
    
    int found = 0;
    for (int i = 0; i < students.length; ++i) {
        Student cur = students.val[i];
        if (cur.id == search_id && cur.name.val != NULL && cur.surname.val != NULL && cur.group.val != NULL) {
            fprintf(out, "=== STUDENT BY ID %u ===\n", search_id);
            fprintf(out, "Name: %s %s\n", cur.name.val, cur.surname.val);
            fprintf(out, "Group: %s\n", cur.group.val);
            fprintf(out, "Mean grade: %.2f\n", student_mean(cur));
            fprintf(out, "====================\n\n");
            
            printf("=== STUDENT BY ID %u ===\n", search_id);
            printf("Name: %s %s\n", cur.name.val, cur.surname.val);
            printf("Group: %s\n", cur.group.val);
            printf("Mean grade: %.2f\n", student_mean(cur));
            printf("====================\n\n");
            
            found = 1;
            break;
        }
    }
    
    if (!found) {
        fprintf(out, "Student with ID %u not found\n\n", search_id);
        printf("Student with ID %u not found\n\n", search_id);
    }
    
    return 0;
}

int above_average_cmd(StudentArr students, FILE *out) {
    if (out == NULL) return FILE_ERROR;
    
    float overall_mean = all_student_mean(students);
    int count = 0;
    
    fprintf(out, "=== STUDENTS ABOVE AVERAGE (%.2f) ===\n", overall_mean);
    printf("=== STUDENTS ABOVE AVERAGE (%.2f) ===\n", overall_mean);
    
    for (int i = 0; i < students.length; ++i) {
        Student cur = students.val[i];
        if (cur.name.val == NULL || cur.surname.val == NULL || cur.group.val == NULL) continue;
            
        float student_avg = student_mean(cur);
        
        if (student_avg > overall_mean) {
            fprintf(out, "%s %s (Group: %s) - Mean: %.2f\n", 
                    cur.name.val, cur.surname.val, cur.group.val, student_avg);
            printf("%s %s (Group: %s) - Mean: %.2f\n", 
                   cur.name.val, cur.surname.val, cur.group.val, student_avg);
            count++;
        }
    }
    
    if (count == 0) {
        fprintf(out, "No students above average found\n");
        printf("No students above average found\n");
    } else {
        fprintf(out, "Total: %d students\n", count);
        printf("Total: %d students\n", count);
    }
    
    fprintf(out, "================================\n\n");
    printf("================================\n\n");
    
    return 0;
}

void cmd_description(Command cmd) {
    int level = 1;
    int j = 1;

    char* cmd_descriptions[7][10] = {
        {"'help' - Print all available commands and their options"},
        {"'print' - Print information about students", "All grades", "Mean grade only", NULL},
        {"'search' - Search students by parameters", "ID", "Name", "Surname", "Group", "Mean", NULL},
        {"'sort' - Sort students by parameters", "ID", "Name", "Surname", "Group", NULL},
        {"'findid' - Find student by ID and display detailed info", NULL},
        {"'aboveavg' - Display students with above average grades", NULL},
        {"'exit' - Exit the program", NULL}
    };

    if (cmd < HELP || cmd > EXIT) return;
    
    printf("%*d. %s.\n", level * 2, cmd + 1, cmd_descriptions[cmd][0]);
    level++;
    
    while (cmd_descriptions[cmd][j] != NULL) {
        printf("%*d) %s.\n", level * 2, j, cmd_descriptions[cmd][j]);
        j++;
    }
}

void help_cmd() {
    int cmd_amount = 7;
    printf("Available commands (you can use them by typing numbers corresponding or command strings):\n");
    for (int i = 0; i < cmd_amount; ++i) {
        cmd_description((Command)i);
    }
}

int wait_param(Command cmd, int *param) {
    if (param == NULL) return 1;
    switch (cmd) {
        case PRINT:
            cmd_description(PRINT);
            break;
        case SEARCH:
            cmd_description(SEARCH);
            break;
        case SORT:
            cmd_description(SORT);
            break;
        default:
            return INCORRECT_OPTION;
    }
    
    printf("Please enter function parameter: ");
    fflush(stdout);
    int scan_read = scanf("%d", param);
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
    if (scan_read == EOF) {
        return EOF;
    } else if (scan_read != 1) {
        printf("Incorrect parameter. Please try again.\n");
        return 1;
    } else {
        *param -= 1;
        return 0;
    }
}

int wait_search_param(SearchCriteria criteria, SearchParameter *searchParam) {
    if (searchParam == NULL) return 1;
    printf("Please enter parameter to search for: ");
    fflush(stdout);
    int scan_read = 0;

    switch (criteria) {
        case ID:
            scan_read = scanf("%u", &searchParam->id);
            break;
        case MEAN:
            scan_read = scanf("%f", &searchParam->mean);
            break;
        case NAME:
        case SURNAME:
        case GROUP:
            if (create_arr(32, &searchParam->str) != 0) return 1;
            scan_read = overfscanf(stdin, "%S", &searchParam->str);
            break;
        default:
            return INCORRECT_OPTION;
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if (scan_read == EOF) {
        return 1;
    } else if (scan_read <= 0) {
        printf("Incorrect parameter format.\n");
        return 1;
    } else {
        return 0;
    }
}

int print_cmd(FILE* out, StudentArr students, int param) {
    if (param < 0 || param > 1) {
        return INCORRECT_OPTION;
    }
    return write_students(out, students, param);
}

int search_cmd(StudentArr students, SearchCriteria criteria, SearchParameter searchParam, StudentArr *found_students) {
    if (found_students == NULL) return 1;
    int found = student_search(students, criteria, searchParam, found_students);
    printf("\nFound %d occurrences:\n", found);
    return 0;
}

int sort_cmd(StudentArr *students, int param) {
    if (students == NULL || students->val == NULL) return 1;
    
    switch (param) {
        case 0:
            qsort(students->val, students->length, sizeof(Student), cmp_student_id);
            break;
        case 1:
            qsort(students->val, students->length, sizeof(Student), cmp_student_name);
            break;
        case 2:
            qsort(students->val, students->length, sizeof(Student), cmp_student_surname);
            break;
        case 3:
            qsort(students->val, students->length, sizeof(Student), cmp_student_group);
            break;
        default:
            return INCORRECT_OPTION;
    }
    return 0;
}

int handle_command(Command cmd, StudentArr *students, FILE *out) {
    if (students == NULL || out == NULL) return 1;
    
    int param = 0;
    int err = 0;
    
    switch (cmd) {
        case HELP:
            help_cmd();
            break;
        case PRINT:
            err = wait_param(cmd, &param);
            if (err == EOF) return EOF;
            if (err) break;
            if (param < 0 || param > 1) {
                printf("Invalid parameter for print command.\n");
                break;
            }
            err = print_cmd(out, *students, param);
            if (err) return err;
            break;
        case SEARCH: {
            err = wait_param(cmd, &param);
            if (err == EOF) return EOF;
            if (err) break;
            if (param < ID || param > MEAN) {
                printf("Invalid parameter for search command.\n");
                break;
            }
            SearchParameter searchParameter = {0};
            int need_cleanup = 0;
            if (param != MEAN) {
                err = create_arr(32, &searchParameter.str);
                if (err) return err;
                need_cleanup = 1;
            }
            err = wait_search_param((SearchCriteria)param, &searchParameter);
            if (err) {
                if (need_cleanup) destroy(&searchParameter.str);
                break;
            }
            StudentArr found = {0};
            err = create_studarr(5, &found);
            if (err) {
                if (need_cleanup) destroy(&searchParameter.str);
                return err;
            }
            err = search_cmd(*students, (SearchCriteria)param, searchParameter, &found);
            if (!err) {
                err = write_students(out, found, 1);
            }

            destroy_stud(&found);
            if (need_cleanup) destroy(&searchParameter.str);
            break;
        }
        case SORT:
            err = wait_param(cmd, &param);
            if (err == EOF) return EOF;
            if (err) break;
            if (param < ID || param > GROUP) {
                printf("Invalid parameter for sort command.\n");
                break;
            }
            err = sort_cmd(students, param);
            if (err) return err;
            break;
        case FIND_ID:
            err = find_id_cmd(*students, out);
            if (err) return err;
            break; 
        case ABOVE_AVERAGE:
            err = above_average_cmd(*students, out);
            if (err) return err;
            break;  
        case EXIT:
            printf("Goodbye!\n");
            fprintf(out, "=== PROGRAM EXITED BY USER ===\n");
            return EOF;  
        default:
            printf("Unknown command.\n");
            break;
    }
    return 0;
}

int dialog_manager(FILE* in, FILE* out) {
    if (in == NULL || out == NULL) return FILE_ERROR;
    DialogFSM state = COMMAND_WAIT;
    Command cmd;
    StudentArr students = {0};
    int err = create_studarr(5, &students);
    if (err) return err;
    err = read_students(in, &students);
    if (err) {
        destroy_stud(&students);
        return err;
    }
    printf("Loaded %d students. Please input your command (or type 'help' for info).\n", students.length);
    
    while (1) {
        switch (state) {
            case COMMAND_WAIT:
                err = wait_command(&cmd);
                if (err) {
                    printf("Goodbye!\n");
                    destroy_stud(&students);
                    return 0;
                }
                state = COMMAND_HANDLE;
                break;
            case COMMAND_HANDLE:
                err = handle_command(cmd, &students, out);
                if (err == EOF) {
                    printf("Program completed successfully.\n");
                    destroy_stud(&students);
                    return 0;
                }
                if (err) {
                    printf("Error executing command.\n");
                    destroy_stud(&students);
                    return err;
                }
                state = COMMAND_WAIT;
                break;
            default:
                destroy_stud(&students);
                return INCORRECT_OPTION;
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return INCORRECT_ARGUMENTS;
    }

    char *in = argv[1];
    char *out = argv[2];

    if (check_paths(in, out)) {
        printf("Error: Input and output files must be different.\n");
        return INCORRECT_ARGUMENTS;
    }

    FILE* inp = fopen(in, "r");
    if (!inp) {
        printf("Error: Cannot open input file '%s'\n", in);
        return FILE_ERROR;
    }

    FILE* outfile = fopen(out, "w");
    if (!outfile) {
        printf("Error: Cannot open output file '%s'\n", out);
        fclose(inp);
        return FILE_ERROR;
    }

    int err = dialog_manager(inp, outfile);
    fclose(outfile);
    fclose(inp);
    return err;
}