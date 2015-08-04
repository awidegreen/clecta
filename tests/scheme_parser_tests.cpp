// include global test defs
#include <gtest/gtest.h>

// stl
#include <sstream>

// clecta
#include <color/parser.hh>


using namespace clecta;

//------------------------------------------------------------------------------

TEST(SchemeParserTest, ValidSingleColor)
{
  std::stringstream file;
  file << "name = foobar\n" 
       << WINDOW_BG << " = #aabbcc\n";

  auto scheme = parse_by_stream(file);

  EXPECT_TRUE(scheme.valid());
  EXPECT_EQ("foobar", scheme.name);
  EXPECT_TRUE(scheme.colors[WINDOW_BG].is_set());
  Color c{0xaa, 0xbb, 0xcc};
  EXPECT_EQ(c, scheme.colors[WINDOW_BG]);
}

//------------------------------------------------------------------------------

TEST(SchemeParserTest, ValidIgnoreComments)
{
  std::stringstream file;
  file 
    << "name = foobar ; a comment\n" 
    << ";name = foobar2\n" 
    << ";another comment\n" 
    << WINDOW_BG << " =aabbcc;halllwelt\n"
    << ";another comment\n" 
    << ";name = foobar3\n";

  auto scheme = parse_by_stream(file);

  EXPECT_TRUE(scheme.valid());
  EXPECT_EQ("foobar", scheme.name);
  EXPECT_TRUE(scheme.colors[WINDOW_BG].is_set());
}

//------------------------------------------------------------------------------

TEST(SchemeParserTest, ValidOverwriteName)
{
  std::stringstream file;
  file 
    << "name = foobar ; a comment\n" 
    << "name = foobar2\n";

  auto scheme = parse_by_stream(file);

  EXPECT_TRUE(scheme.valid());
  EXPECT_EQ("foobar2", scheme.name);
}

//------------------------------------------------------------------------------

TEST(SchemeParserTest, InvalidNoName)
{
  std::stringstream file;
  file 
    << WINDOW_BG << " = #aabbcc\n";

  auto scheme = parse_by_stream(file);

  EXPECT_FALSE(scheme.valid());
}

//------------------------------------------------------------------------------

TEST(SchemeParserTest, InvalidFormatColonInsteadOfEqual)
{
  std::stringstream file;
  file 
    << WINDOW_BG << " = aabbcc\n"
    << "name : foobar\n";

  auto scheme = parse_by_stream(file);

  EXPECT_FALSE(scheme.valid());
}

//------------------------------------------------------------------------------

TEST(SchemeParserTest, InvalidFormatNoValidKeyword)
{
  std::stringstream file;
  file 
    << "name = foobar\n"
    << " bla = aabbcc\n";

  auto scheme = parse_by_stream(file);

  EXPECT_FALSE(scheme.valid());
}

//------------------------------------------------------------------------------
