/* Authored validation of private original heap state, appended after its
 * actual allocation/free source. Never traverses an unchecked arena pointer. */
_Static_assert(sizeof(union heap_unit)==32,"original heap unit stays 32 bytes");
uint32_t jp_sdk_source_header_bytes(void){return sizeof free_head.header;}
uint32_t jp_sdk_source_units(const void *pointer)
{return (uint32_t)((const union heap_unit *)pointer-1)->header.units;}
void jp_sdk_source_reset(void)
{
    free_head.header.next=NULL;free_head.header.units=0;
    free_last=heap_base=heap_first=NULL;heap_size=0;
}
static int jp_sdk_source_node(uintptr_t p,uintptr_t base,uint32_t usable)
{return p>=base && p<base+usable && (p-base)%32==0;}
int jp_sdk_source_validate(uintptr_t base,uint32_t usable,const struct jp_sdk_heap_record *records)
{
    const uintptr_t sentinel=(uintptr_t)&free_head;
    if(!records || !usable || usable%32 || (uintptr_t)heap_base!=base || free_head.header.units ||
       (sentinel>=base && sentinel<base+usable))return 1;
    union heap_unit *node=free_head.header.next;
    uintptr_t previous_end=base;uint32_t count=0;int found_last=free_last==&free_head;
    while(node!=&free_head){
        uintptr_t address=(uintptr_t)node;
        if(++count>usable/32 || !jp_sdk_source_node(address,base,usable) || address<previous_end)return 1;
        uint32_t units=(uint32_t)node->header.units;
        if(!units || units>(base+usable-address)/32)return 1;
        if(node==free_last)found_last=1;
        previous_end=address+(uintptr_t)units*32;node=node->header.next;
    }
    if(!found_last)return 1;
    /* The original free list and native live-record table must partition every
     * unit exactly, with no overlap, gap, nested allocation or changed header. */
    node=free_head.header.next;uint32_t index=0,total=usable/32;
    while(index<total){
        uintptr_t address=base+(uintptr_t)index*32;uint32_t units;
        if(node!=&free_head && (uintptr_t)node==address){
            units=(uint32_t)node->header.units;
            for(uint32_t j=index;j<index+units;j++)if(records[j].units)return 1;
            node=node->header.next;
        }else{
            units=records[index].units;
            if(!units || units>total-index || records[index].requested>(units-1)*32 ||
               ((union heap_unit *)address)->header.units!=units)return 1;
            for(uint32_t j=index+1;j<index+units;j++)if(records[j].units)return 1;
        }
        index+=units;
        if(node!=&free_head && (uintptr_t)node<base+(uintptr_t)index*32)return 1;
    }
    return node!=&free_head;
}
