#include <utility>
#include <algorithm>
#include "search.hh"

using namespace clecta;

namespace 
{ // anonymous namespace

}

Search::Search(bool case_insensitive /* false */) :
  _selected(-1),
  _case_insensitive(case_insensitive)
{ }

//------------------------------------------------------------------------------

Search::Search(const Choices& choices, bool case_insensitive /* false */) :
  _choices(std::move(choices)),
  _selected(-1),
  _case_insensitive(case_insensitive)
{ }

//------------------------------------------------------------------------------

Search::String
Search::selection() const
{
  if ( _selected >= 0 && (size_t)_selected < _matches.size() )
    return _matches.at(_selected).value;

  return L"";
}

//------------------------------------------------------------------------------

void 
Search::query(String term)
{
  _matches.clear();
                              
  if ( _case_insensitive )
    std::transform(term.begin(), term.end(), term.begin(), ::tolower);

  for ( auto& s : _choices )
  {
    // get the score for every choice based on query term
    auto m = get_score(term, s);
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

Search::Match 
Search::get_score(const Search::String& query, Search::String candidate) const
{
  Search::Match m { 0.0, 0, 0, L"" };
  // if query is empty, everything should be found!
  if ( query.size() == 0 ) 
  {
    m.score = 0.1;
    return m;
  }

  if ( _case_insensitive )
    std::transform(
        candidate.begin(), candidate.end(), candidate.begin(), ::tolower);

  float score = 0.0;
  auto found = candidate.find(query);
  if ( found != std::string::npos )
  {
    ++score;
    //score += (found+query.size())/(float)candidate.size();
    score += 1 - (candidate.size()-(found + query.size()))/10.0;
    m.score = score;
    m.begin = found;
    m.end = found + query.size();
  }

  return m;
}
