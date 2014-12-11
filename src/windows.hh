#include <string>
#include <ncurses.h>

namespace clecta
{

class Search;

class ClectaWin
{
public:
  ClectaWin(WINDOW* parent, Search* search) : 
    _parent(parent), _search(search) { }
  virtual ~ClectaWin() {}

  virtual void init() = 0;
  virtual void handle(int key) = 0;
  virtual void draw() = 0;
  virtual WINDOW* curses_win() { return NULL; }
protected:
  WINDOW* _parent;
  Search* _search;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class InputWindow : public ClectaWin
{
public:
  InputWindow(WINDOW* p, Search* search, unsigned height = 1) : 
    ClectaWin(p, search),
    _height(height)
  { init(); }

  void init();
  void handle(int key);
  void draw();
  virtual WINDOW* curses_win() { return _win; }

private:
  WINDOW* _win;
  unsigned _height;
  std::wstring _in;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class ListWindow : public ClectaWin
{
public:
  ListWindow(WINDOW* p, Search* search, unsigned h_space = 1) : 
    ClectaWin(p, search),
    _h_space(h_space),
    _selected_row(-1)
  { init(); }

  void init();
  void handle(int key);
  void draw();
private:
  unsigned _h_space;
  int _selected_row;
  WINDOW* _win;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class StatusWindow : public ClectaWin
{
public:
  StatusWindow(WINDOW* p, Search* search, unsigned height = 1) : 
    ClectaWin(p, search),
    _height(height)
  { init(); }

  void init();
  void handle(int /* key */);
  void draw();
private:
  unsigned _height;
  unsigned _nb_choices;
  WINDOW* _win;
};

} // ns
