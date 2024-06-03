#ifndef __TOOLS_H__
#define __TOOLS_H__

#define ARRAY_SIZE(__ARRAY__) (sizeof(__ARRAY__) / sizeof(*__ARRAY__))

char* print_log(const char* fmt, ...);

void put_to_queue(void* queue, char* value);

#endif /* __TOOLS_H__ */
