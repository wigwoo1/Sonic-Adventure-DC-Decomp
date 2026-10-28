"""Original outer-write rejection and first-allocation exhaustion edges."""
import itertools
from pathlib import Path
from movie_filesystem_command12_vectors import *
from verify_movie_filesystem_command12_allocation import setup

def main():
    e=Evidence();ram=v.seed_ram()
    for channel in (0,7):
        base=Session(ram,channel=channel)
        for kind,(mode,sr) in itertools.product(('inactive','absent','function_off','busy','cache_off','bad_root','latched_root'),PROFILES):
            m,s,f,c=fresh(base,0x8c64b71c,[channel,OUT,B,0],mode,sr);raw(m,OUT,b'MISSING'+bytes(6));d=base.descriptor;root=v.load(m,d+0x20);put(m,d+2,int(kind!='inactive'),1);put(m,d+8,int(kind!='cache_off'),1);put(m,base.d['header'],int(kind!='absent'),1);put(m,base.d['status'],int(kind!='absent'),1);put(m,base.d['status']+0xc,int(kind=='busy'),1);put(m,base.d['info'],0 if kind=='function_off' else 2);put(m,0x8c89cf90+channel*4,int(kind=='latched_root'));blob=bytearray(b'U'*16+bytes(496))
            if kind=='bad_root':blob[7]=0
            raw(m,root,blob)
            for off,value in ((9,0x5a),(10,0xa5),(11,0x33),(12,0x77)):put(m,d+off,value,1)
            wanted=bytearray(m);sp=s.r[15]
            if kind in ('absent','function_off'):
                for off in (8,9,10,11,12):put(wanted,d+off,0,1)
                put(wanted,d+4,0)
            if kind=='bad_root':put(wanted,d+11,0,1);put(wanted,d+12,0,1)
            r,a,s,f=e.compare(m,s,f,c,dict(kind='master_guard',guard=kind,channel=channel,mode=mode));raw(wanted,sp-256,v.sample(a,sp-256,256));e.require(a==wanted,'exact guard/cache/nonstackRAM without counter reset');e.require(s.r[0]==((-253 if kind in ('bad_root','latched_root') else -255)&0xffffffff),'independent original outer error');e.require(not any('write_request_input' in b for b in r['boundaries']),'no write on outer rejection')
        for mode,sr in PROFILES:
            m,s,f,c,fat=setup(base,0x8c6493c6,[A,OUT],int(channel==7),8,(),());c['mode']=mode;c['sr']=sr;s.sr=sr;f.fpscr=0x40001|(mode<<19);raw(m,A,bytes(72));put(m,A,1,1);put(m,A+4,1);put(m,A+0x28,channel);put(m,A+0x38,0xffff);raw(m,OUT,bytes([0xa7])*512);wanted=bytearray(m);sp=s.r[15]
            r,a,s,f=e.compare(m,s,f,c,dict(kind='no_first_block',channel=channel,mode=mode));raw(wanted,sp-256,v.sample(a,sp-256,256));e.require(a==wanted,'first-allocation failure preserves handle/caller/FAT/allnonstackRAM');e.require(s.r[0]==(-252&0xffffffff),'original first-allocation no-space-252');e.require(not any('write_request_input' in b for b in r['boundaries']),'no write without a selected block')
    e.report('movie_filesystem_command12_edge_guards_verification.json',{0x8c64b71c,0x8c6493c6},[Path(__file__),P/'verify_movie_filesystem_command12_allocation.py',P/'analysis/movie_filesystem_command12_audit.json'],'DeclaredmappedABI outermaster admission/status/root error andfirstblock exhaustion,2channels/4CPUFPUprofiles. Alloriginalhelpersrun;independentfullRAM/counters/error/noIO/256byteupperABIscratch plusentireCPU/FPU. No suppliedcalleeoutput ornewwaitlimits;notnaturalacquisitionofeveryguardstate.')
if __name__=='__main__':main()
