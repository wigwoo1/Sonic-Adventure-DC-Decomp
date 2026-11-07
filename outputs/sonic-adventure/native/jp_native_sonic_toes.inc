/* Included after genuine private SDK types, selected original helper and
 * pointer-table storage. No model/player data or game body is defined here. */
#include "jp_native_sonic_toes.h"
#define JP_MOTION_API __declspec(dllimport)
#include "jp_native_motion_store.h"
static struct {struct jp_sonic_toes_info info;int busy;struct jp_motion_view views[12];} toe_owner;
static int toe_access(void)
{
    if(toe_owner.info.thread&&toe_owner.info.thread!=GetCurrentThreadId())return 3;
    if(toe_owner.busy)return 4;
    return 0;
}
static int toe_release_views(void)
{
    while(toe_owner.info.leases){
        int rc=jp_motions_release(toe_owner.views[toe_owner.info.leases-1].token);
        if(rc){toe_owner.info.native_error=(uint32_t)rc;return 5;}
        --toe_owner.info.leases;
    }
    memset(&toe_owner,0,sizeof toe_owner);
    toe_plus_node=toe_minus_node=NULL;
    memset(sonic_toe_plus_z_models,0,sizeof sonic_toe_plus_z_models);
    memset(sonic_toe_minus_z_models,0,sizeof sonic_toe_minus_z_models);
    return 0;
}
int jp_sonic_toes_bind(void)
{
    int rc=toe_access();if(rc)return rc;
    if(toe_owner.info.phase)return 2;
    toe_owner.info.thread=GetCurrentThreadId();toe_owner.info.phase=2;toe_owner.busy=1;
    for(unsigned i=0;i<12;i++){
        rc=jp_motions_acquire(JP_MOTION_MODELS,toe_view_names[i],&toe_owner.views[i]);
        if(rc){toe_owner.info.native_error=(uint32_t)rc;toe_owner.busy=0;int retired=toe_release_views();if(!retired)toe_owner.info.native_error=(uint32_t)rc;return retired?retired:5;}
        ++toe_owner.info.leases;
    }
    for(unsigned i=0;i<4;i++){
        sonic_toe_plus_z_models[i]=toe_owner.views[i].data;
        sonic_toe_minus_z_models[i]=toe_owner.views[i+5].data;
    }
    toe_plus_node=toe_owner.views[4].data;toe_minus_node=toe_owner.views[9].data;
    toe_owner.info.phase=1;toe_owner.busy=0;return 0;
}
int jp_sonic_toes_step(void *player)
{
    int rc=toe_access();if(rc)return rc;
    if(toe_owner.info.phase!=1)return 2;
    if(!player)return 1;
    toe_owner.busy=1;sonic_blend_toes(NULL,NULL,player);++toe_owner.info.steps;toe_owner.busy=0;return 0;
}
int jp_sonic_toes_close(void)
{
    int rc=toe_access();if(rc)return rc;
    if(!toe_owner.info.phase)return 2;
    if(toe_owner.info.phase==1){
        toe_plus_node->model=sonic_toe_plus_z_models[TOE_MODEL];
        toe_minus_node->model=sonic_toe_minus_z_models[TOE_MODEL];
    }
    toe_owner.info.phase=2;toe_owner.busy=1;rc=toe_release_views();toe_owner.busy=0;return rc;
}
int jp_sonic_toes_snapshot(struct jp_sonic_toes_info *out)
{
    int rc=toe_access();if(rc)return rc;
    if(!out)return 1;
    *out=toe_owner.info;return 0;
}
int jp_sonic_toes_layout(uint32_t *out)
{
    if(!out)return 1;
    const uint32_t values[]={sizeof(struct player),__builtin_offsetof(struct player,anim.action),
        __builtin_offsetof(struct player,anim.frame),__builtin_offsetof(struct player,own.sonic.toe_blend_plus_z),
        __builtin_offsetof(struct player,own.sonic.toe_blend_minus_z),sizeof(NJS_MODEL),sizeof(NJS_OBJECT)};
    memcpy(out,values,sizeof values);return 0;
}
