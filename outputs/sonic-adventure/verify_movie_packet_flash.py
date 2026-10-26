"""Original settings/packet execution with real host flash bytes, not helper stubs."""
import collections,hashlib,itertools,json
from pathlib import Path
import write_watch_packet_flash as fast
v=fast.old;P=v.P;ROOT=v.ROOT
START=0x1c000;COOKIE=b'KATANA_FLASH____'

def crc(data):
 value=0xffff
 for byte in data:
  value^=byte<<8
  for _ in range(8):value=((value<<1)^(0x1021 if value&0x8000 else 0))&0xffff
 return value^0xffff

def record(index,payload):
 assert len(payload)==60
 data=index.to_bytes(2,'little')+payload
 return data+crc(data).to_bytes(2,'little')

def bitmap(used):
 bits=bytearray([255])*64
 for i in range(used):bits[i//8]&=~(0x80>>(i%8))
 return bytes(bits)

def profile(capacity=254,records=(),version=0):
 size=(capacity+2)*64;parts=list(v.STANDARD_PARTITIONS);parts[2]=(START,size)
 header=bytearray([255])*64;header[:16]=COOKIE;header[16]=2;header[17]=version
 patches=[(START,list(header))]+[(START+64*(i+1),list(r)) for i,r in enumerate(records)]+[(START+64*(capacity+1),list(bitmap(len(records))))]
 return dict(flash_partitions=parts,flash_patches=patches)

def image(c):return bytearray(v.console_model.inputs(c)[1])

def cases():
 base=dict(entry=v.PACKET,arg5=v.BUFFER,console_info=True,console_flash=True,prime_console=True,full_flash_storage=True,flash_writable=True,runtime_attached=True,mode=0)
 payload=bytes((i*7+19)&255 for i in range(60));existing=profile(records=[record(5,payload)])
 for mask in range(32):yield base|existing|dict(kind='selective-update',arg4=mask)
 for mode,mask in itertools.product((1,4),(0,1,16,31)):
  yield base|existing|dict(kind='selective-update',arg4=mask,mode=mode)
 for mode,mask in itertools.product((0,1,4),(0,31)):
  yield base|dict(kind='blank-image',arg4=mask,mode=mode)
 full=[record(i,bytes(((i*11+j*7+1)&255) for j in range(60))) for i in range(14)]
 for mode in (0,1,4):
  yield base|profile(14,full)|dict(kind='compact-full',arg4=31,mode=mode)
  sparse=list(full);sparse[0]=record(0,bytes([255])*60)
  yield base|profile(14,sparse)|dict(kind='compact-drop-empty',arg4=16,mode=mode)
 for kind,extra in (
  ('readonly-blank',dict(flash_writable=False,arg4=0)),
  ('readonly-update',existing|dict(flash_writable=False,arg4=1)),
  ('readonly-full',profile(14,full)|dict(flash_writable=False,arg4=31)),
  ('small-partition',profile(2)|dict(arg4=0)),
  ('wrong-version',profile(version=1)|dict(arg4=0)),
  ('uninitialized-console',dict(prime_console=False,arg4=0)),
  ('invalid-buffer',dict(arg5=0,arg4=0)),
  ('legacy-platform',dict(full_flash_storage=False,prime_console=False,arg4=0)),
 ):
  yield base|extra|dict(kind=kind)
 # Original word-copy leaf: signed truncation, whole-word count and forward
 # overlap behavior. This body was not in the original registry.
 for count,delta,mode in itertools.product((-2147483648,-5,-4,-3,-1,0,1,3,4,5,7,8,15,16,63,64,65,67),(-4,0,4,0x4000),(0,1,4)):
  yield dict(kind='word-copy',entry=0x8c603c08,arg4=v.DATA,arg5=v.DATA+delta,arg6=count&0xffffffff,mode=mode)
 # Real error/status paths and reused byte/bit/CRC bodies remain visible in
 # the same integration. These cases do not supply a callee's return value.
 for entry,a,b,n,extra in (
  (v.READ,0,v.DEST,0,{}),(v.READ,4,v.DEST,0,{}),(v.READ,0xffffffff,v.DEST,0,{}),(v.READ,0,v.DEST,0,dict(operation=1)),(v.READ,0,v.DEST,0,dict(packet_context=False)),
  (v.WRITE,0,v.DEST,0,{}),(v.WRITE,4,v.DEST,0,{}),(v.WRITE,0,v.DEST,0,dict(operation=2)),(v.WRITE,0,v.DEST,0,dict(packet_context=False)),
  (v.POLL,0,0,0,{}),(v.POLL,0,0,0,dict(operation=3)),(v.POLL,0,0,0,dict(operation=1)),(v.POLL,0,0,0,dict(operation=1,version=1)),
  (v.OPEN,0,v.BUFFER,0,{}),(v.OPEN,5,v.BUFFER,0,{}),(v.FORMAT,2,0,0,{}),(v.FORMAT,0,0,0,dict(packet_context=False)),
  (0x8c603fe4,v.PKTCTX,0,0,dict(retry=3)),(0x8c603fe4,v.PKTCTX,0,0,dict(retry=0,used=4)),(0x8c603fe4,v.PKTCTX,0,0,dict(retry=2,version=1)),
  (0x8c603ec4,v.PKTCTX,0,0,dict(version=1)),(0x8c603ec4,v.PKTCTX,0,0,dict(used=4)),(0x8c603e08,v.PKTCTX,0,0,dict(version=1)),
 ):
  for mode in (0,1,4):yield dict(kind='original-control',entry=entry,arg4=a,arg5=b,arg6=n,mode=mode)|extra
 for entry,a,b,n in (
  (0x8c603c2e,v.DATA,v.DEST,0),(0x8c603c2e,v.DATA,v.DEST,17),(0x8c603c38,v.DEST,17,0),
  (0x8c603bfc,v.DATA,v.DATA,64),(0x8c603bfc,v.DATA,v.DATA+1,64),
  (0x8c603c4c,v.BUFFER,1,0),(0x8c603c4c,v.BUFFER,0,0),
  (0x8c603c84,0,v.DEST,1),(0x8c603c84,7,v.DEST,1),(0x8c603c84,8,v.DEST,1),(0x8c603c84,513,v.DEST,1),
  (0x8c64bda0,0,v.DATA,0),(0x8c64bda0,62,v.DATA,0),
  (0x8c603cd2,v.DATA,0,0),(0x8c603f8c,v.PKTCTX,0,0),
 ):
  yield dict(kind='reused-leaf',entry=entry,arg4=a,arg5=b,arg6=n,mode=0)

def semantics(m,a,before,s,r,path,c):
 checks=2;assert r['full_state_match'] and r['ordered_boundaries_match']
 kind=c['kind']
 if kind=='word-copy':
  signed=c['arg6'] if c['arg6']<0x80000000 else c['arg6']-0x100000000
  words=max(0,signed//4 if signed>=0 else -((-signed)//4))
  expected=bytearray(m);source=before.r[4]&0xffffff;target=before.r[5]&0xffffff
  for i in range(words):expected[target+i*4:target+i*4+4]=expected[source+i*4:source+i*4+4]
  assert a==expected and r['status']==0 and s.pc==before.pr and s.pr==before.pr;checks+=4
  assert s.r[4]==(before.r[4]+words*4)&0xffffffff and s.r[5]==(before.r[5]+words*4)&0xffffffff;checks+=2
 elif kind in ('selective-update','blank-image','compact-full','compact-drop-empty'):
  assert r['status']==0 and s.pc==before.pr and s.pr==before.pr and s.r[0]==0;checks+=4
  initial=image(c);mask=c['arg4'];info=v.console_model.inputs(c)[0][16:24]
  start,size=c.get('flash_partitions',v.STANDARD_PARTITIONS)[2]
  capacity=size//64-2
  if kind=='blank-image':
   header=COOKIE+bytes((2,0));initial[start:start+18]=header
   payload=bytearray([255])*60;used=0
  elif kind.startswith('compact'):
   payload=bytearray((5*11+j*7+1)&255 for j in range(60));used=14
  else:payload=bytearray((j*7+19)&255 for j in range(60));used=1
  if not mask:
   assert a[0x78:0x80]==payload[:8];checks+=1
  else:
   assert a[0x78:0x80]==info;checks+=1
   for bit,indices in enumerate(((7,),(6,),(5,),(4,),(0,1,2,3))):
    if mask&(1<<bit):
     for index in indices:payload[index]=info[index]
   if kind.startswith('compact'):
    header=initial[start:start+64];initial[start:start+size]=bytes([255])*size;initial[start:start+64]=header
    surviving=[]
    for index in range(14):
     if index==0 and kind=='compact-drop-empty':continue
     data=bytes(payload) if index==5 else bytes((index*11+j*7+1)&255 for j in range(60))
     surviving.append(record(index,data))
    for i,data in enumerate(surviving):initial[start+(i+1)*64:start+(i+2)*64]=data
    initial[start+(capacity+1)*64:start+size]=bitmap(len(surviving))
    # Two genuine pending stages, then compaction completion. No shortened poll.
    assert sum(b['target']==hex(v.POLL) for b in r['boundaries'])==4;checks+=1
    assert r['flash_storage_snapshot'][-1]==size;checks+=1
   else:
    initial[start+(used+1)*64:start+(used+2)*64]=record(5,bytes(payload))
    initial[start+(capacity+1)*64:start+size]=bitmap(used+1)
  assert r['flash_storage_sha256']==hashlib.sha256(initial).hexdigest();checks+=1
 elif kind in ('readonly-blank','readonly-update','readonly-full','small-partition','wrong-version'):
  assert r['status']==0 and s.r[0]==0xffffffff and s.pc==before.pr;checks+=3
  assert r['flash_storage_sha256']==hashlib.sha256(image(c)).hexdigest();checks+=1
 elif kind=='uninitialized-console':
  assert r['status']==-4 and r['stop_pc']==hex(v.console_model.SYSINFO);checks+=2
 elif kind=='invalid-buffer':
  assert r['status']==-3 and r['stop_pc']=='0x8c603c40' and r['flash_storage_snapshot'][-2:]==(18,0x4000);checks+=3
 elif kind=='legacy-platform':
  assert r['status']==-8 and r['stop_pc']==hex(v.console_model.FLASH) and r['flash_storage_snapshot'] is None;checks+=3
 return checks

def main():
 ram=v.seed_ram();records=[];coverage=set();edges=set();checks=0
 for i,case in enumerate(cases(),1):
  c=v.Input(case);m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s)
  r,path,a,s,f=fast.compare(m,s,f,c);count=semantics(m,a,before,s,r,path,c);checks+=count
  coverage.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(kind=c['kind'],input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=hex(s.r[0]),steps=len(path),boundaries=r['boundary_comparisons'],flash_storage_snapshot=r['flash_storage_snapshot'],flash_storage_sha256=r['flash_storage_sha256'],independent_checks=count))
  print(i,c['kind'],r['status'],r['stop_pc'],flush=True)
 coverage_rows=[]
 for definition in v.manifest['routines']:
  body=set(v.PER_BODY[int(definition['address'],16)]);wanted=set(map(tuple,definition['conditional_edges']));actual={(hex(a),hex(b)) for a,b in edges}
  coverage_rows.append(dict(address=definition['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(a) for a in sorted(body-coverage)],conditional_outcomes=len(wanted),covered_outcomes=len(wanted&actual),missing_outcomes=[list(x) for x in sorted(wanted-actual)]))
 copy=next(r for r in coverage_rows if r['address']=='0x8c603c08');assert copy['static_rows']==copy['covered_rows']==14 and copy['covered_outcomes']==copy['conditional_outcomes']==4
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),P/'movie_packet_startup_reference.py',P/'flash_storage_reference.py',P/'analysis/movie_packet_startup_translation.json',P/'analysis/movie_packet_flash_connection.json',P/'analysis/flash_storage_verification.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage_rows,compiled_source_count=196,newly_registered_entries=0,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Fresh actual original/native packet/settings execution. Explicit RAM/platform fixtures,mutable flash bytes and declared partition metadata. Optional direct fixture priming invokes the authored console init service before either run and before memory snapshots; full-startup profiles must not use it. Independent whole-image/CRC/bit-selection/append/compaction/readonly/error/forward-overlap oracles supplement full CPU/FPU/RAM/platform/Sound/runtime/boundary/flash comparisons. Two actual pending write phases precede compaction; no helper result/checkpoint/shortenedcopy/completion flag. Measured missing rows/outcomes remain unqualified; no original routine credit in this report. Research write poll guard admits both pending phases; production remains unbounded. No full startup/reset/gameplay/physicalflash claim.')
 (P/'analysis/movie_packet_flash_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Packet flash comparisons',len(records),'checks',checks,'covered',sum(x['covered_rows'] for x in coverage_rows),'of',sum(x['static_rows'] for x in coverage_rows))
if __name__=='__main__':main()
