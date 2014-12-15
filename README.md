# clecta

This a yet another port of [garybernhardt/selecta](https://github.com/garybernhardt/selecta) in C++ using Ncurses for the 
terminal windows. UTF-8 is support.

### Some features
* highlight matching string in match list
* (currently) prefer matches add the end of candidate 
* switch case sensitivity for search `F2`
* NO fuzzy search yet 
* status-line shows some stats about the candidates

### Keys
* `Home` go to the beginning match list
* `End` go to the beginning match list
* `F2` toggle case-sensitivity
* `Arrow-Up` and `Arrow-down` for selecting string from match list 

### to be done 
* fuzzy finding; currently, it only searches for the given query-string over all choices.
* some unit testing 
* more keybindings like `^D` and `^U` (see [selecta](https://github.com/garybernhardt/selecta) for more bindings)
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
