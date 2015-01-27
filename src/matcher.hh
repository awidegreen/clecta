#ifndef MATCHER_HH
#define MATCHER_HH

#include <string>
#include <array>

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
  // called recursively to get score
  double 
  match(size_t haystack_idx, size_t needle_idx, 
        size_t last_idx, double score) const;

  mutable String _needle;
  mutable String _haystack;
  mutable double* _memo;
  mutable double _max_score_per_char;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

} // namespace
 

#endif /* MATCHER_HH */
