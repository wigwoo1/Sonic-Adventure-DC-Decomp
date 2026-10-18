"""Recover the remaining high-level dispatcher, reusing existing native helpers."""
import csv,json,re
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
def dispatch_emit(a,row):
    asm=row['instruction'];m,operand=asm.split(' ',1);nextpc=f'r[15]=0x{a+4:x}u;return 0;'
    if m=='mrs':body=reg(operand.split(',')[0])+'=state->cpsr;'
    elif m=='msr':body='state->cpsr=(state->cpsr&0x00ffff00u)|('+reg(operand.split(',')[1])+'&0xff0000ffu);'
    elif m=='mvn':
        destination,source=operand.split(',');body=reg(destination)+'=~'+val(source)+';'
    elif m in ('rsb','rsbs'):
        destination,left,right=operand.split(',')
        body=reg(destination)+'='+(f'subtract(state,{val(right)},{val(left)})' if m=='rsbs' else f'{val(right)}-{val(left)}')+';'
    elif m in ('ldr','str') and (operand.endswith(']!') or '#-' in operand):
        match=re.fullmatch(r'(r\d+),\[(r\d+),#(-?0x[0-9a-f]+)\](!?)',operand);assert match,asm
        target,base,offset,writeback=match.groups();assert target!=base
        delta=int(offset,0);address=reg(base)+('+' if delta>=0 else '-')+str(abs(delta))+'u'
        body=(f'LOAD({reg(target)},{address},4);' if m=='ldr' else f'STORE({address},{reg(target)},4);')+(reg(base)+'='+address+';' if writeback else '')
    elif asm=='ldrbne r7,[r12,#-0x1f]':body='if(!(state->cpsr&0x40000000u)){LOAD(r[7],r[12]-31u,1);}'
    elif asm=='mov r7,r7, asr #0x3':body='r[7]=(r[7]>>3)|((0u-(r[7]>>31))<<29);'
    elif asm=='movs r1,r1, lsr #0xf':body='temporary=r[1];r[1]=temporary>>15;logic_flags(state,r[1],0,0);state->cpsr=(state->cpsr&~0x20000000u)|(((temporary>>14)&1u)<<29);'
    elif asm=='ldr r3,[r3,r1,lsl #0x2]':body='LOAD(r[3],r[3]+(r[1]<<2),4);'
    else:
        for suffix,predicate in {'cc':'!(state->cpsr&0x20000000u)','gt':'!(state->cpsr&0x40000000u)&&(((state->cpsr>>31)^(state->cpsr>>28))&1u)==0','lt':'(((state->cpsr>>31)^(state->cpsr>>28))&1u)!=0'}.items():
            if m.endswith(suffix):
                code=emit(a,dict(row,instruction=m[:-2]+' '+operand)).split('\n',1)[1].strip().removesuffix('return 0;')
                return f'        case 0x{a:x}u: /* {asm} */\n            if({predicate}){{{code}}}else{{r[15]=0x{a+4:x}u;}}return 0;\n'
        return emit(a,row)
    return f'        case 0x{a:x}u: /* {asm} */\n            '+body+nextpc+'\n'
reused=[set(),set()];reused_sources=['sa_arm_sound_reset_services.c','sa_arm_sound_controls.c','sa_arm_sound_stream_update.c']
for name in reused_sources:
    text=(P/'source'/name).read_text().split('if(driver==0){switch(r[15]){',1)[1]
    halves=text.split('}}else{switch(r[15]){',1);assert len(halves)==2
    for driver,half in enumerate(halves):reused[driver].update(int(a,16) for a in re.findall(r'case (0x[0-9a-f]+)u:',half))
sets=[]
for driver,name in enumerate(('MANATEE.DRV','MASARU.DRV')):
    with (P/'analysis/arm-sequencer-enrichment'/(name+'.arm.bin')/'instructions.tsv').open() as f:rows=list(csv.DictReader(f,delimiter='\t'))
    selected={int(r['address'],16):r for r in rows if (0x48e8+driver*8<=int(r['address'],16)<0x60f0+driver*8 or 0x6d84+driver*8<=int(r['address'],16)<0x8254+driver*4 or r['function_entry']==f'{0x17b0+driver*12:08x}') and int(r['address'],16) not in reused[driver]}
    sets.append(selected)
code=prefix+'''int sa_arm_sound_dispatch_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,int *handled){
 uint32_t *r=state->r,temporary;*handled=1;
 if(driver==0){switch(r[15]){
'''+''.join(dispatch_emit(a,r) for a,r in sorted(sets[0].items()))+''' default:*handled=0;return 0;
 }}else{switch(r[15]){
'''+''.join(dispatch_emit(a,r) for a,r in sorted(sets[1].items()))+''' default:*handled=0;return 0;
 }}
}
'''
(P/'source/sa_arm_sound_dispatcher.c').write_text(code)
prototype='int sa_arm_sound_dispatch_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);\n'
fixture=base if prototype in base else base.replace('static uint32_t subtract',prototype+'static uint32_t subtract',1)
fixture=fixture if 'result=sa_arm_sound_dispatch_step(' in fixture else fixture.replace('            switch(r[15]){','            result=sa_arm_sound_dispatch_step(ram,state,driver,user,read,write,&handled);\n            if(handled){if(result)return result;break;}\n            switch(r[15]){')
(P/'source/sa_arm_sound_dispatcher_fixture.c').write_text(fixture)
(P/'analysis/arm_sound_dispatcher_translation.json').write_text(json.dumps({'drivers':[[r for _,r in sorted(s.items())] for s in sets],'reused_sources':reused_sources,'reused_addresses_by_driver':[[hex(a) for a in sorted(s)] for s in reused]},indent=2)+'\n')
print('Generated',*[len(s) for s in sets],'remaining dispatcher sites per driver; existing helpers reused')
