#include<stdlib.h>
#include<stdio.h>

struct Item {
    int id;
    int price;
    char* name;
} Item;


struct Tomato {
    int id;
    int price;
    char* name;

    char* color;
} Tomato;

struct BoardGame {
    int id;
    int price;
    char* name;

    int minimum_age;
} BoardGame;

void print_item(struct Item* item) {
    printf("Name = %s; Price = %d\n", item->name, item->price);
}

int main() {
    struct Item item_1;
    item_1.id = 1;
    item_1.price = 3;
    item_1.name = "Loaf of Bread";

    // Reading from left to right, the `*` tells us, "item_1_ptr" is a
    // memory address (a number) which stores the address in which the actual
    // item_1 data is stored.
    //
    // Meanwhile, you can think of the previous variable `Item item_1` as
    // the actual data of `item_1` itself, which is why we can assign to it,
    // etc. "It's not just a silly number, it's _real stuff_ in there." Or,
    // at least, that's what the C compiler would want us to think. In reality,
    // the difference between `struct Item foo` and `struct Item* foo` is
    // purely semantics. C is allowing us to pretend that `item_1` is something
    // other than what it truly is: a memory address.
    struct Item* item_1_ptr = &item_1;

    // Notice that the `print_item` function takes a pointer as its argument.
    // It is generally idiomatic to pass struct pointers into C functions
    // instead of the structs themselves. This is just a language idiom; like
    // I said before, there really isn't any difference under the hood.
    print_item(item_1_ptr);

    struct Tomato tomato_item_2;
    tomato_item_2.id = 2;
    tomato_item_2.price = 2;
    tomato_item_2.name = "Roma Tomato";
    tomato_item_2.color = "red";

    // Now it's easier to see why we are using pointers. In C, it's easy to
    // use some tricky syntax like this to tell the language, "hey, I don't
    // care what you know. It's 1979 and I still think I'm smarter than a
    // computer. So, instead of treating this memory address as a Tomato,
    // please treat it as an item, moving forward.
    //
    // It's important to note that this line of code does _NOTHING_ whatsoever;
    // it does not even generate a machine instruction. To make an analogy to
    // TypeScript, this is just like saying;
    //
    //     type Foo<T> = T extends string ? number : never;
    //
    // Does computation happen in this line of code? Sure. But the computation
    // happens at compile-time. Not only does computation not happen at runtime,
    // this code has been entirely stripped away before runtime even begins.
    //
    // I want to emphasize this similarity -- it is a practical example which
    // reveals some fundamental truths about "types." Whether you're at a
    // high level or a low level, there is no such thing. Humans create the
    // types for ourselves. Computers are just electronic circuits.
    //
    // Ironically, the fact that types are not real is the TL;DR of how
    // dynamic OR static typing works. Both of them are invented by humans.
    // However, static types tend to come more naturally to a computer, which
    // is why we think of them as the default. After all, if we have an
    // array full of `Item` structures, that is basically a static type.
    struct Item* item_2 = (void*) &tomato_item_2;

    // Even though `item_2` is really `Tomato`, not `Item`, we can still pass
    // it into the `Item` function. Since the "head of the struct," (the first
    // few fields) are the same, everything works!
    print_item(item_2);

    // Here's another complete example using yet another data-type (BoardGame).
    // As long as the struct heads are the same, we can perform substitution.
    // This is the Liskov Substitution Principle.
    struct BoardGame board_game_item_3;
    board_game_item_3.id = 3;
    board_game_item_3.name = "Scrabble";
    board_game_item_3.price = 30;
    board_game_item_3.minimum_age = 4;
    struct Item* item_3 = (void*) &board_game_item_3;
    print_item(item_3);
}
