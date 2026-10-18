"""Recover the complete 24-stream update and its direct helper paths."""
import csv,json,re
from pathlib import Path
from create_arm_sound_controls import emit
P=Path(__file__).resolve().parent
base=(P/'source/sa_arm_sound_boot_native.c').read_text()
prefix=base[:base.index('SA_EXPORT int sa_arm_sound_boot_native')]
prefix=re.sub(r'^int sa_arm_sound_.*_step\([^\n]*\);\n','',prefix,flags=re.M)
def stream_emit(a,row):
    asm=row['instruction'];m,operand=asm.split(' ',1)
    if asm=='rsb r2,r2,#0x100':
        return f'        case 0x{a:x}u: /* {asm} */\n            r[2]=256u-r[2];r[15]=0x{a+4:x}u;return 0;\n'
    if asm=='movs r1,r1, lsr #0xf':
        return f'        case 0x{a:x}u: /* {asm} */\n            temporary=r[1];r[1]=temporary>>15;logic_flags(state,r[1],0,0);state->cpsr=(state->cpsr&~0x20000000u)|(((temporary>>14)&1u)<<29);r[15]=0x{a+4:x}u;return 0;\n'
    predicates={'cc':'!(state->cpsr&0x20000000u)',
      'gt':'!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0',
      'lt':'(((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0'}
    for suffix,predicate in predicates.items():
        if m.endswith(suffix):
            rewritten=dict(row,instruction=m[:-2]+' '+operand)
            body=emit(a,rewritten).split('\n',1)[1].strip().removesuffix('return 0;')
            return f'        case 0x{a:x}u: /* {asm} */\n            if({predicate}){{{body}}}else{{r[15]=0x{a+4:x}u;}}return 0;\n'
    return emit(a,row)
sets=[];actual_ranges=[]
ranges=((0x70a4,0x70f8),(0x7100,0x7298),(0x72d8,0x73a8),(0x7d64,0x7db0),(0x817c,0x81b8))
for driver,name in enumerate(('MANATEE.DRV','MASARU.DRV')):
    with (P/'analysis/arm-sound-drivers'/(name+'.arm.bin')/'instructions.tsv').open() as f:rows=list(csv.DictReader(f,delimiter='\t'))
    # MASARU's late helper region shifts by four bytes, unlike its main
    # stream routines' eight-byte shift. Use actual branch target evidence.
    spans=[(lo+driver*(8 if index<3 else 4),hi+driver*(8 if index<3 else 4)) for index,(lo,hi) in enumerate(ranges)]
    actual_ranges.append(spans)
    selected={int(r['address'],16):r for r in rows if any(lo<=int(r['address'],16)<hi for lo,hi in spans)}
    assert len(selected)==209,len(selected)
    sets.append(selected)
code=prefix+'''int sa_arm_sound_stream_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
'''+''.join(stream_emit(a,r) for a,r in sorted(sets[0].items()))+''' default:*handled=0;return 0;
 }}else{switch(r[15]){
'''+''.join(stream_emit(a,r) for a,r in sorted(sets[1].items()))+''' default:*handled=0;return 0;
 }}
}
'''
(P/'source/sa_arm_sound_stream_update.c').write_text(code)
fixture=(P/'source/sa_arm_sound_snapshot_fixture.c').read_text().replace('snapshot','stream')
(P/'source/sa_arm_sound_stream_update_fixture.c').write_text(fixture)
(P/'analysis/arm_sound_stream_update_translation.json').write_text(json.dumps({'entries':['0x70a4','0x70ac'],'ranges_by_driver':actual_ranges,'drivers':[[r for _,r in sorted(s.items())] for s in sets]},indent=2)+'\n')
print('Generated',sum(map(len,sets)),'driver-local stream-update instruction sites')
