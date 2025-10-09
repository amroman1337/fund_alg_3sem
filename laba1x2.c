#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum Errors{
    error_invalid_input,
    error_too_large_number,
    error_memory_allocation
} Errors;

int errors(int key) {
    switch (key)
        {
        case error_invalid_input:
            printf("Invalid input\n");
            return 1;
        case error_too_large_number:
            printf("Requested prime number is too large\n");
            return 1;
        case error_memory_allocation:
            printf("Memory allocation failed\n");
            return 1;
        }
        return 0;
}

int find_nth_prime(int n, long long *result) {
    if (n <= 0) {
        return errors(error_invalid_input);
    }
    if (n > 1000000) {
        return errors(error_too_large_number);
    }
    long long limit;
    if (n < 6) {
        limit = 20;
    } else {
        limit = (long long)(n * (log(n) + log(log(n))) * 1.2) + 100;
        if (limit > 100000000) {
            return errors(error_too_large_number);
        }
    }

    char *is_prime = (char*)malloc((limit + 1) * sizeof(char));
    if (is_prime == NULL) {
        return errors(error_memory_allocation);
    }
    
    for (long long i = 0; i <= limit; i++) {
        is_prime[i] = 1;
    }
    is_prime[0] = is_prime[1] = 0;

    for (long long i = 2; i * i <= limit; i++) {
        if (is_prime[i]) {
            for (long long j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }
    int count = 0;
    for (long long i = 2; i <= limit; i++) {
        if (is_prime[i]) {
            count++;
            if (count == n) {
                *result = i;
                free(is_prime);
                return 0;
            }
        }
    }
    
    free(is_prime);
    return errors(error_too_large_number);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Invalid arguments\n");
        return 1;
    }

    int T = atoi(argv[1]);
    if (T <= 0 || T > 10000) {
        return errors(error_invalid_input);
    }
    
    int *requests = (int*)malloc(T * sizeof(int));
    if (requests == NULL) {
        return errors(error_memory_allocation);
    }

    for (int i = 0; i < T; i++) {
        if (scanf("%d", &requests[i]) != 1) {
            free(requests);
            return errors(error_invalid_input);
        }
        
        if (requests[i] <= 0) {
            free(requests);
            return errors(error_invalid_input);
        }
    }

    for (int i = 0; i < T; i++) {
        long long prime;
        int error = find_nth_prime(requests[i], &prime);
        
        if (error == 0) {
            printf("%d: %lld\n", requests[i], prime);
        } else {
            printf("%s for position %d\n", errors(error), requests[i]);
        }
    }
    free(requests);
    return 0;
}