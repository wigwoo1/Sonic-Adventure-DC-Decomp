"""Close packet storage paths using actual bytes/ranges, never helper results."""
import hashlib,json
from pathlib import Path
import verify_movie_packet_flash as base
fast=base.fast;v=base.v;P=v.P

def word(address,value):return (address,list(value.to_bytes(4,'little')))

def cases():
 common=dict(console_flash=True,full_flash_storage=True,flash_writable=True,runtime_attached=True,arg4=v.PKTCTX,arg5=v.DEST,arg6=0)
 start=base.START
 patch_start=word(v.PKTCTX+4,start)
 def case(kind,entry,result,extra=None,patches=()):
  for mode in (0,1,4):
   c=common|dict(kind=kind,entry=entry,expected=result,mode=mode,packet_patches=[patch_start,*patches])
   if extra:c.update(extra)
   yield c
 parts=list(v.STANDARD_PARTITIONS);parts[2]=(0x1fff0,16)
 yield from case('open-read-range',v.OPEN,0xffffffff,dict(arg4=2,arg5=v.BUFFER,flash_partitions=parts))
 for kind,field,value in (('open-wrong-id',16,3),('open-wrong-version',17,2)):
  profile=base.profile();header=profile['flash_patches'][0][1];header[field]=value
  yield from case(kind,v.OPEN,0xfffffff8,profile|dict(arg4=2,arg5=v.BUFFER))
 yield from case('format-version-one',v.FORMAT,0xfffffffa,dict(arg4=1))
 parts=list(v.STANDARD_PARTITIONS);parts[2]=(start,8)
 yield from case('format-program-rejected',v.FORMAT,0xfffffffe,dict(arg4=0,flash_partitions=parts,flash_patches=[(start+8,[0]*10)]))
 yield from case('poll-null-context',v.POLL,0,dict(packet_context=False))
 yield from case('write-negative-index',v.WRITE,0xfffffffb,dict(arg4=0xffffffff))
 yield from case('fill-zero',0x8c603c38,None,dict(arg4=v.DEST,arg5=0))
 yield from case('init-header-range',0x8c603d22,0xffffffff,patches=[word(v.PKTCTX+4,0x1fff0)])
 yield from case('init-bitmap-range',0x8c603d22,0xffffffff,patches=[word(v.PKTCTX+4,0x1ff00)])
 yield from case('init-data-range',0x8c603d22,0xffffffff,dict(capacity=0,flash_patches=[(0x1ffc0,[0]*64)]),patches=[word(v.PKTCTX+4,0x1ff80)])
 yield from case('init-unused-record',0x8c603d22,0,dict(flash_patches=[(start+5*64,[0x7f]+[255]*63)]))
 bad=bytearray(base.record(0,bytes(range(60))));bad[-1]^=1
 yield from case('init-bad-crc',0x8c603d22,0,dict(flash_patches=[(start+64,list(bad)),(start+5*64,[0x7f]+[255]*63)]))
 yield from case('init-id-outside-capacity',0x8c603d22,0,dict(flash_patches=[(start+64,list(base.record(5,bytes(range(60))))),(start+5*64,[0x7f]+[255]*63)]))
 yield from case('read-bad-crc',0x8c603f8c,0xfffffffc,patches=[(v.BUFFER+64,list(bad))])
 yield from case('append-record-rejected',0x8c603ec4,0xfffffffe,dict(flash_patches=[(start+64,[0]*64)]))
 yield from case('compact-header-rejected',0x8c603e08,0xfffffffe,dict(flash_patches=[(start,[0]*64)]))
 yield from case('compact-no-logical-slots',0x8c603e08,0,dict(capacity=0))
 empty_bad=bytearray(base.record(2,bytes([255])*60));empty_bad[-1]^=1
 live=base.record(3,bytes(range(60)))
 data=[(v.BUFFER+128,list(base.record(1,bytes([255])*60))),(v.BUFFER+192,list(empty_bad)),(v.BUFFER+256,list(live))]
 yield from case('compact-skip-and-preserve',0x8c603e08,0,patches=data)
 yield from case('compact-record-rejected',0x8c603e08,0xfffffffe,dict(flash_patches=[(start+64,[0]*64)]),patches=[(v.BUFFER+64,list(live))])
 yield from case('compact-bitmap-rejected',0x8c603e08,0xfffffffe,dict(flash_patches=[(start+5*64,[0]*64)]),patches=[(v.BUFFER+64,list(live))])

def main():
 ram=v.seed_ram();records=[];coverage=set();edges=set();checks=0
 for c in map(v.Input,cases()):
  m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s)
  r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['status']==0 and s.pc==s.pr==before.pr;checks+=3
  if c['expected'] is not None:assert s.r[0]==c['expected'];checks+=1
  assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2
  if c['kind']=='append-record-rejected':
   assert r['flash_storage_snapshot'][-2]==64 and r['flash_storage_snapshot'][5][2]==2;checks+=2
   assert bytes(v.native_flash_bytes)[base.START+64:base.START+128]==bytes(64);checks+=1
   assert bytes(v.native_flash_bytes)[base.START+320:base.START+384]==base.bitmap(1);checks+=1
  if c['kind']=='compact-skip-and-preserve':
   wanted=base.image(c);wanted[base.START+64:base.START+128]=bytes(m[(v.BUFFER+192)&0xffffff:(v.BUFFER+256)&0xffffff]);wanted[base.START+128:base.START+192]=bytes(m[(v.BUFFER+256)&0xffffff:(v.BUFFER+320)&0xffffff]);wanted[base.START+320:base.START+384]=base.bitmap(2)
   assert r['flash_storage_sha256']==hashlib.sha256(wanted).hexdigest() and v.load(a,v.PKTCTX+16)==2;checks+=2
  coverage.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(kind=c['kind'],input=dict(c),status=r['status'],r0=hex(s.r[0]),stop_pc=r['stop_pc'],flash_storage_snapshot=r['flash_storage_snapshot'],flash_storage_sha256=r['flash_storage_sha256']))
  print(len(records),c['kind'],hex(s.r[0]),flush=True)
 files=[Path(__file__),Path(base.__file__),Path(v.__file__),Path(fast.__file__),P/'movie_packet_startup_reference.py',P/'flash_storage_reference.py',P/'analysis/movie_packet_flash_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,executed_rows=[hex(a) for a in sorted(coverage)],executed_outcomes=[[hex(a),hex(b)] for a,b in sorted(edges)],dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Actual malformed extent/header/version/CRC/readonly bit transitions and memory states exercise original error paths; no callee return is supplied. Full original/native machine/RAM/platform/flash effects compare,including a successful bitmap program before a later record program fails. Explicit direct fixtures,not natural boot; no recovery credit assigned here.')
 (P/'analysis/movie_packet_flash_errors_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Packet flash error comparisons',len(records),'checks',checks)
if __name__=='__main__':main()
