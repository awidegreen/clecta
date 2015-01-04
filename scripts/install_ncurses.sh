 #!/bin/sh
set -ex

wget http://ftp.gnu.org/pub/gnu/ncurses/ncurses-5.9.tar.gz
tar xzf ncurses-5.9.tar.gz
cd ncurses-5.9 && ./configure --prefix=/usr && make && sudo make install

