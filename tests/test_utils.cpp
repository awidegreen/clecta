#include "test_utils.hh"

using namespace clecta;

//------------------------------------------------------------------------------

const Match&
clecta::test::match_at(const Search& search, size_t at)
{         
  if ( at+1 > search.matches().size() )
    throw std::out_of_range("matches.size less than 'at'");

  return search.matches().at(at);
}

//------------------------------------------------------------------------------

std::pair<unsigned, clecta::Match> 
clecta::test::contains(const Matches& matches, const String& str)
{
  unsigned cnt = 1;
  for(const auto& m : matches )
  {
    if (m.value == str) 
      return std::make_pair(cnt, m);
    ++cnt;
  }
  // return first as 0 which is FALSE!
  return std::make_pair(0, clecta::Match{});
}

//------------------------------------------------------------------------------

void 
clecta::test::print(const Matches& matches)
{
  unsigned cnt = 1;
  for ( const auto& m : matches )
  {
    std::wcerr << cnt << L": " << m.str();
    ++cnt;
  }
}

//------------------------------------------------------------------------------
