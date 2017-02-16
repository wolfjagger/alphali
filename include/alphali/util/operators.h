#pragma once



#define DEFINE_EXTRA_OPS(classname) \
 inline bool operator!=(const classname& first, const classname& second) { \
  return !(first == second); \
 } \
 inline bool operator<=(const classname& first, const classname& second) { \
  return (first < second) || (first == second); \
 } \
 inline bool operator>(const classname& first, const classname& second) { \
  return !(first <= second); \
 } \
 inline bool operator>=(const classname& first, const classname& second) { \
  return !(first < second); \
 }
