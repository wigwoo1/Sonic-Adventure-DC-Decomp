#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "jp_native_motion_store.h"
/* Exact ABI of the existing qualified generated data registries. */
struct motion_item {const char *name;void *data;size_t host_bytes;
    uint32_t address,original_bytes,kind,original_element,host_element;};
typedef unsigned (*count_fn)(void);
typedef int (*entry_fn)(unsigned,struct motion_item *);
#define BANK(n) unsigned jp_motion_##n##_count(void);int jp_motion_##n##_entry(unsigned,struct motion_item *);
BANK(s_mot) BANK(a_mot) BANK(b_mot) BANK(e_mot) BANK(k_mot) BANK(s_sbmot) BANK(m_sbmot) BANK(models)
#undef BANK
static const count_fn counts[]={jp_motion_s_mot_count,jp_motion_a_mot_count,jp_motion_b_mot_count,jp_motion_e_mot_count,jp_motion_k_mot_count,jp_motion_s_sbmot_count,jp_motion_m_sbmot_count,jp_motion_models_count};
static const entry_fn entries[]={jp_motion_s_mot_entry,jp_motion_a_mot_entry,jp_motion_b_mot_entry,jp_motion_e_mot_entry,jp_motion_k_mot_entry,jp_motion_s_sbmot_entry,jp_motion_m_sbmot_entry,jp_motion_models_entry};
static const char *const files[]={"s_mot.PRS","a_mot.PRS","b_mot.PRS","e_mot.PRS","k_mot.PRS"};
struct saved_item {struct motion_item item;size_t offset;};
struct lease {uint64_t token,generation;uint32_t bank,index;};
static struct {
    struct jp_motion_info info;DWORD thread;int busy,active;
    struct saved_item *items;unsigned char *defaults;struct lease *leases;
    uint32_t first[8],count[8],capacity,total;
    uint64_t bank_generation[8];int32_t requested_character;
} store={.info={.selected_bank=JP_MOTION_NO_BANK,.character=-1}};
static uint64_t next_token=1,next_generation=1;
void jp_original_motion_select(int);
_Static_assert(sizeof(struct motion_item)==48&&sizeof(struct jp_motion_info)==96&&sizeof(struct jp_motion_view)==48,"native motion ABIs");
static int access_owner(void)
{return store.thread&&store.thread!=GetCurrentThreadId()?JP_MOTION_THREAD:store.busy?JP_MOTION_REENTRANT:!store.active?JP_MOTION_STATE:0;}
static int result(int rc){store.info.status=(uint32_t)rc;return rc;}
static int overlap(const void *out,size_t n,const void *base,size_t bytes)
{
    uintptr_t at=(uintptr_t)out,start=(uintptr_t)base;
    return bytes&&at<start+bytes&&start<at+n;
}
static int output_valid(const void *out,size_t n)
{
    uintptr_t at=(uintptr_t)out;
    if(!out||n>UINTPTR_MAX-at||overlap(out,n,&store,sizeof store))return 0;
    if(overlap(out,n,store.items,(size_t)store.total*sizeof *store.items)||
       overlap(out,n,store.defaults,(size_t)store.info.resident_bytes)||
       overlap(out,n,store.leases,(size_t)store.capacity*sizeof *store.leases))return 0;
    for(uint32_t i=0;i<store.total;i++)if(overlap(out,n,store.items[i].item.data,store.items[i].item.host_bytes))return 0;
    return 1;
}
static int bank_busy(uint32_t bank)
{
    for(uint32_t i=0;i<store.capacity;i++)if(store.leases[i].token&&store.leases[i].bank==bank)return 1;
    return 0;
}
static void restore_bank(uint32_t bank)
{
    for(uint32_t i=store.first[bank];i<store.first[bank]+store.count[bank];i++){
        struct saved_item *s=&store.items[i];memcpy(s->item.data,store.defaults+s->offset,s->item.host_bytes);
    }
}
int jp_motions_snapshot(struct jp_motion_info *out)
{
    if(store.thread&&store.thread!=GetCurrentThreadId())return JP_MOTION_THREAD;
    if(!output_valid(out,sizeof *out))return JP_MOTION_ARGUMENT;
    *out=store.info;return 0;
}
int jp_motions_initialize(size_t budget,uint32_t capacity)
{
    if(store.thread&&store.thread!=GetCurrentThreadId())return JP_MOTION_THREAD;
    if(store.busy)return JP_MOTION_REENTRANT;if(store.active)return result(JP_MOTION_STATE);
    if(!budget||budget>(128u<<20)||!capacity||capacity>65536)return result(JP_MOTION_ARGUMENT);
    if(!next_generation)return result(JP_MOTION_LIMIT);
    uint32_t count[8],first[8],total=0;size_t bytes=0;
    for(uint32_t bank=0;bank<8;bank++){
        count[bank]=counts[bank]();first[bank]=total;
        if(!count[bank]||count[bank]>1000000u-total)return result(JP_MOTION_IDENTITY);
        total+=count[bank];
        for(uint32_t i=0;i<count[bank];i++){
            struct motion_item e;
            if(entries[bank](i,&e)||!e.data||!e.name||!e.host_bytes||e.host_bytes>budget-bytes)return result(JP_MOTION_LIMIT);
            bytes+=e.host_bytes;
        }
    }
    size_t metadata=(size_t)total*sizeof(struct saved_item),leases=(size_t)capacity*sizeof(struct lease);
    if(metadata>budget-bytes||leases>budget-bytes-metadata)return result(JP_MOTION_LIMIT);
    struct saved_item *items=malloc(metadata);unsigned char *defaults=malloc(bytes);struct lease *slots=calloc(capacity,sizeof *slots);
    if(!items||!defaults||!slots){free(items);free(defaults);free(slots);return result(JP_MOTION_ALLOCATION);}
    size_t offset=0;
    for(uint32_t bank=0;bank<8;bank++)for(uint32_t i=0;i<count[bank];i++){
        struct saved_item *s=&items[first[bank]+i];
        if(entries[bank](i,&s->item)){free(items);free(defaults);free(slots);return result(JP_MOTION_IDENTITY);}
        s->offset=offset;memcpy(defaults+offset,s->item.data,s->item.host_bytes);offset+=s->item.host_bytes;
    }
    store.items=items;store.defaults=defaults;store.leases=slots;store.capacity=capacity;store.total=total;
    memcpy(store.first,first,sizeof first);memcpy(store.count,count,sizeof count);memset(store.bank_generation,0,sizeof store.bank_generation);
    store.thread=GetCurrentThreadId();store.active=1;store.info.starts++;store.info.phase=1;store.info.creator_thread=store.thread;
    store.info.selected_bank=JP_MOTION_NO_BANK;store.info.character=-1;store.info.ready_mask=1u<<JP_MOTION_MODELS;store.info.live_leases=0;
    store.info.resident_bytes=bytes;store.info.allocated_bytes=bytes+metadata+leases;store.info.generation=next_generation++;
    store.bank_generation[JP_MOTION_MODELS]=store.info.generation;return result(0);
}
/* Only the extracted original selector can call these while its checked
 * higher operation is active. MOTION_AREA is validated as a source-area tag;
 * it is never dereferenced, mapped or used as a native destination. */
