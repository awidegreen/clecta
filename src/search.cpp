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

  if ( _previous_query_string.empty() && _chc_stack.empty() )
    _chc_stack.push(_choices);

  // char removed
  if ( term.size() < _previous_query_string.size() )
    _chc_stack.pop();

  bool add2stack = false;
  // char added
  if ( term.size() > _previous_query_string.size() )
    add2stack = true;

  _previous_query_string = term;

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

  const auto& current_choices = _chc_stack.top();
  MatchFilter<Choices::const_iterator> filter(
      _matcher, _case_sensitive);
  //_matches = filter.get(term, _choices.begin(), _choices.end());
  _matches = filter.get_concurrent(term, 
      current_choices.begin(), current_choices.end());

  if ( add2stack )
  {
    Choices chcs;
    for ( auto& m : _matches )
    {
      chcs.push_back(*(m.choice_iter));
    }
    _chc_stack.push(chcs);
  }
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
