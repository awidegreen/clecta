#ifndef SEARCH_HH
#define SEARCH_HH

#include <queue>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <memory>
#include "matcher.hh"

namespace clecta
{

class Search
{
public:
  typedef std::shared_ptr<Search> Ptr;

  enum { NON_SELECTED = -1 };

  Search(bool case_sensitive = false);
  Search(const Choices& choices, bool case_sensitive = false);
  ~Search();

  /**
   * get the score for every choice based on query term
   * note, copy needed because it will be put to lower internally 
   */
  void query(String term);

  void add_choice(const String& s) { _choices.push_back(s); }

  void selected(int s) { _selected = s; }
  int selected() const { return _selected; }
  String selection() const;
  
  size_t size_choices() const;

  void toggle_case_sensitive() { _case_sensitive = !_case_sensitive; }
  bool case_sensitive() const { return _case_sensitive; }

  /**
   * Returns sorted list of choices which match best to the query.
   */
  const Matches& matches() const { return _matches; };

  /**
   * Register a matcher. If no matcher has be registered yet, it will directly
   * be taken as the current matcher, otherwise it will be queued for selecting
   * a different matcher, see next_matcher().
   */
  void register_matcher(Matcher::Ptr matcher);

  /**
   * Gets the name of the currently selected matcher.
   */
  const char* matcher_name() 
  { 
    return _matcher ? _matcher->name() : "NoMatcher";
  }

  /**
   * Selects the next matcher from the previously registered ones.
   */
  void next_matcher();
private:
  typedef std::queue<Matcher::Ptr> Matchers;
  typedef std::stack<Choices> ChoicesStack;

  Choices _choices;
  Matches _matches;
  int _selected;
  bool _case_sensitive;
  Matcher::Ptr _matcher;
  Matchers _matchers;
  String _previous_query_string;
  ChoicesStack _chc_stack;
};


} // ns

#endif /* SEARCH_HH */
