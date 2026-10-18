"""Generate static C for original type-3 control paths; no runtime opcode interpreter."""
import csv,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
def reg(n):return {'sp':'r[13]','lr':'r[14]','pc':'r[15]'}.get(n,'r['+n[1:]+']')
def val(x):return str(int(x[1:],0))+'u' if x.startswith('#') else reg(x)
def addr(x):return str(int(x,0))+'u' if x.startswith('0x') else reg(x)
def expression(parts):
    v=val(parts[0])
    if len(parts)>1:
        kind,amount=parts[1].split(' #');amount=int(amount,0)
        if kind=='ror':
            assert 0<amount<32
            v='(('+v+'>>'+str(amount)+')|('+v+'<<'+str(32-amount)+'))'
        else:
            assert kind in ('lsl','lsr')
            v='('+v+('<<' if kind=='lsl' else '>>')+str(amount)+')'
    return v
def emit(a,row):
    asm=row['instruction'];m,op=asm.split(' ',1);parts=[x.strip() for x in op.split(',')]
    nextpc=f'r[15]=0x{a+4:x}u;';condition=None
    for suffix,predicate in [('eq','state->cpsr&0x40000000u'),('ne','!(state->cpsr&0x40000000u)'),('mi','state->cpsr&0x80000000u'),('pl','!(state->cpsr&0x80000000u)'),('cs','state->cpsr&0x20000000u'),('hi','(state->cpsr&0x20000000u)&&!(state->cpsr&0x40000000u)')]:
        if m.endswith(suffix):condition=predicate;m=m[:-2];break
    if m in ('b','bl'):
        body=f'r[15]={int(parts[0],0)}u;'
        if m=='bl':body=f'r[14]=0x{a+4:x}u;'+body
        if condition:body='if('+condition+'){'+body+'}else{'+nextpc+'}'
    elif m in ('stmdb','ldmia'):
        assert op.startswith('sp!,{') and op.endswith('}')
        names=op[5:-1].split(',');length=4*len(names)
        if m=='stmdb':body=''.join('STORE(r[13]-'+str(length-i*4)+'u,'+reg(n.strip())+',4);' for i,n in enumerate(names))+f'r[13]-={length}u;'
        else:body=''.join('LOAD('+reg(n.strip())+',r[13]+'+str(i*4)+'u,4);' for i,n in enumerate(names))+f'r[13]+={length}u;'
        if condition:body='if('+condition+'){'+body+'}'
        body+=nextpc
    elif m in ('ldr','ldrb','str','strb'):
        match=re.fullmatch(r'(r\d+),\[(0x[0-9a-f]+|r\d+)(?:,(#0x[0-9a-f]+|r\d+))?\](?:,(#0x[0-9a-f]+|r\d+))?',op)
        assert match,(hex(a),asm)
        n,base,offset,post=match.groups();address=addr(base)+(('+'+val(offset)) if offset else '')
        size=1 if m.endswith('b') else 4
        body=('LOAD('+reg(n)+','+address+','+str(size)+');') if m.startswith('ldr') else ('STORE('+address+','+reg(n)+','+str(size)+');')
        if post:body+=reg(base)+'+='+val(post)+';'
        if condition:body='if('+condition+'){'+body+'}'
        body+=nextpc
    elif m in ('mov','movs'):
        body=reg(parts[0])+'='+expression(parts[1:])+';'
        if m=='movs':assert len(parts)==2;body+='logic_flags(state,'+reg(parts[0])+',0,0);'
        if condition:body='if('+condition+'){'+body+'}'
        if parts[0]!='pc':body+=nextpc
        elif condition:body+='else{'+nextpc+'}'
    elif m=='adr':body=reg(parts[0])+'='+addr(parts[1])+';'+nextpc
    elif m in ('add','adds','sub','subs','and','ands','orr','bic','eor','cmp','cmn','tst'):
        if m in ('cmp','cmn','tst'):left,right=parts[0],parts[1:];dest=None
        else:dest=reg(parts[0]);left,right=parts[1],parts[2:]
        if m in ('cmp','cmn','subs','adds'):
            fn='addition' if m in ('adds','cmn') else 'subtract'
            body=(dest+'=' if dest else '(void)')+fn+'(state,'+val(left)+','+expression(right)+');'
        elif m in ('ands','tst'):
            body='temporary='+val(left)+'&'+expression(right)+';'
            if dest:body+=dest+'=temporary;'
            word=int.from_bytes(bytes.fromhex(row['bytes']),'little');rotated=bool(word&(1<<25) and word&0xf00)
            body+='logic_flags(state,temporary,'+('1' if rotated else '0')+','+expression(right)+');'
        else:
            operator={'add':'+','sub':'-','and':'&','orr':'|','bic':'&~','eor':'^'}[m]
            body=dest+'='+val(left)+operator+expression(right)+';'
        if condition:body='if('+condition+'){'+body+'}'
        body+=nextpc
    elif m in ('mul','mla'):body=reg(parts[0])+'='+val(parts[1])+'*'+val(parts[2])+('+'+val(parts[3]) if m=='mla' else '')+';'+nextpc
    else:raise AssertionError((hex(a),asm))
    return f'        case 0x{a:x}u: /* {asm} */\n            '+body+'return 0;\n'
