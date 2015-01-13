#include <ncurses.h>

#include <iostream>
#include <cctype>
#include <regex>

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
  _cur_pos = 0;

  draw();
}

//------------------------------------------------------------------------------

void
InputWindow::handle(CLECTA_KEY key) 
{
  bool valid = false;

  switch ( key )
  {
    // del char handling
    case 127: 
    case KEY_BACKSPACE:
      if ( _in.size() ) _in.pop_back();
      valid = true;
        _cur_pos--;
      break;
    case CLECTA_KEY_CTRL_U: 
      // clean search string
      _in = L"";
      valid = true;
      break;
    // every other char handling
    default:
      if ( std::isprint(key) ) 
      {
        _in += key;
        _cur_pos++;
        valid = true;
      }
      break;
  };

  if ( valid )
  { // only if valid key, set query
    _search->query(_in);
  }

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
  wmove(_win, 0, _cur_pos+tmp.size());
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

  _max_visible = getmaxy(_win);
  wrefresh(_win);
  draw();
}

//------------------------------------------------------------------------------

void
ListWindow::handle(CLECTA_KEY key)
{
  switch ( key )
  {
    case CLECTA_KEY_CTRL_N:
    case KEY_DOWN:
    {
      auto matches_size = _search->matches().size();
      if (_selected_row < _max_visible-1 && 
          _selected_row < (int)matches_size-1)
      {
        // don't go any further down if there are no more matches or max 
        // height is alread reached.
        _selected_row++;
      }
      // else ignore
      break;

    }
    case CLECTA_KEY_CTRL_P: 
    case KEY_UP:
      if (_selected_row > 0) 
        _selected_row--;
      // else ignore
      break;
    case KEY_HOME:
      _selected_row = 0;
      break;
    case KEY_END:
      _selected_row = _max_visible-1;
      break;
    case KEY_RESIZE:
      std::cerr << "Got resize event" << std::endl;
      // new size, could imply different amount of visible items
      int max_x, max_y;
      getmaxyx(_parent, max_y, max_x);
      wresize(_win, max_y-_h_space, max_x);
      _max_visible = getmaxy(_win);
      break;
    default:
      _selected_row = 0;
      break;
  }

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
    //auto p = std::to_wstring(s.score) + L" " + s.value;
    auto p = s.value;

    mvwaddwstr(_win, line, 0, p.c_str() );
    if ( _selected_row == (int)line )
    {
      // highlight selected row
      mvwchgat(_win, _selected_row, 0, -1, A_REVERSE, 0, nullptr);
      _search->selected(_selected_row);
    }
    mvwchgat(_win, line, s.begin, s.end-s.begin, A_NORMAL, 2 /*colorpair*/, nullptr);
    ++line;
    if ( line == (unsigned)_max_visible ) break; 
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

  // set background color
  wbkgd(_win, COLOR_PAIR(1));

  draw();
}

//------------------------------------------------------------------------------

void
StatusWindow::handle(CLECTA_KEY /* key */) 
{
  draw();
}

//------------------------------------------------------------------------------

void
StatusWindow::draw()
{
  wdeleteln(_win);

  mvwprintw(_win, 0, 0, "filtered: %zu total: %zu selected: %d",   
      _search->matches().size(), 
      _nb_choices,
      _search->selected()+1);

  std::wstring case_sense_str = L"case-sensitive";
  if ( !_search->case_sensitive() )
    case_sense_str = L"case-insensitive";

  auto start_x = getmaxx(_win) - case_sense_str.size();
  mvwaddwstr(_win, 0, start_x, case_sense_str.c_str());

  wrefresh(_win);
}
