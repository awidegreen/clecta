#include <locale.h>

// c lib
#include <getopt.h>

// stl
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "app.hh"
#include "matcher.hh"
#include "config.hh"

//------------------------------------------------------------------------------

#define no_argument 0
#define requires_argument 1 
#define optional_argument 2

//------------------------------------------------------------------------------

const struct option longopts[] = 
{
  {"help",              no_argument,          0, 'h'},
  {"case-sensitive",    no_argument,          0, 'c'},
  {"version",           no_argument,          0, 'v'},
  {0,                   0,                    0,  0 },
};


//------------------------------------------------------------------------------

void version() 
{
  std::wcout 
    << CLECTA_PROJECT_NAME << L" " << CLECTA_VERSION << L" - " 
    << "Copyright 2015 Armin Widegreen <armin.widegreen@gmail.com>\n";
}

void help() 
{
  std::wcout 
    << L"Usage: " << CLECTA_PROJECT_NAME << L" [OPTION] [FILE]\n"\
       L"Let you select a value from a list of lines provided via STDIN or "\
       L"FILE.\n\n"\
       L"Called with no FILE, list of strings will be taken from STDIN\n\n"\
       L"Options:\n"
       L"  -c, --case-sensitive     search will be case-sensitive (F2)\n"\
       L"  -h, --help               show this help\n"\
       L"  -v, --version            show vserion\n"\
       L"\n"\
       L"Examples:\n"\
       L"  find . | clecta             selected line from find result, print to STDOUT\n"\
       L"  vim $(find . | clecta -c)   as before (start with -c) and open result in vim\n"\
    ;
  version();
}


//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
  setlocale(LC_ALL, ""); // add utf8 support!

  bool case_sensitive = false;
  int index = 0, iarg=0;
  while (iarg != -1 )
  {
    iarg = getopt_long(argc, argv, "chv", longopts, &index);

    switch (iarg)
    {
      case 'h':
        help();
        return 0;
      case 'v':
        help();
        return 0;
      case 'c':
        case_sensitive = true;
        break;
      case -1: break; // ignore
      default:
        std::wcerr << iarg <<  "Option unknown, see help!\n" << std::endl;
        return 0;
    }
  }

  using namespace clecta;
  clecta::Choices choices;

  std::wstring line;
  while ( std::getline(std::wcin, line) )
  {
    choices.push_back(line);
  }

  auto search = std::make_shared<clecta::Search>(choices, case_sensitive);
  search->register_matcher(Matcher::Ptr(new CmdTMatcher()));
  search->register_matcher(Matcher::Ptr(new SimpleMatcher()));

  clecta::App app(search);
  app.run();

  auto r = search->selection();
  if ( r != L"") std::wcout << r << std::endl;

  return 0;
}
