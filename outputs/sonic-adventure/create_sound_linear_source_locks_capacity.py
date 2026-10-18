import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
specs=[('sound_linear_source_lock_enter',0x8c65e08c,0x8c65e0a4),('sound_linear_source_lock_exit',0x8c65e0a4,0x8c65e0bc),('sound_linear_source_capacity',0x8c65e182,0x8c65e1b2)]
for stem,start,end in specs:
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_linear_source_65e000.tsv'],check=True)
 source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3
 source.write_text(s[:a]+f'/* Retail linear-source routine {start:x}..{end:x}. */'+s[z:])
 (p/f'build_{stem}.ps1').write_text((p/'build_sound_source_lock_enter.ps1').read_text().replace('sound_source_lock_enter',stem))
s=(p/'verify_sound_source_locks.py').read_text().replace('sound_source_lock_','sound_linear_source_lock_').replace('sound_source_locks','sound_linear_source_locks')
s=s.replace('0x8c65e368','0x8c65e08c').replace('0x8c65e380','0x8c65e0a4').replace('enter65e368..65e380 and exit65e380..65e398','enter65e08c..65e0a4 and exit65e0a4..65e0bc')
(p/'verify_sound_linear_source_locks.py').write_text(s)
s=(p/'register_sound_source_locks.py').read_text().replace('sound_source_lock_','sound_linear_source_lock_').replace('sound_source_locks','sound_linear_source_locks').replace('0x8c65e368','0x8c65e08c').replace('0x8c65e380','0x8c65e0a4').replace('0x8c65e398','0x8c65e0bc').replace('Sound source interrupt mask helpers:', 'Linear source interrupt mask helpers:')
(p/'register_sound_linear_source_locks.py').write_text(s)
s=(p/'verify_sound_source_capacity.py').read_text().replace('sound_source_capacity','sound_linear_source_capacity').replace('0x8c65e462','0x8c65e182')
s=s.replace('0x8c010300','0x8c65e16e').replace('0xa20028','0xa2001c').replace('0x12345678','0x8ca20000')
a=s.index('    def service(');z=s.index('    s=State()',a)
cb='''    getter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_linear_source_identity_get.dll')).sa_sound_linear_source_identity_get;getter.argtypes=fn.argtypes
    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        assert address==0x8c65e16e and (r[4],r[5])==(0x8ca20000,0xfffffffd)
        actual.append((r[4],r[5]))
        return getter(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
'''
s=s[:a]+cb+s[z:]
s=s.replace('reference(ram,s,{0x8c65e16e:hook}', 'reference(ram,s,{}').replace(' and actual==expected','')
s=s.replace('count^0xabcdef01 if mode==0 else 0','0')
a=s.index("scope='");z=s.index("'),indent=2)",a)
scope='280 full retail RAM/CPU/FPU/SR/PR comparisons of linear-source capacity query65e182..65e1b2. Modes0/1 and invalid2/ffffffff/128,seven full-word capacity patterns,optional controlled native identity-getter callback,both banks,two stacks. Independent mode1 returns remaining+c,mode0 returns0,invalid returns0 and optionally callback(userdata,-3);all64 object-plus-guard bytes preserved. Callback executes natively,no modeled calls;retail default handler and connected reservation/fullgame unverified.'
s=s[:a]+'scope='+repr(scope)+s[z+1:];(p/'verify_sound_linear_source_capacity.py').write_text(s)
reg=(p/'register_sound_work_accessors.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a)
reg=reg[:a]+"for stem,start,end in [('sound_linear_source_capacity',0x8c65e182,0x8c65e1b2)]:"+reg[z:]
reg=reg.replace("evidence['cases_passed']==140","evidence['cases_passed']==280")
(p/'register_sound_linear_source_capacity.py').write_text(reg)
