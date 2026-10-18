#include "native_disc_queue.h"
int sa_disc_read_sectors(sa_disc_read_at,void *,uint64_t,uint64_t,uint32_t,uint32_t,uint32_t,uint32_t,uint8_t *,uint64_t,uint32_t *);
uint32_t sa_disc_queue_size(void) { return (uint32_t)sizeof(struct sa_disc_queue); }
int sa_disc_queue_init(struct sa_disc_queue *q,sa_disc_read_at read,void *user,uint64_t offset,uint64_t sectors,uint32_t lba,uint32_t stride) {
    if(!q || !read || (stride!=2048 && stride!=2352))return -2;
    q->read=read;q->user=user;q->file_offset=offset;q->track_sectors=sectors;
    q->track_lba=lba;q->stride=stride;q->next_handle=1;q->cursor=0;q->busy=0;
    for(unsigned i=0;i<8;i++){q->jobs[i].state=SA_DISC_FREE;q->jobs[i].handle=0;}
    return 0;
}
static struct sa_disc_job *find(struct sa_disc_queue *q,uint32_t handle) {
    for(unsigned i=0;i<8;i++)if(q->jobs[i].state && q->jobs[i].handle==handle)return &q->jobs[i];
    return 0;
}
int sa_disc_queue_submit(struct sa_disc_queue *q,uint32_t first,uint32_t count,uint8_t *output,uint64_t capacity,uint32_t *handle) {
    if(!q || !handle || (!output && count))return -2;
    if(q->busy)return -6;
    if(first<q->track_lba || (uint64_t)first-q->track_lba>q->track_sectors ||
       count>q->track_sectors-((uint64_t)first-q->track_lba) || (uint64_t)count*2048>capacity ||
       (count && (uint64_t)first+count-1>UINT32_MAX))return -3;
    if(!q->next_handle || q->next_handle>0x7fffffffu)return -4;
    for(unsigned i=0;i<8;i++)if(!q->jobs[i].state) {
        struct sa_disc_job *job=&q->jobs[i];
        job->handle=q->next_handle++;job->state=SA_DISC_QUEUED;job->lba=first;job->count=count;
        job->completed=0;job->kind=0;job->error=0;job->output=output;job->capacity=capacity;
        *handle=job->handle;return 0;
    }
    return -5;
}
int sa_disc_queue_step(struct sa_disc_queue *q) {
    if(!q)return -2;
    if(q->busy)return -6;
    for(unsigned n=0;n<8;n++) {
        unsigned index=(q->cursor+n)%8;struct sa_disc_job *job=&q->jobs[index];
        if(job->state!=SA_DISC_QUEUED)continue;
        q->cursor=(index+1)%8;
        if(job->kind==1) {
            /* Native platform identity, not a copied firmware version. */
            static const uint8_t version[]="SA-NATIVE v 1.00";
            for(unsigned i=0;i<16;i++)job->output[i]=version[i];
            job->state=SA_DISC_COMPLETE;return 0;
        }
        if(job->completed==job->count){job->state=SA_DISC_COMPLETE;return 0;}
        uint32_t done=0;q->busy=1;
        int error=sa_disc_read_sectors(q->read,q->user,q->file_offset,q->track_sectors,q->track_lba,q->stride,
            job->lba+job->completed,1,job->output+(uint64_t)job->completed*2048,job->capacity-(uint64_t)job->completed*2048,&done);
        q->busy=0;job->completed+=done;
        if(error){job->error=error;job->state=SA_DISC_FAILED;}
        else if(job->completed==job->count)job->state=SA_DISC_COMPLETE;
        return 0; /* Transport failure is recorded on its owning request. */
    }
    return 0;
}
int sa_disc_queue_status(struct sa_disc_queue *q,uint32_t handle,uint32_t *state,uint32_t *completed,int32_t *error) {
    if(!q || !state || !completed || !error)return -2;
    struct sa_disc_job *job=find(q,handle);if(!job)return -7;
    *state=job->state;*completed=job->completed;*error=job->error;return 0;
}
int sa_disc_queue_cancel(struct sa_disc_queue *q,uint32_t handle) {
    if(!q)return -2;if(q->busy)return -6;
    struct sa_disc_job *job=find(q,handle);if(!job)return -7;
    if(job->state==SA_DISC_QUEUED)job->state=SA_DISC_CANCELLED;
    return 0;
}
int sa_disc_queue_release(struct sa_disc_queue *q,uint32_t handle) {
    if(!q)return -2;if(q->busy)return -6;
    struct sa_disc_job *job=find(q,handle);if(!job)return -7;
    if(job->state==SA_DISC_QUEUED)return -6;
    job->state=SA_DISC_FREE;job->handle=0;return 0;
}
int sa_disc_queue_submit_version(struct sa_disc_queue *q,uint8_t *output,uint64_t capacity,uint32_t *handle) {
    if(!q || !output || !handle)return -2;
    if(capacity<16)return -3;
    int status=sa_disc_queue_submit(q,q->track_lba,0,output,capacity,handle);
    if(status)return status;
    find(q,*handle)->kind=1;return 0;
}
int sa_disc_queue_transfer_bytes(struct sa_disc_queue *q,uint32_t handle,uint32_t *bytes) {
    if(!q || !bytes)return -2;
    struct sa_disc_job *job=find(q,handle);if(!job)return -7;
    uint64_t size=job->kind==1?(job->state==SA_DISC_COMPLETE?16u:0u):(uint64_t)job->completed*2048;
    if(size>UINT32_MAX)return -3;
    *bytes=(uint32_t)size;return 0;
}
