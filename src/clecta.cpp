#include <locale.h>

// stl
#include <list>
#include <string>
#include <iostream>
#include <fstream>

#include "app.hh"
#include "matcher.hh"

int main()
{
  setlocale(LC_ALL, ""); // add utf8 support!

  clecta::Search::Choices choices;

  std::wstring line;
  while ( std::getline(std::wcin, line) )
  {
    choices.push_back(line);
  }

  auto search = new clecta::Search(choices);

  clecta::App app(search);
  app.run();

  auto r = search->selection();
  if ( r != L"") std::wcout << r << std::endl;

  return 0;
}
