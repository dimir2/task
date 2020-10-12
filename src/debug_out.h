#ifndef DEBUG_OUT_H
#define DEBUG_OUT_H


#define DEBUG

#define TRUE  (1)
#define FALSE (0)

#ifdef DEBUG
#define DEBUG_OUT TRUE
#else
#define DEBUG_OUT FALSE
#endif

#define debug_print(fmt, ...) \
        do { if (DEBUG_OUT) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

#endif // #ifndef DEBUG_OUT_H