"""Original fixed handle-pool allocation and full72-byte clear in declared ABI inputs."""
from pathlib import Path
from movie_filesystem_command11_vectors import *
POOL=0x8c8a1210;COUNT=0x8c8a1450;ERROR=0x8c8a1454

def main():
    e=Evidence();base=Session(v.seed_ram())
    for count,free in ((-1,None),(0,None),(1,None),(1,0),(3,None),(3,0),(3,2),(8,None),(8,0),(8,3),(8,7)):
        for mode,sr in PROFILES:
            m,s,f,c=fresh(base,0x8c64b2a6,[7],mode,sr)
            pool=bytearray((i*17+29)&255 for i in range(8*72))
            for i in range(8):pool[i*72]=0 if i==free else 1
            raw(m,POOL,pool);put(m,COUNT,count);put(m,ERROR,0x11223344)
            wanted=bytearray(m);success=free is not None and free<count
            if success:put(wanted,POOL+free*72,1,1);put(wanted,POOL+free*72+12,free)
            else:put(wanted,ERROR,-249)
            r,a,s,f=e.compare(m,s,f,c,dict(kind='allocate',count=count,free=free,mode=mode,sr=hex(sr)))
            e.require(a==wanted,'exact pool/global RAM including unselected records and sentinels')
            e.require(s.r[0]==(POOL+free*72 if success else 0),'first free signed-count scan or exact failure')
    for offset in (0,72,7*72):
        for mode,sr in PROFILES:
            m,s,f,c=fresh(base,0x8c64b2d4,[POOL+offset],mode,sr);raw(m,POOL-16,bytes((i*11+7)&255 for i in range(8*72+32)));wanted=bytearray(m);raw(wanted,POOL+offset,bytes(72))
            r,a,s,f=e.compare(m,s,f,c,dict(kind='clear',offset=offset,mode=mode,sr=hex(sr)))
            e.require(a==wanted,'exact72-byte clear with surrounding records/guards preserved')
            e.require(s.r[0]==POOL+offset,'original memset return pointer')
    e.report('movie_filesystem_command11_handles_verification.json',{0x8c64b2a6,0x8c64b2d4},[Path(__file__),P/'analysis/movie_filesystem_command11_audit.json'], 'Declared coherent handlepool/count inputs: signednegative/zero/full/first/interior/last allocation;exactfirstfree flag/index andfailure-249 global;full72byte clear/guards acrossfourCPUFPUprofiles. Originalmemsetexecutes;not naturalpool-exhaustion acquisition ornewcreditforregistered4rowwrapper. WholeRAM/allCPU/FPU compared.')
if __name__=='__main__':main()
