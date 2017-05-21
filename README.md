# Rufunge

Rufunge is a befunge clone which aims to make the language more practical,
because that is obviously what befunge needs.

## Aims

* Should be a "superset" of befunge: a befunge program should work in rufunge.
* Adds good support for subprogram libraries.
* Adds file access.

## Features

* From Befunge:
    * Program flow, ext
    * Output
    * Arithmetic, Stack Manip,
    * Get put
    * Todo:
        * Input
        * Jump
        * Wrapping canvas
* Rufunge superset
    * Load other Rufunge scripts as instructions.
    * Todo:
        * load C++ files as symbols to call.
        * Standard library:
            * String manip (length, compare)
            * file system
            * networking
            * graphics

# Usage

## Compilation

```Shell
sudo apt-get install build-essential cmake
cmake .
make -j3
```

## Run

```Shell
./bin/a path/to/rufunge.rf
```

eg `./bin/a examples/hello_world.rf`

## Unit Tests

```Shell
./test.sh
```
