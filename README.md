# clecta

This a yet another port of garybernhardt/selecta in C++ using Ncurses for the 
terminal windows. UTF-8 is support.

### Keys
* `Home` go to the beginning match list
* `End` go to the beginning match list

### Not done yet
* the fuzzy finding is not done yet. Currently, it only searches for the given
query-string over all choices.
* some unit testing 
* more keybindings like `^D` and `^U` (see [selecta](https://github.com/garybernhardt/selecta) for more bindings)
* `cmake` installation target

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
