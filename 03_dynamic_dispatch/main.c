#include<stdlib.h>
#include<stdio.h>
#include <time.h>

void func_a() {
    printf("called func a\n");
}

void func_b() {
    printf("called func b\n");
}

void call_fn(void* func) {
    // The syntax for telling C to treat this void pointer as a pointer to a
    // function and to invoke the underlying code is wonky; I certainly
    // needed ChatGPT's help. But, we can convince ourselves that programs
    // are data, just like anything else, and this void pointer could point
    // to a function as easily as it could point to an integer, string, array,
    // or struct.
    ((void (*)())func)();
}

void dynamic_dispatch_example() {
    printf("func a lives at %d\n", &func_a);
    printf("func b lives at %d\n", &func_b);
    void* func_a_ptr = &func_a;
    void* func_b_ptr = &func_b;

    void* ptr;
    if (rand() % 2) {
        ptr = func_a_ptr;
    } else {
        ptr = func_b_ptr;
    }

    // At this point, we can't know which function `ptr` is pointing to. It's
    // just a memory address to some plain old data, and in this case, the
    // data at the address are machine instructions which can be fed into
    // the CPU, so let's use our handy dandy `call_fn` helper, which will take
    // an arbitrary pointer and call whatever the heck is there.
    call_fn(ptr);
}


////////////////////////////////// Obligatory /////////////////////////////////


int main() {
    srand(time(NULL));
    dynamic_dispatch_example();
    return 0;
}
