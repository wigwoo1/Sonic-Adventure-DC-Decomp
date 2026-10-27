"""Independent in-memory removal, FAT-store and directory-dirty oracles."""
import itertools
from pathlib import Path
from movie_filesystem_delete_vectors import *
from verify_movie_filesystem_chain_direct import expected as chain_expected
REMOVE=0x8c649c68;FAT_STORE=0x8c64b18e;DIRTY=0x8c64b28c;ZERO=0x8c64b2dc;FAT=0x8cdc0000
def signed(x):return x if x<0x80000000 else x-0x100000000
def scenarios():
    base=dict(head=20,count=2,limit=32,links={20:21,21:0xfffa},index=0)
    return [base|x for x in (dict(kind='valid_two'),dict(kind='missing',missing=True),dict(kind='zero_count',count=0),dict(kind='one',count=1,links={20:0xfffa}),dict(kind='three',count=3,links={20:21,21:22,22:0xfffa}),dict(kind='self',links={20:20}),dict(kind='early_end',links={20:0xfffa}),dict(kind='head_out',head=32),dict(kind='next_out',links={20:32}),dict(kind='wrong_last',count=1,links={20:21}),dict(kind='index1',index=1),dict(kind='index15',index=15),dict(kind='index16',index=16),dict(kind='index31',index=31),dict(kind='high_head',limit=65535,head=65534,count=1,links={65534:0xfffa}),dict(kind='unlisted_fffb',limit=65535,links={20:0xfffb,0xfffb:0xfffa}),dict(kind='max_count_bad',count=65535,links={20:0xfffa}))]
