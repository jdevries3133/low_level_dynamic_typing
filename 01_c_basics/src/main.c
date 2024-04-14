#include "./main.h"


int main() {
    printf("main is invoked\n");
    subroutine();
    printf("subroutine is over\n");
    return 0;
}

void subroutine() {
    printf("subroutine is invoked\n");
}
