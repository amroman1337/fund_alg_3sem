#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __APPLE__
    #define SYS_ERR_MSGS mac_error_messages
#elif __linux__
    #define SYS_ERR_MSGS linux_error_messages
#else
    #define SYS_ERR_MSGS generic_error_messages
#endif

typedef enum Errors {
    zero_error,
    sign_error, 
    overflow,
    memory_error,
    input_error,
    calculation_error
} Errors;

int handle_error(Errors key) {
    switch (key) {
        case zero_error:
            printf("Division by zero is not allowed\n");
            return 1;
        case sign_error:
            printf("Number must be >= 0\n");
            return 1;
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
        default:
            printf("Unknown error\n");
            return 1;
    }
}

static const char *mac_error_messages[] = {
    "Success", "Operation not permitted", "No such file or directory(mac)"
};

static const char *linux_error_messages[] = {
    "Success", "Operation not permitted", "No such file or directory(linux)"
};

static const char *generic_error_messages[] = {
    "Success", "Unknown error"
};

// a
void *memchr(const void *str, int c, size_t n) {
    if (str == NULL) return NULL;
    const unsigned char *p = (const unsigned char *)str;
    unsigned char uc = (unsigned char)c;
    for (size_t i = 0; i < n; i++) {
        if (p[i] == uc) {
            return (void *)(p + i);
        }
    }
    return NULL;
}

// b
int memcmp(const void *str1, const void *str2, size_t n) {
    if (str1 == NULL || str2 == NULL) return 0;
    const unsigned char *p1 = (const unsigned char *)str1;
    const unsigned char *p2 = (const unsigned char *)str2;
    
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}

// c
void *memcpy(void *dest, const void *src, size_t n) {
    if (dest == NULL || src == NULL || dest == src) return dest;
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

// d
void *memset(void *str, int c, size_t n) {
    if (str == NULL) return NULL;
    unsigned char *p = (unsigned char *)str;
    unsigned char uc = (unsigned char)c;
    
    for (size_t i = 0; i < n; i++) {
        p[i] = uc;
    }
    return str;
}

// e
char *strncat(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL) return dest;
    char *d = dest;
    while (*d != '\0') d++;
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++) {
        *d++ = src[i];
    }
    *d = '\0';
    return dest;
}

// f
char *strchr(const char *str, int c) {
    if (str == NULL) return NULL;
    unsigned char uc = (unsigned char)c;
    
    while (*str != '\0') {
        if (*str == uc) {
            return (char *)str;
        }
        str++;
    }
    return (uc == '\0') ? (char*)str : NULL;
}

// g
int strncmp(const char *str1, const char *str2, size_t n) {
    if (str1 == NULL || str2 == NULL) return 0;
    
    for (size_t i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }
        if (str1[i] == '\0') {
            break;
        }
    }
    return 0;
}

// h
char *strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || dest == src) return dest;
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

// i
size_t strcspn(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) return 0;
    const char *p1, *p2;
    
    for (p1 = str1; *p1 != '\0'; p1++) {
        for (p2 = str2; *p2 != '\0'; p2++) {
            if (*p1 == *p2) {
                return (size_t)(p1 - str1);
            }
        }
    }
    return (size_t)(p1 - str1);
}

// j
char *strerror(int errnum) {
    size_t count = sizeof(SYS_ERR_MSGS) / sizeof(SYS_ERR_MSGS[0]);
    if (errnum < 0 || (size_t)errnum >= count) {
        return "Unknown error";
    }
    return (char *)SYS_ERR_MSGS[errnum];
}

// k
size_t strlen(const char *str) {
    if (str == NULL) return 0;
    const char *p = str;
    while (*p != '\0') p++;
    return (size_t)(p - str);
}

// l
char *strpbrk(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) return NULL;
    for (; *str1 != '\0'; str1++) {
        const char *p = str2;
        while (*p != '\0') {
            if (*str1 == *p) {
                return (char *)str1;
            }
            p++;
        }
    }
    return NULL;
}

// m
char *strrchr(const char *str, int c) {
    if (str == NULL) return NULL;
    const char *last = NULL;
    unsigned char uc = (unsigned char)c;
    
    while (*str != '\0') {
        if (*str == uc) {
            last = str;
        }
        str++;
    }
    return (uc == '\0') ? (char*)str : (char*)last;
}

