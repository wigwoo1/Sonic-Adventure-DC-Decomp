"""Existing original getters with real native lower reads and SoundRAM bytes."""
import collections,hashlib,itertools,json
from pathlib import Path
import write_watch_sound_read as fast
v=fast.old;P=v.P;ROOT=v.ROOT
def cases():
 for entry,address,mode in itertools.product((v.FIELD,v.BYTE,v.WORD),(0,1,2,3,4,0x1ffffc,0x1fffff,0x200000,0x200001,0x7fffffff,0x80000000,0xffffffff),(0,1,4)):
  yield v.Input(entry=entry,address=address,mode=mode,sound_pattern='ramp')
 for entry,extra in itertools.product((v.FIELD,v.BYTE,v.WORD),(dict(output=0x8d000000),dict(output=0xa0801000),dict(stack=0x8c000000),dict(sound_unconfigured=True),dict(null_bus=True))):
  yield v.Input(dict(entry=entry,address=32,mode=0,sound_pattern='ramp')|extra)
def semantics(m,a,before,fp,s,f,r,path,c):
 checks=2;assert r['full_state_match'] and r['ordered_boundaries_match']
 wanted=bytearray(bytes(range(256))*(0x200000//256));address=c['address'];signed=address if address<0x80000000 else address-0x100000000;accepted=signed<0x200000
 if r['status']==0:
  assert s.r[0]==0 and s.r[15]==before.r[15] and s.pr==before.pr and bytes(f)==fp;checks+=4
  output=before.r[5];count=1 if c['entry']==v.BYTE else 4
  if not accepted:assert not any(r['sound_state']) and a[v.OUT&0xffffff:(v.OUT&0xffffff)+16]==bytes([0x66])*16;checks+=2
  else:
   effective=(0xa0800000+(address&0xfffffffc if c['entry']==v.BYTE else address))&0xffffffff;index=(effective&0x1fffffff)-0x800000
   assert 0<=index<=0x200000-4 and not effective&3;checks+=2
   data=bytes(wanted[index:index+4])
   if c['entry']==v.BYTE:data=bytes([wanted[index+(address&3)]])
   if 0xc000000<=output&0x1fffffff<0xd000000:
    assert bytes(a[output&0xffffff:(output&0xffffff)+count])==data;checks+=1
    assert a[(v.OUT&0xffffff)+count:(v.OUT&0xffffff)+16]==bytes([0x66])*(16-count);checks+=1
   else:
    begin=(output&0x1fffffff)-0x800000;wanted[begin:begin+count]=data;checks+=1
   assert r['sound_state'][0]==1 and r['sound_state'][2]==4;checks+=2
  assert r['sound_ram_sha256']==hashlib.sha256(wanted).hexdigest();checks+=1
 else:
  assert r['status'] in (-2,-3) and r['exception_state'][-1]==0;checks+=2
 return checks
def main():
 records=[];coverage=set();edges=set();checks=0;ram=v.seed_ram()
 for c in cases():
  m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fast.compare(m,s,f,c);count=semantics(m,a,before,fp,s,f,r,path,c);checks+=count;coverage.update(path)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],sound_state=r['sound_state'],sound_ram_sha256=r['sound_ram_sha256'],full_state_match=r['full_state_match'],ordered_boundaries_match=r['ordered_boundaries_match'],independent_checks=count));print(len(records),hex(c['entry']),hex(c['address']),r['status'],flush=True)
 rows=[]
 for definition in v.manifest['routines']:
  body=set(v.PER_BODY[int(definition['address'],16)]);wanted=set(map(tuple,definition['conditional_edges']));actual={(hex(a),hex(b)) for a,b in edges};rows.append(dict(address=definition['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(a) for a in sorted(body-coverage)],conditional_outcomes=len(wanted),covered_outcomes=len(wanted&actual),missing_outcomes=[list(x) for x in sorted(wanted-actual)]))
 assert all(not r['missing_rows'] and not r['missing_outcomes'] for r in rows)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_sound_read_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Three existing70-row/6outcome getters execute actual10d460andmappedSoundRAM. Full machine/RAM/banked/FPU/SR/PR/bus/device/boundary state matches original instructions. Independent signed gate,byte-lane/word selection,whole2MiBSoundbytes/output guards,Soundoutput and actual unaligned/outside/unconfigured/invalid-pointer failures checked. No lower helper result or payload is supplied. Canonical existing routines get zero additional recovery credit.')
 (P/'analysis/movie_sound_read_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Sound read comparisons',len(records),'checks',checks,rows)
if __name__=='__main__':main()
