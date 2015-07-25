#ifndef MATCHFILTER_HH
#define MATCHFILTER_HH

#include <thread>
#include <algorithm>    
#include <vector>    
#include <iostream>
#include <chrono>
#include "matcher.hh"

//#define TAKE_MEASUREMENT 1 

namespace clecta
{

template<typename ChoicesIterator>
class MatchFilter
{
public:
  MatchFilter(const Matcher::Ptr& matcher, bool case_sensitive = false) :
    _matcher(matcher),
    _case_sensitive(case_sensitive)
  { }


  Matches get_concurrent(
      String query_str, 
      ChoicesIterator choicesBegin, ChoicesIterator choicesEnd);

  Matches get(
      String query_str, 
      ChoicesIterator choicesBegin, ChoicesIterator choicesEnd);

private:
  Matcher::Ptr _matcher;
  bool _case_sensitive;

  struct calc_choices
  {
    void operator()(ChoicesIterator first, ChoicesIterator last,
        Matcher::Ptr matcher, const String& query, bool case_sensitive,
        Matches& matches_storage);
  };
};

//------------------------------------------------------------------------------

template<typename ChoicesIterator>
void 
MatchFilter<ChoicesIterator>::calc_choices::operator()(
    ChoicesIterator first,
    ChoicesIterator last,
    Matcher::Ptr matcher,
    const String& query,
    bool case_sensitive,
    Matches& matches_storage)
{
  ChoicesIterator current = first;
  while (current != last)
  {
    auto haystack = *current;
    if ( !case_sensitive )
      std::transform(haystack.begin(), 
          haystack.end(), haystack.begin(), ::tolower);

    auto m = matcher->get_score(query, haystack);
    if ( m.score > 0.0 )
    {
      m.value = *current;
      m.choice_iter = current;
      matches_storage.push_back(m);
    }
    ++current;
  }

}

//------------------------------------------------------------------------------

template<typename ChoicesIterator>
Matches 
MatchFilter<ChoicesIterator>::get_concurrent(
    String query_str,
    ChoicesIterator choicesBegin, ChoicesIterator choicesEnd)
{
  if ( !_case_sensitive )
    std::transform(query_str.begin(), query_str.end(), 
        query_str.begin(), ::tolower);
  
  unsigned long const len = std::distance(choicesBegin, choicesEnd);

  unsigned long const min_p_thread = 25;
  unsigned long const max_threads = (len+min_p_thread+1)/min_p_thread;

  unsigned long const hw_threads = std::thread::hardware_concurrency();
  auto const num_threads  = 
    std::min(hw_threads != 0 ? hw_threads : 2, max_threads);

  unsigned long const block_size = len / num_threads;

#ifdef TAKE_MEASUREMENT 
  std::wcerr << "concurrent" << std::endl
    << "len choices: " << len << std::endl
    << "query_str: "  << query_str << std::endl 
    << "case_sensitive: "  << (_case_sensitive ? "true" : "false") << std::endl 
    << "hw_threads: "  << hw_threads << std::endl 
    << "num_threads: " << num_threads << std::endl
    << "block_size: " << block_size << std::endl;
  auto _start_ = std::chrono::system_clock::now();
#endif

  std::vector<Matches> matches_from_thread(num_threads);
  std::vector<std::thread> threads(num_threads-1);

  ChoicesIterator block_start = choicesBegin;
  for (unsigned long i = 0; i < (num_threads-1); ++i)
  {
    ChoicesIterator block_end = block_start;
    std::advance(block_end, block_size);

    threads[i] = std::thread(
        MatchFilter::calc_choices(),
        block_start, 
        block_end, 
        _matcher, std::ref(query_str), 
        _case_sensitive, 
        std::ref(matches_from_thread[i]));
    block_start = block_end;
  }

  MatchFilter::calc_choices()(
    block_start, 
    choicesEnd, 
    _matcher, std::ref(query_str), 
    _case_sensitive, 
    matches_from_thread[num_threads-1]);

  std::for_each(threads.begin(), threads.end(), 
      std::mem_fn(&std::thread::join));

  Matches result;

  for (auto& matches_th : matches_from_thread )
  {
    std::copy(matches_th.begin(), matches_th.end(), std::back_inserter(result));
  }

#ifdef TAKE_MEASUREMENT 
  auto _end_filter_ = std::chrono::system_clock::now();
#endif

  std::sort(result.begin(), result.end(), 
      [](const Match& a, const Match& b) 
      {
        return b.score < a.score;
      });

#ifdef TAKE_MEASUREMENT 
  auto _end_ = std::chrono::system_clock::now();
  auto _elapsed_filter = 
    std::chrono::duration_cast<std::chrono::milliseconds>(_end_filter_ - _start_);
  auto _elapsed_sort = 
    std::chrono::duration_cast<std::chrono::milliseconds>(_end_ - _end_filter_);
  auto _elapsed_total = 
    std::chrono::duration_cast<std::chrono::milliseconds>(_end_ - _start_);
  
  std::wcerr << "filter duration: " << _elapsed_filter.count() << std::endl
    << "sort duration: " << _elapsed_sort.count() << std::endl
    << "total duration: " << _elapsed_total.count() << std::endl
    << "----------------------------------------" << std::endl;
#endif

  return result;
}

//------------------------------------------------------------------------------

template<typename ChoicesIterator>
Matches 
MatchFilter<ChoicesIterator>::get(
    String query_str,
    ChoicesIterator choicesBegin, ChoicesIterator choicesEnd)
{
  if ( !_case_sensitive )
    std::transform(query_str.begin(), query_str.end(), 
        query_str.begin(), ::tolower);

#ifdef TAKE_MEASUREMENT 
  std::wcerr 
    << "single_thread" << std::endl
    << "query_str: "  << query_str << std::endl 
    << "case_sensitive: "  << (_case_sensitive ? "true" : "false") << std::endl;

  auto _start_ = std::chrono::system_clock::now();
#endif

  auto current = choicesBegin;

  Matches result;

  MatchFilter::calc_choices()(
    choicesBegin, 
    choicesEnd, 
    _matcher, std::ref(query_str), 
    _case_sensitive, 
    result);

#ifdef TAKE_MEASUREMENT 
  auto _end_filter_ = std::chrono::system_clock::now();
#endif

  std::sort(result.begin(), result.end(), 
      [](const Match& a, const Match& b) 
      {
        return b.score < a.score;
      });

#ifdef TAKE_MEASUREMENT 
  auto _end_ = std::chrono::system_clock::now();
  auto _elapsed_filter = 
    std::chrono::duration_cast<std::chrono::milliseconds>(_end_filter_ - _start_);
  auto _elapsed_sort = 
    std::chrono::duration_cast<std::chrono::milliseconds>(_end_ - _end_filter_);
  auto _elapsed_total = 
    std::chrono::duration_cast<std::chrono::milliseconds>(_end_ - _start_);
  
  std::wcerr << "filter duration: " << _elapsed_filter.count() << std::endl
    << "sort duration: " << _elapsed_sort.count() << std::endl
    << "total duration: " << _elapsed_total.count() << std::endl
    << "----------------------------------------" << std::endl;
#endif

  return result;
}

} // end of ns

#ifdef TAKE_MEASUREMENT 
#undef TAKE_MEASUREMENT
#endif

#endif /* MATCHFILTER_HH */
