#include "matcher.hh"

#include <algorithm>
#include <limits>
#include <sstream>

using namespace clecta;

//------------------------------------------------------------------------------

String 
Match::str() const 
{
  std::wstringstream ss;
  ss << L"\"" << value << L"\": [begin:" << begin 
    << L", end:" << end 
    << L", score:" << score 
    << L"]" << std::endl;
  return ss.str();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

Match
SimpleMatcher::get_score(const String& needle, const String& haystack) const
{
  Match m;
  // if query is empty, everything should be found!
  if ( needle.size() == 0 ) 
  {
    m.score = 0.1;
    return m;
  }

  double score = 0.0;
  auto found = haystack.find(needle);
  if ( found == std::string::npos )
    return m;

  ++score; // since it is found, increase it already by one!
  // get a score somewhere between 0 and 1 (double)
  auto relative_len = haystack.size() - needle.size();

  if ( relative_len == 0 )
  { // same length ... good!
    score += 1.1;
  }
  else 
  {
    score += 1 - (((relative_len)-found)/(double)(relative_len));
  }

  m.begin = found;
  m.end = found + needle.size();
  m.score = score;
  m.value = haystack;

  return m;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

Match 
CmdTMatcher::get_score(const String& needle, const String& haystack) const
{

  double memo_size = needle.size() * haystack.size();
  
  Match def_match(std::numeric_limits<double>::max());
  std::vector<Match> matches(memo_size, def_match);
  MemoInfo m_info{
    matches,
    1.0,
    needle,
    haystack
  };

  auto m = match(&m_info, 0, 0, 0, 0.0);

  return m;
}

//------------------------------------------------------------------------------

Match 
CmdTMatcher::match(
    CmdTMatcher::MemoInfo* memo_info,
    size_t haystack_idx, 
    size_t needle_idx, 
    size_t last_idx, 
    double score) const 
{
  double seen_score = 0.0;
  auto& needle = memo_info->needle;
  auto& haystack = memo_info->haystack;

  auto memo_idx = haystack_idx;
  auto& current_mem = memo_info->memo[needle_idx * needle.size() + memo_idx];
  auto memoized = current_mem.score;

  if ( memoized != std::numeric_limits<double>::max() )
    return memoized;

  if (memo_info->haystack.size() - haystack_idx < memo_info->needle.size() - needle_idx) 
  {
    score = 0.0;
    memo_info->memo[needle_idx * needle.size() + memo_idx] = score;
    return score;
  }

  for ( auto i = needle_idx; i < needle.size(); ++i )
  {
    auto c = needle[i];
    bool found = false;

    for ( auto j = haystack_idx; 
          j <= haystack.size() - (needle.size() - i);
          ++j, ++haystack_idx)
    {
      auto d = haystack[j];
      if ( c == d )
      {
        found = true;
        double score_for_char = memo_info->max_score_per_char;
        auto distance = j - last_idx;
        if ( distance > 1 )
        {
          double factor = 1.0;
          auto last = haystack[j-1];
          auto current = haystack[j];
          if ( last == L'/' )
            factor = 0.9;
          else if (last == L'-' ||
                   last == L'_' ||
                   last == L' ' ||
                   (last >= L'0' && last <= L'9'))
            factor = 0.8;
          else if (last >= L'a' && last <= L'z' &&
                   current >= 'A' && current <= 'Z')
            factor = 0.8;
          else if (last == L'.')
            factor = 0.7;
          else
          {
            // if no "special" chars behind char, factor diminishes
            // as distance from last matched char increases
            factor = (1.0 / distance) * 0.75;
          }
          score_for_char *= factor;
        }

        if (++j < haystack.size() )
        {
          // bump cursor one char to the right and
          // use recursion to try and find a better match
          Match sub_match = match(memo_info, j, i, last_idx, score);
          if (sub_match.score > seen_score)
            seen_score = sub_match.score;
        }

        score += score_for_char;
        last_idx = haystack_idx++;
        break;
      }
    }

    if (!found) 
    {
      score = 0.0;
      memo_info->memo[needle_idx * needle.size() + memo_idx] = score;
      return score;
    }
  }

  score = score > seen_score ? score : seen_score;
  memo_info->memo[needle_idx * needle.size() + memo_idx] = score;
  return score;
}

//------------------------------------------------------------------------------
