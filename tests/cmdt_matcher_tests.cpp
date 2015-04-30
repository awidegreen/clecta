// include global test defs
#include <gtest/gtest.h>

#include <matcher.hh>

#include "test_utils.hh"

using namespace clecta;

//------------------------------------------------------------------------------

namespace 
{
  auto get_search = clecta::test::get_search<CmdTMatcher>;
}

//------------------------------------------------------------------------------

TEST(CmdTMatcherTest, GetScore)
{
  CmdTMatcher matcher;
  
  String haystack = L"barfbar";
  auto m = matcher.get_score(L"bar", haystack);

  EXPECT_EQ( 0, m.begin );
  EXPECT_EQ( 2, m.end );
  EXPECT_EQ( 3, m.score );
  EXPECT_EQ( haystack, m.value );
}

//------------------------------------------------------------------------------

TEST(CmdTMatcherTest, EmptyMatchListWhenNoChoices)  
{
  Search::Choices choices = { };
  auto search = get_search(choices, L"");
  EXPECT_TRUE( search.matches().empty() );
  search.query(L"foo");
  EXPECT_TRUE( search.matches().empty() );
}

//------------------------------------------------------------------------------

TEST(CmdTMatcherTest, EmptyMatchListWhenNoMatches)
{
  Search::Choices choices = {
    L"foo/bar",
    L"foo/baz",
    L"bing",
  };
  auto search = get_search(choices, L"xyz");
  EXPECT_TRUE( search.matches().empty() );
}

//------------------------------------------------------------------------------

TEST(CmdTMatcherTest, ReturnMatchingPaths)
{
  Search::Choices choices = {
    L"foo/bar",
    L"foo/baz",
    L"bing",
  };

  auto search = get_search(choices, L"z");
  auto m = test::match_at(search,0);
  EXPECT_EQ( L"foo/baz", m.value );
  EXPECT_EQ( 6, m.begin );
  EXPECT_EQ( 6, m.end );

  search.query(L"bg");
  m = test::match_at(search,0);
  EXPECT_EQ( 0, m.begin );
  EXPECT_EQ( 3, m.end );
}

//------------------------------------------------------------------------------

TEST(CmdTMatcherTest, EmptyQueryMatchAll)
{
  Search::Choices choices = {
    L"foo",
  };

  auto search = get_search(choices, L"");
  auto m = test::match_at(search, 0);
  EXPECT_EQ( L"foo", m.value );
  EXPECT_EQ( 0, m.begin );
  EXPECT_EQ( 0, m.end );
}

//------------------------------------------------------------------------------

TEST(CmdTMatcherTest, DontMatchSubstrOfQuery)
{
  Search::Choices choices = {
    L"foo",
  };

  auto search = get_search(choices, L"foo...");
  EXPECT_TRUE( search.matches().empty() );
}

//------------------------------------------------------------------------------

//TEST(CmdTMatcherTest, PrioritizeShorterOverLongerMatches)
//{
  //Search::Choices choices = {
    //L"src/foobar.cpp",
    //L"src/foo.cpp",
  //};

  //auto search = get_search(choices, L"src");
  //EXPECT_EQ( L"src/foo.cpp", match_val_at(search, 0) );
  //EXPECT_EQ( L"src/foobar.cpp", match_val_at(search, 1) );
//}
