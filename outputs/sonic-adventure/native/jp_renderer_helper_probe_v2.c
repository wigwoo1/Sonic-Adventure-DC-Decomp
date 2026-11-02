/* Authored test boundary around the real generated renderer helper bodies.
 * This probe includes selected exact helper spans. No graphics,
 * texture or presentation services are replaced by dummy definitions. */
#ifndef JP_NATIVE_RENDERER_SOURCE
#error "Provide the private generated renderer source path"
#endif
#include JP_NATIVE_RENDERER_SOURCE
#define EXPORT __declspec(dllexport)
static unsigned prefetch_count;
static void *prefetch_last;
/* A real native CPU cache hint, with instrumentation for this helper probe. */
void _builtin_prefetch(void *p)
{
    prefetch_count++; prefetch_last=p; __builtin_prefetch(p);
}
EXPORT unsigned jp_rh_next_vector(const NJS_VECTOR *in, float w, float *out)
{
    const NJS_VECTOR *cursor=in;
    shz_vec4_t v=next_vector(&cursor,w);
    out[0]=v.x;out[1]=v.y;out[2]=v.z;out[3]=v.w;
    return (unsigned)((const unsigned char *)cursor-(const unsigned char *)in);
}
EXPORT int jp_rh_project(const float *in,float cx,float cy,void *out)
{
    struct projecting p={.z=0,.rhw=in[2],.x=in[0],.y=in[1]};
    struct projection by={.cx=cx,.cy=cy,.scale_x=0,.scale_y=0,.near=0};
    return project_end(out,p,&by)==out;
}
EXPORT void jp_rh_take_point(const void *points,const Uint32 *offsets,int index,void *out)
{
    struct strips s={0};s.points=points;s.offsets=offsets;
    struct vertex_words p=take_point(&s,index);
    __builtin_memcpy(out,&p,sizeof p);
}
EXPORT unsigned jp_rh_send(void *out,Uint32 control,const void *words,float u,float v)
{
    struct vertex_words p;__builtin_memcpy(&p,words,sizeof p);
    struct vertex *end=send_textured(out,control,p,u,v);
    return (unsigned)((unsigned char *)end-(unsigned char *)out);
}
EXPORT int jp_rh_next_index(const Sint16 *in,unsigned *stride)
{
    const Sint16 *cursor=in;int value=next_index(&cursor);
    *stride=(unsigned)((const unsigned char *)cursor-(const unsigned char *)in);
    return value;
}
EXPORT unsigned jp_rh_next_uv(const NJS_TEX *in,float *out)
{
    const NJS_TEX *cursor=in;next_uv(&cursor,&out[0],&out[1]);
    return (unsigned)((const unsigned char *)cursor-(const unsigned char *)in);
}
EXPORT unsigned jp_rh_prefetch_count(void) { return prefetch_count; }
EXPORT void *jp_rh_prefetch_last(void) { return prefetch_last; }
