#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
SA_EXPORT void sa_signed_u8_clamp_8c0548e0(struct sa_cpu_state*s){
 int32_t input=(int32_t)s->r[4];
 uint32_t greater_than_255=(uint32_t)(input>255);
 int32_t value=input;
 if(value<0)value=0;
 if(value>255)value=255;
 s->r[0]=(uint32_t)value;s->r[4]=(uint32_t)value;s->r[5]=255u;
 s->sr=(s->sr&~1u)|greater_than_255;
 s->pc=s->pr;
}
