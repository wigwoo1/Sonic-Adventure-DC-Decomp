from pathlib import Path
root=Path(__file__).resolve().parents[1]
args='uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)'
for name,start,entries in [('sa_input_device_request','8c64753c',['8c64756e','8c6475bc']),('sa_input_press_service','8c64839a',['8c6483f6','8c648400','8c648412','8c648426'])]:
 p=root/'source'/(name+'.c');s=p.read_text()
 if name+'_core' in s:continue
 s=s.replace('SA_STAGE_EXPORT int '+name+'(', 'static int '+name+'_core(uint32_t entry,')
 cases='switch(entry){case 0:goto L'+start+';'+''.join('case 0x'+e+'u:goto L'+e+';' for e in entries)+'default:return -4;}'
 s=s.replace('goto L'+start+';',cases,1)
 for suffix,entry in [('', '0'),('_resume','*pr')]:s+='\nSA_STAGE_EXPORT int '+name+suffix+'('+args+'){return '+name+'_core('+entry+',m,f,sr,r,pr,fpul,macl,ctx,call); }\n'
 p.write_text(s)
