#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

typedef enum Errors {
    sep_sign = 0,
    end,
    memory_error,
    invalid_input,
    overflow
} Errors;

typedef enum kOpts {
    OPT_Q,
    OPT_M,
    OPT_T
} kOpts;

int errors(int key) {
    switch (key) {
        case invalid_input:
            printf("Incorrect arguments(input)");
            return 1;
        case memory_error:
            printf("Memory mistake");
            return 1;
        case overflow:
            printf("Overflow in discriminant calculation");
            return 1;
        default:
            printf("%d", key);
    }
}

int compareNumbers(double epsilon, double a, double b) {
    if (fabs(a - b) <= epsilon) {
        return 1;
    }
    return 0;
}

int my_atof(const char* string, double* res) {
    int sign = 1;
    int signs = 0;
    int dots = 0;
    double result = 0.0;
    double fraction = 1.0;
    double d_null = 0.0;
    const double CONST_EPS = 0.0000000001;

    for (int i = 0; string[i] != '\0'; ++i) {
        if (string[i] == '-' && signs == 0) {
            signs = 1;
            sign = -1;
        }
        else if (string[i] == '.' && dots == 0) {
            dots = 1;
        }
        else if (string[i] >= '0' && string[i] <= '9' && dots == 0) {
            result = result * 10.0 + (string[i] - '0');
        }
        else if (string[i] >= '0' && string[i] <= '9' && dots == 1) {
            fraction /= 10.0;
            result += (string[i] - '0') * fraction;
        }
        else {
            return invalid_input;
        }
    }

    if (compareNumbers(CONST_EPS, result, d_null)) {
        return invalid_input;
    }

    *res = result * sign;
    return 0;
}


int GetOpts(int argc, char** argv, kOpts* option, double* parametrs) {
    int key = 0;
    int param_index = 0;

    for (int i = 1; i < argc; i++) {
        const char* procceding_option = argv[i];
        if ((procceding_option[0] == '/' || procceding_option[0] == '-') && (procceding_option[2] == '\0')) {
            key++;
            switch (procceding_option[1]) {
            case 'q':
                *option = OPT_Q;
                break;
            case 'm':
                *option = OPT_M;
                break;
            case 't':
                *option = OPT_T;
                break;
            default:
                return 1;
            }
        }
        else if (key == 1) {
            if (param_index >= 4) {
                return invalid_input;
            }
            int incorrect_input = my_atof(argv[i], &parametrs[param_index]);
            if (incorrect_input) {
                return 1;
            }
            param_index++;
        }
        else {
            return invalid_input;
        }
    }
    return 0;
}

int solutionsOfEqual(double epsilon, double A, double B, double C) {
    if ((fabs(B) > sqrt(DBL_MAX)) || (fabs(A) > DBL_MAX / (4.0 * fmax(fabs(C), 1.0)))) {
        return overflow;
    }

    double D = B * B - 4 * A * C;
    double nul = 0.00;

    if (compareNumbers(epsilon, A, nul)&&compareNumbers(epsilon, B, nul)&&compareNumbers(epsilon, C, nul)) {
        printf("Infinity solutions\n\n");
        return 0;
    }
    if (compareNumbers(epsilon, A, nul) && compareNumbers(epsilon, B, nul)) {
        printf("No solutions\n\n");
        return 0;
    }
    if (compareNumbers(epsilon, A, nul)) {
        printf("x = %f\n\n", (-C / B));
        return 0;
    }
    if (D > 0) {
        double x1 = (-B + sqrt(D)) / (2 * A);
        double x2 = (-B - sqrt(D)) / (2 * A);
        printf("x1 = %f, x2 = %f\n\n", x1, x2);
        return 0;
    }
    if (compareNumbers(epsilon, D, nul)) {
        double x = (B * -1.00 / (2 * A));
        printf("x1 = x2 = %f\n\n", x);
        return 0;
    }
    printf("No solutions. Discriminant < 0\n\n");
}

int findUniq(double epsilon, double A, double B, double C) {
    if (compareNumbers(epsilon, A, B)) {
        return 3;
    }
    if (compareNumbers(epsilon, A, C)) {
        return 2;
    }
    return 1;
}


