#include <utility>
#include <algorithm>

#include "search.hh"
#include "match_filter.hh"

using namespace clecta;


//------------------------------------------------------------------------------

Search::Search(bool case_sensitive /* false */) :
  _selected(NON_SELECTED),
  _case_sensitive(case_sensitive) 
{ }

//------------------------------------------------------------------------------

Search::Search(const Choices& choices, bool case_insensitive /* false */) :
  _choices(std::move(choices)),
  _selected(NON_SELECTED),
  _case_sensitive(case_insensitive)
{ }

//------------------------------------------------------------------------------

Search::~Search()
{
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

  // no matcher selected, bail out!
  if ( !_matcher )
  {
    Match m;
    m.value = L"no matcher registered, unable to filter";
    _matches.push_back(m);
    return;
  }

  if ( term.empty() )
  {
    std::for_each(_choices.begin(), _choices.end(), [this](const String& str)
        {
          Match m;
          m.value = str;
          _matches.push_back(m);
        });
    return;
  }
  
  MatchFilter<Choices::const_iterator> concurrent_filter(
      _matcher, _case_sensitive);
  _matches = concurrent_filter.apply(term, _choices.begin(), _choices.end());
                              
  //if ( !_case_sensitive )
    //std::transform(term.begin(), term.end(), term.begin(), ::tolower);

  //for ( auto& s : _choices )
  //{
    //auto haystack = s;
    //if ( !_case_sensitive )
      //std::transform(haystack.begin(), haystack.end(), haystack.begin(), ::tolower);

    //// get the score for every choice based on query term
    //auto m = _matcher->get_score(term, haystack);
    //if ( m.score > 0.0 )
    //{
      //m.value = s;
      //_matches.push_back(m);
    //}
  //}

  //// sort all matches based on previous calculated score, via lambda
  //std::sort(_matches.begin(), _matches.end(), 
      //[](const Match& a, const Match& b) 
      //{
        //return b.score < a.score;
      //});
}

//------------------------------------------------------------------------------

size_t 
Search::size_choices() const
{
  return _choices.size();
}

//------------------------------------------------------------------------------

void 
Search::next_matcher()
{
  if ( _matchers.empty() || !_matcher ) return;

  auto tmp = std::move(_matcher);
  _matcher = _matchers.front();
  _matchers.pop();
  _matchers.push(std::move(tmp));
}

//------------------------------------------------------------------------------

void 
Search::register_matcher(Matcher::Ptr matcher)
{
  if ( !_matcher )
    _matcher = matcher;
  else
    _matchers.push(std::move(matcher));
}

//------------------------------------------------------------------------------
