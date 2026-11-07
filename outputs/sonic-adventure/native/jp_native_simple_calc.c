/* Native value equivalents of the two original inline matrix/light helpers. */
#include "jp_native_simple_calc.h"
#include "jp_native_sprite_numeric.h"
#include "jp_native_action_numeric.h"
/* Value-only shared ABI; avoid redeclaring SDK matrix functions with host types. */
float *jp_ninja_current_matrix_storage(void);
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
#include <malloc.h>
#include <string.h>
void SimpleCalcModelMatrix(NJS_POINT3 *out,NJS_POINT3 *in)
{
    float values[3];jp_sprite_transform_point((const float *)in,values);
    memcpy(out,values,12);
}
void SimpleCalcModelLight(NJS_VECTOR *out,NJS_VECTOR *in,Float scale)
{
    uint32_t input[4]={0,0,0,0},matrix[16],v[4],zero[4],word;
    memcpy(input,in,12);memcpy(matrix,jp_ninja_current_matrix_storage(),64);
    for(unsigned i=0;i<3;i++)v[i]=sh4_fipr(input,matrix+i*4,1);
    memcpy(&v[3],&scale,4);memcpy(zero,v,12);zero[3]=0;
    word=sh4_fipr(v,zero,1);float length;memcpy(&length,&word,4);
    float factor=jp_sprite_multiply(jp_action_fsrra(length),scale),result[3];
    for(unsigned i=0;i<3;i++){float x;memcpy(&x,v+i,4);result[i]=jp_sprite_multiply(factor,x);}
    memcpy(out,result,12);
}
static unsigned char *allocation,*records;
static uint32_t capacity;
static struct jp_simple_projection_queues queues;
static size_t point_extent,normal_extent;
static int active,loop_error;
Sint32 _njSimpleCalcModel(NJS_MODEL *);
int jp_simple_calc_guards(void)
{
    if(!allocation)return 4;
    for(unsigned i=0;i<32;i++)
        if(allocation[i]!=0xa5 || records[(size_t)capacity*32+i]!=0x5a)return 5;
    return 0;
}
int jp_simple_calc_shutdown(void)
{
    if(active)return 4;
    int status=allocation?jp_simple_calc_guards():0;
    _aligned_free(allocation);allocation=records=NULL;capacity=0;
    memset(&queues,0,sizeof queues);return status;
}
int jp_simple_calc_initialize(uint32_t count,size_t budget)
{
    if(active || !count || count>65536)return 1;
    size_t cost=(size_t)count*32+64;
    if(cost>budget)return 2;
    unsigned char *next=_aligned_malloc(cost,32);
    if(!next)return 3;
    memset(next,0xa5,32);memset(next+32,0,(size_t)count*32);
    memset(next+32+(size_t)count*32,0x5a,32);
    int error=jp_simple_calc_shutdown();
    if(error){_aligned_free(next);return error;}
    allocation=next;records=next+32;capacity=count;return 0;
}
void *jp_simple_calc_records(void){return records;}
struct jp_simple_projection_queues *jp_simple_calc_queues(void){return &queues;}
void jp_simple_calc_loop(struct jp_simple_calc_work *work,NJS_MATRIX *unused,unsigned kind)
{
    (void)unused;
    if(!active || work->out!=records){loop_error=4;return;}
    loop_error=jp_simple_project(work,(size_t)capacity*32,point_extent,normal_extent,&queues,kind);
}
int jp_simple_calc_model(NJS_MODEL *model,size_t points,size_t normals,
                         NJS_VECTOR *light,Float scale,int32_t *result)
{
    if(!model || !light || !result || model->nbPoint<0 ||
       (size_t)model->nbPoint>points ||
       (model->nbPoint && !model->points) ||
       (model->normals && (size_t)model->nbPoint>normals))return 1;
    if(active || !allocation)return 4;
    if((uint32_t)model->nbPoint>capacity)return 2;
    int error=jp_simple_calc_guards();if(error)return error;
    NJS_VERTEX_BUF *saved=_nj_vertex_buf_;Int saved_count=_nj_vertex_num_;
    _nj_vertex_buf_=(NJS_VERTEX_BUF *)records;_nj_vertex_num_=(Int)capacity;
    point_extent=points;normal_extent=normals;loop_error=0;active=1;
    jp_simple_calc_context_light(light,scale);
    Sint32 value=_njSimpleCalcModel(model);
    _nj_vertex_buf_=saved;_nj_vertex_num_=saved_count;active=0;
    error=jp_simple_calc_guards();if(error)return error;
    if(loop_error)return loop_error;
    *result=value;return 0;
}