def main():
    assert v.effects.DLL.name=='movie_filesystem_delete.dll';e=Evidence();ram=v.seed_ram()
    for channel in (0,7):
        base=Session(ram,channel=channel)
        for (mode,sr),index in itertools.product(PROFILES,(0,15,16,31,255,256,65535,0xffffffff,0xfffffff0)):
            m,s,f,c=fresh(base,DIRTY,[channel,index],mode,sr);before=bytes(m);regs=list(s.r);target=(base.descriptor+0x48+(signed(index)>>4))&0xffffffff
            wanted=bytearray(m);put(wanted,target,1,1)
            r,a,s,f=e.compare(m,s,f,c,dict(kind='dirty',channel=channel,index=hex(index),mode=mode))
            regs[0]=base.descriptor+0x48;regs[1]=1;regs[2]=0x8c89c550;regs[3]=328;regs[4]=base.descriptor;regs[5]=target
            e.require(a==wanted and list(s.r)==regs and s.sr==sr,'exact dirty byte/arithmetic/all GPRs/no other RAM')
            e.require(r['final_macl']==328*channel and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'],'dirty multiply/special state')
        for mode,sr in PROFILES:
            m,s,f,c=fresh(base,ZERO,[channel],mode,sr);wanted=bytes(m);regs=list(s.r);regs[0]=0
            r,a,s,f=e.compare(m,s,f,c,dict(kind='canonical_zero',channel=channel,mode=mode));e.require(a==wanted and list(s.r)==regs and s.sr==sr and r['final_macl']==c['macl'],'registered no-op exact RAM/CPU;zero duplicate credit')
    pairs=((511,0),(511,511),(511,512),(511,0xffffffff),(511,0x80000000),(511,0x7fffffff),(0xffffffff,0),(0xffffffff,0xffffffff),(0x80000000,0x80000000),(0x80000000,0xffffffff))
    for channel,endian in itertools.product((0,7),(0,1)):
        base=Session(ram,channel=channel,endian=endian);order='big' if endian else 'little'
        for (limit,index),value,(mode,sr) in itertools.product(pairs,(0,0x1234abcd),PROFILES):
            m,s,f,c=fresh(base,FAT_STORE,[channel,index,value],mode,sr);put(m,base.descriptor+0x24,FAT);put(m,base.output,limit);before=bytes(m);sp=s.r[15]
            wanted=bytearray(before);permitted=signed(index)<=signed(limit);target=(FAT+((index<<1)&0xffffffff))&0xffffffff
            if permitted:raw(wanted,target,(value&65535).to_bytes(2,order))
            r,a,s,f=e.compare(m,s,f,c,dict(kind='fat_store',channel=channel,endian=endian,index=hex(index),limit=hex(limit),value=hex(value),mode=mode))
            raw(wanted,sp-8,v.sample(a,sp-8,8));e.require(a==wanted,'exact conditional endian halfword store and only original8byte stack')
            calls=[b for b in r['boundaries'] if b['target'] in ('0x8c66081e','0x8c660826')];e.require(len(calls)==int(permitted),'actual original SDK-selected writer or no call')
            e.require(r['final_macl']==328*channel and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'],'FAT helper multiply/special state')
        for case,(mode,sr) in itertools.product(scenarios(),PROFILES):
            m,s,f,c=fresh(base,REMOVE,[channel,B],mode,sr);raw(m,B,b'SONIC\0');put(m,base.output+0x30,case['limit']);put(m,base.output,511);put(m,base.descriptor+0x28,A);put(m,base.descriptor+0x24,FAT);raw(m,A,bytes(1024))
            data=bytearray(32);data[0]=0xcc;data[1]=0xa5;data[2:4]=case['head'].to_bytes(2,order);data[4:16]=(b'OTHER' if case.get('missing') else b'SONIC')+bytes(7);data[16:24]=bytes((0x20,0x26,0x10,0x27,0x23,0x59,0x58,3));data[24:26]=case['count'].to_bytes(2,order);raw(m,A+case['index']*32,data)
            for at,value in case['links'].items():raw(m,FAT+at*2,value.to_bytes(2,order))
            valid,visited=chain_expected(case);free=visited if not valid else [];before=bytes(m);sp=s.r[15];regs=list(s.r);pr=s.pr;wanted=bytearray(before)
            if not case.get('missing'):
                raw(wanted,A+case['index']*32,bytes(32));put(wanted,base.descriptor+0x48+(case['index']>>4),1,1)
                for head in free:
                    if head<=511:raw(wanted,FAT+head*2,(0xfffc).to_bytes(2,order))
            r,a,s,f=e.compare(m,s,f,c,dict(kind='remove_'+case['kind'],channel=channel,endian=endian,mode=mode,index=case['index'],head=case['head'],count=case['count'],limit=case['limit']))
            raw(wanted,sp-184,v.sample(a,sp-184,184));e.require(a==wanted,'independent whole nonstack RAM:only record/dirty/FAT changes;184byte max nested original stack')
            e.require(s.r[0]==(0xffffff05 if case.get('missing') else 0),'independent result;bad chains still clear record without freeing links')
            gets=[b['arguments'][:2] for b in r['boundaries'] if b['target']=='0x8c64b1c0'];e.require(gets==[[channel,head] for head in visited+free],'independent validation then free lookup order')
            stores=[b['arguments'][:3] for b in r['boundaries'] if b['target']=='0x8c64b18e'];e.require(stores==[[channel,head,0xfffc] for head in free],'exact original free-link calls including skipped out-of-root writes')
            marks=[b['arguments'][:2] for b in r['boundaries'] if b['target']=='0x8c64b28c'];e.require(marks==([] if case.get('missing') else [[channel,case['index']]]),'exact original dirty sector index')
            e.require([v.load(a,sp-4*i) for i in range(1,8)]==[regs[14],regs[13],regs[12],regs[11],regs[10],regs[9],pr],'exact own register/PR saves')
            e.require(not r['cache_purge_sequence'] and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'],'no new cache effects or unaffected special-state changes')
        print('DELETE DIRECT',channel,endian,len(e.records),flush=True)
    e.report('movie_filesystem_delete_direct_verification.json',{REMOVE,FAT_STORE,DIRTY,ZERO},[Path(__file__),P/'verify_movie_filesystem_chain_direct.py'],scope='Declared mapped direct helper inputs;real original interface constructor/selector/search/entrydecode/chainvalidator/FATreader/writer/memoryfill. Independent exact dirty arithmetic/allGPR/RAM; signed inclusive FAT bounds/endian16bit writes/wrappedindices;filemissing/zero/validone/two/three/badchain/highhead/reserved/index0/1/15/16/31 cases. Found records arecleared/marked evenifvalidationfails;FATfreedonlyonvalidchain andwithinoriginalrootlimit. FullnonstackRAM/own saves/184byte max nestedstack verified. No suppliedcalleeoutput/decodedcallbackreturn. Canonical64b2dc getszero duplicatecredit. This isRAM mutation,notpersisted deletion/fullhandler/game.')
if __name__=='__main__':main()
