"""Independent descending FAT allocation and synchronous persistence controls."""
import itertools
from pathlib import Path
from movie_filesystem_persist_vectors import *
FAT=0x8cdc0000;SCAN=0x8c6489d4;ALLOC=0x8c64894a;CONTIG=0x8c64af4a;DIRECTORY=0x8c64ab36;FAT_SAVE=0x8c64aecc;FAT_PERSIST=0x8c64afe0

def scan(marker,low,high,words):
    visits=[]
    for index in range(high,low-1,-1):
        visits.append(index)
        if words.get(index,0x7777)&65535==marker&65535:return index,visits
    return -1,visits

def alloc(case):
    split=case['root']-case['fat_count'];a,first=scan(case['marker'],split-case['dir_count'],split-1,case['words'])
    if a>0:return a,first
    b,second=scan(case['marker'],case['low'],case['low']+case['length']-1,case['words'])
    return (b if b>0 else -1),first+second

def contiguous(case):
    high=case['low']+case['length']-1;searches=[];probes=[];reader_order=[]
    for _ in range(64):
        found,visits=scan(0xfffc,case['low'],high,case['words']);searches+=visits;reader_order+=visits
        if found<0:return -1,searches,probes,reader_order
        for at in range(case['need']):
            head=found-at;probes.append(head);reader_order.append(head)
            if case['words'].get(head,0x7777)&65535!=0xfffc:high=head-1;break
        else:return found-case['need']+1,searches,probes,reader_order
    raise AssertionError('finite independent allocation oracle')

