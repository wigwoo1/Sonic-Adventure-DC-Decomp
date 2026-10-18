from pathlib import Path
p=Path(__file__).resolve().parent;name='sound_stream_work_update'
path=p/f'source/sa_{name}.c';s=path.read_text().replace('if((*sr&1u))goto L8c65bb4e;','if((*sr&1u))return call(ctx,0x8c65bb4eu,m,f,sr,r,pr);')
a=s.index('/* Static C recovery');z=s.index('SA_STAGE_EXPORT',a);path.write_text(s[:a]+'/* Retail public audio work update65bb88..65bb9c. State2 tail-dispatches tick; state1 initialization. */\n'+s[z:])
(p/f'build_{name}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',name))
r=(p/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x8c65bb88').replace('0x8c65e79c','0x8c65bb9c').replace('0x64e798','0x64bb88').replace('0x64e79c','0x64bb9c').replace('==140','==140')
(p/f'register_{name}.py').write_text(r)
s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x8c65bb88')
s=s.replace("struct.pack_into('<I',ram,0xa2001c,count)","ram[0xa20001]=count&255")
s=s.replace("    def call(*args):raise AssertionError('Unexpected service')",'''    actual=[];expected=[]
    def service(address,r,trace):
        assert address==(0x8c65bb4e if count&255==2 else 0x8c65b7a8)
        assert r[4]==0x8ca20000 and r[15]==0x8cf00000
        trace.append(address);r[0]=0x12345678
    def call(ctx,address,mem,f,sr,r,pr):service(address,r,actual);return 0
    def hook(address):
        def run(mem,r,sr):service(address,r,expected);return sr
        return run''')
s=s.replace('reference(ram,s,fpu=f','reference(ram,s,{a:hook(a) for a in [0x8c65bb4e,0x8c65b7a8]},fpu=f')
s=s.replace('assert regs[0]==count',"assert actual==expected==([0x8c65b7a8] if count&255==1 else [0x8c65bb4e] if count&255==2 else [])\n    assert regs[0]==(0x12345678 if count&255 in [1,2] else ctypes.c_int8(count&255).value&0xffffffff)")
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='140 full retail RAM/CPU/FPU/SR/PR comparisons of public work update65bb88..65bb9c. Seven state bytes0/1/2/127/128/255,five neighboring phase fixtures,banks,varied stack. Independent signed-byte load, state1 initialization/state2 tick tail calls, other states no call and all64 bytes unchanged. Subordinate initialization/tick modeled; fullgame unverified."+s[z:]
s=s.replace('    @Call\n    actual=[]','    actual=[]').replace('    def call(ctx,','    @Call\n    def call(ctx,')
(p/f'verify_{name}.py').write_text(s)
