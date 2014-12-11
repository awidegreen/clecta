#include <list>
#include <vector>
#include <string>

namespace clecta
{

class Search
{
public:
  typedef std::list<std::wstring> Choices;
  typedef std::vector<std::wstring> Matches;

  Search();
  Search(const Choices& choices);

  void max_matches(unsigned size) { _max_matches = size; }

  void query(const std::wstring& term);

  void add_choice(const std::wstring s) { _choices.push_back(s); }

  void selected(int s) { _selected = s; }
  int selected() const { return _selected; }
  std::wstring selection() const;
  
  size_t size_choices() const;
  /**
   * Returns sorted list of choices which match best to the query.
   */
  const Matches& matches() const { return _matches; };
private:
  Choices _choices;
  Matches _matches;
  unsigned _max_matches;
  int _selected;
};

} // ns
