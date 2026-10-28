"""Independent original FAT seek inputs, including sentinel and remaining-count errors."""
from pathlib import Path
from movie_filesystem_command14_vectors import *

def main():
    e=Evidence();ram=v.seed_ram()
    cases=[('zero',20,3,0,{20:21,21:22,22:0xfffa}),('one',20,3,1,{20:21,21:22,22:0xfffa}),('two',20,3,2,{20:21,21:22,22:0xfffa}),('end',20,3,3,{20:21,21:22,22:0xfffa}),('past_end',20,3,5,{20:21,21:22,22:0xfffa}),('sentinel_empty',0xfffa,0,1,{}),('sentinel_inconsistent',0xfffa,1,1,{}),('remaining_zero',20,0,1,{20:21}),('short_chain',20,3,2,{20:0xfffa}),('unsigned_max_skip',20,3,0xffffffff,{20:21,21:22,22:0xfffa})]
    for endian in (0,1):
        base=Session(ram,channel=7 if endian else 0,endian=endian);channel=base.channel
        for kind,head,remaining,offset,chain in cases:
            for mode,sr in PROFILES:
                m,s,f,c=fresh(base,0x8c64bb4c,[A,offset,0x12345678],mode,sr);d=base.descriptor;meta=v.load(m,d+0x1c);fat=v.load(m,d+0x24);put(m,meta,511)
                order='big' if endian else 'little';raw(m,fat,(0xfffa).to_bytes(2,order)*512)
                for block,nxt in chain.items():raw(m,fat+block*2,nxt.to_bytes(2,order))
                raw(m,A,bytes((j*17+29)&255 for j in range(72)));put(m,A+0x28,channel);put(m,A+0x38,head);put(m,A+0x34,remaining);put(m,A+0x40,0x12345678)
                wanted=bytearray(m);current=head;left=remaining;steps=0;result=left
                while steps<offset:
                    if current==0xfffa:result=0 if left==0 else -65536;break
                    if left==0:result=-65536;break
                    current=chain[current];left=(left-1)&0xffffffff;steps+=1;result=left
                put(wanted,A+0x40,current);put(wanted,A+0x34,left);sp=s.r[15]
                r,a,s,f=e.compare(m,s,f,c,dict(kind=kind,channel=channel,endian=endian,mode=mode,head=head,remaining=remaining,offset=hex(offset)))
                raw(wanted,sp-128,v.sample(a,sp-128,128));e.require(a==wanted,'exact independent handle current/remaining update and all nonstackRAM;128byte upperABI scratch');e.require(s.r[0]==result&0xffffffff,'original independent seek result');e.require(not any('write_request_input' in b for b in r['boundaries']) and not r['cache_purge_sequence'],'seek performs no device write or cache purge')
    e.report('movie_filesystem_command14_seek_verification.json',{0x8c64bb4c},[Path(__file__),P/'analysis/movie_filesystem_command14_audit.json'],'Declared mapped handle/FAT ABI inputs across2channels/2SDKbyteorders/4CPUFPUprofiles. Original seek resets current to head,does not reset remaining,walks FAToffset times;FFFA+zero returns0,FFFA+nonzero or remaining0 before end returns-65536. Includesoffset0/1/2/end/pastend/unsignedmax with finite chain. ActualSDKreader runs. Independent entire nonstackRAM/return plusfulloriginal-nativeCPU/FPU/stack/ordered effects. No naturalacquisitionclaim for inconsistent helperinputs;no suppliedcalleeoutput ornewwaitbounds.')
if __name__=='__main__':main()
