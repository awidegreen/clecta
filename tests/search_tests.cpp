// include global test defs
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <search.hh>

using namespace clecta;

namespace 
{
  std::pair<unsigned, clecta::Search::Match> 
  contains(const Search::Matches& matches, const Search::String& str)
  {
    unsigned cnt = 1;
    for(const auto& m : matches )
    {
      if (m.value == str) 
        return std::make_pair(cnt, m);
      ++cnt;
    }
    // return first as 0 which is FALSE!
    return std::make_pair(0, clecta::Search::Match());
  }


//------------------------------------------------------------------------------

  std::wstring 
  print(const Search::Match& m)
  {
    std::wstringstream ss;
    ss << m.value << L": [begin:" << m.begin 
      << L", end:" << m.end 
      << L", score:" << m.score 
      << L"]" << std::endl;
    return ss.str();
  }
  
//------------------------------------------------------------------------------

  void 
  print(const Search::Matches& matches)
  {
    unsigned cnt = 1;
    for ( const auto& m : matches )
    {
      std::wcerr << cnt << L":" << print(m);
      ++cnt;
    }
  }
}

//------------------------------------------------------------------------------

TEST(SearchTest, InitEmpty)
{
  Search search;

  EXPECT_EQ(false, search.case_insensitive());

  EXPECT_EQ(L"", search.selection());

  EXPECT_EQ(0, search.size_choices());

  auto& matches = search.matches();
  EXPECT_EQ(0, matches.size());
  EXPECT_EQ(Search::NON_SELECTED, search.selected());
}

//------------------------------------------------------------------------------

TEST(SearchTest, InitWithChoices)
{
  Search::Choices choices = {
    L"foo", 
    L"bar", 
    L"hallo", 
    L"welt", 
    L"bla", 
  };

  Search search(choices);
  auto matches = search.matches();
  EXPECT_EQ(0, matches.size());
  EXPECT_EQ(Search::NON_SELECTED, search.selected());

  search.query(L"");
  EXPECT_EQ(Search::NON_SELECTED, search.selected());

  matches = search.matches();
  EXPECT_EQ(5, matches.size());
}

//------------------------------------------------------------------------------

TEST(SearchTest, AddChoices)
{
  Search::Choices choices = {
    L"foo", 
    L"bar", 
    L"hallo", 
    L"welt", 
    L"bla", 
  };

  Search search;

  for (auto& c : choices )
  {
    search.add_choice(c);
  }

  auto matches = search.matches();
  EXPECT_EQ(0, matches.size());

  search.query(L"");
  EXPECT_EQ(Search::NON_SELECTED, search.selected());

  matches = search.matches();
  EXPECT_EQ(5, matches.size());
  EXPECT_EQ(Search::NON_SELECTED, search.selected());
}

//------------------------------------------------------------------------------

TEST(SearchTest, FilterSingleChar)
{
  Search::Choices choices = {
    L"foo", 
    L"bar", 
    L"hallo", 
    L"welt", 
    L"bla", 
  };
  Search search(choices);

  search.query(L"b");

  auto matches = search.matches();
  EXPECT_EQ(2, matches.size());

  EXPECT_TRUE( contains(matches, L"bar").first );
  EXPECT_TRUE( contains(matches, L"bla").first );
  EXPECT_FALSE(contains(matches, L"welt").first);
  EXPECT_FALSE(contains(matches, L"not at all").first);
}

//------------------------------------------------------------------------------

TEST(SearchTest, FilterScoreTest)
{
  Search::Choices choices = {
    L"fooa", 
    L"fooaa", 
    L"foo", 
    L"bbbfooaa", 
    L"bbbfoo", 
    L"bar", 
  };
  Search search(choices);

  search.query(L"foo");

  auto matches = search.matches();
  EXPECT_EQ(5, matches.size());

  // order by score, 
  // 
  // starting with exact Match, the first match!
  auto m = contains(matches, L"foo");
  EXPECT_EQ( 1, m.first );
  EXPECT_EQ( 2.1, m.second.score );

  print(matches);
}

//------------------------------------------------------------------------------

