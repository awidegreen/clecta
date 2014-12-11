#include <ncurses.h>

#include "app.hh"

using namespace clecta;

//------------------------------------------------------------------------------

App::App() : 
  _search(new Search())
{
  init();
}
//------------------------------------------------------------------------------

App::App(Search* search) : 
  _search(search)
{
  init();
}

//------------------------------------------------------------------------------

App::~App()
{
  delete _search;
  delete _inputw;
  delete _listw;
  delscreen(_screen);
}

//------------------------------------------------------------------------------

void 
App::init()
{
  //initscr();
  auto term = fopen("/dev/tty", "rw+");
  _screen = newterm(NULL, term, term);
  set_term(_screen);
  start_color();
  use_default_colors();


  init_pair(1, COLOR_WHITE, COLOR_BLUE);

  curs_set(0);
  noecho(); // let me print chars myself
  cbreak(); // let me handle new line etc, execpt C-c et. al.

  _inputw = new clecta::InputWindow(stdscr, _search, 1);
  _statusw = new clecta::StatusWindow(stdscr, _search, 1);
  _listw = new clecta::ListWindow(stdscr, _search, 2);

  auto max_y = getmaxy(stdscr);
  _search->max_matches(max_y);

  _inputw->init();
  _statusw->init();
  _listw->init();

}

//------------------------------------------------------------------------------

void
App::run()
{

  for (;;)
  {
    wint_t c;           
    int r = wget_wch(_inputw->curses_win(), &c);
    if ( r == ERR )
    {
      endwin();
      return;
    }

    if ( c == KEY_ENTER || c == '\n' )
    {
      endwin();
      return;
    }

    dispatch(c, r == KEY_CODE_YES );
  }

}

//------------------------------------------------------------------------------

void
App::dispatch(int key, bool is_key_code)
{

  if ( is_key_code  )
  { // we do not handle any other special keys and newline!
    switch (key)
    {
      case KEY_DOWN:
      case KEY_UP:
      case KEY_HOME:
      case KEY_END:
        break;
      default:
        return;
    }
  }

  _inputw->handle(key);
  _listw->handle(key);
  _statusw->handle(key);
}
