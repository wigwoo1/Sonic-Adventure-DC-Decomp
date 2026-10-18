"""Static recovery of the tick counter and three channel ramp loops."""
import csv,json,re
from pathlib import Path
from create_arm_sound_controls import emit
P=Path(__file__).resolve().parent
base=(P/'source/sa_arm_sound_boot_native.c').read_text()
prefix=base[:base.index('SA_EXPORT int sa_arm_sound_boot_native')]
prefix=re.sub(r'^int sa_arm_sound_.*_step\([^\n]*\);\n','',prefix,flags=re.M)
prefix+='''static uint32_t addition(struct sa_arm_state *s,uint32_t a,uint32_t b){
 uint32_t v=a+b,flags=v&0x80000000u;
 if(!v)flags|=0x40000000u;if((uint64_t)a+b>0xffffffffu)flags|=0x20000000u;
 flags|=(~(a^b)&(a^v)&0x80000000u)>>3;
 s->cpsr=(s->cpsr&0x0fffffffu)|flags;return v;
}
'''
def ramp_emit(a,row):
    if row['instruction']=='movcc r3,#0x0':
        return f'        case 0x{a:x}u: /* movcc r3,#0x0 */\n            if(!(state->cpsr&0x20000000u))r[3]=0;r[15]=0x{a+4:x}u;return 0;\n'
    return emit(a,row)
sets=[];entries=[]
for driver,name in enumerate(('MANATEE.DRV','MASARU.DRV')):
    addresses=[a+8*driver for a in (0x6620,0x6640,0x66e0,0x6780)];entries.append([hex(a) for a in addresses])
    with (P/'analysis/arm-sound-drivers'/(name+'.arm.bin')/'instructions.tsv').open() as f:rows=list(csv.DictReader(f,delimiter='\t'))
    selected={int(r['address'],16):r for r in rows if r['function_entry'] in {f'{a:08x}' for a in addresses}}
    assert len(selected)==134,len(selected)
    sets.append(selected)
code=prefix+'''int sa_arm_sound_ramp_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
'''+''.join(ramp_emit(a,r) for a,r in sorted(sets[0].items()))+''' default:*handled=0;return 0;
 }}else{switch(r[15]){
'''+''.join(ramp_emit(a,r) for a,r in sorted(sets[1].items()))+''' default:*handled=0;return 0;
 }}
}
'''
(P/'source/sa_arm_sound_ramps.c').write_text(code)
fixture=(P/'source/sa_arm_sound_snapshot_fixture.c').read_text().replace('snapshot','ramp')
(P/'source/sa_arm_sound_ramps_fixture.c').write_text(fixture)
(P/'analysis/arm_sound_ramps_translation.json').write_text(json.dumps({'entries_by_driver':entries,'drivers':[[r for _,r in sorted(s.items())] for s in sets]},indent=2)+'\n')
print('Generated',sum(map(len,sets)),'driver-local counter/ramp instruction sites')
