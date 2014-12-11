#include <utility>
#include "search.hh"

using namespace clecta;

Search::Search() :
  _max_matches(5),
  _selected(-1)
{ }

//------------------------------------------------------------------------------

Search::Search(const Choices& choices) :
  _choices(std::move(choices)),
  _max_matches(5),
  _selected(-1)
{

}

//------------------------------------------------------------------------------

std::wstring
Search::selection() const
{
  if ( _selected >= 0 && (size_t)_selected < _matches.size() )
    return _matches.at(_selected);

  return L"";
}

//------------------------------------------------------------------------------

void 
Search::query(const std::wstring& term)
{
  _matches.clear();
  unsigned ctr = 0;

  for ( auto& s : _choices )
  {
    ++ctr;

    auto found = s.find(term);
    if ( found != std::string::npos )
      _matches.push_back(s);
     
    // only show max 
    if ( ctr >= _max_matches ) break;
  }

}

//------------------------------------------------------------------------------

size_t 
Search::size_choices() const
{
  return _choices.size();
}
