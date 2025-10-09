#include <stdio.h>
#include <math.h>

double my_atof(const char* str) {
    double result = 0.0;
    double fraction = 1.0;
    int sign = 1;
    int i = 0;

    if (str[i] == '-') {
        sign = -1;
        ++i;
    }

    while (str[i] != '.' && str[i] != '\0') {
        result = result * 10.0 + (str[i] - '0');
        ++i;
    }

    if (str[i] == '.') {
        ++i;
    }

    while (str[i] != '\0') {
        fraction /= 10.0;
        result += (str[i] - '0') * fraction;
        ++i;
    }

    return sign * result;
}


int GetOpts(int argc, char** argv, double* eps) {
    if (argc != 2) {
        return 1;
    }
    *eps = my_atof(argv[1]);
    if (eps < 0) {
        printf("invalid epsilon");
        return 2;
    }
    return 0;
}

double integral(double (*func)(double), double a, double b, double eps) {
    double prev = -1.0, current = 0.0;
    int n = 10;
    double step;

    while (fabs(current - prev) > eps) {
        prev = current;
        current = 0.0;
        step = (b - a) / n;
        for (int i = 0; i < n; ++i) {
            double x1 = a + i * step;
            double x2 = a + (i + 1) * step;
            current += (func(x1) + func(x2)) * step / 2.0;
        }
        n *= 2;
    }
    return current;
}

double f1(double x) {
    if (fabs(x) < 1e-15) return 1.0;
    return log(1.0 + x) / x;
}

double f2(double x) {
    return exp(-x*x / 2.0);
}

double f3(double x) {
    if (x >= 1.0 - 1e-15) return 0.0;
    return log(1.0 / (1.0 - x));
}

double f4(double x) {
    if (fabs(x) < 1e-15) return 1.0;
    return pow(x, x);
}

int main(int argc, char** argv) {
    double eps = 0.0;
    if (GetOpts(argc, argv,  &eps)) {
        printf("%s\n", "Incorrect option");
        return 1;
    }
    printf("%f\n", integral(f1, 0, 1 ,eps));
    printf("%f\n", integral(f2, 0, 1 ,eps));
    printf("%f\n", integral(f3, 0, 1 ,eps));
    printf("%f\n", integral(f4, 0, 1 ,eps));
    return 0;
}