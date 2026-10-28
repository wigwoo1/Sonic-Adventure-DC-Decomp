"""Independent timestamp/BCD, fixed-block comparison and empty-slot ABI vectors."""
from pathlib import Path
from movie_filesystem_command12_vectors import *

def checked(e,m,s,f,c,label,result,wanted=None,scratch=64):
    wanted=bytearray(m) if wanted is None else wanted;sp=s.r[15]
    r,a,s,f=e.compare(m,s,f,c,label);raw(wanted,sp-scratch,v.sample(a,sp-scratch,scratch));e.require(a==wanted,'independent exact nonstack RAM/guards and bounded ABI scratch');e.require(s.r[0]==result&0xffffffff,'independent original return');return a,s

def main():
    e=Evidence();base=Session(v.seed_ram())
    for number in (0,1,9,10,15,59,99,100,255,655,999,65535):
        for mode,sr in PROFILES:
            m,s,f,c=fresh(base,0x8c64b390,[number],mode,sr);checked(e,m,s,f,c,dict(kind='bcd',number=number,mode=mode),(number//10*16+number%10)&0xffffffff)
    for i in range(8):
        year=(0,99,100,2026,9999,65535,2000,1900)[i];fields=bytes(((0,9,10,59,99,255)[(i+j)%6] for j in range(5)))+bytes([i]);stamp=year.to_bytes(2,'little')+fields
        for mode,sr in (PROFILES[0],PROFILES[2]):
            m,s,f,c=fresh(base,0x8c64b3ec,[OUT,A],mode,sr);raw(m,A,stamp);raw(m,OUT-8,bytes([0xa7])*24);wanted=bytearray(m)
            bcd=lambda n:((n//10)*16+n%10)&255
            encoded=bytes([bcd(year//100),bcd(year%100)])+bytes(map(bcd,fields[:5]))+bytes([(6,0,1,2,3,4,5,255)[i]])
            raw(wanted,OUT,encoded);r,a,s,f=e.compare(m,s,f,c,dict(kind='timestamp',input=stamp.hex(),mode=mode));sp=0x8cf00000;raw(wanted,sp-64,v.sample(a,sp-64,64));e.require(a==wanted,'exact8byte original BCD/time/day mapping with guards/source unchanged')
    for kind in ('equal','first','middle','last','several','beyond','alias'):
        for mode,sr in PROFILES:
            m,s,f,c=fresh(base,0x8c660d8c,[A,B],mode,sr);left=bytes((j*13+29)&255 for j in range(516));right=bytearray(left)
            for at in {'first':[0],'middle':[255],'last':[511],'several':[0,16,511],'beyond':[512]}.get(kind,[]):right[at]^=255
            raw(m,A,left);raw(m,B,right)
            if kind=='alias':s.r[5]=A
            checked(e,m,s,f,c,dict(kind='compare512',case=kind,mode=mode),-1 if kind in ('first','middle','last','several') else 0)
    for limit,free in ((0,None),(-1,None),(1,None),(1,0),(32,None),(32,0),(32,15),(32,31)):
        for mode,sr in PROFILES:
            m,s,f,c=fresh(base,0x8c64ae04,[0],mode,sr);meta=v.load(m,base.descriptor+0x1c);directory=v.load(m,base.descriptor+0x28);put(m,meta+0x30,limit);blob=bytearray((j*7+3)&255 for j in range(1024))
            for i in range(32):blob[i*32]=0 if i==free else (0x80 if i&1 else 0x33)
            raw(m,directory,blob);checked(e,m,s,f,c,dict(kind='empty_directory_slot',limit=limit,free=free,mode=mode),free if free is not None and free<limit else -1)
    e.report('movie_filesystem_command12_primitives_verification.json',{0x8c64b390,0x8c64b3ec,0x8c660d8c,0x8c64ae04},[Path(__file__),P/'analysis/movie_filesystem_command12_audit.json'],'Declared mappedABI vectors:originalBCD fornonnegative0..65535 samples;timestampyear16/timebyte8/daymapping includingtruncation;fixed512bytecomparison equal/first/middle/last/multiple/outside/aliased inputs;firstemptydirectorysignedlimit/nonzero-type scan. AlloriginalSDKarithmetic callees execute;exactRAM/guards/source/output/allCPU/FPU comparison.64byte upperABI scratch,notnaturalacquisitionofeveryinput/globalgamecoverage.')
if __name__=='__main__':main()