int* findSame(int Uniq) {
    static int answer[2];
    if (Uniq == 1) {
        answer[0] = 2;
        answer[1] = 3;
    } else if (Uniq == 2) {
        answer[0] = 1;
        answer[1] = 3;
    } else {
        answer[0] = 1;
        answer[1] = 2;
    }
    return answer;
}

int allDifferent(double epsilon, double A, double B, double C) {
    if (compareNumbers(epsilon, A, B)) {
        return 0;
    }
    if (compareNumbers(epsilon, A, C)) {
        return 0;
    }
    if (compareNumbers(epsilon, B, C)) {
        return 0;
    }
    return 1;
}

int HandlerOptQ(double* parametrs, int argc) {
    if (!(argc == 4)) {
        return invalid_input;
    }
    double epsilon = parametrs[0];
    if ((compareNumbers(epsilon, parametrs[1], parametrs[2])) && (compareNumbers(epsilon, parametrs[2], parametrs[3]))) {
        solutionsOfEqual(epsilon, parametrs[1], parametrs[2], parametrs[3]);
    }
    else if (allDifferent(epsilon, parametrs[1], parametrs[2], parametrs[3])) {
        for (int i = 1; i < 4; i++) {
            for (int j = 1; j < 4; j++) {
                for (int k = 1; k < 4; k++) {
                    if ((i != j) && (j != k) && (i != k)) {
                        printf("A: %f; B: %f; C: %f\n", parametrs[i], parametrs[j], parametrs[k]);
                        solutionsOfEqual(epsilon, parametrs[i], parametrs[j], parametrs[k]);
                    }
                }
            }
        }
    }
    else {
        int uniq = findUniq(epsilon, parametrs[1], parametrs[2], parametrs[3]);
        int same1 = findSame(uniq)[0];
        int same2 = findSame(uniq)[1];
        printf("A: %f; B: %f; C: %f\n", parametrs[uniq], parametrs[same1], parametrs[same2]);
        solutionsOfEqual(epsilon, parametrs[uniq], parametrs[same1], parametrs[same2]);
        printf("A: %f; B: %f; C: %f\n", parametrs[same1], parametrs[uniq], parametrs[same2]);
        solutionsOfEqual(epsilon, parametrs[same1], parametrs[uniq], parametrs[same2]);
        printf("A: %f; B: %f; C: %f\n", parametrs[same1], parametrs[same2], parametrs[uniq]);
        solutionsOfEqual(epsilon, parametrs[same1], parametrs[same2], parametrs[uniq]);
    }
}

int HandlerOptM(double* parametrs, int argc) {
    if (argc != 2) {
        printf("You have a mistake with the number of arguments\n");
        return 1;
    }
    if (!((parametrs[0] == floor(parametrs[0])) && (parametrs[1] == floor(parametrs[1])))) {
        printf("Numbers in this key must be integers\n");
        return 1;
    }
    int A = (int)parametrs[0];
    int B = (int)parametrs[1];
    if (B == 0) {
        printf("Division by zero is not allowed\n");
        return 1;
    }
    if (A % B == 0) {
        printf("First is divided into second\n");
    } else {
        printf("First is NOT divided into second\n");
    }
}

int TriangleSides(double epsilon, double A, double B, double C) {
    if ((compareNumbers(epsilon, (A * A + B * B), C * C))) {
        return 1;
    }
    if (compareNumbers(epsilon, (A * A + C * C), B * B)) {
        return 1;
    }
    if (compareNumbers(epsilon, (B * B + C * C), A * A)) {
        return 1;
    }
    return 0;
}

int HandlerOptT(double* parametrs, int argc) {
    if (argc != 4) {
        return invalid_input;
    }
    if (TriangleSides(parametrs[0], parametrs[1], parametrs[2], parametrs[3])) {
        printf("this parametrs can be sides of the right-angled triangle\n");
        return 0;
    }
    printf("this parametrs can NOT be sides of the right-angled triangle\n");
}

int main(int argc, char** argv) {
    kOpts opt = 0;
    int key;
    double parametrs[4];
    int (*handlers[3])(double*, int) = {
        HandlerOptQ,
        HandlerOptM,
        HandlerOptT
    };

    if (GetOpts(argc, argv, &opt, parametrs)) {
        key = GetOpts(argc, argv, &opt, parametrs);
        return errors(key);
    }
    
    handlers[opt](parametrs, argc - 2); 

    return 0;
}

