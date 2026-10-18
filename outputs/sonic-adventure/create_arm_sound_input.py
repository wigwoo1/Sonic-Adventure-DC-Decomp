"""Generate static native byte-input parser and compact-ring producer."""
import csv,json,re
from pathlib import Path
from create_arm_sound_controls import emit
P=Path(__file__).resolve().parent
base=(P/'source/sa_arm_sound_boot_native.c').read_text();prefix=base[:base.index('SA_EXPORT int sa_arm_sound_boot_native')]
prefix=re.sub(r'^int sa_arm_sound_.*_step\([^\n]*\);\n','',prefix,flags=re.M)
prefix=re.sub(r'static uint32_t subtract\([^}]+}\n','',prefix)
sets=[]
for driver,name in enumerate(('MANATEE.DRV','MASARU.DRV')):
 rows=list(csv.DictReader((P/'analysis/arm-sequencer-enrichment'/f'{name}.arm.bin/instructions.tsv').open(),delimiter='\t'))
 selected={int(r['address'],16):r for r in rows if 0x1598+driver*12<=int(r['address'],16)<=0x1768+driver*12}
 assert len(selected)==109
 sets.append(selected)
code=prefix+'''int sa_arm_sound_input_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
'''+''.join(emit(a,r) for a,r in sorted(sets[0].items()))+''' default:*handled=0;return 0;
 }}else{switch(r[15]){
'''+''.join(emit(a,r) for a,r in sorted(sets[1].items()))+''' default:*handled=0;return 0;
 }}
}
'''
(P/'source/sa_arm_sound_input.c').write_text(code)
fixture=(P/'source/sa_arm_sound_snapshot_fixture.c').read_text().replace('snapshot','input')
(P/'source/sa_arm_sound_input_fixture.c').write_text(fixture)
(P/'analysis/arm_sound_input_translation.json').write_text(json.dumps({'drivers':[[r for _,r in sorted(rows.items())] for rows in sets]},indent=2)+'\n')
print('Generated 109 parser/ring producer instruction sites per driver')