int jp_motion_source_load(char *name,void *area)
{
    if(!store.active||!store.busy||store.thread!=GetCurrentThreadId()||(uintptr_t)area!=UINT32_C(0x0cc00000))return result(JP_MOTION_IDENTITY);
    uint32_t bank=0;while(bank<5&&strcmp(name,files[bank]))bank++;
    if(bank==5)return result(JP_MOTION_IDENTITY);
    if(bank_busy(bank)||(store.info.selected_bank<5&&bank_busy(store.info.selected_bank)))return result(JP_MOTION_BUSY);
    if(!next_generation)return result(JP_MOTION_LIMIT);
    restore_bank(bank);store.info.ready_mask=(store.info.ready_mask&~31u)|(1u<<bank);
    store.info.selected_bank=bank;store.info.character=store.requested_character;
    store.info.generation=next_generation++;store.bank_generation[bank]=store.info.generation;store.info.selections++;return result(0);
}
void jp_motion_bad_character(int character)
{(void)character;result(JP_MOTION_CHARACTER);}
int jp_motions_select(int32_t character)
{
    int rc=access_owner();if(rc)return rc;
    store.busy=1;store.requested_character=character;result(0);jp_original_motion_select(character);store.busy=0;
    return (int)store.info.status;
}
int jp_motions_reset_board(uint32_t bank)
{
    int rc=access_owner();if(rc)return rc;
    if(bank!=JP_MOTION_SONIC_BOARD&&bank!=JP_MOTION_TAILS_BOARD)return result(JP_MOTION_ARGUMENT);
    if(bank_busy(bank))return result(JP_MOTION_BUSY);if(!next_generation)return result(JP_MOTION_LIMIT);
    store.busy=1;restore_bank(bank);store.info.generation=next_generation++;store.bank_generation[bank]=store.info.generation;
    store.info.ready_mask|=1u<<bank;store.info.board_resets++;store.busy=0;return result(0);
}
int jp_motions_acquire(uint32_t bank,const char *name,struct jp_motion_view *out)
{
    int rc=access_owner();if(rc)return rc;
    if(bank>=8||!name||!output_valid(out,sizeof *out))return result(JP_MOTION_ARGUMENT);
    if(!(store.info.ready_mask&(1u<<bank)))return result(JP_MOTION_STATE);
    size_t length=0;while(length<128&&name[length])length++;if(!length||length==128)return result(JP_MOTION_ARGUMENT);
    uint32_t i=0;for(;i<store.count[bank];i++)if(!strcmp(name,store.items[store.first[bank]+i].item.name))break;
    if(i==store.count[bank])return result(JP_MOTION_NOT_FOUND);
    uint32_t slot=0;while(slot<store.capacity&&store.leases[slot].token)slot++;
    if(slot==store.capacity)return result(JP_MOTION_BUSY);if(!next_token)return result(JP_MOTION_LIMIT);
    struct motion_item *e=&store.items[store.first[bank]+i].item;
    struct lease lease={next_token++,store.bank_generation[bank],bank,i};store.leases[slot]=lease;
    struct jp_motion_view view={lease.token,lease.generation,e->data,e->host_bytes,bank,i,e->kind,0};*out=view;
    store.info.acquires++;store.info.live_leases++;return result(0);
}
int jp_motions_release(uint64_t token)
{
    int rc=access_owner();if(rc)return rc;if(!token)return result(JP_MOTION_ARGUMENT);
    uint32_t i=0;while(i<store.capacity&&store.leases[i].token!=token)i++;
    if(i==store.capacity)return result(JP_MOTION_STATE);
    memset(&store.leases[i],0,sizeof store.leases[i]);store.info.live_leases--;store.info.releases++;return result(0);
}
int jp_motions_shutdown(void)
{
    if(store.thread&&store.thread!=GetCurrentThreadId())return JP_MOTION_THREAD;
    if(store.busy)return JP_MOTION_REENTRANT;if(!store.active)return 0;
    if(store.info.live_leases)return result(JP_MOTION_BUSY);
    store.busy=1;for(uint32_t bank=0;bank<8;bank++)restore_bank(bank);
    free(store.items);free(store.defaults);free(store.leases);store.items=NULL;store.defaults=NULL;store.leases=NULL;
    store.capacity=store.total=0;store.info.phase=2;store.info.allocated_bytes=0;store.info.ready_mask=0;
    store.info.selected_bank=JP_MOTION_NO_BANK;store.info.character=-1;store.active=store.busy=0;store.thread=0;return result(0);
}
