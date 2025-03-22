#pragma once

#if defined(__GNUC__) || defined(__clang__)
# define atrb_deprecated __attribute__((deprecated))
# define atrb_unused     __attribute__((unused))
# define atrb_pure       __attribute__((pure))
# define atrb_const      __attribute__((const))
#else
# define atrb_deprecated
# define atrb_unused
# define atrb_pure
# define atrb_const
#endif
