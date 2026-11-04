"""Model-producer instruction service built on the frozen primitive executor.
Explicit GBR loads, SETT/CLRT, signed full products and supervisor bank switching. No game
dependency hooks, cache timing, DMA scheduling or GPU behavior is supplied.
"""
import hashlib,inspect,linecache,types
from primitive_reference import make_reference as primitive_reference
def make_reference(executor):
    original=inspect.getsource(executor.reference);source=original
    old='        elif word & 0xF0FF == 0x400E: sr = regs[n]'
    new='''        elif word & 0xF0FF == 0x400E:
            target=regs[n]
            if (target^sr)&0x20000000:
                assert special is not None and target&sr&0x40000000, 'model bank switch requires explicit supervisor bank state'
                other=special['r_bank'];assert len(other)==8
                previous=regs[:8];regs[:8]=other;special['r_bank']=previous
            sr=target'''
    assert source.count(old)==1;source=source.replace(old,new)
    anchor='        elif word == 9: pass'
    assert source.count(anchor)==1
    source=source.replace(anchor,'''        elif word & 0xF00F == 0x300D: # DMULS.L
            assert special is not None, 'signed full product requires MACH state'
            product=signed(regs[n],32)*signed(regs[m],32)
            macl=product&MASK;special['mach']=(product>>32)&MASK
        elif word == 0x0018: sr |= 1 # SETT
        elif word == 0x0008: sr &= ~1 # CLRT
        elif word & 0xFF00 == 0xC600:
            assert special is not None, 'GBR load requires explicit register state'
            regs[0]=read32((special['gbr']+4*(word&255))&MASK)
'''+anchor)
    filename='<model-supervisor-register-reference>'
    linecache.cache[filename]=(len(source),None,source.splitlines(keepends=True),filename)
    namespace=dict(vars(executor));exec(compile(source,filename,'exec'),namespace)
    proxy=types.SimpleNamespace(**dict(vars(executor),reference=namespace['reference']))
    reference,hashes=primitive_reference(proxy)
    hashes.update(frozen_original_reference_sha256=hashlib.sha256(original.encode()).hexdigest(),model_register_extension_sha256=hashlib.sha256(source.encode()).hexdigest())
    return reference,hashes

def verify_register_extension(executor,reference):
    import struct
    base=executor.BASE;ram=bytearray(16<<20);code=base+0xd00000;gbr=base+0xd01000;checks=0
    def run(ops,sr,r8=0,r9=0):
        struct.pack_into('<'+'H'*len(ops),ram,code-base,*ops)
        s=executor.State();s.pc=code;s.pr=code+len(ops)*2;s.sr=sr;s.r[:]=[0x100+i for i in range(16)];s.r[8]=r8;s.r[9]=r9
        special={'gbr':gbr,'r_bank':[0x200+i for i in range(8)]}
        reference(ram,s,special=special);return s,special
    for bank in range(2):
        sr=0x40000000|(bank<<29)
        values=[0,1,0x7fff,0xffff,0x7fffffff,0x80000000,0xffff0000,0xffffffff]
        for a in values:
            for b in values:
                s,sp=run([0x389d],sr,a,b)
                aa=a-(1<<32) if a>>31 else a;bb=b-(1<<32) if b>>31 else b
                product=aa*bb
                assert sp['macl']==product&0xffffffff and sp['mach']==(product>>32)&0xffffffff and s.r[8]==a and s.r[9]==b and s.sr==sr
                checks+=1
        for displacement in range(256):
            value=(0x98765432^displacement*0x1020304)&0xffffffff
            struct.pack_into('<I',ram,gbr-base+4*displacement,value)
            s,sp=run([0xc600|displacement],sr)
            assert s.r[0]==value and list(s.r)[1:8]==[0x100+i for i in range(1,8)] and s.sr==sr
            checks+=1
        for t in [0,1]:
            s,sp=run([0x0018],sr|t);assert s.sr==sr|1 and list(s.r)[:8]==list(range(0x100,0x108));checks+=1
            s,sp=run([0x0008],sr|t);assert s.sr==sr and list(s.r)[:8]==list(range(0x100,0x108));checks+=1
        target=sr^0x20000000
        s,sp=run([0x480e],sr,target)
        assert s.sr==target and list(s.r)[:8]==list(range(0x200,0x208)) and sp['r_bank']==list(range(0x100,0x108));checks+=1
        s,sp=run([0x480e,0x490e],sr,target,sr)
        assert s.sr==sr and list(s.r)[:8]==list(range(0x100,0x108)) and sp['r_bank']==list(range(0x200,0x208));checks+=1
    return checks
