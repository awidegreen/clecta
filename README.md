# clecta

[![Build Status](https://travis-ci.org/awidegreen/clecta.svg?branch=master)](https://travis-ci.org/awidegreen/clecta)

This is yet another port of [garybernhardt/selecta](https://github.com/garybernhardt/selecta) in C++ using Ncurses for the terminal window. UTF-8 is support.

### Some features
* highlight matching string in match list
* (currently) prefer matches add the end of candidate 
* switch case sensitivity for search `F2`
* NO fuzzy search yet 
* status-line shows some stats about the candidates
* resize support

![Demo](https://raw.github.com/awidegreen/clecta/master/demo.gif)

### Keys
* `Home` go to the beginning match list
* `End` go to the beginning match list
* `F2` toggle case-sensitivity
* `Arrow-Up` and `Arrow-Down` for selecting string from match list 
* `^U` Empty search term
* `^N` alias for `Arrow-Down`
* `^P` alias for `Arrow-Up`

### to be done 
* fuzzy finding; currently, it only searches for the given query-string over all choices.
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
```

### License
Copyright (c) Armin Widegreen

Distributed under the BSD-Clause-2 License, see LICENSE.

[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/awidegreen/clecta/trend.png)](https://bitdeli.com/free "Bitdeli Badge")
