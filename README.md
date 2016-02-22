# Rufunge

Rufunge is a befunge clone which aims to make the language more practical,
because that is obviously what befunge needs.

## Aims

* Should be a "superset" of befunge: a befunge program should work in rufunge.
* Adds good support for subprogram libraries.
* Adds file access.

## Features

* From befunge
    * Program flow, ext
    * Output
    * Arithmetic, Stack Manip,
    * Get put
    * Todo:
        * Input
        * Jump
        * Wrapping canvas
* rufunge superset
    * Foundations are in place, but no mechanism to load subprograms yet
    * todo:
        * load rufunge files as symbols to call.
        * load C++ files as symbols to call.
* Standard libraries
    * todo:
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
