// include global test defs
#include <gtest/gtest.h>

#include "test_utils.hh"

using namespace clecta;

//------------------------------------------------------------------------------

namespace 
{
  auto get_search = clecta::test::get_search<SimpleMatcher>;
}

//------------------------------------------------------------------------------

TEST(SimpleMatcherTest, FilterSingleChar)
{
  Search::Choices choices = {
    L"foo", 
    L"bar", 
    L"hallo", 
    L"welt", 
    L"bla", 
  };

  auto search = get_search(choices, L"b");
  auto matches = search.matches();
  EXPECT_EQ(2, matches.size());

  EXPECT_TRUE( test::contains(matches, L"bar").first );
  EXPECT_TRUE( test::contains(matches, L"bla").first );
  EXPECT_FALSE(test::contains(matches, L"welt").first);
  EXPECT_FALSE(test::contains(matches, L"not at all").first);
}

//------------------------------------------------------------------------------

TEST(SimpleMatcherTest, FilterScore)
{
  Search::Choices choices = {
    L"fooa", 
    L"fooaa", 
    L"foo", 
    L"bbbfooaa", 
    L"bbbfoo", 
    L"bar", 
  };
  auto search = get_search(choices, L"foo");

  auto matches = search.matches();
  EXPECT_EQ(5, matches.size());

  // order by score, 
  // 
  // starting with exact Match, the first match!
  auto m = test::contains(matches, L"foo");
  EXPECT_EQ( 1, m.first );
  EXPECT_EQ( 2.1, m.second.score );

  m = test::contains(matches, L"bbbfoo");
  EXPECT_EQ( 2, m.first );
  EXPECT_EQ( 2.0, m.second.score );

  m = test::contains(matches, L"bbbfooaa");
  EXPECT_EQ( 3, m.first );
  EXPECT_EQ( 1.6, m.second.score );

  m = test::contains(matches, L"fooa");
  EXPECT_EQ( 4, m.first );
  EXPECT_EQ( 1, m.second.score );            

  m = test::contains(matches, L"fooaa");
  EXPECT_EQ( 5, m.first );
  EXPECT_EQ( 1, m.second.score );

  m = test::contains(matches, L"bar");
  EXPECT_FALSE( m.first );

  m = test::contains(matches, L"something else");
  EXPECT_FALSE( m.first );

  //print(matches);
}

//------------------------------------------------------------------------------

TEST(SimpleMatcherTest, FilterWithSpace)
{
  auto paul = L"Paul is not Simon";
  auto tisit = L"This is it";

  Search::Choices choices = {
    tisit,
    L"Show it",
    L"Transit",
    L"Hello World!",
    paul,
    L"foo bar",
  };

  auto search = get_search(choices, L"is");
  auto matches = search.matches();;

  auto m = test::contains(matches, paul);
  EXPECT_EQ( 1, m.first );
  EXPECT_TRUE( m.second.score > 1.3 && m.second.score < 1.4);

  m = test::contains(matches, tisit);
  EXPECT_EQ( 2, m.first );
}
  
//------------------------------------------------------------------------------

//TEST(SimpleMatcherTest, ScoreCalc)
//{
  //std::wstring candidate = L"Paulis  no"; // 10 char
  //std::wstring query = L"is"; // "is" ends at 7 so it sould have match + .7

  //Search search;

  //auto m = search.get_score(query, candidate);

  //EXPECT_EQ( 1.5, m.score );

  //candidate = L"Paul is not Simon an";
  //m = search.get_score(query, candidate);
  //EXPECT_TRUE( m.score > 1.25 && m.score < 1.30 );
//}
