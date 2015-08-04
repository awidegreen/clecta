// include global test defs
#include <gtest/gtest.h>

// clecta
#include <color/parser.hh>

using namespace clecta;

//------------------------------------------------------------------------------

TEST(ColorTest, Valid)
{
  std::string color_str = "1213de";

  Color exp{0x12, 0x13, 0xde};
  auto c = get_color_from_str(color_str);

  EXPECT_TRUE(c.is_set());
  EXPECT_EQ(exp, c);
}

//------------------------------------------------------------------------------

TEST(ColorTest, ValidWithHash)
{
  std::string color_str = "#fe1200";

  Color exp{0xfe, 0x12, 0x00};
  auto c = get_color_from_str(color_str);

  EXPECT_TRUE(c.is_set());
  EXPECT_EQ(exp, c);
}

//------------------------------------------------------------------------------

TEST(ColorTest, InvalidHexOutOfRange)
{
  std::string color_str = "#uuaabb";

  auto c = get_color_from_str(color_str);

  EXPECT_FALSE(c.is_set());
}

//------------------------------------------------------------------------------

TEST(ColorTest, InvalidToLong)
{
  std::string color_str = "1234567";

  auto c = get_color_from_str(color_str);

  EXPECT_FALSE(c.is_set());
}

//------------------------------------------------------------------------------

TEST(ColorTest, InvalidToShort)
{
  std::string color_str = "#1";
  auto c = get_color_from_str(color_str);
  EXPECT_FALSE(c.is_set());

  color_str = "12345";
  c = get_color_from_str(color_str);
  EXPECT_FALSE(c.is_set());
}

//------------------------------------------------------------------------------

TEST(ColorTest, InvalidEmptyString)
{
  std::string color_str = "";

  auto c = get_color_from_str(color_str);
  EXPECT_FALSE(c.is_set());

  color_str = "#";
  c = get_color_from_str(color_str);
  EXPECT_FALSE(c.is_set());
}

//------------------------------------------------------------------------------
