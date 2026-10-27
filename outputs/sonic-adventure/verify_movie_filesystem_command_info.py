"""Real file information callees with declared coherent SDK/cache ABI inputs."""
import itertools
from pathlib import Path
from movie_filesystem_command_vectors import *
from verify_movie_filesystem_command_lookup import entry
ENTRY=0x8c648534
def information(data,endian,block_size=512):
    order='big' if endian else 'little';blocks=int.from_bytes(data[24:26],order)
    return ((blocks*block_size)&0xffffffff).to_bytes(4,'little')+blocks.to_bytes(2,'little')+bytes((int(data[0]==0xcc),data[1]))+int.from_bytes(data[26:28],order).to_bytes(2,'little')+timestamp(data[16:24])
def main():
    assert v.effects.DLL.name=='movie_filesystem_command.dll'
    e=Evidence();ram=v.seed_ram();kinds=('inactive','absent','function_off','busy','disabled','bad_root','latched','missing','good33','goodcc','large_size')
    for channel,endian in itertools.product((0,7),(0,1)):
        base=Session(ram,channel=channel,endian=endian)
        for kind,(mode,sr) in itertools.product(kinds,PROFILES):
            m,s,f,c=fresh(base,ENTRY,[channel,B,OUT],mode,sr);descriptor=base.descriptor;root=v.load(m,descriptor+0x20)
            raw(m,B,b'SONIC\0');raw(m,OUT,b'\xa5'*32);put(m,descriptor+2,int(kind!='inactive'),1);put(m,descriptor+8,int(kind!='disabled'),1);put(m,descriptor+0xb,0xa5,1);put(m,descriptor+0xc,0x5a,1)
            present=kind!='absent';put(m,base.d['header'],int(present),1);put(m,base.d['status'],int(present),1);put(m,base.d['status']+0xc,int(kind=='busy'),1);put(m,base.d['info'],0 if kind=='function_off' else 2)
            put(m,0x8c89cf90+channel*4,int(kind=='latched'));put(m,base.output+0x30,2);put(m,base.output+0x2c,0xffffffff if kind=='large_size' else 511);put(m,descriptor+0x28,A)
            raw(m,root,b'U'*16+bytes(496))
            if kind=='bad_root':put(m,root+15,0,1)
            data=bytearray(entry(0x33 if kind=='good33' else 0xcc,b'OTHER' if kind=='missing' else b'SONIC',255,endian));data[1]=0xa5;data[16:24]=bytes((0x20,0x26,0x10,0x27,0x23,0x59,0x58,7));data[24:26]=(0xffff if kind=='large_size' else 2).to_bytes(2,'big' if endian else 'little');data[26:28]=(0x8001).to_bytes(2,'big' if endian else 'little');raw(m,A,data+entry(0,b'',0,endian))
            unavailable=kind in ('inactive','absent','function_off','busy','disabled')
            if unavailable:put(m,descriptor+0x20,0xdeadbeef)
            expected_result=0xffffff01 if unavailable else 0xffffff03 if kind in ('bad_root','latched') else 0xffffff05 if kind=='missing' else 0
            expected=bytearray(m);sp=s.r[15];regs=list(s.r);pr=s.pr
            if expected_result==0:raw(expected,OUT,information(data,endian,0 if kind=='large_size' else 512))
            if kind in ('absent','function_off'):
                for offset in (8,9,10,11,12):put(expected,descriptor+offset,0,1)
                put(expected,descriptor+4,0)
            if kind=='bad_root':put(expected,descriptor+0xb,0,1);put(expected,descriptor+0xc,0,1)
            r,a,s,f=e.compare(m,s,f,c,dict(channel=channel,endian=endian,kind=kind,mode=mode))
            # Original nested info+finder+name/entry frames reach108bytes.
            raw(expected,sp-108,v.sample(a,sp-108,108));e.require(a==expected,'independent all nonstackRAM/output/padding/cache effects')
            e.require(s.r[0]==expected_result,'independent info error/result mapping')
            e.require([v.load(a,sp-o) for o in (4,8,12,60,64,68)]==[regs[14],regs[13],pr,328*channel,B,descriptor],'exact own save/argument frame')
            targets=[b['target'] for b in r['boundaries']]
            e.require(('0x8c64ad56' in targets)==(not unavailable and kind not in ('bad_root','latched')),'lookup only after real readiness/root check')
            e.require(('0x8c64b472' in targets)==(expected_result==0),'date conversion only after real match')
            e.require((s.sr&~1)==(sr&~1) and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'],'unaffected status/special registers')
        print('FILE INFO',channel,endian,len(e.records),flush=True)
    e.report('movie_filesystem_command_info_verification.json',{ENTRY},[Path(__file__),P/'verify_movie_filesystem_command_lookup.py'],scope='Declared coherent SDK/cache helper ABI inputs:original initializer/selector andreal availability/busy/root/cleanup/finder/decoder/date callees. No callee result supplied. Twochannels/endian/fourprofiles;inactive/absent/functionoff/busy/cacheoff/badsignature/latch/missing/type33/typeCC/uint32 sizewrap. Independent fullnonstackRAM/output/padding/cache effects and108byte maxoriginal nestedstack;exactownframe,errors/selection/status/FPU. Positive natural task acquisition checked separately;physical hardware/fullfilesystem/game unfinished.')
if __name__=='__main__':main()