// n
char *strstr(const char *haystack, const char *needle) {
    if (haystack == NULL || needle == NULL) return NULL;
    if (*needle == '\0') {
        return (char *)haystack;
    }
    
    for (; *haystack != '\0'; haystack++) {
        const char *h = haystack;
        const char *n = needle;
        
        while (*h != '\0' && *n != '\0' && *h == *n) {
            h++;
            n++;
        }
        if (*n == '\0') {
            return (char *)haystack;
        }
    }
    
    return NULL;
}

// o
static char *strtok_save = NULL;
char *strtok(char *str, const char *delim) {
    if (delim == NULL) return NULL;
    if (str == NULL) {
        if (strtok_save == NULL) return NULL;
        str = strtok_save;
    }
    while (*str != '\0') {
        const char *d = delim;
        int found = 0;
        while (*d != '\0') {
            if (*str == *d) {
                found = 1;
                break;
            }
            d++;
        }
        if (!found) break;
        str++;
    }
    
    if (*str == '\0') {
        strtok_save = NULL;
        return NULL;
    }
    char *token_start = str;
    while (*str != '\0') {
        const char *d = delim;
        int found = 0;
        while (*d != '\0') {
            if (*str == *d) {
                found = 1;
                break;
            }
            d++;
        }
        if (found) {
            *str = '\0';
            strtok_save = str + 1;
            return token_start;
        }
        str++;
    }
    strtok_save = NULL;
    return token_start;
}

int main() {
    printf("Testing string functions:\n\n");
    
    char buffer[100];
    char *dynamic_str = malloc(50);
    if (dynamic_str == NULL) {
        handle_error(memory_error);
        return 1;
    }
    
    memset(buffer, 0, sizeof(buffer));
    
    const char *test_str = "Hello, World!";
    const char *search_str = "World";
    
    printf("1. memcpy: ");
    memcpy(buffer, test_str, strlen(test_str) + 1);
    printf("%s\n", buffer);
    
    printf("2. memchr 'W': ");
    char *found = memchr(buffer, 'W', strlen(buffer));
    printf("%s\n", found ? found : "Not found");
    
    printf("3. memcmp: ");
    int cmp_result = memcmp("Hello", "Hello", 5);
    printf("%s\n", cmp_result == 0 ? "Equal" : "Different");
    
    printf("4. memset: ");
    memset(buffer, 'X', 5);
    buffer[5] = '\0';
    printf("%s\n", buffer);
    
    printf("5. strncat: ");
    strcpy(buffer, "Hello");
    strncat(buffer, ", World!", 7);
    printf("%s\n", buffer);
    
    printf("6. strchr 'o': ");
    char *chr_result = strchr("Hello", 'o');
    printf("%s\n", chr_result ? chr_result : "Not found");
    
    printf("7. strncmp: ");
    int strcmp_result = strncmp("Hello", "Hello", 5);
    printf("%s\n", strcmp_result == 0 ? "Equal" : "Different");
    
    printf("8. strncpy: ");
    strncpy(buffer, "Test string", 11);
    buffer[11] = '\0';
    printf("%s\n", buffer);
    
    printf("9. strcspn: ");
    size_t cspn_result = strcspn("Hello World", " \t\n");
    printf("Length until space: %zu\n", cspn_result);
    
    printf("10. strerror: ");
    printf("Error 0: %s\n", strerror(0));
    
    printf("11. strlen: ");
    printf("'Hello' length: %zu\n", strlen("Hello"));
    
    printf("12. strpbrk: ");
    char *pbrk_result = strpbrk("Hello World", "aeiou");
    printf("First vowel: %c\n", pbrk_result ? *pbrk_result : '?');
    
    printf("13. strrchr 'l': ");
    char *rchr_result = strrchr("Hello", 'l');
    printf("%s\n", rchr_result ? rchr_result : "Not found");
    
    printf("14. strstr: ");
    char *strstr_result = strstr("Hello World", "World");
    printf("%s\n", strstr_result ? strstr_result : "Not found");
    
    printf("15. strtok:\n");
    char tok_str[] = "This,is,a,test";
    char *token = strtok(tok_str, ",");
    while (token != NULL) {
        printf("  Token: %s\n", token);
        token = strtok(NULL, ",");
    }
    
    printf("\nTesting error handler:\n");
    handle_error(zero_error);
    handle_error(memory_error);
    handle_error(input_error);
    
    free(dynamic_str);
    
    printf("\nSuccess!\n");
    return 0;
}