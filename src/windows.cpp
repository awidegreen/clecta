#include <ncurses.h>

#include "windows.hh"
#include "search.hh"

#define INWIN_PROMPT_PEFEX L">>> "

using namespace clecta;


//------------------------------------------------------------------------------

void InputWindow::init()
{
  int max_x, max_y;
  getmaxyx(_parent, max_y, max_x);
  _win = derwin(_parent, _height, max_x, 0, 0);
  keypad(_win, true);

  draw();
}

//------------------------------------------------------------------------------

void
InputWindow::handle(int key) 
{
  switch ( key )
  {
    // ignore this keys!
    case KEY_UP: case KEY_DOWN: 
    case KEY_HOME: case KEY_END:
      return;
    case 127:  
    case KEY_BACKSPACE:
      if ( _in.size() ) _in.pop_back();
      break;
    default:
      _in += key;
  };

  _search->query(_in);

  draw();
}

//------------------------------------------------------------------------------

void
InputWindow::draw()
{

  wdeleteln(_win);
  std::wstring tmp = INWIN_PROMPT_PEFEX;
  tmp += _in;

  mvwaddwstr(_win, 0, 0, tmp.c_str());
  wrefresh(_win);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void 
ListWindow::init()
{
  int max_x, max_y;
  getmaxyx(_parent, max_y, max_x);
  _win = derwin(_parent, max_y-_h_space, max_x, _h_space, 0);
  //wmove(_win, 0, 1);
  //box(_win, '|', '-');
  wrefresh(_win);
  draw();
}

//------------------------------------------------------------------------------

void
ListWindow::handle(int key)
{
  if ( key == KEY_DOWN )
  {
    if (_selected_row < (int)_search->matches().size()-1 )
      _selected_row++;
    // else ignore
  }
  else if ( key == KEY_UP )
  {  
    if (_selected_row > 0) 
      _selected_row--;
    // else ignore
  } 
  else if ( key == KEY_HOME )
    _selected_row = 0;
  else if ( key == KEY_END )
    _selected_row = (int)_search->matches().size()-1;
  else 
    _selected_row = 0;
  draw();
}

//------------------------------------------------------------------------------

void
ListWindow::draw()
{;
  wclear(_win);
  const auto& matches = _search->matches();
  unsigned line = 0; // start at line 0

  for (auto& s : matches)
  {
    mvwaddwstr(_win, line, 0, s.c_str() );
    ++line;
  }

  // highlight seleted row
  if ( _selected_row > -1 && matches.size() != 0)       
  {
    mvwchgat(_win, _selected_row, 0, -1, A_REVERSE, 1, nullptr);
    _search->selected(_selected_row);
  }

  wrefresh(_win);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void StatusWindow::init()
{
  _nb_choices = _search->size_choices();
  int max_x, max_y;
  getmaxyx(_parent, max_y, max_x);
  _win = derwin(_parent, _height, max_x, 1, 0);
  keypad(_win, true);

  wbkgd(_win, COLOR_PAIR(1));

  draw();
}

//------------------------------------------------------------------------------

void
StatusWindow::handle(int /* key */) 
{
  draw();
}

//------------------------------------------------------------------------------

void
StatusWindow::draw()
{
  wdeleteln(_win);

  mvwprintw(_win, 0, 0, "shown: %zu/%zu selected: %d",   
      _search->matches().size(), 
      _nb_choices,
      _search->selected()+1);

  wrefresh(_win);
}
