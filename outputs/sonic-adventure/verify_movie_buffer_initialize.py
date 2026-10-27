"""Original buffer initialization, real CPU/DMA providers and independent data oracle."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_buffer_initialize as fast
import vram_aperture_reference as aperture
v=fast.old;P=v.P;ROOT=v.ROOT
INIT,DMA,MIP,VQ,WAIT=0x8c65393a,0x8c6534e0,0x8c6544a0,0x8c654520,0x8c651000
DESC,SOURCE=0x8cdb0000,0x8cd00000
TABLE,ENQUEUE,SEQUENCE,ISSUED,ACTIVE,QUEUE=0x8c8a2b38,0x8c8a2d38,0x8c8a2d40,0x8c8a2d44,0x8c8a2d4c,0x8c8a2d50
MIP_TOTAL=(0,0,0,172,684,2732,10924,43692,174764,699052,2796204)
VQ_CODE=(0,0,0,128,128,256,1024,0,0,0,128)
VQ_MIP_CODE=(0,0,0,128,128,512,2048,6,8,16,48)
VQ_BODY=(0,1,4,16,64,256,1024,4096,16384,65536,262144)
VQ_MIP_BODY=(1,2,6,22,86,342,1366,5462,21846,87382,349526)

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def case(entry=INIT,kind=2,flags=0,width=8,length=64,offset=0x4000,aligned=False,mask=0,**extra):
 source=SOURCE if aligned else SOURCE+4
 args=[DESC,source,extra.get('option6',0),extra.get('option7',0)] if entry==INIT else [offset,source,width,flags] if entry in (MIP,VQ) else [offset,source,length,0] if entry==DMA else [0x12345678]
 return dict(entry=entry,mode=extra.pop('mode',0),initialize_arguments=args,buffer_kind=kind,buffer_flags=flags,buffer_width=width,buffer_length=length,buffer_offset=offset,buffer_source=source,buffer_mask=mask,dma_operation=0x8201,buffer_wait_busy=extra.pop('wait_busy',False))|extra

def prepare(ram,c):
 m,s,f=v.prepare(ram,c);s.sr=(s.sr&~0xf0)|c['buffer_mask']
 # Explicit empty, coherent submission queues. No completion state is supplied.
 for queue in range(2):
  v.store(m,ENQUEUE+queue*4,0);v.store(m,ISSUED+queue*4,7)
  for slot in range(8):v.store(m,TABLE+queue*256+slot*32+24,0)
 v.store(m,ACTIVE,7);v.store(m,QUEUE,0);v.store(m,0x8c8a317c,0)
 for j,x in enumerate((c['buffer_kind'],1,0,c['buffer_width'],c['buffer_width'],c['buffer_length'],c['buffer_flags'],c['buffer_offset'])):v.store(m,DESC+4*j,x)
 m[SOURCE&0xffffff:(SOURCE&0xffffff)+0x10000]=bytes(((i*73)^(i>>3)^0x5a)&255 for i in range(0x10000))
 if c['entry']==WAIT and c['buffer_wait_busy']:v.store(m,TABLE,0x12345678);v.store(m,TABLE+24,1)
 if c.get('null_descriptor'):s.r[4]=0
 return m,s,f

def segments(c):
 entry=c['entry'];flags=c['buffer_flags'];width=c['buffer_width'];kind=c['buffer_kind'];offset=c['buffer_offset'];source=c['buffer_source'];length=c['buffer_length']
 if entry==WAIT:return []
 if entry==INIT:
  if c.get('option6')==1 or c.get('option7')==1:return []
  entry=VQ if kind&0xffff==3 else MIP if flags&5==5 and not flags&0x28 else DMA
 if entry==DMA:return [(offset,source,length)]
 index=width.bit_length()-1
 if entry==MIP:return [(offset+4,source,MIP_TOTAL[index]+4)]
 code=(VQ_MIP_CODE if flags&1 else VQ_CODE)[index];body=(VQ_MIP_BODY[index]+10) if flags&1 else VQ_BODY[index]
 return [(offset,source,code),(offset+2048,source+code,body)]

def oracle(m,c):
 expected=bytearray(m);vram=bytearray(bytes(range(256))*(0x800000//256));writes=[];writebacks=[];dma_count=0;dma_bytes=0;active=False;last_token=None
 def copy(offset,source,count):
  assert count>=4 and not count&3
  for i in range(0,count,4):
   address=0xa4000000+offset+i;mapped=aperture.mapping(address);value=v.load(m,source+i);v.store(vram,mapped-0xa5000000,value);writes.append([address,mapped,value,2])
 for offset,source,length in segments(c):
  fallback=bool(offset&31 or source&31 or c['buffer_mask'])
  if fallback:copy(offset,source,(length//4)*4);continue
  writebacks.extend(range(source,source+length+1,32));bulk=length&~31;tail=length&31
  if bulk:
   queue=1;slot=v.load(expected,ENQUEUE+4);node=TABLE+256+slot*32;flags=0x19
   for field,value in ((0,v.load(expected,SEQUENCE)),(4,source),(12,0x11000000+offset),(20,bulk),(24,flags)):v.store(expected,node+field,value)
   v.store(expected,ENQUEUE+4,(slot+1)&7);v.store(expected,SEQUENCE,(v.load(expected,SEQUENCE)+1)&0xffffffff)
   if not active:
    v.store(expected,node+24,0x1a);v.store(expected,ACTIVE,slot);v.store(expected,QUEUE,queue);v.store(expected,ISSUED+4,(v.load(expected,ISSUED+4)+1)&7);active=True
    copy(offset,source,bulk);dma_count+=1;dma_bytes+=bulk
   last_token=v.load(expected,node);v.store(expected,0x8c8a3178,last_token)
  else:last_token=0;v.store(expected,0x8c8a3178,0)
  if tail:copy(offset+bulk,source+bulk,tail)
 return expected,vram,writes,writebacks,dma_count,dma_bytes,last_token

def cases():
 for kind,flags in itertools.product((2,0xabcd0003),(0,1,4,5,13,37,0xffffffff)):
  yield case(kind=kind,flags=flags,width=8)
 for width,flags,entry in itertools.product((8,16,32,64),(0,1),(VQ,)):
  yield case(entry,width=width,flags=flags)
 for width in (8,16,32,64,128):yield case(MIP,width=width)
 for aligned,mask,offset,length in itertools.product((False,True),(0,0xf0),(0x4000,0x4004),(4,32,36,64,68)):
  yield case(DMA,aligned=aligned,mask=mask,offset=offset,length=length)
 yield case(DMA,aligned=True,length=0)
 for kind,flags in ((2,0),(3,0),(3,1)):yield case(kind=kind,flags=flags,aligned=True)
 for option6,option7 in ((1,0),(0,1),(1,1)):yield case(option6=option6,option7=option7,null_descriptor=True)
 for busy in (False,True):yield case(WAIT,wait_busy=busy)
 for mode in (1,4):yield case(kind=3,flags=1,width=16,mode=mode)

def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 for address,values in ((0x8c671c08,MIP_TOTAL),(0x8c671b78,VQ_CODE),(0x8c671b94,VQ_MIP_CODE),(0x8c671c60,VQ_BODY),(0x8c671c8c,VQ_MIP_BODY)):assert tuple(v.load(ram,address+i*4) for i in range(11))==values
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);expected,vram,writes,writebacks,dma_count,dma_bytes,token=oracle(m,c)
  r,path,a,s,f=fast.compare(m,s,f,c);busy=c['entry']==WAIT and c['buffer_wait_busy'];status=-4 if busy else 0
  result=4 if c['entry']==INIT and (c.get('option6')==1 or c.get('option7')==1) else 0 if c['entry']==INIT else 1
  assert r['full_state_match'] and r['ordered_boundaries_match'] and r['status']==status,(i,c,r['status'],r['stop_pc']);n=3
  if not busy:assert s.r[0]==result and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]);n+=4
  else:assert r['stop_pc']=='0x8c65100c' and s.r[15]==before.r[15]-8 and s.pr==0x8c65100a;n+=3
  assert bytes(f)==fp and r['vram_sha256']==hashlib.sha256(vram).hexdigest() and r['aperture_events']==writes;n+=3
  assert r['cache_writeback_sequence']==writebacks and not r['aperture_state'][1];n+=2
  for address,size in ((TABLE,512),(ENQUEUE,24),(DESC,32),(SOURCE,0x10000),(0x8c8a3178,4)):
   assert v.sample(a,address,size)==v.sample(expected,address,size),(i,c,hex(address));n+=1
  device=r['pvr_dma_state'];assert device[5]==device[6]==dma_count and device[4]==dma_bytes and device[7]==(1<<19 if dma_count else 0);n+=4
  assert not any(r['sound_state']) and r['runtime_state'] is None;n+=2;checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   ins=v.ROWS.get(pc,'');distance=2 if ins.startswith(('bt/s ','bf/s ')) else 1
   if ins.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],r0=s.r[0],steps=len(path),independent_checks=n,device_completed_transfers=dma_count,device_bytes=dma_bytes));print(i,hex(c['entry']),r['status'],dma_count,flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  wanted=set(body['reachable_addresses']);wanted_edges=set(map(tuple,body['conditional_edges']));actual={hex(a) for a in seen};actual_edges={(hex(a),hex(b)) for a,b in edges}
  coverage.append(dict(address=body['address'],static_rows=len(wanted),covered_rows=len(wanted&actual),conditional_outcomes=len(wanted_edges),covered_outcomes=len(wanted_edges&actual_edges),missing_rows=sorted(wanted-actual),missing_outcomes=[list(e) for e in sorted(wanted_edges-actual_edges)]))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),P/'vram_aperture_reference.py',P/'pvr_dma_reference.py',P/'buffer_transfer_reference_cpu.py',v.MP,P/'analysis/movie_buffer_initialize_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original initialization/ordinary/mip/VQ providers andwait withindependententireVRAM/source/descriptor/submissionqueue/cachetrace/device-count oracles. Explicitcoherent emptySDKqueues;actualenqueue/kick/channel2 copy/completionhardwarepending;asynchronouswork remainsactiveuntiloriginalISRretirement. MixedCPUremainder/pendingDMA ordering retained. No gamequeuecompletion/helperresult injection. Waitcompleted-token success andbusy researchstop arechecked,notclaimedend-to-endbusy-to-completeIRQlifecycle. No physicaltiming/gameplay claim;separatequalification required.')
 (P/'analysis/movie_buffer_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Buffer direct',len(records),checks,[(x['address'],x['covered_rows'],x['static_rows'],x['covered_outcomes'],x['conditional_outcomes']) for x in coverage])
if __name__=='__main__':main()
