#ifndef __TOOLS_H__
#define __TOOLS_H__

#define ARRAY_SIZE(__ARRAY__) (sizeof(__ARRAY__) / sizeof(*__ARRAY__))

void logout(const char* fmt, ...);

#endif /* __TOOLS_H__ */
