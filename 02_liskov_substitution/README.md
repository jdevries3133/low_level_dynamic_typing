# Liskov Substitution Principle

The [Liskov Substitution
Principle](https://en.wikipedia.org/wiki/Liskov_substitution_principle) states
that if thing B satisfies all of the properties of thing A, it can be
substituted for thing A transparently.

In our example, we have three structs;

- `Item`
- `Tomato`
- `BoardGame`

These represent different types of things that can be in our store, but you'll
notice that `Tomato` and `BoardGame` are perfect sub-types of `Item`. If you
only look at the first three fields, they are indistinguishable. In fact, we can
say that `Tomato` is `Item`, and `BoardGame` also is `Item`.

In C, the compiler even specifies that structs will be laid out in memory
in the exact order that you specify them. You'll notice that if you change the
order of the first three fields in any of the struct definitions to mismatch,
the program will break and start printing gibberish. If you swap `id` and
`price`, you'll notice that the `id` starts getting printed out instead of the
`price`! If you swap, say, the `id` and the `name`, you might see the program
crash, since `printf` will end up treating a field which is actually an integer
as a string. This is why `structs` in C, C++, and Rust have that name. They're
not just classes or interfaces which are a glob of fields, they communicate
precise details about the bytes we should expect to have in-memory, and even the
order in which they appear (except for Rust, unless the `#[repr(C)]` macro is
applied to a `struct`).

If you've ever programmed in JavaScript, this experience of being smacked in the
face with surprising program errors only because you, the programmer, are not
keeping your data structures organized should feel very familiar.
