#ifndef UTILS_H_IZ5NTG6C
#define UTILS_H_IZ5NTG6C

#include <algorithm>

template <class InputIt1, class InputIt2, class OutputIt1, class OutputIt2>
void twoWayDiff(InputIt1 first1,
                InputIt1 last1,
                InputIt2 first2,
                InputIt2 last2,
                OutputIt1 out1,
                OutputIt2 out2)
{
    std::set_difference(first1, last1, first2, last2, out1);
    std::set_difference(first2, last2, first1, last1, out2);
}


#endif /* end of include guard: UTILS_H_IZ5NTG6C */
