#ifndef APP_HH
#define APP_HH

#include "search.hh"
#include "windows.hh"

namespace clecta
{

class App
{
public:
  App();
  App(Search::Ptr search);
  ~App();
  
  void run();

private:
  void dispatch(CLECTA_KEY key, bool is_key_code);
  void init();

  Search::Ptr _search;
  SCREEN* _screen;
  FILE* _terminal;

  ClectaWin::Ptr _inputw;
  ClectaWin::Ptr _statusw;
  ClectaWin::Ptr _listw;

};

} // ns

#endif /* APP_HH */
