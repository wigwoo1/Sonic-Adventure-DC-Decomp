/* Authored value translation of njModelS.c's four SH-4 projection loops. */
#include "jp_native_simple_projection.h"
#include "jp_native_sprite_numeric.h"
#include "jp_native_action_numeric.h"
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
#include <string.h>
extern NJS_SCREEN _nj_screen_;
extern Float _nj_near_clip_,_nj_far_clip_;
float *jp_ninja_current_matrix_storage(void);
void njSimpleCalcModel_Loop1(struct jp_simple_calc_work *,NJS_MATRIX *);
void njSimpleCalcModel_Loop2(struct jp_simple_calc_work *,NJS_MATRIX *);
void njSimpleCalcModel_Loop3(struct jp_simple_calc_work *,NJS_MATRIX *);
void njSimpleCalcModel_Loop4(struct jp_simple_calc_work *,NJS_MATRIX *);
static struct jp_simple_calc_work *active_work;
static struct jp_simple_projection_queues *active_queues;
static unsigned active_kind;
static int active_error;
static uint32_t bits(float f){uint32_t word;memcpy(&word,&f,4);return word;}

void jp_simple_projection_loop(struct jp_simple_calc_work *work,unsigned kind)
{
    if(work!=active_work || kind!=active_kind || !active_queues){active_error=3;return;}
    if(!work->count)return;
    uint32_t matrix[16];
    memcpy(matrix,jp_ninja_current_matrix_storage(),sizeof matrix);
    for(Sint32 i=0;i<work->count;i++){
        uint32_t point[4];
        float p[3];
        memcpy(point,&work->points[i],12);
        point[3]=0x3f800000u;
        /* The projected record uses XYZ. The fourth dot product has no value consumer. */
        for(unsigned row=0;row<3;row++){
            uint32_t cause;
            uint32_t word=sh4_fipr__dot(point,1,matrix+row,4,1,1,&cause);
            memcpy(&p[row],&word,sizeof word);
        }
        uint32_t flags=0;float x=p[0],y=p[1],depth;
        if(kind>=3 && jp_sprite_gt(_nj_near_clip_,p[2])){
            depth=p[2];
        }else{
            depth=jp_action_fsrra(jp_sprite_multiply(p[2],p[2]));
            x=jp_sprite_add(jp_sprite_multiply(jp_sprite_multiply(work->scale_x,x),depth),_nj_screen_.cx);
            y=jp_sprite_add(jp_sprite_multiply(jp_sprite_multiply(work->scale_y,y),depth),_nj_screen_.cy);
            if(kind>=3){
                flags=(uint32_t)jp_sprite_gt(y,work->clip_bottom);
                flags=(flags<<1)|(uint32_t)jp_sprite_gt(work->clip_top,y);
                flags=(flags<<1)|(uint32_t)jp_sprite_gt(x,work->clip_right);
                flags=(flags<<1)|(uint32_t)jp_sprite_gt(work->clip_left,x);
            }
        }
        if(kind>=3){
            flags=(flags<<1)|(uint32_t)jp_sprite_gt(p[2],_nj_far_clip_);
            flags=(flags<<1)|(uint32_t)jp_sprite_gt(_nj_near_clip_,p[2]);
        }
        uintptr_t address=(uintptr_t)work->out+(size_t)i*32;
        uint32_t *q=active_queues->words[(address>>5)&1];
        if(kind==1 || kind==3)memcpy(q,&work->normals[i],12);
        q[3]=flags;q[4]=bits(x);q[5]=bits(y);q[6]=bits(depth);
        memcpy((void *)address,q,32);
        if(kind>=3){work->clip_and&=flags;work->clip_or|=flags;}
    }
}
int jp_simple_project(struct jp_simple_calc_work *work,size_t output_bytes,
                      size_t points,size_t normals,
                      struct jp_simple_projection_queues *queues,unsigned kind)
{
    if(!work || !queues || kind<1 || kind>4 || work->count<0)return 1;
    size_t count=(size_t)work->count;
    if(count>SIZE_MAX/32 || output_bytes<count*32 || points<count ||
       (count && (!work->out || !work->points || ((uintptr_t)work->out&31))) ||
       ((kind==1 || kind==3) && count && (!work->normals || normals<count)))return 1;
    if(active_work)return 2;
    active_work=work;active_queues=queues;active_kind=kind;active_error=0;
    switch(kind){
    case 1:njSimpleCalcModel_Loop1(work,NULL);break;
    case 2:njSimpleCalcModel_Loop2(work,NULL);break;
    case 3:njSimpleCalcModel_Loop3(work,NULL);break;
    case 4:njSimpleCalcModel_Loop4(work,NULL);break;
    }
    int error=active_error;active_work=NULL;active_queues=NULL;
    return error;
}
