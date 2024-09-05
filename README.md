# Low-Level Dynamic Typing

Computers are rocks we tricked into doing math. For example, we can create a
physical electrical circuit which will perform [addition on its
inputs](https://en.wikipedia.org/wiki/Adder_\(electronics\)). Meanwhile, if
you're starting to learn programming, you're probably using Python, JavaScript,
SQL, or even functions in Excel or Google Sheets! You may know that these
languages have a feature called dynamic typing. It's a big part of what makes
them so easy to learn and use. Dynamic typing means that we can run programs
which add floating point values to integers, multiply strings, compare objects
of different types, etc. For example, JavaScript can very happily run this code:

```
> console.log(1 + 'cat')
1cat
```

Python can multiply a string by a number;

```python
print('cat' * 3)
```

In Python, we can compare different types of numbers:

```
if 1 > 0.9: print('yup')
```

This is true even though inside the computer, 1 is represented as;

```
00000000000000000000000000000001
```

While according to [IEEE
754](https://www.h-schmidt.net/FloatConverter/IEEE754.html), the value `0.9` is
represented as:

```
00111111011001100110011001100110
```

In Python, we can even do mathematical operations between integers, floating
point numbers, and _complex numbers_ which contain an imaginary part!

```python
print(1 + 3 - 1.2 / 4 + 4j)
```

Stop and take a second to think about that -- how the heck did that work? It's
easy to convince yourself that computers are just magic machines with random
rules, but dynamic typing is not magic, and this repo breaks down how it works
under the hood.

## On Smart Rocks

Let's review the lower-level details of how a computer works.

A computer has a CPU, which takes a bit of instruction, and has some amount of,
"slots," or, "positions," for data to be stored. For example, this is roughly
how a computer adds 1 + 1 and prints out the result;

```
LOAD 1 INTO POSITION 1
LOAD 1 INTO POSITION 2
ADD POSITIONS: 1, 2
# *result* is a special position where the result of the add operation is stored
LOAD *result* INTO POSITION 3
PRINT POSITION 3
```

The CPU reads the first word, which is called an instruction, `LOAD`, `ADD`, or
`PRINT` in our simple CPU. Modern CPUs have a bunch of instructions. For
example, [x86_64 CPUs have over 1,500
instructions](https://en.wikipedia.org/wiki/X86_instruction_listings)!
From the CPU's perspective, instructions are a fixed-size sequence of bits. For
example;

```
00 = LOAD
01 = ADD
10 = PRINT
```

These sequences are sent into the CPU as pulses of electricity, and they
ultimately determine the physical circuits through which the _following_
sequences of electricity will flow. [Click here to learn more about how that
routing operation works](https://electronics.stackexchange.com/a/465537).

For example, when we say;  `ADD POSITIONS: 1, 2`, the value at position 1 and
the value at position 2 are fed into the `ADD` circuit inside the CPU, and the
CPU stores the result of the computation in a special third memory location
named `*result*`.

Instructions like `ADD`, which _do stuff_ are important, but instructions like
`LOAD` or `MOVE` are perhaps even more important; instructions which _prepare_
the computer to do stuff. In fact, when it comes to making computers do dynamic
things, we'll notice that the exciting things happen as the computer moves data
around, and follows references to data (like `LOAD`). In fact, it turns out that
all this zipping around between different bits of code, and dynamically pushing
around data while the program runs is the reason that dynamic languages are
slower than their lower-level counterparts.

### Aside: Looping

There is one more instruction called `JUMP` which is how loops happen. `JUMP` is
also important to our story, so it's worth noting here in our review. For
example, here's how we can revise our program from before to print `1 + 1` ten
times.

```
# This will be `i`, keeping track of the state of our loop
1. LOAD 0 INTO POSITION 10


2. LOAD 1 INTO POSITION 1
3. LOAD 1 INTO POSITION 2
4. ADD POSITIONS: 1, 2
5. LOAD *result* INTO POSITION 3
6. PRINT POSITION 3

# Here, we're incrementing `i`, which requires quite a few instructions!
7. LOAD 1 INTO POSITION 9
8. ADD POSITIONS: 9, 10
9. LOAD **result** INTO POSITION 10

# we explain LOAD_IF_GT next
10. LOAD_IF_GT POSITION 10, 10, POSITION 8, STEP 12, STEP 2
11. JUMP TO POSITION 8
12. EXIT

```

`LOAD_IF_GT` is saying, "if the value in position 10 is greater than 10, load
step 12 into position 8. Otherwise, load step 2 into position 8." The CPU does
not differentiate between instructions, values, memory locations, and spots in
the program. All of these are just pulses of electricity, or numbers. If you
look at how [binary counting
works](https://www.cmu.edu/gelfand/lgc-educational-media/digital-education-modules/dem-documents/new-the-world-of-the-internet-handouts.pdf),
you should understand that pulses of electricity can represent numbers and vice
versa.

### Aside: Value Dereferencing

I've been treating references (like `POSITION 1`) and values somewhat
interchangeably here. For example, in the previous program, we said, `JUMP TO
POSITION 8`, but we know that the value stored in `POSITION 8` is either `STEP
13` or `STEP 2` based on the result of `LOAD_IF_GT`. In general, we see that
there is a layer of indirection: `POSITION 8` uses the number `8` to restore to
a location in the computer's memory, but there is a value in that location which
is an entirely different number. This distinction is important, and we see it
appear in low-level programming languages. A computer needs to do some work to
extract the value which is stored behind a reference. Again, I want to emphasize
here that references and values are both just numbers, which are both just
pulses of electricity flowing through a machine.

The way that this works is super simple. In programs, we generally use the
star (`*`) to dereference, which means to extract a value from a position.
If you think of memory as a bunch of slots;

```
POSITION 1 = 0
POSITION 2 = 10
POSITION 3 = 0
POSITION 4 = 'fish'
```

Then, I might expect the following;

```
>>> PRINT (POSITION 4)
4
>>> PRINT *(POSITION 4)
'fish'
>>> PRINT *(POSITION 2)
10
```

### Aside: Byte Addressability

A position in a computer is typically a number between zero and
18,446,744,073,709,551,615! Each of these positions points to a bucket of eight
bits, which we call a byte. This is eight slots in which we can store a one or a
zero.

In practice, this means that we can store up to 2 ^ 8 unique values in each
position. That means we can store a number between zero and 255. Or, if we
arbitrarily assign numbers to letters, we can fit all the letters in the English
language, plus all the symbols, plus a few secret characters to do fancy stuff
like break one line into two lines. This is what [ASCII character
encoding](https://en.wikipedia.org/wiki/ASCII) does.

This means that a single slot in a computer can't really store `'fish'`. It can
only store the letter `f`. To store `'fish'`, you'd actually need four
positions.

Since positions in a computer are a number which **points** to a box of 8 bytes,
we say that these special values are, "pointers." Again, they're not any
different from regular numbers or strings or anything else. Everything is just
pulses of electricity at the end of the day. We impose our human ideas on the
pulses of electricity, because otherwise it's all too confusing for us.

We learned before that `*` is used to dereference a pointer. The opposite of `*`
is `&`, which gives us the address of a piece of data; it's typically used to
create a pointer. For example, in the C programming language, I might say;

```c
#include<stdio.h>

int main() {
    // `value` literally is 6
    int value = 6;
    // ptr stores the memory address of the number 6
    int* ptr = &value;
    printf("the number 6 is in slot %d\n", ptr);
    printf("my value: %d\n", *ptr);
}
```

If you run this program, you'll notice that the first line of output probably
changes every time you run the program, but no matter what, it always points to
the number 6.

## On Dynamic Typing

At this point, hopefully you're stumped about how this smart rock with a bunch
of electricity flowing through it is able to run JavaScript/Python/Ruby/Php. For
that matter, how were we able to build any high-level programs off of this
primitive foundation? How can a computer, which doesn't know the difference
between a number or a letter have the ability to execute this code;

```python
def add(a, b):
    return a + b;

print(add({}, []))
print(add(1, 2))
print(add("hi ", "there"))

class num(int):
    def __init__(self, n):
        self.n = n
    def __add__(self, other):
        print("nah, I'm good")
        return 0

print(add(num(1), "?"))
```

If a bit of data, a number, and a string all take up different amounts of
storage in a computer, how can I do this in Python, and it "just works?"

```python
wallet = 1
wallet = False
wallet = 'fish'
```

Similarly, how can we write programs which dynamically interact with any of
those three values, and the program still works?

```python
def what_is_in_your_wallet(wallet):
    if wallet == 1:
        print("You have $1 in your wallet")
    elif wallet == False:
        print("You do not have your wallet right now.")
    elif wallet == "fish":
        print("Um, sir, your wallet is a fish...")
```

# Low-Level Dynamic Typing

OK, time to get un-confused. It's time to see how low-level dynamic typing
works! By the end of this adventure, we'll be able to create a program in C
which is able to run highly dynamic code, like;

```c
Object* a = str("Very ");
Object* b = str("cool");
Object* c = int(5);
Object* d = str("!");

print(
  add(
    add(a, b),
    mul(c, d)
  )
);
```

# How to Use This

In each chapter, read the `README.md` side-by-side with `main.c`. Run `make` in
the chapter folder to compile and run `main.c`.
