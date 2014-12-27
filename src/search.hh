#include <list>
#include <vector>
#include <string>

namespace clecta
{

class Search
{
public:
  typedef std::wstring String;

  /**
   * Score value is dynamitic where 0 is the worst!
   */
  struct Match {
    float score;
    size_t begin;
    size_t end;
    String value;
  };

  typedef std::list<String> Choices;
  typedef std::vector<Match> Matches;

  enum { NON_SELECTED = -1 };

  Search(bool case_insensitive = false);
  Search(const Choices& choices, bool case_insensitive = false);

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

  void toggle_case_sensitive() { _case_insensitive = !_case_insensitive; }
  bool case_insensitive() const { return _case_insensitive; }

  /**
   * Returns sorted list of choices which match best to the query.
   */
  const Matches& matches() const { return _matches; };
private:
  Search::Match get_score(const Search::String& query, Search::String candidate) const;

  Choices _choices;
  Matches _matches;
  int _selected;
  bool _case_insensitive;
};

} // ns
