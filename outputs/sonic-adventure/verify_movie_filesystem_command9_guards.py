"""Original relocation and type-CC name-query guard oracles."""
import itertools
from pathlib import Path
from movie_filesystem_command9_vectors import *
MASTER,NAME=0x8c64a828,0x8c64a3fa
FAT=0x8cdc0000;DIR=0x8cdd0000

def main():
    e=Evidence();ram=v.seed_ram();cases=[(entry,kind) for entry in (MASTER,NAME) for kind in ('inactive','absent','function_off','busy','cache_off','bad_root','latched_root')]+[(MASTER,k) for k in ('feature_zero','has_CC','bad_link')]
    for channel in (0,7):
        base=Session(ram,channel=channel)
        for (entry,kind),(mode,sr) in itertools.product(cases,PROFILES):
            m,s,f,c=fresh(base,entry,[channel,OUT],mode,sr);desc=base.descriptor
            put(m,desc+2,int(kind!='inactive'),1);put(m,desc+8,int(kind!='cache_off'),1)
            for off,val in ((9,0x55),(10,0xaa),(11,0x33),(12,0x77)):put(m,desc+off,val,1)
            put(m,base.d['header'],int(kind!='absent'),1);put(m,base.d['status'],int(kind!='absent'),1);put(m,base.d['info'],0 if kind=='function_off' else 2);put(m,base.d['status']+0xc,int(kind=='busy'),1)
            put(m,0x8c89cf90+channel*4,int(kind=='latched_root'));root=bytearray(512);root[:16]=b'U'*16
            if kind=='bad_root':root[5]=0
            raw(m,v.load(m,desc+0x20),root);put(m,desc+0x24,FAT);put(m,desc+0x28,DIR);raw(m,FAT,(0xfffc).to_bytes(2,'little')*32);raw(m,DIR,bytes(32*8));put(m,base.output,31);put(m,base.output+0x30,8);put(m,base.output+0x38,0 if kind=='feature_zero' else 128)
            if kind=='has_CC':raw(m,DIR,bytes([0xcc])+bytes(3)+b'PROGRAM'+bytes(5)+bytes(16))
            if kind=='bad_link':raw(m,FAT,(8).to_bytes(2,'little'))
            raw(m,OUT,bytes([0xa5])*512);before=bytes(m);sp=s.r[15];wanted=bytearray(before)
            if kind in ('absent','function_off'):
                for off in (8,9,10,11,12):put(wanted,desc+off,0,1)
                put(wanted,desc+4,0)
            if kind=='bad_root':put(wanted,desc+11,0,1);put(wanted,desc+12,0,1)
            expected=-253 if kind in ('bad_root','latched_root') else -245 if kind in ('feature_zero','has_CC','bad_link') else -255
            r,a,s,f=e.compare(m,s,f,c,dict(entry=hex(entry),kind=kind,channel=channel,mode=mode,sr=hex(sr)))
            raw(wanted,sp-320,v.sample(a,sp-320,320));e.require(a==wanted,'exact nonstack RAM/cache and bounded original nested scratch')
            e.require(s.r[0]==expected&0xffffffff,'independent original guard result')
            family=[b['target'] for b in r['boundaries'] if b['target'] in ('0x8c64a828','0x8c64a3fa','0x8c64a7ba')]
            wanted_calls=[hex(entry)]+(['0x8c64a3fa'] if kind in ('has_CC','bad_link') else [])+(['0x8c64a7ba'] if kind=='bad_link' else [])
            e.require(family==wanted_calls,'independent gate ordering')
            e.require(not r['cache_purge_sequence'] and not any('write_request_input' in b for b in r['boundaries']),'no I/O after guard rejection')
        print('COMMAND9 GUARDS',channel,len(e.records),flush=True)
    e.report('movie_filesystem_command9_guards_verification.json',{MASTER,NAME},[Path(__file__)],scope='Declared coherent mapped admission/presence/function/busy/cache/root/latch inputs. Original callees run. Feature0,foundtypeCCname(success0),invalidFAT rejectoriginalmaster with-245. No counter initialization invented;all nonstackRAM/counters/cache/FPU/calleeGPR and bounded320byte scratch checked plusentire original/native state. No naturalacquisition orpositiveblockmove claim fromtheseABIguards.')
if __name__=='__main__':main()
