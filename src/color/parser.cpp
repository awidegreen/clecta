#include "parser.hh"

#include <fstream>
#include <algorithm>

namespace
{
  void strip_space(std::string& str)
  {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
  }

  void lower(std::string& str)
  {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  }

}

//------------------------------------------------------------------------------

namespace clecta
{
const char* WINDOW_BG               = "window-bg";
const char* WINDOW_FONT_BG          = "window-font-bg";
const char* WINDOW_FONT_FG          = "window-font-fg";
const char* STATUS_BG               = "statusbar-bg";
const char* STATUS_FONT_FG          = "statusbar-font-fg";
const char* STATUS_FONT_BG          = "statusbar-font-bg";
const char* SELECTION_BG            = "selection-bg";
const char* SELECTION_FONT_FG       = "selection-font-fg";
const char* SELECTION_FONT_BG       = "selection-font-bg";
const char* SELECTION_MATCH_FONT_FG = "selection-match-font-fg";
const char* SELECTION_MATCH_FONT_BG = "selection-match-font-bg";

//------------------------------------------------------------------------------

ColorScheme 
parse_by_file(const std::string& file)
{
  std::ifstream f(file);
  if (!f.is_open())
    return ColorScheme();
  
  return std::move(parse_by_stream(f));
}

//------------------------------------------------------------------------------

ColorScheme 
parse_by_stream(std::istream& in)
{
  ColorScheme scheme;

  std::string line;
  auto eq_pos = std::string::npos;
  while ( std::getline(in, line) )
  {
    strip_space(line);
    lower(line);
    // ignore lines with comments or empty lines
    if (line.empty() || line.find(";") == 0 )
      continue;
    eq_pos = line.find("=");
    auto type = line.substr(0, eq_pos);
    auto val = line.substr(eq_pos+1);

    // there can be a comment after the value 
    auto cmnt_end_pos = val.find(";");
    if (cmnt_end_pos != std::string::npos )
      val = val.substr(0, cmnt_end_pos);

    if ( type == "name" )
    { 
      scheme.name = val;
      continue;
    }
    
    auto it = scheme.colors.find(type);
    if ( it != scheme.colors.end() )
    {        
      auto c = get_color_from_str(val);
      if ( c.is_set() ) 
        it->second = std::move(c);
    }     

    // found another entry which should be parsed 
    if ( it == scheme.colors.end() ) 
      return ColorScheme();
  }

  return std::move(scheme);
}

//------------------------------------------------------------------------------

} // end of ns
