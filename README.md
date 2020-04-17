# pprintf - A format string printing function with extra functionality
`pprintf()` is essentially a `printf()` wrapper that allows a programmer to augment format strings
with colors and text formatting (i.e. bold, underline, etc). 

## Features
`pprintf()` currently allows for the following formats:
- Italics (%I)
- Bold (%D)
- Underline (%U)
- Red (%R)
- Green (%G)
- Yellow (%Y)
- Blue (%B)
- Magenta (%M)
- Cyan (%C)
- White (%W)

## Usage
The library can be compiled using the `make library` command.

`pprintf()` is designed to be a drop-in replacement for `printf()`.
The format characters listed above can be appended in any order to a `printf()` format character (ex: `%CIUs` for a cyan, italicized, and underlined text).
The following is an example for formatting a string to be bolded, underlined, and red:
```C
pprintf("This is a %DURs string.\n", "test");
```

When compiling the above code, include the `pprintf.h` header file and use the following command:
```
gcc file.c -o file -L/path/to/libpprintf.a -lpprintf
```

The function can be tested using the command `make test`. The program will test each of the possible formattings and compare them to an expected value. 
Additionally, the function can be tested for memory leaks using the `make leak` command.

## Limitations
Currently, the only `printf()` format string that is compatible is `%s`. 
