"""Original heap creation/free comparisons and independent interval checks."""
import collections,hashlib,itertools,json
from pathlib import Path
import verify_movie_heap as v
INIT,FREE=0x8c602128,0x8c602288
FIXED_SENTINEL=0x8c88ef1c
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def cases():
 for mode,sr,offset,size in itertools.product((0,4),(0x400003f3,0x700003a2),(0,1,31),(0,32,0x201f)):
  yield dict(entry=INIT,mode=mode,sr=sr,pool=v.ARENA+0x1000+offset,pool_bytes=size)
 # Free nodes and the returned block are intentionally non-overlapping ranges.
 profiles=(('isolated',[(0x1000,4),(0x5000,5)],(0x3000,3)),
           ('upper_merge',[(0x1000,4),(0x3060,5)],(0x3000,3)),
           ('lower_merge',[(0x1000,4),(0x5000,5)],(0x1080,3)),
           ('both_merge',[(0x1000,4),(0x10e0,5)],(0x1080,3)),
           ('empty',[],(0x3000,3)),('below_all',[(0x1000,4),(0x5000,5)],(0x800,2)),
           ('above_all',[(0x1000,4),(0x5000,5)],(0x9000,3)))
 for mode,sr,profile,cursor in itertools.product((0,4),(0x400003f3,0x700003a2),profiles,(0,1,2)):
  name,nodes,freed=profile
  yield dict(entry=FREE,mode=mode,sr=sr,free_profile=name,nodes=nodes,freed=freed,cursor=cursor)
 for mode in (0,4):yield dict(entry=FREE,mode=mode,null_payload=True,sr=0x700003f3)
 for entry in (INIT,FREE):yield dict(entry=entry,mode=0,sr=0,pool=v.ARENA+0x1000,pool_bytes=64)
 yield dict(entry=INIT,mode=0,pool=0,pool_bytes=64)
 yield dict(entry=INIT,mode=0,pool=v.ARENA+0x1000,pool_bytes=64,bad_stack=True)
 yield dict(entry=FREE,mode=0,invalid_heap=True)
def prepare(ram,c):
 m,s,f=v.old.prepare(ram,c);s.pc=c['entry'];m[v.ARENA&0xffffff:(v.ARENA&0xffffff)+v.ARENA_SIZE]=bytes([0xa5])*v.ARENA_SIZE
 if c['entry']==INIT:
  s.r[4]=c.get('pool',v.ARENA+0x1000);s.r[5]=c.get('pool_bytes',64)
 else:
  sentinel=v.ARENA+0x8000;nodes=[(sentinel,0)]+[(v.ARENA+at,size) for at,size in c.get('nodes',[])]
  nodes=sorted(nodes)
  for i,(at,size) in enumerate(nodes):v.store(m,at,nodes[(i+1)%len(nodes)][0]);v.store(m,at+4,size)
  v.store(m,v.ROOT_WORD,nodes[c.get('cursor',0)%len(nodes)][0])
  offset,size=c.get('freed',(0x3000,3));header=v.ARENA+offset;v.store(m,header+4,size);s.r[4]=0 if c.get('null_payload') else header+32
  if c.get('invalid_heap'):v.store(m,v.ROOT_WORD,0)
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f
def live_nodes(memory):
 root=v.load(memory,v.ROOT_WORD);nodes=[];at=root
 while True:
  assert at not in {a for a,size in nodes},'Repeated node before returning to cursor'
  nodes.append((at,v.load(memory,at+4)));at=v.load(memory,at)
  if at==root:return sorted(nodes)
  assert len(nodes)<20,'Fixtures must terminate'
def semantics(m,a,before,s,record,c):
 if record['status']:return 0
 assert s.r[15]==before.r[15] and s.sr&0xf0==before.sr&0xf0;checks=2
 if c['entry']==INIT:
  aligned=(c['pool']+31)&0xffffffe0;size=c['pool_bytes']&0xffffffe0
  assert v.load(a,0x8c88ef40)==aligned and v.load(a,0x8c88ef48)==aligned and v.load(a,0x8c88ef44)==size
  assert v.load(a,v.ROOT_WORD)==FIXED_SENTINEL and v.load(a,FIXED_SENTINEL)==aligned and v.load(a,aligned)==FIXED_SENTINEL
  assert v.load(a,FIXED_SENTINEL+4)==0 and v.load(a,aligned+4)==size//32 and s.r[14]==before.r[14];checks+=9
 elif c.get('null_payload'):assert a==m;checks+=1
 else:
  sentinel=v.ARENA+0x8000;old=[(at,size) for at,size in live_nodes(m) if at!=sentinel]
  header=before.r[4]-32;units=v.load(m,header+4);intervals=sorted(old+[(header,units)]);merged=[]
  for at,size in intervals:
   if merged and merged[-1][0]+merged[-1][1]*32==at:
    prev,n=merged[-1];merged[-1]=(prev,n+size)
   else:merged.append((at,size))
  assert [(at,size) for at,size in live_nodes(a) if at!=sentinel]==merged
  assert sum(size for at,size in live_nodes(a))==sum(size for at,size in old)+units;checks+=2
 return checks
def main():
 ram=v.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 for c in cases():
  m,s,f=prepare(ram,c);before=v.effects.State.from_buffer_copy(s);record,path,a,s,f=v.compare(m,s,f,c)
  expected=-3 if c.get('bad_stack') or c.get('pool')==0 or c.get('invalid_heap') else (-4 if not c.get('sr',0x40000000)&0x40000000 else 0)
  assert record['status']==expected,(c,record['status'],record['stop_pc'])
  record['independent_checks']=semantics(m,a,before,s,record,c);records.append(record);coverage.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
 rows=[]
 for routine in v.manifest['routines']:
  if int(routine['address'],16) not in (INIT,FREE):continue
  body=set(v.PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(v.__file__),v.MP,v.P/'analysis/movie_heap_connection.json']
 report=dict(cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),independent_checks=sum(r['independent_checks'] for r in records),coverage_by_routine=rows,
             dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},cases=records,
             scope='Original initialization and release with explicit direct pools and circular lists. Independent interval coalescing checks isolated, predecessor, successor, both-neighbor and wraparound insertion, null release and faults. Direct pools do not initialize startup.')
 (v.P/'analysis/movie_heap_initialization_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('cases_passed','status_counts','independent_checks','coverage_by_routine')})
if __name__=='__main__':main()
