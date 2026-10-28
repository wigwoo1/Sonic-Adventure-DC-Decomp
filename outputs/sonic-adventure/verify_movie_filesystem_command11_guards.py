"""Original size/open/read guards and declared mapped EOF/marker inputs."""
import itertools
from pathlib import Path
from movie_filesystem_command11_vectors import *

def main():
    e=Evidence();ram=v.seed_ram()
    for channel in (0,7):
        base=Session(ram,channel=channel)
        for entry in (0x8c64a360,0x8c648d24,0x8c64965c):
            kinds=('inactive','absent','function_off','busy','cache_off','bad_root','latched_root')
            if entry==0x8c64965c:kinds=kinds[1:]
            for kind,(mode,sr) in itertools.product(kinds,PROFILES):
                args=[A,OUT] if entry==0x8c64965c else [channel,OUT,OUT+32]
                m,s,f,c=fresh(base,entry,args,mode,sr);raw(m,OUT,b'MISSING'+bytes(6));raw(m,OUT+32,bytes([0x5a])*16)
                raw(m,A,bytes([0x6c])*72);put(m,A+0x28,channel);put(m,A+0x34,1);put(m,A+0x40,20)
                descriptor=base.descriptor;root=v.load(m,descriptor+0x20)
                put(m,descriptor+2,int(kind!='inactive'),1);put(m,descriptor+8,int(kind!='cache_off'),1)
                put(m,base.d['header'],int(kind!='absent'),1);put(m,base.d['status'],int(kind!='absent'),1)
                put(m,base.d['status']+0xc,int(kind=='busy'),1);put(m,base.d['info'],0 if kind=='function_off' else 2)
                put(m,0x8c89cf90+channel*4,int(kind=='latched_root'))
                data=bytearray(512);data[:16]=b'U'*16
                if kind=='bad_root':data[7]=0
                raw(m,root,data)
                for off,val in ((9,0x5a),(10,0xa5),(11,0x33),(12,0x77)):put(m,descriptor+off,val,1)
                wanted=bytearray(m);sp=s.r[15]
                if entry==0x8c648d24:put(wanted,OUT+32,0)
                if kind in ('absent','function_off'):
                    for off in (8,9,10,11,12):put(wanted,descriptor+off,0,1)
                    put(wanted,descriptor+4,0)
                if kind=='bad_root':put(wanted,descriptor+11,0,1);put(wanted,descriptor+12,0,1)
                result=-253 if kind in ('bad_root','latched_root') else -255
                r,a,s,f=e.compare(m,s,f,c,dict(kind=kind,channel=channel,entry=hex(entry),mode=mode,sr=hex(sr)))
                raw(wanted,sp-132,v.sample(a,sp-132,132))
                e.require(a==wanted,'exact nonstack RAM/output-pointer/cache effects;132byte upper nested ABI scratch')
                e.require(s.r[0]==result&0xffffffff,'independent original admission/root error')
                e.require(not any('write_request_input' in b for b in r['boundaries']) and not r['cache_purge_sequence'],'no device write or cache-purge side effects')
        for current,remaining,result in ((0xfffa,1,-65536),(0xfffc,1,-191),(0xffff,1,-191),(20,0,0),(0xfffa,0,0)):
            for mode,sr in PROFILES:
                m,s,f,c=fresh(base,0x8c64965c,[A,OUT],mode,sr);descriptor=base.descriptor;root=v.load(m,descriptor+0x20)
                put(m,descriptor+2,1,1);put(m,descriptor+8,1,1);put(m,base.d['header'],1,1);put(m,base.d['status'],1,1);put(m,base.d['status']+0xc,0,1);put(m,base.d['info'],2);put(m,0x8c89cf90+channel*4,0);raw(m,root,b'U'*16+bytes(496))
                raw(m,A,bytes([0x6c])*72);put(m,A+0x28,channel);put(m,A+0x34,remaining);put(m,A+0x40,current);raw(m,OUT,bytes([0xa7])*512);wanted=bytearray(m);sp=s.r[15]
                r,a,s,f=e.compare(m,s,f,c,dict(kind='read_marker',channel=channel,current=current,remaining=remaining,mode=mode))
                raw(wanted,sp-84,v.sample(a,sp-84,84));e.require(a==wanted,'marker/EOF preserves handle/caller data/all nonstack RAM')
                e.require(s.r[0]==result&0xffffffff,'independent marker/EOF result')
                e.require(not any(b['target']=='0x8c647610' for b in r['boundaries']),'marker/EOF rejects before block I/O')
    for mode,sr in PROFILES:
        base=Session(ram);m,s,f,c=fresh(base,0x8c64919e,[0,0],mode,sr);wanted=bytearray(m);sp=s.r[15]
        r,a,s,f=e.compare(m,s,f,c,dict(kind='close_null',mode=mode));raw(wanted,sp-32,v.sample(a,sp-32,32));e.require(a==wanted,'NULL close preserves nonstack RAM');e.require(s.r[0]==(-256&0xffffffff),'NULL close original-256')
    e.report('movie_filesystem_command11_guards_verification.json',{0x8c64a360,0x8c648d24,0x8c64965c,0x8c64919e},[Path(__file__),P/'analysis/movie_filesystem_command11_audit.json'], 'Declared coherent mapped ABI inputs for size/open/read admission/device/cache/root/latch guards,read markers/EOF andNULLclose. Originalcalleeoutputs never supplied;root/status/readiness/invalidation execute. FullCPU/FPU/RAM comparison plusindependentnonstackRAM/cache/output-pointer/error expectations and132/84/32byte upperABIscratch. Naturalacquisitionofeveryguard/invalidmarkerstate notclaimed. Dirtyclose persistencebranches unqualified.')
if __name__=='__main__':main()
