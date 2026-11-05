/* Authored diagnostic inputs/observations for original SDK pool routines.
 * Does not export sdLibInit, driver initialization, G2 or other success stubs. */
_Static_assert(sizeof(struct SDS_MEM_BLK)==sizeof(struct sd_memblk),"native opaque descriptor span");
_Static_assert(sizeof(struct sd_memblk)==48,"native descriptor ABI");
static struct sd_work peer_work;
static struct SDS_MEM_BLK peer_blocks[SD_MEMBLK_MAX];
__declspec(dllexport) int peer_initialize(void)
{
    memset(&peer_work,0,sizeof peer_work);sdgWrkPtr=&peer_work;
    return sdMemBlkInit(peer_blocks,SD_MEMBLK_MAX);
}
__declspec(dllexport) void *peer_table(void){return peer_blocks;}
__declspec(dllexport) long peer_count(void){return peer_work.memblk_num;}
__declspec(dllexport) long peer_error(void){return sdgErrResult;}
__declspec(dllexport) void *peer_error_api(void){return sdgErrApi;}
__declspec(dllexport) unsigned long peer_block_bytes(void){return sizeof(struct sd_memblk);}
__declspec(dllexport) int peer_record(SDMEMBLK handle,unsigned long long *out)
{
    if(!out||!handle)return 1;
    struct sd_memblk *block=(struct sd_memblk *)handle;
    if(block<(struct sd_memblk *)peer_blocks||block>=(struct sd_memblk *)(peer_blocks+SD_MEMBLK_MAX))return 1;
    out[0]=block->used;out[1]=block->stat;out[2]=(size_t)block->data;out[3]=(unsigned long)block->size;
    out[4]=(size_t)block->callback;out[5]=(size_t)block->callback_first_arg;out[6]=block->done;out[7]=block->unk_1c;return 0;
}
