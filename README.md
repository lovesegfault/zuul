# zuul
A dead simple test framework for C

## Motivation

zuul aims to be a simple, magic-free, extensible text framework for C99. I felt
the need for something like this when working on the C projects at 
[p-challenges](https://github.com/bemeurer/p-challenges). After going through
a few alternatives and not being satisfied, I decided to make my own. Mostly I
was annoyed by the overwhelming use of weird macros, GNU extensions, and 
non-portable code. zuul has no macros, in fact it's composed solely of 3 
functions, and works with any compiler, and with all POSIX systems.

## Features

* Portable
    - zuul works on all Linux, MacOS, BSD, and other POSIX systems
* Simple
    - zuul is composed of only 3 functions!
* Extensible
    - zuul can easily be modified to attend to your needs
* Fast
    - zuul does status output on a separate thread
* Pretty
    - zuul produces beautiful, readable output
* Interoperable
    - zuul can output to JSON and other formats
    
## Non-goals

zuul's main objective is to be simple, and magic-free. This, to some, might
equate to it being lackluster; that's okay. zuul will never come with the
kitchen sink, but you can, and I recommend you to, modify it to suit your
specific needs. Think you've improved it? Make a PR!

## Usage
##### NOT STABLE


## Demo

[![asciicast](https://asciinema.org/a/0WyeNeFC5Ma3bS9uOEnTJPefY.png)](https://asciinema.org/a/0WyeNeFC5Ma3bS9uOEnTJPefY)

