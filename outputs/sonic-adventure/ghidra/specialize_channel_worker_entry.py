from pathlib import Path
p=Path(__file__).resolve().parents[1]/'source/sa_input_channel_worker.c';s=p.read_text()
if 'sa_input_channel_worker_core' not in s:
 s=s.replace('SA_STAGE_EXPORT int sa_input_channel_worker(', 'static int sa_input_channel_worker_core(uint32_t entry,')
 s=s.replace('goto L8c647980;','if(entry==0x8c647980u)goto L8c647980;else if(entry==0x8c647982u)goto L8c647982;else return -4;',1)
 args='uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)'
 s+='\nSA_STAGE_EXPORT int sa_input_channel_worker('+args+'){return sa_input_channel_worker_core(0x8c647980u,m,f,sr,r,pr,fpul,macl,ctx,call);}\n'
 s+='SA_STAGE_EXPORT int sa_input_channel_worker_entry('+args+'){return sa_input_channel_worker_core(0x8c647982u,m,f,sr,r,pr,fpul,macl,ctx,call);}\n'
 p.write_text(s)
if 'sa_input_channel_worker_resume(' not in s:
 s=s.replace('else return -4;', 'else if(entry==0x8c647b74u)goto L8c647b74;else return -4;',1)
 args='uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)'
 s+='SA_STAGE_EXPORT int sa_input_channel_worker_resume('+args+'){return sa_input_channel_worker_core(*pr,m,f,sr,r,pr,fpul,macl,ctx,call);}\n'
 p.write_text(s)

if 'entry==0x8c6479fau' not in s:
 s=s.replace('else return -4;', 'else if(entry==0x8c6479fau)goto L8c6479fa;else if(entry==0x8c647a08u)goto L8c647a08;else return -4;',1)
 p.write_text(s)