def main():
    sets=[];metadata=[]
    for driver,name in enumerate(('MANATEE.DRV','MASARU.DRV')):
        with (P/'analysis/arm-sound-drivers'/(name+'.arm.bin')/'instructions.tsv').open() as stream:rows=list(csv.DictReader(stream,delimiter='\t'))
        entry=0x1818 if driver==0 else 0x1820;dsp=0x5c20 if driver==0 else 0x5c28
        selected={int(r['address'],16):r for r in rows if (int(r['address'],16)>=entry and r['function_entry']=='000009d0') or r['function_entry']==f'{dsp:08x}'}
        # Ghidra classified this computed-jump branch table as data. The original
        # dispatcher executes these nine ARM B words; retain exact source bytes.
        payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
        image=(P.parents[1]/payloads[driver]['payload_path']).read_bytes();derived=[]
        for address in range(0x2240+driver*8,0x2264+driver*8,4):
            raw=image[address:address+4];word=int.from_bytes(raw,'little');assert word>>24==0xea
            immediate=word&0xffffff
            if immediate&0x800000:immediate-=1<<24
            target=address+8+4*immediate
            assert any(int(r['address'],16)==target for r in rows)
            row={'address':f'{address:08x}','length':'4','bytes':raw.hex(),'instruction':f'b 0x{target:08x}','flows':f'{target:08x}','function_entry':'000009d0'}
            selected[address]=row;derived.append(row)
        sets.append(selected);metadata.append({'control_entry':hex(entry),'dsp_helper':hex(dsp),'instruction_addresses':[hex(a) for a in sorted(selected)],'derived_branch_table':derived})
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
    code=prefix+'''int sa_arm_sound_control_step(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
     void *user,sa_arm_read read,sa_arm_write write,int *handled){
     uint32_t *r=state->r,temporary;*handled=1;
     if(driver==0){switch(r[15]){
    '''+''.join(emit(a,r) for a,r in sorted(sets[0].items()))+'''        default:*handled=0;return 0;
     }}else{switch(r[15]){
    '''+''.join(emit(a,r) for a,r in sorted(sets[1].items()))+'''        default:*handled=0;return 0;
     }}
    }
    '''
    (P/'source/sa_arm_sound_controls.c').write_text(code)
    prototype='int sa_arm_sound_control_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);\n'
    if prototype not in base:base=base.replace('static uint32_t subtract',prototype+'static uint32_t subtract',1)
    if 'result=sa_arm_sound_control_step' not in base:
        base=base.replace('            switch(r[15]){','            result=sa_arm_sound_control_step(ram,state,driver,user,read,write,&handled);\n            if(handled){if(result)return result;break;}\n            switch(r[15]){')
    root.write_text(base)
    (P/'analysis/arm_sound_controls_translation.json').write_text(json.dumps({'drivers':metadata},indent=2)+'\n')
    print('Generated',sum(len(s) for s in sets),'driver-local static control/DSP instruction sites')

if __name__=='__main__':main()
