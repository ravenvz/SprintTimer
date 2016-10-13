#ifndef OPTIONWRAPPER_H_RQGL6JUL
#define OPTIONWRAPPER_H_RQGL6JUL

/* Wrapper to include optional type until C++17 arrives */

#ifdef _MSC_VER
#include <boost/optional/optional.hpp>
using boost::optional;
using boost::make_optional;
#else
#include <experimental/optional>
using std::experimental::optional;
using std::experimental::make_optional;
#endif

#endif /* end of include guard: OPTIONWRAPPER_H_RQGL6JUL */
