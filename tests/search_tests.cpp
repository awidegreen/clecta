// include global test defs
#include <gtest/gtest.h>

#include <fstream>

#include <matcher.hh>
#include <search.hh>
#include "test_utils.hh"

#define TEST_FILE_50K_WORDS "./tests/words50k.txt"

using namespace clecta;

//------------------------------------------------------------------------------

TEST(SearchTest, InitEmptyWithNoMatcher)
{
  Search search;

  EXPECT_EQ(false, search.case_sensitive());

  EXPECT_EQ(L"", search.selection());

  EXPECT_EQ(0, search.size_choices());

  EXPECT_EQ( std::string("NoMatcher") , search.matcher_name());

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
  search.register_matcher(Matcher::Ptr(new SimpleMatcher()));

  auto matches = search.matches();
  EXPECT_EQ(0, matches.size());
  EXPECT_EQ(Search::NON_SELECTED, search.selected());
  EXPECT_EQ(5, search.size_choices());

  search.query(L"");
  EXPECT_EQ(Search::NON_SELECTED, search.selected());

  matches = search.matches();
  EXPECT_EQ(5, matches.size());
  EXPECT_EQ(5, search.size_choices());
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
  search.register_matcher(Matcher::Ptr(new SimpleMatcher()));
  EXPECT_EQ(0, search.size_choices());

  for (auto& c : choices )
  {
    search.add_choice(c);
  }
  EXPECT_EQ(5, search.size_choices());

  auto matches = search.matches();
  EXPECT_EQ(0, matches.size());

  search.query(L"");
  EXPECT_EQ(Search::NON_SELECTED, search.selected());

  matches = search.matches();
  EXPECT_EQ(5, matches.size());
  EXPECT_EQ(Search::NON_SELECTED, search.selected());
  EXPECT_EQ(5, search.size_choices());
}


//------------------------------------------------------------------------------

TEST(SearchTest, CaseSensitive)
{
  Search::Choices choices = {
    L"show it",
    L"Transit",
    L"Hello World!",
    L"fOO bar",
    L"fOo bar",
  };

  Search search(choices);
  search.register_matcher(Matcher::Ptr(new SimpleMatcher()));
  EXPECT_FALSE(search.case_sensitive());

  search.query(L"o");
  auto matches = search.matches();
  EXPECT_EQ( 4, matches.size() );

  unsigned c = 0;
  for ( auto& i : choices )
  {
    switch (c)
    {
      case 0: case 2: case 3: case 4:
      {
        auto m = test::contains(matches, i);
        EXPECT_TRUE( m.first );
      }
      default: break;
    }

    ++c;
  }

  // now case-sense test
  search.toggle_case_sensitive();
  EXPECT_TRUE(search.case_sensitive());

  search.query(L"o");
  matches = search.matches();

  EXPECT_EQ( 3, matches.size() );

  c = 0;
  for ( auto& i : choices )
  {
    switch (c)
    {
      case 0: case 2: case 4:
      {
        auto m = test::contains(matches, i);
        EXPECT_TRUE( m.first );
      }
      default: break;
    }

    ++c;
  }


}

//------------------------------------------------------------------------------

TEST(SearchTest, Selection)
{
  Search::Choices choices = {
    L"foo", 
    L"bar", 
    L"hallo", 
    L"welt", 
    L"bla", 
  };

  String empty = L"";

  Search search(choices);
  search.register_matcher(Matcher::Ptr(new SimpleMatcher()));

  EXPECT_EQ(Search::NON_SELECTED, search.selected());
  EXPECT_EQ(empty, search.selection());

  search.query(empty);

  EXPECT_EQ(Search::NON_SELECTED, search.selected());
  EXPECT_EQ(empty, search.selection());

  search.selected(-2);
  EXPECT_EQ(empty, search.selection());

  search.selected(0);
  EXPECT_EQ(L"foo", search.selection());

  search.selected(3);
  EXPECT_EQ(L"welt", search.selection());

  search.selected(4);
  EXPECT_EQ(L"bla", search.selection());

  search.selected(0);
  EXPECT_EQ(L"foo", search.selection());

  search.selected(-1);
  EXPECT_EQ(empty, search.selection());
}

//------------------------------------------------------------------------------

TEST(SearchPerformanceTest, TestFile50kCaseInSensitive)
{
  Search::Choices choices;

  std::wifstream file;
  file.open(TEST_FILE_50K_WORDS);
  EXPECT_TRUE(file.is_open());

  std::wstring line;
  while ( std::getline(file, line) )
  {
    choices.push_back(line);
  }

  Search search(choices);
  search.register_matcher(Matcher::Ptr(new SimpleMatcher()));
  EXPECT_EQ(50000, search.size_choices());
  EXPECT_FALSE(search.case_sensitive());
  
  search.query(L"mac");
}

//------------------------------------------------------------------------------

TEST(SearchPerformanceTest, TestFile50kCaseSensitive)
{
  Search::Choices choices;

  std::wifstream file;
  file.open(TEST_FILE_50K_WORDS);
  EXPECT_TRUE(file.is_open());

  std::wstring line;
  while ( std::getline(file, line) )
  {
    choices.push_back(line);
  }

  Search search(choices);
  search.register_matcher(Matcher::Ptr(new SimpleMatcher()));
  EXPECT_EQ(50000, search.size_choices());
  EXPECT_FALSE(search.case_sensitive());

  search.toggle_case_sensitive();
  EXPECT_TRUE(search.case_sensitive());
  search.query(L"mac");
}

//------------------------------------------------------------------------------
