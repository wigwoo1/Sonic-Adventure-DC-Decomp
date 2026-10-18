"""Static recovery of sequencer code, indirect handlers and direct dependencies."""
import csv,json,re,struct
from pathlib import Path
from create_arm_sound_controls import emit,reg,val
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
def sequence_emit(a,row):
    asm=row['instruction'];m,operand=asm.split(' ',1)
    if m=='mlas':
        names=[reg(p.strip()) for p in operand.split(',')]
        body=f'{names[0]}={names[1]}*{names[2]}+{names[3]};logic_flags(state,{names[0]},0,0);'
        return f'        case 0x{a:x}u: /* {asm}; C follows TI925T reference, not ARMv4 hardware proof */\n            {body}r[15]=0x{a+4:x}u;return 0;\n'
    if m=='mvnvs':
        destination,source=[p.strip() for p in operand.split(',')]
        return f'        case 0x{a:x}u: /* {asm} */\n            if(state->cpsr&0x10000000u){reg(destination)}=~{val(source)};r[15]=0x{a+4:x}u;return 0;\n'
    match=re.fullmatch(r'(r\d+),\[(r\d+),(r\d+),lsl #0x2\]',operand)
    if m=='ldr' and match:
        destination,base,index=match.groups()
        return f'        case 0x{a:x}u: /* {asm} */\n            LOAD({reg(destination)},{reg(base)}+({reg(index)}<<2),4);r[15]=0x{a+4:x}u;return 0;\n'
    predicates={'cc':'!(state->cpsr&0x20000000u)','le':'(state->cpsr&0x40000000u)||(((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0'}
    for suffix,predicate in predicates.items():
        if m.endswith(suffix):
            body=emit(a,dict(row,instruction=m[:-2]+' '+operand)).split('\n',1)[1].strip().removesuffix('return 0;')
            return f'        case 0x{a:x}u: /* {asm} */\n            if({predicate}){{{body}}}else{{r[15]=0x{a+4:x}u;}}return 0;\n'
    return emit(a,row)
sets=[];tables=[]
payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
for driver,name in enumerate(('MANATEE.DRV','MASARU.DRV')):
    with (P/'analysis/arm-sequencer-enrichment'/(name+'.arm.bin')/'instructions.tsv').open() as f:rows=list(csv.DictReader(f,delimiter='\t'))
    selected={int(r['address'],16):r for r in rows if 0x4210+driver*8<=int(r['address'],16)<0x48c4+driver*8 or 0x63dc+driver*8<=int(r['address'],16)<0x63f8+driver*8}
    image=(P.parents[1]/payloads[driver]['payload_path']).read_bytes();metadata=[]
    for literal,count in ((0x48d8+driver*8,7),(0x48dc+driver*8,16)):
        pointer=struct.unpack_from('<I',image,literal)[0];targets=list(struct.unpack_from('<'+'I'*count,image,pointer))
        assert set(targets)<=set(selected)
        assert not any(pointer<=a<pointer+count*4 for a in selected)
        metadata.append({'literal':literal,'pointer':pointer,'count':count,'bytes':image[pointer:pointer+count*4].hex(),'targets':targets})
    sets.append(selected);tables.append(metadata)
code=prefix+'''int sa_arm_sound_sequence_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
'''+''.join(sequence_emit(a,r) for a,r in sorted(sets[0].items()))+''' default:*handled=0;return 0;
 }}else{switch(r[15]){
'''+''.join(sequence_emit(a,r) for a,r in sorted(sets[1].items()))+''' default:*handled=0;return 0;
 }}
}
'''
(P/'source/sa_arm_sound_sequencer.c').write_text(code)
(P/'source/sa_arm_sound_sequencer_fixture.c').write_text((P/'source/sa_arm_sound_snapshot_fixture.c').read_text().replace('snapshot','sequence'))
(P/'analysis/arm_sound_sequencer_translation.json').write_text(json.dumps({'drivers':[[r for _,r in sorted(s.items())] for s in sets],'tables_by_driver':tables,
 'mla_carry_limit':'Native preserves C like the TI925T validation model; ARMv4 MLAS C is architecturally unpredictable. V is preserved.'},indent=2)+'\n')
print('Generated',sum(map(len,sets)),'driver-local sequencer instruction sites')
