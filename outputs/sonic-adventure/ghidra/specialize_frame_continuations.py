from pathlib import Path
p=Path(__file__).resolve().parents[1]/'source/sa_input_frame_manager.c'
s=p.read_text()
if 'sa_input_frame_manager_core' not in s:
 s=s.replace('SA_STAGE_EXPORT int sa_input_frame_manager(', 'static int sa_input_frame_manager_core(uint32_t entry,')
 s=s.replace('goto L8c668e60;','switch(entry){case 0:goto L8c668e60;case 0x8c668e92u:goto L8c668e92;case 0x8c668edcu:goto L8c668edc;case 0x8c668ef2u:goto L8c668ef2;default:return -4;}',1)
 args='uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)'
 s+='\nSA_STAGE_EXPORT int sa_input_frame_manager('+args+'){return sa_input_frame_manager_core(0,m,f,sr,r,pr,fpul,macl,ctx,call);}\n'
 s+='SA_STAGE_EXPORT int sa_input_frame_manager_resume('+args+'){return sa_input_frame_manager_core(*pr,m,f,sr,r,pr,fpul,macl,ctx,call);}\n'
 p.write_text(s)
