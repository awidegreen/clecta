#include "search.hh"
#include "windows.hh"

namespace clecta
{

class App
{
public:
  App();
  App(Search* search);
  ~App();
  
  void run();

  std::string get_selection();
private:
  void dispatch(int key, bool is_key_code);
  void init();

  Search* _search;
  SCREEN* _screen;

  ClectaWin* _inputw;
  ClectaWin* _statusw;
  ClectaWin* _listw;
};

} // ns
