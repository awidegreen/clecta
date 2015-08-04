#ifndef PARSER_HH
#define PARSER_HH

#include <istream> 

#include "color.hh"

namespace clecta
{

ColorScheme parse_by_file(const std::string& file);
ColorScheme parse_by_stream(std::istream& in);

} // end of namespace


#endif /* PARSER_HH */
