// include global test defs
#include <gtest/gtest.h>

#include <matcher.hh>

using namespace clecta;

TEST(CmdTMatcherTest, Simple)
{
  CmdTMatcher matcher;
  
  auto m = matcher.get_score(L"bar", L"bar");
  std::wcerr << m.str() << std::endl;
}

