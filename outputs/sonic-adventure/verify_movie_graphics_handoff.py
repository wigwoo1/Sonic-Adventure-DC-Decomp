"""Original graphics-memory queries with independent linked-list accounting."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_graphics_handoff as fast
v=fast.old;P=v.P;ROOT=v.ROOT
QUERY,PAIR,COLLECT=v.ACCOUNT_QUERY,v.ACCOUNT_PAIR,v.ACCOUNT_COLLECT
META,POOL,CACHE_HEAD,CACHE=0x8c67c2e0,0x8c8a83e4,0x8c67c308,0x8c8c03e4
OUT,GLOBAL=0x8cdb0000,0x8c88f7a4
TYPES={0x1000:(16,144),0x2000:(16,160),0x3000:(16,192),0x4000:(16,224),0x5000:(8,512),0x6000:(4,864),0x7000:(2,2048),0x8000:(1,3424)}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def signed(x):return x-(1<<32) if x&0x80000000 else x
def store(m,a,value):v.store(m,a,value&0xffffffff)
def patch(m,a,data):m[a&0xffffff:(a&0xffffff)+len(data)]=data
def case(entry=COLLECT,**extra):
 data=dict(entry=entry,mode=extra.pop('fpu_mode',0),account_arguments=[extra.pop('selector',0)] if entry==QUERY else extra.pop('outputs',[OUT+i*16 for i in range(4 if entry==COLLECT else 2)]),account_free=[[],[]],account_busy=[[],[]],account_cache=[],account_global=0x76543210)
 return data|extra
def prepare(ram,c):
 m,s,f=v.prepare(ram,c);patch(m,META,bytes([0xa5])*40);store(m,CACHE_HEAD,0);patch(m,OUT,bytes([0x77])*128);store(m,GLOBAL,c['account_global'])
 for group,(offset,items,is_busy) in enumerate(((8,c['account_free'][0],False),(0,c['account_busy'][0],True),(28,c['account_free'][1],False),(20,c['account_busy'][1],True))):
  nodes=[POOL+(group*4+i)*24 for i in range(len(items))];assert len(items)<=4;store(m,META+offset,nodes[0] if nodes else 0)
  for i,(node,item) in enumerate(zip(nodes,items)):
   size,flags=item if is_busy else (item,1);patch(m,node,bytes([0x5a])*24);store(m,node,flags);store(m,node+8,nodes[i+1] if i+1<len(nodes) else 0);store(m,node+16,size)
 for i,record in enumerate(c['account_cache']):
  node=CACHE+i*76;patch(m,node,bytes([0xa5])*76);store(m,node,record['tag']);store(m,node+4,CACHE+(i+1)*76 if i+1<len(c['account_cache']) else 0)
  slots=record['slots'];assert len(slots)==16
  for j,value in enumerate(slots):store(m,node+12+j*4,value)
 if c['account_cache']:store(m,CACHE_HEAD,CACHE)
 if c.get('account_bad_lists'):
  for offset in (0,8,20,28):store(m,META+offset,0x8cfef000)
  store(m,CACHE_HEAD,0x8cfef000)
 return m,s,f
def accounting(m):
 totals=[];maxima=[]
 for bank in range(2):
  total=0;maximum=0
  for offset,filtered in ((8,False),(0,True)):
   node=v.load(m,META+20*bank+offset);visits=set()
   while node:
    assert node not in visits and len(visits)<32;visits.add(node)
    if not filtered or not v.load(m,node)&4:
     size=v.load(m,node+16);total=(total+size)&0xffffffff
     if signed(size)>signed(maximum):maximum=size
    node=v.load(m,node+8)
  totals.append(total);maxima.append(maximum)
 cache_total=0;node=v.load(m,CACHE_HEAD);visits=set()
 while node:
  assert node not in visits and len(visits)<32;visits.add(node);count,size=TYPES.get(v.load(m,node)&0xf000,(0,0))
  cache_total=(cache_total+size*sum(v.load(m,node+12+i*4)==0 for i in range(count)))&0xffffffff;node=v.load(m,node+4)
 return [totals[0],totals[1],cache_total,max(maxima,key=signed)]
def cases():
 frees=([],[32],[64,16,32],[0,0xffffffff,0x80000000,0x7fffffff]);busy_lists=([],[(64,0),(128,4),(16,0)],[(8,4),(0xfffffff0,0xffff)])
 for free,used,swap in itertools.product(frees,busy_lists,(False,True)):
  a,b=(free,[17,17]) if not swap else ([17,17],free)
  yield case(account_free=[a,b],account_busy=[used,list(reversed(used))])
 for tag,pattern,flags in itertools.product(TYPES,('empty','full','mixed'),(0,15)):
  slots=[0 if pattern=='empty' or pattern=='mixed' and i%2==0 else 0x8ce00000+i*32 for i in range(16)]
  yield case(account_cache=[dict(tag=tag|flags,slots=slots)])
 for tag in (0,0x9000,0xffff):yield case(account_cache=[dict(tag=tag,slots=[0]*16)])
 chain=[dict(tag=0x1000,slots=[0]*16),dict(tag=0x5000,slots=[1 if i%3 else 0 for i in range(16)]),dict(tag=0x8000,slots=[0]*16)]
 yield case(account_cache=chain,account_free=[[0xffffffff,17],[64]],account_busy=[[(0x7fffffff,0),(16,4)],[(32,0)]])
 for entry,mode in itertools.product((COLLECT,PAIR,QUERY),(0,1,4)):
  yield case(entry,fpu_mode=mode,account_cache=chain,account_free=[[32,64],[128]],account_busy=[[(8,0),(16,4)],[(256,0)]])
 for selector,value,nonempty in itertools.product((0,1,2,3,0xffffffff),(0,0xffffffff),(False,True)):
  yield case(QUERY,selector=selector,account_global=value,account_free=[[32,64],[128]] if nonempty else [[],[]],account_cache=chain if nonempty else [])
 for entry in (COLLECT,PAIR):
  count=4 if entry==COLLECT else 2
  for outputs in ([OUT]*count,[META+8+i*4 for i in range(count)],[0xa5000040+i*16 for i in range(count)]):yield case(entry,outputs=outputs,account_free=[[32],[64]],account_cache=chain)
  for alias,offset in itertools.product((0x05000000,0x85000000,0xa5000000),(0,0x7ffff0)):
   yield case(entry,outputs=[alias+offset+i*4 for i in range(count)],account_free=[[32],[64]],account_cache=chain)
  for bad in range(count):
   outputs=[OUT+i*16 for i in range(count)];outputs[bad]=0
   yield case(entry,outputs=outputs,account_free=[[32],[64]],account_cache=chain,account_bad_output=bad)
  for invalid in (0xa5800000,0xa4fffffc,0xa4000000):yield case(entry,outputs=[invalid]+[OUT+i*16 for i in range(1,count)],account_free=[[32],[64]],account_cache=chain,account_bad_output=0)
 for mode in (0,4):yield case(QUERY,selector=2,account_global=0x12345678,account_bad_lists=True,fpu_mode=mode)
def semantic(m,a,before,fp,s,f,r,c):
 assert r['full_state_match'] and r['ordered_boundaries_match'];n=2;entry=c['entry'];expected=bytearray(m);events=[];stops={COLLECT:[0x8c663c56,0x8c663c5e,0x8c663c62,0x8c663c74],PAIR:[0x8c653de8,0x8c653dee]}
 if entry==QUERY and c['account_arguments'][0]==2:
  result=c['account_global'];assert not any(b['target'] in (hex(PAIR),hex(COLLECT)) for b in r['boundaries']);n+=1
 else:
  values=accounting(m)
  if entry==QUERY:result=(sum(values[:3])&0xffffffff) if c['account_arguments'][0]==0 else values[3]
  else:
   values=values if entry==COLLECT else [sum(values[:3])&0xffffffff,values[3]];result=0
   for i,(address,value) in enumerate(zip(c['account_arguments'],values)):
    if i==c.get('account_bad_output'):
     assert r['status']==-3 and r['stop_pc']==hex(stops[entry][i]);n+=2;break
    if address&0x1f000000==0x0c000000:store(expected,address,value)
    else:events.append([14,address,value,4])
  if entry==QUERY:assert sum(b['target']==hex(PAIR) for b in r['boundaries'])==1;n+=1
 if c.get('account_bad_output') is None:
  assert r['status']==0 and s.r[0]==result and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;n+=5
 for address,size in ((META,40),(POOL,4*4*24),(CACHE_HEAD,4),(CACHE,max(1,len(c['account_cache']))*76),(OUT,128),(GLOBAL,4)):
  assert v.sample(a,address,size)==v.sample(expected,address,size),(hex(entry),hex(address),c);n+=1
 assert r['vram_writes']==events and not any(r['sound_state']) and r['runtime_state'] is None;n+=3
 return n
def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fast.compare(m,s,f,c);n=semantic(m,a,before,fp,s,f,r,c);checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   ins=v.ROWS.get(pc,'');distance=2 if ins.startswith(('bt/s ','bf/s ')) else 1
   if ins.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=s.r[0],steps=len(path),independent_checks=n));print(i,hex(c['entry']),r['status'],flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  wanted=set(body['reachable_addresses']);wanted_edges=set(map(tuple,body['conditional_edges']));actual={hex(a) for a in seen};actual_edges={(hex(a),hex(b)) for a,b in edges}
  coverage.append(dict(address=body['address'],static_rows=len(wanted),covered_rows=len(wanted&actual),conditional_outcomes=len(wanted_edges),covered_outcomes=len(wanted_edges&actual_edges),missing_rows=sorted(wanted-actual),missing_outcomes=[list(e) for e in sorted(wanted_edges-actual_edges)]))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_graphics_handoff_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Three original accounting/query bodies with independent bank free/non-active totals,signed maxima,32bit sum wrap,eightcachetype slotcounts/block sizes,zero/nonzero fields,masking,selector/global result,output-alias ordering/VRAMwrites andactual partialinvalid-outputprefixes. Directfinitecoherentmappedmetadata fixtures;callee stack distinct frominputs/outputs;not a guestcheckpoint. FullCPU/FPU/RAM/VRAM/Sound/device/orderedboundaries compare. Allactualproviders execute,no substitutedcounts/returns. Coverage measured;no registration absentseparatequalification;not fullstartup/physicaltiming/gameplay.')
 (P/'analysis/movie_graphics_handoff_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Accounting',len(records),checks,coverage)
if __name__=='__main__':main()
