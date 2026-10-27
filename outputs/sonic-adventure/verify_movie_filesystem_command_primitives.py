"""Independent bounded names, selected raw-entry reads and timestamp packing."""
import itertools
from pathlib import Path
from movie_filesystem_command_vectors import *
LENGTH,COMPARE,DECODE,DATE=0x8c64ac62,0x8c64ac90,0x8c64acda,0x8c64b472
def main():
    assert v.effects.DLL.name=='movie_filesystem_command.dll'
    e=Evidence();ram=v.seed_ram();base=Session(ram)
    values=[b'\0'+b'Q'*11]+[bytes((65+j%26 for j in range(n)))+b'\0'+b'Z'*(12-n) for n in (1,2,5,11)]+[b'ABCDEFGHIJKL',b'\xff'*12,b'A\0DIFFERENT!',b'ABCDEFGHIJKLMORE']
    for data,(mode,sr) in itertools.product(values,PROFILES):
        m,s,f,c=fresh(base,LENGTH,[A],mode,sr);raw(m,A,data);before=bytes(m);regs=list(s.r);n=len(name(data));last=data[n] if n<12 else data[11]
        regs[0]=regs[5]=regs[6]=n;regs[3]=last if last<128 else last|0xffffff00;regs[4]=A+n;regs[7]=12
        r,a,s,f=e.compare(m,s,f,c,dict(kind='length',data=data.hex(),mode=mode))
        e.require(a==before,'length RAM unchanged');e.require(list(s.r)==regs and s.sr==(sr|1),'independent all length GPR/SR');e.require(r['final_macl']==c['macl'] and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'],'length special registers')
    # Exactly twelve accessible bytes at the end of mapped RAM: no thirteenth load.
    for mode,sr in PROFILES:
        at=0x8cfffff4;m,s,f,c=fresh(base,LENGTH,[at],mode,sr);raw(m,at,b'X'*12);before=bytes(m);r,a,s,f=e.compare(m,s,f,c,dict(kind='length_end_of_ram',mode=mode));e.require(a==before and s.r[0]==12 and s.r[4]==0x8d000000,'bounded12 without extra load')
    pairs=[(a,b) for a,b in zip(values,values)]+[(b'ABC\0',b'AB\0'),(b'XBC\0',b'ABC\0'),(b'ABX\0',b'ABC\0'),(b'ABCDEFGHIJKLxxx',b'ABCDEFGHIJKLyyy'),(b'A\0xx',b'A\0yy'),(b'\xff\x80\0',b'\xff\x81\0')]
    for (left,right),(mode,sr) in itertools.product(pairs,PROFILES):
        m,s,f,c=fresh(base,COMPARE,[A,B],mode,sr);raw(m,A,left);raw(m,B,right);wanted=bytearray(m);put(wanted,s.r[15]-4,s.r[14]);put(wanted,s.r[15]-8,s.pr);put(wanted,s.r[15]-16,A);put(wanted,s.r[15]-12,B)
        r,a,s,f=e.compare(m,s,f,c,dict(kind='compare',left=left.hex(),right=right.hex(),mode=mode));e.require(a==wanted,'compare exact four stack writes');e.require(s.r[0]==int(name(left)==name(right)),'independent bounded string equality')
    for endian in (0,1):
        base=Session(ram,endian=endian)
        for seed,(mode,sr) in itertools.product((0,1,33,127,128,255),PROFILES):
            entry=bytes((seed+i*37)&255 for i in range(32));m,s,f,c=fresh(base,DECODE,[A,OUT],mode,sr);raw(m,A,entry);before=v.sample(m,OUT,48);wanted=decoded(before,entry,endian);expected=bytearray(m);raw(expected,OUT,wanted)
            for offset,value in ((4,s.r[14]),(8,s.r[13]),(12,s.r[12]),(16,s.pr)):put(expected,s.r[15]-offset,value)
            r,a,s,f=e.compare(m,s,f,c,dict(kind='entry',endian=endian,seed=seed,mode=mode));e.require(a==expected,'exact raw-entry fields/gaps/stack writes');e.require(r['boundaries'][-1]['target']==('0x8c66080c' if endian else '0x8c660816'),'actual selected word callback')
    base=Session(ram)
    dates=[bytes((0x20,0x26,0x10,0x27,0x23,0x59,0x58,i)) for i in range(8)]+[bytes([x])*8 for x in (0,0x80,0x99,0xff)]
    for data,(mode,sr) in itertools.product(dates,PROFILES):
        m,s,f,c=fresh(base,DATE,[OUT,A],mode,sr);raw(m,A,data);expected=bytearray(m);raw(expected,OUT,timestamp(data))
        for offset,value in ((4,s.r[14]),(8,s.r[13]),(12,s.r[8]),(16,s.pr)):put(expected,s.r[15]-offset,value)
        r,a,s,f=e.compare(m,s,f,c,dict(kind='timestamp',raw=data.hex(),mode=mode));e.require(a==expected,'exact date bytes/eight-byte output/stack writes');e.require(r['boundaries'].count({})==0 and [b['target'] for b in r['boundaries']].count('0x8c64b3b4')==7,'seven original BCD calls')
    e.report('movie_filesystem_command_primitives_verification.json',{LENGTH,COMPARE,DECODE,DATE},[Path(__file__)],scope='Declared raw byte ABI inputs. Independent fullRAM/exactstack/output-gap/name/BCD/date/weekday oracles;fourCPUFPUprofiles,actual original initializer/BELEselector,12byteRAMboundary,highbytes/nulls/differentlength/firstlatemismatch. All machine state andordered boundaries compared with original instruction model. Two registered primitives reused withoutduplicate credit;whole fileinfo/command qualification separate.')
if __name__=='__main__':main()
