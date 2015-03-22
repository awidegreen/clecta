#ifndef MATCHER_HH
#define MATCHER_HH

#include <string>
#include <vector>

namespace clecta
{

typedef std::wstring String;

/**
 * Score value is dynamic where 0 is the worst!
 */
struct Match {
  double score;
  size_t begin;
  size_t end;
  String value;
  String str() const;
  Match() : score(0.0), begin(0), end(0), value(L"") { }
  Match(double default_score) : score(default_score), begin(0), end(0), value(L"") { }
};

//------------------------------------------------------------------------------

/**
 * Matcher base class
 */
class Matcher
{
public:
  virtual ~Matcher() { }

  virtual Match 
  get_score(const String& needle, const String& haystack) const = 0;
private:
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class SimpleMatcher : public Matcher
{
public: 
  // virtual
  Match 
  get_score(const String& needle, const String& haystack) const;
private:

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class CmdTMatcher : public Matcher
{
public:
  CmdTMatcher() {}
  // virtual
  Match 
  get_score(const String& needle, const String& haystack) const;
private:
  struct MemoInfo
  { 
    std::vector<Match> memo;
    double max_score_per_char;
    const String& needle;
    const String& haystack;
  };

  // called recursively to get score
  Match 
  match(MemoInfo* memo_info,
        size_t haystack_idx, size_t needle_idx, 
        size_t last_idx, double score) const;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

} // namespace
 

#endif /* MATCHER_HH */
