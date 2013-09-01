#ifndef UTILITY_H_
#define UTILITY_H_

template <class T, unsigned int N>
char (&size_helper(T (&arr)[N]))[N];

#define ARR_SIZE(arr) (sizeof(size_helper(arr)))

#endif // UTILITY_H_
