#include <locale.h>

// stl
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "app.hh"
#include "matcher.hh"

int main()
{
  setlocale(LC_ALL, ""); // add utf8 support!

  using namespace clecta;
  clecta::Choices choices;

  std::wstring line;
  while ( std::getline(std::wcin, line) )
  {
    choices.push_back(line);
  }

  auto search = std::make_shared<clecta::Search>(choices);
  search->register_matcher(Matcher::Ptr(new SimpleMatcher()));
  search->register_matcher(Matcher::Ptr(new CmdTMatcher()));

  clecta::App app(search);
  app.run();

  auto r = search->selection();
  if ( r != L"") std::wcout << r << std::endl;

  return 0;
}
