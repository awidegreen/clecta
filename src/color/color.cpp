#include "color.hh"

namespace clecta
{

//------------------------------------------------------------------------------

bool
Color::operator==(const Color& rhs) const
{ 
  return r == rhs.r && g == rhs.g && b == rhs.b; 
}

//------------------------------------------------------------------------------

bool
Color::is_set() const
{ 
  return r != NOT_SET &&  b != NOT_SET && g != NOT_SET; 
}

//------------------------------------------------------------------------------

/* static inline */
int
Color::curses_val(int color_portion)
{ 
  return static_cast<int>(color_portion*1000/255);
}

//------------------------------------------------------------------------------

static ColorScheme::ColorMap DEFAULT_COLOR_MAP {
  {WINDOW_BG, Color()},
  {WINDOW_FONT_FG, Color()},
  {WINDOW_FONT_BG, Color()},
  {STATUS_BG, Color()},
  {STATUS_FONT_FG, Color()},
  {STATUS_FONT_BG, Color()},
  {SELECTION_BG, Color()},
  {SELECTION_FONT_FG, Color()},
  {SELECTION_FONT_BG, Color()},
  {SELECTION_MATCH_FONT_FG, Color()},
  {SELECTION_MATCH_FONT_BG, Color()},
};

//------------------------------------------------------------------------------

ColorScheme::ColorScheme() : 
  colors(DEFAULT_COLOR_MAP)
{ }

//------------------------------------------------------------------------------

Color get_color_from_str(const std::string& color_str)
{                           
  auto size = color_str.size();

  if ( color_str.empty() ) return Color();
  size_t start_pos = (color_str[0] == '#') ? 1 : 0;
  if ( size-start_pos != 6 ) return Color();

  Color c;

  auto r_s = color_str.substr(start_pos, 2);
  start_pos+=2;
  auto g_s = color_str.substr(start_pos, 2);
  start_pos+=2;
  auto b_s = color_str.substr(start_pos, 2);

  try
  {
    c.r = std::stoi(r_s, nullptr, 16);
    c.b = std::stoi(b_s, nullptr, 16);
    c.g = std::stoi(g_s, nullptr, 16);
  } catch (std::invalid_argument&) {
  } catch (std::out_of_range&) {
  }

  return std::move(c);
}

} // end of namespace
