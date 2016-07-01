#ifndef INCLUDED_ARRAY_SIZE_H
#define INCLUDED_ARRAY_SIZE_H 1

/* https://ccodearchive.net/info/build_assert.html */
#define BUILD_ASSERT_OR_ZERO(cond) (sizeof(char [1 - 2*!(cond)]) - 1)

/* https://ccodearchive.net/info/array_size.html
 * https://zubplot.blogspot.fr/2015/01/gcc-is-wonderful-better-arraysize-macro.html
 */
#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
#define _array_size_chk(arr)                                            \
        BUILD_ASSERT_OR_ZERO(!__builtin_types_compatible_p(__typeof__(arr), \
                                                        __typeof__(&(arr)[0])))
#else
#define _array_size_chk(arr) BUILD_ASSERT_OR_ZERO((void*)&(arr) == &(arr)[0])
#endif
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + _array_size_chk(arr))

#endif /* INCLUDED_ARRAY_SIZE_H */
