// include global test defs
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>

#include <search.hh>

#define TEST_FILE_50K_WORDS "./tests/words50k.txt"

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

  void 
  print(const Search::Matches& matches)
  {
    unsigned cnt = 1;
    for ( const auto& m : matches )
    {
      std::wcerr << cnt << L": " << m.str();
      ++cnt;
    }
  }
}

//------------------------------------------------------------------------------

TEST(SearchTest, InitEmpty)
{
  Search search;

  EXPECT_EQ(false, search.case_sensitive());

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

TEST(SearchTest, FilterScore)
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

  m = contains(matches, L"bbbfoo");
  EXPECT_EQ( 2, m.first );
  EXPECT_EQ( 2.0, m.second.score );

  m = contains(matches, L"bbbfooaa");
  EXPECT_EQ( 3, m.first );
  EXPECT_EQ( 1.6, m.second.score );

  m = contains(matches, L"fooa");
  EXPECT_EQ( 4, m.first );
  EXPECT_EQ( 1, m.second.score );            

  m = contains(matches, L"fooaa");
  EXPECT_EQ( 5, m.first );
  EXPECT_EQ( 1, m.second.score );

  m = contains(matches, L"bar");
  EXPECT_FALSE( m.first );

  m = contains(matches, L"something else");
  EXPECT_FALSE( m.first );

  //print(matches);
}

//------------------------------------------------------------------------------


TEST(SearchTest, FilterWithSpace)
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

  Search search(choices);

  search.query(L"is");

  auto matches = search.matches();;

  auto m = contains(matches, paul);
  EXPECT_EQ( 1, m.first );
  EXPECT_TRUE( m.second.score > 1.3 && m.second.score < 1.4);

  m = contains(matches, tisit);
  EXPECT_EQ( 2, m.first );
}
  
//------------------------------------------------------------------------------

TEST(SearchTest, ScoreCalc)
{
  std::wstring candidate = L"Paulis  no"; // 10 char
  std::wstring query = L"is"; // "is" ends at 7 so it sould have match + .7

  Search search;

  auto m = search.get_score(query, candidate);

  EXPECT_EQ( 1.5, m.score );

  candidate = L"Paul is not Simon an";
  m = search.get_score(query, candidate);
  EXPECT_TRUE( m.score > 1.25 && m.score < 1.30 );
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
        auto m = contains(matches, i);
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
        auto m = contains(matches, i);
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

  Search::String empty = L"";

  Search search(choices);

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
  EXPECT_EQ(50000, search.size_choices());
  EXPECT_FALSE(search.case_sensitive());

  search.toggle_case_sensitive();
  EXPECT_TRUE(search.case_sensitive());
  search.query(L"mac");
}

//------------------------------------------------------------------------------
