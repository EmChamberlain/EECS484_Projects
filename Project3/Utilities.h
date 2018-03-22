#ifndef EECS484P3_UTILITIES_H
#define EECS484P3_UTILITIES_H

#include <cstdlib>                                  // for size_t


using Key = int;
using Record = int;

const constexpr size_t kLeafOrder = 1;              // order of leaf nodes, must be at least 1
const constexpr size_t kInnerOrder = 1;             // order of inner nodes, must be at least 1
extern const char* kPrintPrefix;
extern const int kIndentIncr;

#endif
