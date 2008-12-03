#ifndef _MACROS_H_
#define _MACROS_H_

// Evaluates to the squared value
#ifndef SQR
#define SQR(time) ((time)*(time))
#endif

#define STRINGIFY(a) (#a)

#ifndef M_PI
#define M_PI ((double)3.14159265)
#endif

#ifndef M_E
#define M_E ((double)2.71828183)
#endif

#ifndef NDEBUG
#define FLUSH_GL_ERROR() for(;;){while(glGetError()!=GL_NO_ERROR);break;}
#define CHECK_GL_ERROR() \
for(;;) {                                                         \
    GLenum errorCode;                                             \
    while( (errorCode=glGetError()) != GL_NO_ERROR)               \
    {                                                             \
        FAIL("Line #" + itos(__LINE__) +                          \
             ": " + (const char*)gluErrorString(errorCode));      \
    }                                                             \
break;}
#else
#define FLUSH_GL_ERROR() ((void) 0)
#define CHECK_GL_ERROR() ((void) 0)
#endif

#endif
