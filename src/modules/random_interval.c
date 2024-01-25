#include "random_interval.h"
int rand_interval(int a, int b) {
    if(a == 0) a = b;
    int c = b - a + 1;
    return rand() % c + a;
}
double drand_interval(double a, double b) {
    if(a == 0) a = b;
    int c = b - a + 1;
    return (double)(rand() % c + a);
}
void rand_init(void) {
    srand(time(NULL));
}
double db_sign(double number) {
    int sign = rand_interval(1, 2);
    if(sign == 1) return number;
    else return -number;
}
int int_sign(int number) {
    int sign = rand_interval(1, 2);
    if(sign == 1) return number;
    else return -number;
}

