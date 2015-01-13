#include <ncurses.h>

//#include <iostream>

#include "app.hh"
#include "keys.hh"

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
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);

  curs_set(0);
  noecho(); // let me print chars myself
  cbreak(); // let me handle new line etc, execpt C-c et. al.

  _search->query(L"");

  _inputw = new clecta::InputWindow(stdscr, _search, 1);
  _statusw = new clecta::StatusWindow(stdscr, _search, 1);
  _listw = new clecta::ListWindow(stdscr, _search, 2);
}

//------------------------------------------------------------------------------

void
App::run()
{

  for (;;)
  {
    CLECTA_KEY c;           
    int r = wget_wch(_inputw->curses_win(), &c);
    if ( r == ERR )
    {
      endwin();
      return;
    }

    //std::cerr << "keyname: " << c << std::endl;
    //std::cerr << "keyname ctrl: " << CTRL(c) << std::endl;

    switch (c) 
    {
      //--- return key handling
      case KEY_ENTER: 
      case '\n': 
      case '\r':
        endwin();
        return;
      //-----
      //--- ESC/ ALT+key handling, see man getch
      case 0x1b: // ESC 
        if ( wget_wch(_inputw->curses_win(), &c) == ERR )
        {
          //std::wcerr << L"ESC was pressed!" << std::endl;
          // well that was escape!
          exit(1);
        }
        //std::wcerr << L"ALT+" << key_name(c) << L" pressed."<< std::endl;
        r = KEY_CODE_YES;
        break;
      //-----
      default:
        if ( is_pressed_with_ctrl_key(c) )
        {
          //std::wcerr << L"ctrl was pressed!" << std::endl;
          r = KEY_CODE_YES;
          c = map_ctrl_key(c);
        }
        break;
    }

    dispatch(c, r == KEY_CODE_YES );

  }

}

//------------------------------------------------------------------------------

void
App::dispatch(CLECTA_KEY key, bool is_key_code)
{

  if ( is_key_code  )
  { 
    switch (key)
    {
      case KEY_DOWN:
      case KEY_UP:
      case KEY_HOME:
      case KEY_END:
      case KEY_RESIZE:
        break;
      case KEY_F(2):
        _search->toggle_case_sensitive();
        break;
      default:
        if ( is_clecta_key(key) )
          break;
        else
          return;
    }
  }

  _inputw->handle(key);
  _listw->handle(key);
  _statusw->handle(key);
}
