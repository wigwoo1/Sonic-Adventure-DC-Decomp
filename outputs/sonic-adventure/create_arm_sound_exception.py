"""Recover original vectors and exception/fast-interrupt handler instructions."""
import csv,json,re
from pathlib import Path
from create_arm_sound_controls import emit
P=Path(__file__).resolve().parent
base=(P/'source/sa_arm_sound_boot_native.c').read_text();prefix=base[:base.index('SA_EXPORT int sa_arm_sound_boot_native')]
prefix=re.sub(r'^int sa_arm_sound_.*_step\([^\n]*\);\n','',prefix,flags=re.M)
prefix+='#include "arm_sound_exception.h"\n'
def exception_emit(a,row):
 asm=row['instruction']
 if a in (0x240,0x364):
  body='uint32_t target;LOAD(target,r[13],4);'
  if a==0x364:body+='r[13]+=4u;'
  body+='uint32_t mode=state->cpsr&31u;for(unsigned i=0;i<7;++i)banks->exception_r8_r14[i]=r[8+i];'
  body+='if(mode==0x11u){for(unsigned i=0;i<7;++i)r[8+i]=banks->svc_r8_r14[i];}'
  body+='else if(mode!=0x13u){r[13]=banks->svc_r8_r14[5];r[14]=banks->svc_r8_r14[6];}'
  body+='for(unsigned i=0;i<7;++i)banks->svc_r8_r14[i]=r[8+i];'
  body+='state->cpsr=banks->spsr;r[15]=target&~3u;banks->returned=1u;return 0;'
  return f'        case 0x{a:x}u: /* {asm} */\n            {{'+body+'}\n'
 if asm=='str lr,[sp,#0x0]':body='STORE(r[13],r[14],4);'
 elif asm=='mvn r9,#0x0':body='r[9]=~0u;'
 else:return emit(a,row)
 return f'        case 0x{a:x}u: /* {asm} */\n            '+body+f'r[15]=0x{a+4:x}u;return 0;\n'
sets=[]
for name in ('MANATEE.DRV','MASARU.DRV'):
 rows=list(csv.DictReader((P/'analysis/arm-sequencer-enrichment'/f'{name}.arm.bin/instructions.tsv').open(),delimiter='\t'))
 selected={int(r['address'],16):r for r in rows if 4<=int(r['address'],16)<=0x1c or 0x234<=int(r['address'],16)<=0x364}
 assert len(selected)==79;sets.append(selected)
code=prefix+'''int sa_arm_sound_exception_step(uint8_t *ram,struct sa_arm_state *state,struct sa_arm_exception_banks *banks,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
'''+''.join(exception_emit(a,r) for a,r in sorted(sets[0].items()))+''' default:*handled=0;return 0;
 }}else{switch(r[15]){
'''+''.join(exception_emit(a,r) for a,r in sorted(sets[1].items()))+''' default:*handled=0;return 0;
 }}
}
'''
(P/'source/sa_arm_sound_exception.c').write_text(code)
(P/'analysis/arm_sound_exception_translation.json').write_text(json.dumps({'drivers':[[r for _,r in sorted(s.items())] for s in sets]},indent=2)+'\n')
print('Generated 79 original vector/exception instruction sites per driver')
