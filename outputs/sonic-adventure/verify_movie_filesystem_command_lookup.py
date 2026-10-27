"""Independent directory search from raw entries and caller name bytes."""
import itertools
from pathlib import Path
from movie_filesystem_command_vectors import *
ENTRY=0x8c64ad56
def entry(kind,label,seed=0,endian=0):
    data=bytearray((seed+j*31)&255 for j in range(32));data[0]=kind;data[4:16]=(label+bytes(12))[:12]
    return bytes(data)
def configurations(endian):
    make=lambda kind,label,seed=0:entry(kind,label,seed,endian)
    return [dict(count=0,entries=[],query=b'A'),dict(count=-1,entries=[],query=b'A'),
        dict(entries=[make(0x33,b'SONIC')],query=b'SONIC'),dict(entries=[make(0xcc,b'SONIC')],query=b'SONIC'),
        dict(entries=[make(0,b'SONIC'),make(5,b'SONIC'),make(0x33,b'OTHER'),make(0xcc,b'SONIC',255)],query=b'SONIC'),
        dict(entries=[make(0x33,b'OTHER'),make(0xcc,b'ANOTHER')],query=b'MISSING'),
        dict(entries=[make(0x33,b'ABCDEFGHIJKL')],query=b'ABCDEFGHIJKLMORE'),
        dict(entries=[make(0x33,b'A\0GARBAGE')],query=b'A\0DIFFERENT'),
        dict(entries=[make(0x33,b'')],query=b''),dict(entries=[make(0x33,b'\xff\x80')],query=b'\xff\x80'),
        dict(entries=[make(0xcc,b'ABX'),make(0xcc,b'ABC')],query=b'ABC')]
def main():
    assert v.effects.DLL.name=='movie_filesystem_command.dll'
    e=Evidence();ram=v.seed_ram()
    for channel,endian in itertools.product((0,7),(0,1)):
        base=Session(ram,channel=channel,endian=endian)
        for config,(mode,sr) in itertools.product(configurations(endian),PROFILES):
            count=config.get('count',len(config['entries']));m,s,f,c=fresh(base,ENTRY,[channel,B,OUT],mode,sr)
            put(m,base.output+0x30,count);put(m,base.descriptor+0x28,A if count>0 else 0xdeadbeef)
            raw(m,A,b''.join(config['entries']));raw(m,B,config['query']+b'\0');raw(m,OUT,b'\xa5'*64);before=bytes(m);sp=s.r[15]
            index=next((i for i,d in enumerate(config['entries'][:max(count,0)]) if d[0] in (0x33,0xcc) and name(d[4:16])==name(config['query'])),None)
            wanted=decoded(v.sample(m,OUT,64),config['entries'][index],endian) if index is not None else bytearray(v.sample(m,OUT,64))
            regs=list(s.r);pr=s.pr
            r,a,s,f=e.compare(m,s,f,c,dict(channel=channel,endian=endian,count=count,types=[d[0] for d in config['entries']],query=config['query'].hex(),mode=mode))
            e.require(s.r[0]==(0xffffffff if index is None else index),'independent first matching index/notfound')
            e.require(v.sample(a,OUT,64)==wanted,'independent decoded entry/output gaps')
            expected=bytearray(before);raw(expected,OUT,wanted);raw(expected,sp-40,v.sample(a,sp-40,40));e.require(a==expected,'only output and maximum original nested stack change')
            e.require([v.load(a,sp-o) for o in (4,8,12,16,20,24)]==[regs[14],regs[13],regs[12],pr,B,OUT],'exact own frame save/argument cells')
            e.require(r['final_macl']==328*channel and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'],'finder special registers')
        print('FILE LOOKUP',channel,endian,len(e.records),flush=True)
    e.report('movie_filesystem_command_lookup_verification.json',{ENTRY},[Path(__file__)],scope='Declared coherent helper inputs:original initializer/selector own SDK callback pointers;rawdirectory bytes,caller name andsigneddirectory count supplied asABI/cacheinputs. Independent first-match index,rawentrydecode/untouchedgaps,exactownframe andallmemoryoutside maxnestedstack;all machine state/orderedcalls compareoriginal. Types33/CC/zero/unknown,firstlatefound/missing,empty/highbyte/12byte names,zero/negativecounts,twochannels/endian/fourprofiles. Not natural hardware/cache acquisition;original task proof separate.')
if __name__=='__main__':main()
