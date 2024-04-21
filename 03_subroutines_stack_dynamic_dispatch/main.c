#include<stdlib.h>
#include<stdio.h>
#include <time.h>


/////////////////////// Part 1: Subroutines & the Stack ///////////////////////

// Load the stack up with `n` stack frames by calling into myself recursively.
// Every time I call myself, an additional function call is added to the stack.
// Then, I'll return a pointer from the top-most stack frame back through
// all the other stack frames, and finally back to the caller. This will
// return a value to deallocated memory. When we try to print it, let's see
// what happens!
int* tall_boi(int height) {
    if (height > 0) {
        return tall_boi(height - 1);
    }
    int random = rand();
    printf("value is %d\n", random);
    return &random;
}

void subroutines_and_stack() {
    int* value = tall_boi(5);
    // At this point, `value` is a pointer to a part of the stack which is
    // gone. Since the stack frames 5-layers-deep into `tall_boi` have been
    // popped off the stack at this point, we have an invalid reference.
    // In practice, though, you'll see that this bit of code works in most
    // cases. This is probably because the stack just isn't tall enough for
    // the memory to be overwritten.
    printf("maybe this works; %d\n", *value);

    // On the other hand, if we push many more stack frames onto the stack,
    // _then_ generate the random number very high on the stack, _then_ pop
    // all those frames off the stack, we'll find that we do end up with a
    // reference which is out of bounds, and we can create a segmentation
    // fault.
    int* other_value = tall_boi(500000000);
    printf("this probably won't; %d\n", *value);

    // Of course, a segmentation fault is not guaranteed!
    printf("It's your lucky day!\n");
}

///////////////////////// Part 2: Functions are Data //////////////////////////
//
// Dynamic dispatch puts the procedural reality of a computer into focus. Think
// about the term: sub-routines. C's runtime uses stack-based evaluation of
// routines. When you call a function, it is pushed onto the call stack. When
// a function is returned, it is popped from the call stack. Functions which
// are high in the call stack can receive pointers to memory which is lower
// in the call stack, but if they return references to values which they
// allocated on the stack, that won't work, because when the runtime pops that
// function from the stack, that variable will be deallocated.
//
// Let's explore this world of stack-based subroutines a bit before we go into
// dynamic dispatch.

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
    // Comment-in for part 1 (this causes a segmentation fault!)
    // subroutines_and_stack();

    // Comment-in for part 2
    dynamic_dispatch_example();
    printf("Hey, nothing is running yet. Look at the end of main.c to get started!\n");
    return 0;
}
