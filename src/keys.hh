#ifndef KEYS_HH
#define KEYS_HH

#include <ncurses.h>
 
#define CLECTA_KEY wint_t

namespace clecta
{
  
//------------------------------------------------------------------------------

enum CLECTA_KEYS
{
  CLECTA_KEY_CTRL_A = KEY_MAX+1,
  CLECTA_KEY_CTRL_N,
  CLECTA_KEY_CTRL_U,
  CLECTA_KEY_CTRL_P,
  CLECTA_KEY_ALT_B,
  CLECTA_KEY_ALT_D,
  CLECTA_KEY_ALT_F,
  CLECTA_KEY_NOT_MAPPED
};

//------------------------------------------------------------------------------

CLECTA_KEY map_alt_key(CLECTA_KEY c);

//------------------------------------------------------------------------------

CLECTA_KEY map_ctrl_key(CLECTA_KEY c);

//------------------------------------------------------------------------------

bool is_pressed_with_ctrl_key(CLECTA_KEY c);

//------------------------------------------------------------------------------

bool is_clecta_key(CLECTA_KEY key);

//------------------------------------------------------------------------------

} //namespace 


#endif /* KEYS_HH */
