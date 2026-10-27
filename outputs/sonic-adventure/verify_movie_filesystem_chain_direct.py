"""Independent count-bounded FAT validation and registered address arithmetic."""
import itertools
from pathlib import Path
from movie_filesystem_chain_vectors import *
CHAIN=0x8c64b300;ADDRESS=0x8c64b274;FAT=0x8cdc0000
def scenarios():
    base=dict(head=20,count=2,limit=32,links={20:21,21:0xfffa})
    cases=[dict(kind='valid_two'),dict(kind='missing',missing=True),dict(kind='zero',count=0),dict(kind='valid_one',count=1,links={20:0xfffa}),dict(kind='valid_three',count=3,links={20:21,21:22,22:0xfffa}),dict(kind='head_out',head=32),dict(kind='head_out_zero',head=32,count=0),dict(kind='self',links={20:20}),dict(kind='early_end',links={20:0xfffa}),dict(kind='reserved_fffc',links={20:0xfffc}),dict(kind='reserved_ffff',links={20:0xffff}),dict(kind='next_out',links={20:32}),dict(kind='wrong_last',count=1,links={20:21}),dict(kind='cycle',count=3,links={20:21,21:20}),dict(kind='late_out',count=3,links={20:21,21:32}),dict(kind='unlisted_fffb',limit=65535,links={20:0xfffb,0xfffb:0xfffa}),dict(kind='high_head',limit=65535,head=65534,count=1,links={65534:0xfffa}),dict(kind='max_count_early_end',count=65535,links={20:0xfffa}),dict(kind='second_entry',index=1)]
    return [base|c for c in cases]
def expected(case):
    if case.get('missing'):return 0xffffff05,[]
    head=case['head'];count=case['count'];limit=case['limit'];visited=[]
    if head>=limit:return 0xffffff41,visited
    for i in range(count):
        visited.append(head);link=case['links'].get(head,0)
        if i==count-1:
            if link!=0xfffa:return 0xffffff41,visited
        elif link==head or link in (0xfffa,0xfffc,0xffff) or link>=limit:return 0xffffff41,visited
        head=link
    return 0,visited
def main():
    assert v.effects.DLL.name=='movie_filesystem_chain.dll'
    e=Evidence();ram=v.seed_ram()
    for channel in (0,7):
        base=Session(ram,channel=channel)
        for index,pointer,(mode,sr) in itertools.product((0,1,31,0x07ffffff,0x08000000,0x80000000,0xffffffff,0x10000001),(A,0xfffffff0),PROFILES):
            m,s,f,c=fresh(base,ADDRESS,[channel,index],mode,sr);put(m,base.descriptor+0x28,pointer);before=bytes(m);regs=list(s.r)
            regs[0]=((index<<5)+pointer)&0xffffffff;regs[1]=pointer;regs[2]=0x8c89c550;regs[3]=328;regs[4]=base.descriptor
            r,a,s,f=e.compare(m,s,f,c,dict(kind='address',channel=channel,index=hex(index),pointer=hex(pointer),mode=mode))
            e.require(a==before and list(s.r)==regs,'independent entireRAM/all address GPRs')
            e.require(s.sr==((sr&~1)|((index>>27)&1)) and r['final_macl']==328*channel and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'],'shiftcarry/address special state')
    for channel,endian in itertools.product((0,7),(0,1)):
        base=Session(ram,channel=channel,endian=endian)
        for case,(mode,sr) in itertools.product(scenarios(),PROFILES):
            m,s,f,c=fresh(base,CHAIN,[channel,B],mode,sr);raw(m,B,b'SONIC\0');put(m,base.output+0x30,case['limit']);put(m,base.descriptor+0x28,A);put(m,base.descriptor+0x24,FAT);raw(m,A,bytes(1024))
            data=bytearray(32);data[0]=0xcc;data[4:16]=(b'OTHER' if case.get('missing') else b'SONIC')+bytes(7);data[2:4]=case['head'].to_bytes(2,'big' if endian else 'little');data[24:26]=case['count'].to_bytes(2,'big' if endian else 'little');index=case.get('index',0);raw(m,A+32*index,data)
            for at,value in case['links'].items():raw(m,FAT+at*2,value.to_bytes(2,'big' if endian else 'little'))
            result,visited=expected(case);before=bytes(m);sp=s.r[15];regs=list(s.r);pr=s.pr
            r,a,s,f=e.compare(m,s,f,c,dict(kind=case['kind'],channel=channel,endian=endian,mode=mode,head=case['head'],count=case['count'],limit=case['limit'],expected_visited=visited))
            wanted=bytearray(before);raw(wanted,sp-104,v.sample(a,sp-104,104));e.require(a==wanted,'only maximum original104byte nested stack changes')
            e.require(s.r[0]==result,'independent validation result')
            calls=[b['arguments'][:2] for b in r['boundaries'] if b['target']=='0x8c64b1c0'];e.require(calls==[[channel,h] for h in visited],'exact original FAT lookup order/count')
            entry_calls=[b['arguments'][:2] for b in r['boundaries'] if b['target']=='0x8c64b274'];e.require(entry_calls==([] if case.get('missing') else [[channel,index]]),'actual found entry index to registered helper')
            e.require([v.load(a,sp-o) for o in (4,8,12,16,20)]==[regs[14],regs[13],regs[12],regs[11],pr],'exact own original saves')
            e.require(r['final_macl']==328*channel and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'] and (s.sr&~1)==(sr&~1),'integer special/status preservation')
            e.require(not r['cache_purge_sequence'],'no invented cache side effect')
        print('CHAIN DIRECT',channel,endian,len(e.records),flush=True)
    e.report('movie_filesystem_chain_direct_verification.json',{CHAIN,ADDRESS},[Path(__file__)],scope='Declared mapped helper inputs;original SDK initializer/selector own realBELEcallback pointers. Independent address uint32 arithmetic/carry/fullGPR/fullRAM;chain result/exact FAT order/counter bound/104byte original nestedstack/own saves/fullnonstackRAM. Missing/zero/one/two/three/maxcounts,head/range/last-marker failures,self/earlyend/FFFC/FFFF/cycle cases. Extended mapped highlimit vectors deliberately test helper semantics including unlistedFFFB andhighhead;not avalidcardformat/naturalacquisition claim. Registered64b274 adapter getszero duplicatecredit. Original/fullnative machineandorderedboundaries compared;physicalhardware/fullfiletransfer unfinished.')
if __name__=='__main__':main()
