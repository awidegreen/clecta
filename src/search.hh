#include <list>
#include <vector>
#include <string>
#include "matcher.hh"

namespace clecta
{

class Search
{
public:
  typedef std::list<String> Choices;
  typedef std::vector<Match> Matches;

  enum { NON_SELECTED = -1 };

  Search(bool case_sensitive = false);
  Search(const Choices& choices, bool case_sensitive = false);
  ~Search();

  /**
   * get the score for every choice based on query term
   * note, copy needed because it will be put to lower internally 
   */
  void query(String term);
  void matcher(Matcher* matcher) { _matcher = matcher; }

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
private:

  Choices _choices;
  Matches _matches;
  int _selected;
  bool _case_sensitive;
  Matcher* _matcher;
};


} // ns
