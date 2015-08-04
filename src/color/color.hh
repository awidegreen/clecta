#ifndef COLOR_HH
#define COLOR_HH

#include <string>
#include <map>

namespace clecta
{

//------------------------------------------------------------------------------

extern const char* WINDOW_BG;
extern const char* WINDOW_FONT_BG;
extern const char* WINDOW_FONT_FG;
extern const char* STATUS_BG;
extern const char* STATUS_FONT_FG;
extern const char* STATUS_FONT_BG;
extern const char* SELECTION_BG;
extern const char* SELECTION_FONT_FG;
extern const char* SELECTION_FONT_BG;
extern const char* SELECTION_MATCH_FONT_FG;
extern const char* SELECTION_MATCH_FONT_BG;

//------------------------------------------------------------------------------

struct Color
{
  enum { NOT_SET = -1 };
  Color() : r(NOT_SET), g(NOT_SET), b(NOT_SET) {}
  Color(int r_, int g_, int b_) : r(r_),  g(g_), b(b_) {}
  bool is_set() const;
  bool operator==(const Color& rhs) const;

  static inline int curses_val(int color_portion);

  int r, g, b;
};

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

Color get_color_from_str(const std::string& color_str);

//------------------------------------------------------------------------------

struct ColorScheme
{
  typedef  std::map<std::string, Color> ColorMap;
  ColorScheme();

  std::string name;
  ColorMap colors; 

  bool valid() const { return !name.empty(); }
};

//------------------------------------------------------------------------------

} // ns

#endif /* COLOR_HH */
