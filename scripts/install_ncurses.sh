 #!/bin/sh
set -ex

wget http://ftp.gnu.org/pub/gnu/ncurses/ncurses-5.9.tar.gz
tar xzf ncurses-5.9.tar.gz
cd ncurses-5.9 
./configure --prefix=/usr --mandir=/usr/share/man \
 --with-shared --with-normal --without-debug --without-ada \
 --enable-widec --enable-pc-files --with-cxx-binding --with-cxx-shared \
  && make && sudo make install

