"""Original sequential type-CC writer rejects occupied blocks and the extent boundary."""
import itertools
from pathlib import Path
from movie_filesystem_command13_vectors import *

def main():
    e=Evidence();ram=v.seed_ram()
    for endian in (0,1):
        base=Session(ram,channel=7 if endian else 0,endian=endian);channel=base.channel
        for kind,(mode,sr) in itertools.product(('occupied_first','occupied_next','at_capacity','past_capacity'),PROFILES):
            m,s,f,c=fresh(base,0x8c6494e2,[A,OUT],mode,sr);d=base.descriptor;meta=v.load(m,d+0x1c);fat=v.load(m,d+0x24)
            for off in (2,8,10,11,12):put(m,d+off,1,1)
            put(m,base.d['header'],1,1);put(m,base.d['status'],1,1);put(m,base.d['status']+0xc,0,1);put(m,base.d['info'],2);put(m,0x8c89cf90+channel*4,0)
            raw(m,v.load(m,d+0x20),b'U'*16+bytes(496));put(m,meta,511);put(m,meta+0x38,1)
            order='big' if endian else 'little';raw(m,fat,(0xfffc).to_bytes(2,order)*512)
            count=0 if kind=='occupied_first' else 1 if kind in ('occupied_next','at_capacity') else 2
            if kind.startswith('occupied'):raw(m,fat+count*2,(0xfffa).to_bytes(2,order))
            raw(m,A,bytes(72));put(m,A,1,1);put(m,A+4,2);put(m,A+8,1);put(m,A+0x28,channel);put(m,A+0x30,count);put(m,A+0x38,0xffff);raw(m,OUT,bytes([0xa7])*512)
            wanted=bytearray(m);sp=s.r[15];r,a,s,f=e.compare(m,s,f,c,dict(kind=kind,endian=endian,channel=channel,mode=mode));raw(wanted,sp-256,v.sample(a,sp-256,256))
            e.require(a==wanted,'independent entire nonstackRAM/handle/FAT/data unchanged;256byte upperABI scratch');e.require(s.r[0]==(-252&0xffffffff),'independent original no-space-252');e.require(not any('write_request_input' in b for b in r['boundaries']),'no device write on rejected sequential block')
        for mode,sr in PROFILES:
            m,s,f,c=fresh(base,0x8c648e36,[channel,OUT,A,B],mode,sr);d=base.descriptor;meta=v.load(m,d+0x1c)
            for off in (2,8,10,11,12):put(m,d+off,1,1)
            put(m,base.d['header'],1,1);put(m,base.d['status'],1,1);put(m,base.d['status']+0xc,0,1);put(m,base.d['info'],2);put(m,0x8c89cf90+channel*4,0)
            raw(m,v.load(m,d+0x20),b'U'*16+bytes(496));put(m,meta+0x38,0);raw(m,OUT,b'NEWFILE'+bytes(6));raw(m,B,bytes([0x5a])*16)
            wanted=bytearray(m);put(wanted,B,0);sp=s.r[15];r,a,s,f=e.compare(m,s,f,c,dict(kind='explicit_mapped_zero_capacity',channel=channel,mode=mode));raw(wanted,sp-256,v.sample(a,sp-256,256))
            e.require(a==wanted,'zero-capacity open clears output pointer only;exact nonstackRAM/256byte upperABI scratch');e.require(s.r[0]==(-248&0xffffffff),'independent original open -248');e.require(not any('write_request_input' in b for b in r['boundaries']),'no device write without CC capacity')
    e.report('movie_filesystem_command13_edge_guards_verification.json',{0x8c6494e2,0x8c648e36},[Path(__file__),P/'analysis/movie_filesystem_command13_audit.json'],'Declared coherent mapped-ABI sequential block/extent rejection and explicit parsed-zero CC-capacity open,2channels/2SDKbyteorders/4CPUFPUprofiles. Actual SDK endian reader and all original callees run. Exact nonstackRAM/noIO/return plus entireCPU/FPU/ordered effects. No claims of natural acquisition of every helper input; no supplied calleeoutputs or new wait limits.')
if __name__=='__main__':main()
