#include <stdio.h>
#include <math.h>
#include <limits.h>

typedef enum kOpts {
    OPT_H,
    OPT_P,
    OPT_S,
    OPT_E,
    OPT_A,
    OPT_F
} kOpts;

typedef enum Errors {
    zero_error,
    sign_error,
    overflow
} Errors;

int errors(int key) {
    switch (key)
        {
        case zero_error:
            printf("You can't divide on zero\n");
            return 1;
        case sign_error:
            printf("Your number must be >= 0\n");
            return 1;
        case overflow:
            printf("Number is too big(overflow)\n");
            return 1;
        }
        return 0;
}

int GetOpts(int argc, char** argv, kOpts *option, int *number);
int int_checker(char *s);
int my_strlen(char *str);
int my_atoi(char *s);
int HandlerOptH(const int number, int *res); 
int HandlerOptP(const int number);
int HandlerOptS(const int number, char *res);
int HandlerOptE(const int number, long long result[10][10]);
int HandlerOptA(const int number);
int HandlerOptF(const int number);

int GetOpts(int argc, char** argv, kOpts *option, int *number){
    if (argc != 3) return 1;
    int fl = 2;
    for (int i = 0; i < my_strlen(argv[1]); i++) {
        if (int_checker(argv[1]) != 0) {
                fl--;
                break;
        }
    
    }
    if (fl == 2) {
        if (my_strlen(argv[1]) < 9) {
            *number = my_atoi(argv[fl - 1]);
        }
        else return 1;
    }

    else if (fl == 1){ 
        for (int j = 0; j < my_strlen(argv[2]); j++) {
            if (int_checker(argv[fl + 1]) != 0) {
                return 1;
            }
        }
        if (my_strlen(argv[2]) < 9) {
            *number = my_atoi(argv[2]);
        }
        else return 1;

    }
    if (my_strlen(argv[fl]) != 2) return 1;
    if (argv[fl][0] == '-' || argv[fl][0] == '/') {
        switch (argv[fl][1]) {
            case 'h': {
                *option = OPT_H;
                return 0;
            }
            case 'p': {
                *option = OPT_P;
                return 0;
            }
            case 's': {
                *option = OPT_S;
                return 0;
            }
            case 'e': {
                *option = OPT_E;
                return 0;
            }
            case 'a': {
                *option = OPT_A;
                return 0;
            }
            case 'f': {
                *option = OPT_F;
                return 0;
            }
            default:
                return 1;
        }  
    }
    return 1;
}

int my_atoi(char *s) {
    int result = 0;
    for (int i = 0; i < my_strlen(s); i++) {
        result = result * 10 + (s[i] - '0');
    }    
    return result;
}

int my_strlen(char *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

int int_checker(char *s) {
    char *alph = "0123456789";
    int fl = 0;
    int length = my_strlen(s);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < 10; j++) {
            if (s[i] == alph[j]) {
                fl++;
                break;
            }
        }
    }
    return length - fl;
}

int HandlerOptH(const int number, int *res){
    int fl = 0;
    int temp_number = number;
    if (temp_number == 0) return -1;
    for (int i = temp_number; i <= 100; i += temp_number){
        if (i % temp_number == 0){ 
            res[fl] = i;
            fl++;
        }
    }
    return fl;
}

int HandlerOptP(const int number){
    int temp_number = number;
    if (temp_number == 0 || temp_number == 1) return -1;
    for (int i = 2; i*i <= temp_number; i++){
        if (temp_number % i == 0){
            return 1;
        }
    }
    return 0;
}

int HandlerOptS(const int number, char *res) {
    int fl = 0;
    int count = 1;
    int currentOsn = 1;
    int currentAns = 0;
    int temp_number = number;

    if (temp_number == 0) return -1;
    while(currentOsn <= temp_number){
        currentOsn *= 16;
        count++;
    }

    count--;
    currentOsn /= 16;
    fl = count;
    
    while (temp_number > 0){
        while(temp_number >= currentOsn){
            temp_number -= currentOsn;
            currentAns += 1;
        }
        if (currentAns < 10) res[count] = '0' + currentAns;
        else{
            *(res + count) = 'A' + (currentAns - 10);
            
        }
        count--;
        currentAns = 0;
        currentOsn /= 16;
    }
    while(count > 0){
        res[count] = '0';
        count--;
    }
    return fl;
}

int HandlerOptE(const int number, long long result[10][10]){
    int temp_number = number;
    if (temp_number >= 1 && temp_number <= 10){
        for (int i = 1; i <= 10; i++){
            int base = i;
            result[i - 1][0] = base;
            for (int j = 1; j <= temp_number; j++){
                result[i - 1][j] = result[i - 1][j - 1] * base;
            }
        }
        return 0;
    }
    else return -1;
}

int HandlerOptA(const int number) {
    int temp_number = number;
    int sum = (1 + temp_number) / 2;
    if (sum > INT_MAX / temp_number) {
        return -1;
    }
    return sum * temp_number;
}

int HandlerOptF(const int number){
    int temp_number = number;
    int fac = 1;
    if (temp_number == 0) return 1;
    for (int i = 2; i <= temp_number; i++){
        if (fac > INT_MAX / i) return -1;
        fac *= i;
    }
    return fac;
}

int main (int argc, char** argv) {
    kOpts opt = 0;
    int procceed_number = 0;

    if (GetOpts(argc, argv, &opt, &procceed_number)) {
        printf("%s\n", "Incorrect arguments");
        return 1;
    }

    switch (opt) {
        case OPT_H: {
            int res[100] = {0};
            int count = HandlerOptH(procceed_number, res);
            if (count == -1) {
                errors(zero_error);
            } else if (count > 0) {
                for (int i = 0; i < count; i++) {
                    printf("%d\n", res[i]);
                }
            }
            break;
        }
        case OPT_P: {
            int x = HandlerOptP(procceed_number);
            if (x == -1) {
                printf("No answer!\n");
            } else if (x == 0) {
                printf("%d is prime\n", procceed_number);
            } else {
                printf("%d is not prime\n", procceed_number);
            }
            break;
        }
        case OPT_S: {
            char res[9] = {'0'};
            int count = HandlerOptS(procceed_number, res);
            if (count == -1) {
                printf("0\n");
            } else {
                for (int i = count; i > 0; i--) {
                    printf("%c ", res[i]);
                }
                printf("\n");
            }
            break;
        }
        case OPT_E: {
            long long result[10][10] = {0};
            int fl = HandlerOptE(procceed_number, result);
            if (fl == -1) {
                errors(overflow);
            } else {
                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j <= procceed_number; j++) {
                        printf("%lld ", result[i][j]);
                    }
                    printf("\n");
                }
            }
            break;
        }
        case OPT_A: {
            int x = HandlerOptA(procceed_number);
            if (x == -1) {
                errors(overflow);
            } else {
                printf("%d\n", x);
            }
            break;
        }
        case OPT_F: {
            int x = HandlerOptF(procceed_number);
            if (x == -1) {
                errors(overflow);
            } else {
                printf("%d\n", x);
            }
            break;
        }
        default:
            printf("Unknown option\n");
            break;
    }
    return 0;
}