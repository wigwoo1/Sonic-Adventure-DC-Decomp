"""Recover the three sound-state snapshot loops into static native C."""
import csv,json,re
from pathlib import Path
from create_arm_sound_controls import emit
P=Path(__file__).resolve().parent
base=(P/'source/sa_arm_sound_boot_native.c').read_text()
prefix=base[:base.index('SA_EXPORT int sa_arm_sound_boot_native')]
prefix=re.sub(r'^int sa_arm_sound_.*_step\([^\n]*\);\n','',prefix,flags=re.M)
def snapshot_emit(address,row):
    # This routine has exactly one unshifted ORRS instruction per driver.
    # Its register operand preserves incoming C and V while updating N/Z.
    if row['instruction']=='orrs r5,r2,r5':
        return f'        case 0x{address:x}u: /* orrs r5,r2,r5 */\n            r[5]=r[2]|r[5];logic_flags(state,r[5],0,0);r[15]=0x{address+4:x}u;return 0;\n'
    return emit(address,row)
sets=[]
for driver,name in enumerate(('MANATEE.DRV','MASARU.DRV')):
    entry=0x63f8+8*driver
    with (P/'analysis/arm-sound-drivers'/(name+'.arm.bin')/'instructions.tsv').open() as f:
        rows=list(csv.DictReader(f,delimiter='\t'))
    selected={int(r['address'],16):r for r in rows if r['function_entry']==f'{entry:08x}'}
    assert sum(r['instruction']=='orrs r5,r2,r5' for r in selected.values())==1
    sets.append(selected)
code=prefix+'''int sa_arm_sound_snapshot_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
'''+''.join(snapshot_emit(a,r) for a,r in sorted(sets[0].items()))+''' default:*handled=0;return 0;
 }}else{switch(r[15]){
'''+''.join(snapshot_emit(a,r) for a,r in sorted(sets[1].items()))+''' default:*handled=0;return 0;
 }}
}
'''
(P/'source/sa_arm_sound_snapshot.c').write_text(code)
(P/'analysis/arm_sound_snapshot_translation.json').write_text(json.dumps({'entries':['0x63f8','0x6400'],'drivers':[[r for _,r in sorted(s.items())] for s in sets]},indent=2)+'\n')
print('Generated',sum(map(len,sets)),'driver-local snapshot instruction sites')
