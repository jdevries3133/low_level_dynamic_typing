# Functions are Data

Dynamic dispatch puts the procedural reality of a computer into focus. Think
about the term: sub-routines. C's runtime uses stack-based evaluation of
routines. When you call a function, it is pushed onto the call stack. When
a function is returned, it is popped from the call stack. Functions which
are high in the call stack can receive pointers to memory which is lower
in the call stack. In other words, you can pass a stack-allocated pointer
into a function, but you can never return one. The stack is a temporary
scratch-pad available to your code. When functions are returned, the stack
may be deallocated, or it may later be overwritten when new functions are
pushed onto the stack.

Dynamic dispatch is a key component in low-level dynamic type-systems,
because we typically call a function at a random memory address whenever
a method on an object is invoked.

We often think of C as being a statically typed language, and higher-level
languages like Python being dynamic, but this comparison doesn't really hold up.
Static and dynamic are programming _styles_ in C, not firm constraints. Try
running the `./example` a couple times (after compiling with `make`). You will
notice that a different function is called each time, because our program
performs dynamic dispatch!