def main():
    assert v.effects.DLL.name=='movie_filesystem_persist.dll';e=Evidence();ram=v.seed_ram()
    lower=[dict(kind='empty',low=3,high=2,words={}),dict(kind='none',low=4,high=6,words={}),dict(kind='upper',low=4,high=6,words={6:0xfffc}),dict(kind='lower',low=4,high=6,words={4:0xfffc}),dict(kind='middle',low=4,high=6,words={5:0xfffc}),dict(kind='signed_word',low=4,high=6,marker=0x8001,words={5:0x8001}),dict(kind='truncated_marker',low=4,high=6,marker=0x1234fffc,words={5:0xfffc}),dict(kind='negative_mapped',low=-2,high=0,words={-1:0xfffc}),dict(kind='zero_index',low=0,high=0,words={0:0xfffc}),dict(kind='descending_choice',low=4,high=10,words={5:0xfffc,8:0xfffc})]
    frontbase=dict(root=20,fat_count=2,dir_count=2,low=4,length=5,marker=0xfffc,words={})
    front=[frontbase|x for x in (dict(kind='primary_upper',words={17:0xfffc,8:0xfffc}),dict(kind='primary_lower',words={16:0xfffc}),dict(kind='fallback_upper',words={8:0xfffc}),dict(kind='fallback_lower',words={4:0xfffc}),dict(kind='none'),dict(kind='empty_primary',dir_count=0,words={8:0xfffc}),dict(kind='primary_zero_fallback',root=1,fat_count=0,dir_count=1,words={0:0xfffc,8:0xfffc}),dict(kind='primary_zero_rejected',root=1,fat_count=0,dir_count=1,words={0:0xfffc}),dict(kind='fallback_zero_rejected',dir_count=0,low=0,length=1,words={0:0xfffc}))]
    contbase=dict(low=4,length=8,need=2,words={})
    blocks=[contbase|x for x in (dict(kind='zero_need',need=0,words={11:0xfffc}),dict(kind='one',need=1,words={11:0xfffc}),dict(kind='two',words={11:0xfffc,10:0xfffc}),dict(kind='skip_gap',words={11:0xfffc,7:0xfffc,6:0xfffc}),dict(kind='three',need=3,words={11:0xfffc,10:0xfffc,7:0xfffc,6:0xfffc,5:0xfffc}),dict(kind='none'),dict(kind='one_at_lower',words={4:0xfffc}),dict(kind='span_below_pool',words={4:0xfffc,3:0xfffc}),dict(kind='empty_pool',length=0,words={3:0xfffc}))]
    for channel,endian in itertools.product((0,7),(0,1)):
        base=Session(ram,channel=channel,endian=endian);order='big' if endian else 'little'
        def execute(entry,args,label,words,frame,fields=None,wanted_result=None,scan_visits=None,probe_visits=None,reader_visits=None):
            m,s,f,c=fresh(base,entry,[channel]+list(args),mode,sr);put(m,base.descriptor+0x24,FAT);raw(m,FAT-8,(0x7777).to_bytes(2,order)*520)
            for offset,value in (fields or {}).items():put(m,base.output+offset,value)
            for index,value in words.items():raw(m,FAT+index*2,(value&65535).to_bytes(2,order))
            before=bytes(m);sp=s.r[15];regs=list(s.r);pr=s.pr
            r,a,s,f=e.compare(m,s,f,c,dict(kind=label,channel=channel,endian=endian,mode=mode,arguments=[hex(x&0xffffffff) for x in args],fields=fields or {},words=words))
            wanted=bytearray(before);raw(wanted,sp-frame,v.sample(a,sp-frame,frame));e.require(a==wanted,'all nonstack RAM unchanged;only original bounded nested frame')
            e.require(s.r[0]==(wanted_result&0xffffffff),'independent allocation/control result')
            if scan_visits is not None:
                calls=[b['arguments'][:2] for b in r['boundaries'] if b['target'] in ('0x8c66080c','0x8c660816')]
                visits=scan_visits+(probe_visits or []) if entry!=CONTIG else reader_visits
                if visits is not None:e.require(calls==[[FAT,(i<<1)&0xffffffff] for i in visits],'exact descending SDK reader order')
            if entry==CONTIG:
                probes=[b['arguments'][:2] for b in r['boundaries'] if b['target']=='0x8c64b1c0'];e.require(probes==[[channel,i&0xffffffff] for i in probe_visits],'independent contiguous candidate probes')
                scans=[b for b in r['boundaries'] if b['target']=='0x8c6489d4'];e.require(sum(max(0,(signed(b['arguments'][3])-signed(b['arguments'][2])+1)) for b in scans)>=len(scan_visits),'bounded real descending searches')
            e.require(not r['cache_purge_sequence'] and not any('write_request_input' in b for b in r['boundaries']),'no invented cache/I/O side effects')
            e.require(r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'] and (s.sr&~1)==(sr&~1),'unaffected special andstatus state')
        for case,(mode,sr) in itertools.product(lower,PROFILES):
            marker=case.get('marker',0xfffc);result,visits=scan(marker,case['low'],case['high'],case['words']);execute(SCAN,[marker,case['low'],case['high']],'scan_'+case['kind'],case['words'],24,wanted_result=result,scan_visits=visits)
        for case,(mode,sr) in itertools.product(front,PROFILES):
            result,visits=alloc(case);fields={8:case['root'],16:case['fat_count'],24:case['dir_count'],28:case['low'],32:case['length']};execute(ALLOC,[case['marker']],'alloc_'+case['kind'],case['words'],44,fields,result,visits)
        for case,(mode,sr) in itertools.product(blocks,PROFILES):
            result,visits,probes,reader_order=contiguous(case);fields={16:case['need'],28:case['low'],32:case['length']};execute(CONTIG,[],'contiguous_'+case['kind'],case['words'],56,fields,result,visits,probes,reader_order)
        for count,(mode,sr) in itertools.product((0,1,2,3),PROFILES):
            words={507-i:(0xfffa if i==count-1 else 506-i) for i in range(count)};execute(DIRECTORY,[],f'clean_directory_{count}',words,52,{20:507,24:count},0)
        for entry,(mode,sr) in itertools.product((FAT_SAVE,FAT_PERSIST),PROFILES):execute(entry,[],f'zero_FAT_{entry:x}',{},64,{16:0},0)
        print('PERSIST DIRECT',channel,endian,len(e.records),flush=True)
    e.report('movie_filesystem_persist_direct_verification.json',{SCAN,ALLOC,CONTIG,DIRECTORY,FAT_SAVE,FAT_PERSIST},[Path(__file__),P/'analysis/movie-filesystem-directory/8c64b1c0_instructions.tsv'],scope='Independent descending lowest/highest bounds/low16 marker andtwo-region allocation/contiguous-gap oracles. Original SDK constructor/endianness/readers/FATgetter execute. Zero need andspanbelowpool retainedasoriginalbehavior,mappedABIcasesnot validcardformatclaims. Exactwhole nonstackRAM/frame bounds (directory44+getter8=52)/FPU/calleeGPR/specialstate;allclean/zerocount directory andzeroFAT synchronous controls create noI/O. Doesnotqualify asynchronous writer paths alone;no suppliedcalleeoutputs/frames/results.')

def signed(x):return x if x<0x80000000 else x-0x100000000
if __name__=='__main__':main()