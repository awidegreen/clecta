#include <utility>
#include <algorithm>

#include "search.hh"

using namespace clecta;

namespace 
{ // anonymous namespace

}


//------------------------------------------------------------------------------

Search::Search(bool case_sensitive /* false */) :
  _selected(NON_SELECTED),
  _case_sensitive(case_sensitive),
  _matcher(new SimpleMatcher)
{ }

//------------------------------------------------------------------------------

Search::Search(const Choices& choices, bool case_insensitive /* false */) :
  _choices(std::move(choices)),
  _selected(NON_SELECTED),
  _case_sensitive(case_insensitive),
  _matcher(new SimpleMatcher)
{ }

//------------------------------------------------------------------------------

Search::~Search()
{
  if ( _matcher ) delete _matcher;
}

//------------------------------------------------------------------------------

String
Search::selection() const
{
  if ( _selected != NON_SELECTED && (size_t)_selected < _matches.size() )
    return _matches.at(_selected).value;

  return L"";
}

//------------------------------------------------------------------------------

void 
Search::query(String term)
{
  _matches.clear();
                              
  if ( !_case_sensitive )
    std::transform(term.begin(), term.end(), term.begin(), ::tolower);

  for ( auto& s : _choices )
  {
    auto haystack = s;
    if ( !_case_sensitive )
      std::transform(haystack.begin(), haystack.end(), haystack.begin(), ::tolower);

    // get the score for every choice based on query term
    auto m = _matcher->get_score(term, haystack);
    if ( m.score > 0.0 )
    {
      m.value = s;
      _matches.push_back(m);
    }
  }

  // sort all matches based on previous calculated score, via lambda
  std::sort(_matches.begin(), _matches.end(), 
      [](const Match& a, const Match& b) 
      {
        return b.score < a.score;
      });
}

//------------------------------------------------------------------------------

size_t 
Search::size_choices() const
{
  return _choices.size();
}

//------------------------------------------------------------------------------

