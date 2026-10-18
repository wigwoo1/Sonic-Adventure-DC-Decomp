from pathlib import Path
p=Path(__file__).resolve().parents[1]/'source/sa_input_work_yield.c';s=p.read_text()
if 'sa_input_work_yield_core' not in s:
 s=s.replace('SA_STAGE_EXPORT int sa_input_work_yield(', 'static int sa_input_work_yield_core(uint32_t entry,')
 s=s.replace('goto L8c668e1e;','if(entry==0)goto L8c668e1e;else if(entry==0x8c668e48u)goto L8c668e48;else return -4;',1)
 args='uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)'
 s+='\nSA_STAGE_EXPORT int sa_input_work_yield('+args+'){return sa_input_work_yield_core(0,m,f,sr,r,pr,fpul,macl,ctx,call);}\n'
 s+='SA_STAGE_EXPORT int sa_input_work_yield_resume('+args+'){return sa_input_work_yield_core(*pr,m,f,sr,r,pr,fpul,macl,ctx,call);}\n'
 p.write_text(s)
