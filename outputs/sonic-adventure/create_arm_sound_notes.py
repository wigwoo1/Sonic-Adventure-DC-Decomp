"""Static native recovery of the original type-1 note and allocation paths."""
import csv,json,re
from pathlib import Path
from create_arm_sound_controls import emit
P=Path(__file__).resolve().parent
sets=[];metadata=[]
for driver,name in enumerate(('MANATEE.DRV','MASARU.DRV')):
    with (P/'analysis/arm-sound-drivers'/(name+'.arm.bin')/'instructions.tsv').open() as stream:rows=list(csv.DictReader(stream,delimiter='\t'))
    end=0x135c if driver==0 else 0x1364
    selected={int(r['address'],16):r for r in rows if 0xad4<=int(r['address'],16)<end}
    sets.append(selected);metadata.append({'note_handler':'0xad4','allocation_helper':hex(0x1244+driver*8),'instruction_addresses':[hex(a) for a in sorted(selected)]})
root=P/'source/sa_arm_sound_boot_native.c';base=root.read_text()
prefix=base[:base.index('SA_EXPORT int sa_arm_sound_boot_native')]
prefix=re.sub(r'^int sa_arm_sound_.*_step\([^\n]*\);\n','',prefix,flags=re.M)
prefix+='''static uint32_t addition(struct sa_arm_state *s,uint32_t a,uint32_t b){
 uint32_t v=a+b,flags=v&0x80000000u;
 if(!v)flags|=0x40000000u;if((uint64_t)a+b>0xffffffffu)flags|=0x20000000u;
 flags|=(~(a^b)&(a^v)&0x80000000u)>>3;
 s->cpsr=(s->cpsr&0x0fffffffu)|flags;return v;
}
'''
code=prefix+'''int sa_arm_sound_note_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
'''+''.join(emit(a,r) for a,r in sorted(sets[0].items()))+'''        default:*handled=0;return 0;
 }}else{switch(r[15]){
'''+''.join(emit(a,r) for a,r in sorted(sets[1].items()))+'''        default:*handled=0;return 0;
 }}
}
'''
(P/'source/sa_arm_sound_notes.c').write_text(code)
prototype='int sa_arm_sound_note_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);\n'
if prototype not in base:base=base.replace('static uint32_t subtract',prototype+'static uint32_t subtract',1)
if 'result=sa_arm_sound_note_step' not in base:
    base=base.replace('            switch(r[15]){','            result=sa_arm_sound_note_step(ram,state,driver,user,read,write,&handled);\n            if(handled){if(result)return result;break;}\n            switch(r[15]){')
root.write_text(base)
(P/'analysis/arm_sound_notes_translation.json').write_text(json.dumps({'drivers':metadata},indent=2)+'\n')
print('Generated',sum(len(s) for s in sets),'driver-local note/allocation instruction sites')
