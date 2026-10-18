import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent;stem='sound_linear_source_query';start=0x8c65e1d0;end=0x8c65e242
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_linear_source_65e000.tsv'],check=True)
source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+'/* Retail linear-source reservation query 8c65e1d0..8c65e242. */'+s[z:])
(p/f'build_{stem}.ps1').write_text((p/'build_sound_linear_source_capacity.ps1').read_text().replace('sound_linear_source_capacity',stem))
s=(p/'verify_sound_source_query.py').read_text().replace('sound_source_query',stem).replace('0x8c65e4b8',hex(start))
s=s.replace('[0,12,80]','[0,12,80,0x80000000,0xffffffff]').replace('[0,7,100]','[0,7,100,0x80000000,0xffffffff]').replace('0x8c010100','0x8c65e16e')
a=s.index("    struct.pack_into('<8I'");z=s.index('    before=',a)
s=s[:a]+"    struct.pack_into('<6I',ram,0xa4000c,available,position,0x8ca10000,80,callback,obj)\n"+s[z:]
a=s.index('    def service(');z=s.index('    s=State()',a)
s=s[:a]+'''    dependencies={}
    for address,dep in [(0x8c65e08c,'sound_linear_source_lock_enter'),(0x8c65e0a4,'sound_linear_source_lock_exit'),(0x8c65e16e,'sound_linear_source_identity_get')]:
        fun=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dep}.dll')),'sa_'+dep);fun.argtypes=fn.argtypes;dependencies[address]=fun
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        assert address in dependencies,hex(address)
        actual.append(address)
        if address==0x8c65e16e:assert (r[4],r[5])==(obj,0xfffffffd)
        return dependencies[address](mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
'''+s[z:]
a=s.index('    reference(');z=s.index('    wanted=',a)
s=s[:a]+'''    reference(ram,s,{},fpu=f,special={'macl':0})
    assert fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
'''+s[z:]
a=s.index('    if mode in [0,1]:');z=s.index('    cases+=1',a)
s=s[:a]+'''    if mode==1:
        signed=lambda v:ctypes.c_int32(v).value
        count=available if signed(available)<signed(limit) else limit;pointer=(0x8ca10000+position)&0xffffffff
        struct.pack_into('<2I',wanted,0xc,(available-count)&0xffffffff,(position+count)&0xffffffff)
    else:count=pointer=0
    assert native[0xa40000:0xa40040]==wanted
    assert native[0xa20000:0xa20010]==bytes([0x66])*4+struct.pack('<2I',pointer,count)+bytes([0x66])*4
    assert regs[15]==0x8cf00000 and (sr.value&0xf0)==0x60
    assert actual==[0x8c65e08c]+([callback] if mode not in [0,1] and callback else [])+[0x8c65e0a4]
'''+s[z:]
a=s.index("(out/'analysis/")
s=s[:a]+'''scope='800 full retail RAM/CPU/FPU/SR/PR comparisons of linear-source query65e1d0..65e242. Modes0/1 and invalid2/ffffffff,five available/request patterns including signed extremes,two positions,optional controlled native callback,both FPU banks. Native interrupt lock enter/exit and identity-getter callback; no modeled calls. Independent complete64-byte source window and descriptor guards: mode1 signed minimum request/capacity,base+offset pointer,offset advances and capacity decreases modulo32;other modes empty descriptor,invalid optionally callback(userdata,-3). Original interrupt mask restored. Pool construction,connected reservation return and game callers unverified.'
(out/'analysis/sound_linear_source_query_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\\n')
print(cases,'linear source query comparisons passed')
'''
(p/f'verify_{stem}.py').write_text(s)
reg=(p/'register_sound_linear_source_capacity.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a)
reg=reg[:a]+'for stem,start,end in '+repr([(stem,start,end)])+':'+reg[z:];reg=reg.replace("==280","==800")
(p/f'register_{stem}.py').write_text(reg)
