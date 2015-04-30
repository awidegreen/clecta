#ifndef TEST_UTILS_HH
#define TEST_UTILS_HH

// stl
#include <stdexcept>
#include <iostream> 

#include <search.hh> 

namespace clecta
{
namespace test
{
  //----------------------------------------------------------------------------

  template<class Matcher>
  Search
  get_search(const Search::Choices choices, const String& query)
  {
    Search s{choices};
    s.matcher(new Matcher());
    s.query(query);
    return s;
  }

  //----------------------------------------------------------------------------

  const Match&
  match_at(const Search& search, size_t at);

  //----------------------------------------------------------------------------

  std::pair<unsigned, clecta::Match> 
  contains(const Search::Matches& matches, const String& str);

  //----------------------------------------------------------------------------

  void 
  print(const Search::Matches& matches);

  //----------------------------------------------------------------------------
  
}
}

#endif /* TEST_UTILS_HH */
