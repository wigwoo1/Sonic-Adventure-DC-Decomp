"""Original command14 size-query admission/root guards with independent RAM effects."""
import itertools
from pathlib import Path
from movie_filesystem_command14_vectors import *

def main():
    e=Evidence();ram=v.seed_ram()
    for channel in (0,7):
        base=Session(ram,channel=channel)
        for entry in (0x8c64bbb2,):
            kinds=('inactive','absent','function_off','busy','cache_off','bad_root','latched_root')
            if entry==0x8c6494e2:kinds=kinds[1:]
            for kind,(mode,sr) in itertools.product(kinds,PROFILES):
                args=[channel,OUT,B,0]
                m,s,f,c=fresh(base,entry,args,mode,sr);put(m,s.r[15],0);raw(m,OUT,b'MISSING'+bytes(6));raw(m,OUT+32,bytes([0x5a])*16);raw(m,A,bytes([0x6c])*72);put(m,A+0x28,channel);put(m,A+4,int(kind!='clean'))
                d=base.descriptor;root=v.load(m,d+0x20);put(m,d+2,int(kind!='inactive'),1);put(m,d+8,int(kind!='cache_off'),1)
                put(m,base.d['header'],int(kind!='absent'),1);put(m,base.d['status'],int(kind!='absent'),1);put(m,base.d['status']+0xc,int(kind=='busy'),1);put(m,base.d['info'],0 if kind=='function_off' else 2);put(m,0x8c89cf90+channel*4,int(kind=='latched_root'))
                data=bytearray(b'U'*16+bytes(496))
                if kind=='bad_root':data[7]=0
                raw(m,root,data)
                for off,val in ((9,0x5a),(10,0xa5),(11,0x33),(12,0x77)):put(m,d+off,val,1)
                wanted=bytearray(m);sp=s.r[15]
                if entry==0x8c648e36:put(wanted,OUT+32,0)
                if kind in ('absent','function_off'):
                    for off in (8,9,10,11,12):put(wanted,d+off,0,1)
                    put(wanted,d+4,0)
                if kind=='bad_root':put(wanted,d+11,0,1);put(wanted,d+12,0,1)
                result=-253 if kind in ('bad_root','latched_root') else -255
                r,a,s,f=e.compare(m,s,f,c,dict(kind=kind,entry=hex(entry),channel=channel,mode=mode,sr=hex(sr)))
                raw(wanted,sp-256,v.sample(a,sp-256,256));e.require(a==wanted,'exact nonstack RAM/output/handle clear/cache effects;256byte upper ABI scratch');e.require(s.r[0]==result&0xffffffff,'independent original guard result')
                e.require(not any('write_request_input' in b for b in r['boundaries']) and not r['cache_purge_sequence'],'no rejected/clean-closed device write or purge')
    e.report('movie_filesystem_command14_guards_verification.json',{0x8c64bbb2},[Path(__file__),P/'analysis/movie_filesystem_command14_audit.json'],'Declared coherent mapped-ABI inactive/presence/function/busy/cache/root/latch inputs across2channels/4CPUFPUprofiles. All actual callees execute; independent wholeRAM/output/cache/error/FPU/calleeGPR checks;256byte upperABI scratch,fullstack compared. Stabledevice domain only. No injected calleeoutputs/savedframes/request IDs.')
if __name__=='__main__':main()
