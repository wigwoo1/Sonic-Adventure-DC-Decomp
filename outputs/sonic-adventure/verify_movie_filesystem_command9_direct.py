"""Independent mapped-ABI oracles for original relocation scans and typed lookup."""
import itertools
from pathlib import Path
from movie_filesystem_command9_vectors import *
FAT=0x8cdc0000;DIR=0x8cdd0000
LOW,HIGH,PREV,VALID,HEAD,LOOKUP,NAME=0x8c64a6ae,0x8c64a6fc,0x8c64b080,0x8c64a7ba,0x8c64a74e,0x8c64adc6,0x8c64a3fa

def records_for(case,order):
    out=bytearray(32*8)
    for at,type_,head in case.get('entries',[]):
        row=bytearray(32);row[0]=type_;row[1]=0xa5;row[2:4]=(head&65535).to_bytes(2,order);row[4:16]=bytes((at*17+j*3+1)&255 for j in range(12));row[16:24]=bytes((0x20,0x26,0x10,0x28,0x12,0x34,0x56,3));row[24:26]=(2).to_bytes(2,order);row[26:28]=(1).to_bytes(2,order);out[at*32:(at+1)*32]=row
    return out

def main():
    e=Evidence();ram=v.seed_ram()
    cases=[]
    def add(entry,**c):cases.append(dict(entry=entry,limit=8,root=15,words={},**c))
    for c in (dict(kind='empty',limit=0),dict(kind='all_free'),dict(kind='all_bad',words={i:0xffff for i in range(8)}),dict(kind='first',words={0:1}),dict(kind='skip_markers',words={0:0xffff,2:0xfffa}),dict(kind='last',words={7:0xfffa})):cases.append(dict(entry=LOW,limit=8,root=15,words={})|c)
    for c in (dict(kind='empty',limit=0,start=0),dict(kind='all_free',start=0),dict(kind='none',start=0,words={i:0xffff for i in range(8)}),dict(kind='upper_only',start=7),dict(kind='above',start=8),dict(kind='skip',start=0,words={7:0xffff,6:3}),dict(kind='exclude_lower',start=1,words={i:0xffff for i in range(1,8)})):cases.append(dict(entry=HIGH,limit=8,root=15,words={})|c)
    for c in (dict(kind='empty',limit=0,target=20),dict(kind='none',target=20),dict(kind='zero',target=20,words={0:20}),dict(kind='upper',target=20,words={7:20}),dict(kind='descending',target=20,words={3:20,7:20}),dict(kind='signed_low16',target=0x12348000,words={2:0x8000}),dict(kind='all_marker',target=0xffff,words={i:0xffff for i in range(8)})):cases.append(dict(entry=PREV,limit=8,root=15,words={})|c)
    for c in (dict(kind='empty',root=-1),dict(kind='markers',words={0:0xffff,1:0xfffa}),dict(kind='lower_good',words={0:7}),dict(kind='lower_bad',words={0:8}),dict(kind='upper_good',words={8:15}),dict(kind='upper_bad',words={8:16}),dict(kind='zero_split',limit=0,words={0:15}),dict(kind='all_lower',limit=16,words={15:15})):cases.append(dict(entry=VALID,limit=8,root=15,words={})|c)
    for c in (dict(kind='empty',limit=0),dict(kind='none'),dict(kind='first',entries=[(0,0x33,20)]),dict(kind='last',entries=[(7,0xcc,20)]),dict(kind='any_nonzero',entries=[(0,0x80,20)]),dict(kind='skip_zero',entries=[(0,0,20),(3,0x33,20)]),dict(kind='fullword_argument',target=0x10014,entries=[(0,0x33,20)]),dict(kind='first_match',entries=[(1,0x33,20),(4,0xcc,20)])):cases.append(dict(entry=HEAD,limit=8,root=15,words={},target=20,entries=[])|c)
    for entry in (LOOKUP,NAME):
        for c in (dict(kind='empty',limit=0),dict(kind='none',entries=[(0,0x33,20)]),dict(kind='first',entries=[(0,0xcc,20)]),dict(kind='last',entries=[(0,0x33,22),(7,0xcc,20)]),dict(kind='first_match',entries=[(2,0xcc,20),(4,0xcc,21)])):cases.append(dict(entry=entry,limit=8,root=15,words={},entries=[])|c)
    for channel,endian in itertools.product((0,7),(0,1)):
        base=Session(ram,channel=channel,endian=endian);order='big' if endian else 'little'
        for case,(mode,sr) in itertools.product(cases,PROFILES):
            entry=case['entry'];limit=case['limit'];root=case['root'];words={i:0xfffc for i in range(32)}|case['words'];directory=records_for(case,order);args=[channel]+([case['start']] if entry==HIGH else [case.get('target',0)] if entry in (PREV,HEAD) else [OUT] if entry in (LOOKUP,NAME) else [])
            m,s,f,c=fresh(base,entry,args,mode,sr);put(m,base.descriptor+0x24,FAT);put(m,base.descriptor+0x28,DIR);put(m,base.output,root);put(m,base.output+0x30,limit)
            for at,value in words.items():raw(m,FAT+at*2,(value&65535).to_bytes(2,order))
            raw(m,DIR,directory);raw(m,OUT,bytes([0xa5])*64);put(m,base.descriptor+8,1,1);put(m,0x8c89cf90+channel*4,0)
            if entry==NAME:
                raw(m,v.load(m,base.descriptor+0x20),b'U'*16+bytes(496))
            before=bytes(m);sp=s.r[15];expected=bytearray(before)
            if entry==LOW:result=next((i for i in range(limit) if words[i] not in (0xfffc,0xffff)),-1)
            elif entry==HIGH:result=next((i for i in range(limit-1,case['start']-1,-1) if words[i]==0xfffc),-1)
            elif entry==PREV:result=next((i for i in range(limit-1,-1,-1) if words[i]&65535==case['target']&65535),-1)
            elif entry==VALID:result=-1 if any(words[i] not in (0xfffc,0xffff,0xfffa) and words[i]>=(limit if i<limit else root+1) for i in range(root+1)) else 0
            elif entry==HEAD:result=next((DIR+i*32 for i in range(limit) if directory[i*32] and int.from_bytes(directory[i*32+2:i*32+4],order)==case['target']),0)
            else:
                index=next((i for i in range(limit) if directory[i*32]==0xcc),-1)
                if entry==LOOKUP:
                    result=index
                    if index>=0:raw(expected,OUT,decoded(v.sample(m,OUT,44),directory[index*32:(index+1)*32],endian))
                else:
                    result=0 if index>=0 else -251
                    if index>=0:raw(expected,OUT,directory[index*32+4:index*32+16]+bytes(1))
            r,a,s,f=e.compare(m,s,f,c,dict(entry=hex(entry),case=case,channel=channel,endian=endian,mode=mode,sr=hex(sr)))
            frame=192 if entry in (LOOKUP,NAME) else 64;raw(expected,sp-frame,v.sample(a,sp-frame,frame));e.require(a==expected,'exact nonstack RAM and bounded original ABI scratch')
            e.require(s.r[0]==result&0xffffffff,'independent scan/lookup/validation result')
            e.require(not r['cache_purge_sequence'] and not any('write_request_input' in b for b in r['boundaries']),'no storage/cache side effects')
            e.require(r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'] and (s.sr&~1)==(sr&~1),'unaffected status and special state')
        print('COMMAND9 DIRECT',channel,endian,len(e.records),flush=True)
    e.report('movie_filesystem_command9_direct_verification.json',{LOW,HIGH,PREV,VALID,HEAD,LOOKUP,NAME},[Path(__file__)],scope='Declared coherent mapped helper ABI only. Independent first/last/descending/free/bad/reserved/split-bound/signed-low16/directory-type0xCC/namecopy/decodedrecord oracles. Original constructor/SDK endian readers/FATgetter/decoder run;exact nonstackRAM/FPU/calleeGPR and bounded64/192byte ABI scratch plusstrict fullnativeoriginalstate. No fabricatedcalleeoutput. Mainrelocation andnaturalacquisition requireseparate tasks;no fullfamilyqualification fromthissuite alone.')
if __name__=='__main__':main()
