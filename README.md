# clecta

[![Build Status](https://travis-ci.org/awidegreen/clecta.svg?branch=master)](https://travis-ci.org/awidegreen/clecta)

This is yet another port of [garybernhardt/selecta](https://github.com/garybernhardt/selecta) in C++ using Ncurses for the terminal window. UTF-8 is supported.

### Some features
* highlight matching string in match list
* (currently) prefer matches add the end of candidate 
* switch case sensitivity for search `F2`
* fuzzy search yet using Command-T select algorithm
* status-line shows some stats about the candidates
* resize support

![Demo](https://raw.github.com/awidegreen/clecta/master/demo.gif)

### Keys
* `Home` go to the beginning match list
* `End` go to the beginning match list
* `F2` toggle case-sensitivity
* `F3` switch the matcher
* `Arrow-Up` and `Arrow-Down` for selecting string from match list 
* `^U` Empty search term
* `^N` alias for `Arrow-Down`
* `^P` alias for `Arrow-Up`

### to be done 
* mutlithreading for `Search::get_score`; this can be done Matcher independent
* move matcher in its own (static) library
* ability to load matcher via a library 
* add more key bindings, see [readline](https://en.wikipedia.org/wiki/Bash_%28Unix_shell%29#Keyboard_shortcuts)
* `cmake` installation target
* Improve matches/candidate handling (no copy)

### Examples

```
cat $(find . | clecta)
```

### Dependencies
* Ncurses

### Installation
```
git clone https://github.com/awidegreen/clecta 
mkdir clecta/build
cd clecta/build
cmake ..
make
# or use: make -j`nproc`
```
### Tests
In the build directory, use:
* the *test* target: `make test` or
* run the tests manually: `tests/clecta-tests`

### License
Copyright (c) Armin Widegreen

Distributed under the BSD-Clause-2 License, see LICENSE.

[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/awidegreen/clecta/trend.png)](https://bitdeli.com/free "Bitdeli Badge")
