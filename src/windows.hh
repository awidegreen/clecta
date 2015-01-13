#include <string>
#include <ncurses.h>

#include "keys.hh"

namespace clecta
{

class Search;

class ClectaWin
{
public:
  ClectaWin(WINDOW* parent, Search* search) : 
    _parent(parent), _search(search) { }
  virtual ~ClectaWin() {}

  virtual void handle(CLECTA_KEY key) = 0;
  virtual void draw() = 0;
  virtual WINDOW* curses_win() { return NULL; }
protected:
  virtual void init() = 0;
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

  void handle(CLECTA_KEY key);
  void draw();
  virtual WINDOW* curses_win() { return _win; }

private:
  void init();
  WINDOW* _win;
  unsigned _height;
  std::wstring _in;

  // preparation for moving the cursor in the search term (M-b, M-a, C-d, ...).
  // for just editing search term at the end, this is certainly not needed
  // Anyhow, it used to set the cursor at the appropiate position.
  unsigned _cur_pos;
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
    _selected_row(-1),
    _max_visible(0)
  { init(); }

  void handle(CLECTA_KEY key);
  void draw();
private:
  void init();
  unsigned _h_space;
  int _selected_row;
  int _max_visible;
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

  void handle(CLECTA_KEY /* key */);
  void draw();
private:
  void init();
  unsigned _height;
  unsigned _nb_choices;
  WINDOW* _win;
};

} // ns
