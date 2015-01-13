#include "keys.hh"

#define CTRL_KEY_MASK 0x7F

// get key CHAR that was pressed together with CTRL
#define CTRL(c) (((c) - '@') & CTRL_KEY_MASK)

using namespace clecta;

//------------------------------------------------------------------------------

CLECTA_KEY clecta::map_alt_key(CLECTA_KEY c)
{
  switch( c )
  {
    case 'b':
      return CLECTA_KEY_ALT_B;
    case 'd':
      return CLECTA_KEY_ALT_D;
    case 'f':
      return CLECTA_KEY_ALT_F;
    default:
      return CLECTA_KEY_NOT_MAPPED;
  }
}

//------------------------------------------------------------------------------

CLECTA_KEY clecta::map_ctrl_key(CLECTA_KEY c)
{
  auto ch = CTRL(c);
  switch( ch )
  {
    case 'N':
      return CLECTA_KEY_CTRL_N;
    case 'P':
      return CLECTA_KEY_CTRL_P;
    case 'U':
      return CLECTA_KEY_CTRL_U;
    default:
      return CLECTA_KEY_NOT_MAPPED;
  }
}

//------------------------------------------------------------------------------

bool clecta::is_pressed_with_ctrl_key(CLECTA_KEY c)
{
  if ( c > CTRL_KEY_MASK ) return false;

  auto ch = CTRL(c);

  // was CTRL-A .. CTRL-Z ?
  return ch >= 'A' && ch <= 'Z';
}

//------------------------------------------------------------------------------
 
bool clecta::is_clecta_key(CLECTA_KEY key)
{
  return key >= CLECTA_KEY_CTRL_A && key < CLECTA_KEY_NOT_MAPPED /*only less*/; 
}
