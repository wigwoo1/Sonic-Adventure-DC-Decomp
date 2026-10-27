"""Independent fixed-record, metadata predicate and input-table oracles."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_slot_initialize as fast
import movie_slot_initialize_fixtures as fixtures
v=fast.old;P=v.P

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def case(entry,**extra):return dict(entry=entry,mode=extra.pop('mode',0),slot_default_record=dict(flags=14),kind='normal')|extra
def cases():
 for mode,fill in itertools.product((0,1,4),(0,0xa5,0xff)):
  yield case(v.SLOT_ROOT,mode=mode,slot_fill=fill)
  yield case(v.SLOT_BATCH,mode=mode,slot_fill=fill)
 for mode,index in itertools.product((0,1,4),(0,1,2,7,8,13,14,19,20,23)):
  yield case(v.SLOT_ONE,mode=mode,slot_index=index,slot_arguments=[fixtures.SCRATCH,0x76543210,0x89abcdef,0x13579bdf])
 for mode,index,row,col in itertools.product((0,1,4),(0,1,2,7,8,13,14,19,20,23),range(4),range(4)):
  yield case(v.SLOT_TABLE,mode=mode,slot_index=index,slot_arguments=[index,0,0,0],slot_default_record=dict(flags=14,status12=col,status13=row),kind='table',table_row=row,table_col=col)
 # Test every short-circuit predicate with real metadata decoding, not returns.
 records=[dict(flags=0),dict(flags=4),dict(flags=14),dict(flags=14,metadata=[1,5,0x10,0]),dict(flags=14,metadata=[0,4,0x10,0]),dict(flags=14,metadata=[0,5,0,0]),dict(flags=14,metadata=[0,5,0x10,0x40]),dict(flags=14,metadata=[0,5,0x10,0x20]),dict(flags=14,metadata=[0,5,0x1f,0x1f]),dict(flags=0x8000000e)]
 for mode,record in itertools.product((0,1,4),records):
  yield case(v.SLOT_STATUS,mode=mode,slot_arguments=[1,0,0,0],slot_default_record=record)
 for mode,flags,data in itertools.product((0,1,4),(0,4,14,0x80000004),([0,0,0,0],[255,255,255,255],[7,5,0x1f,0x31])):
  yield case(v.SLOT_DECODE,mode=mode,slot_arguments=[fixtures.SCRATCH,0,0,0],slot_default_record=dict(flags=flags,metadata=data),kind='decode')
 for mode in (0,1,4):yield case(v.SLOT_CLEAR,mode=mode,slot_arguments=[fixtures.SCRATCH,0x76543210,0x89abcdef,0x13579bdf])
 # Actual unsigned overflow makes the original clear helper skip its writes.
 for mode,address in itertools.product((0,1,4),(0xfffffa00,0xffffffff)):
  yield case(v.SLOT_CLEAR,mode=mode,slot_arguments=[address,0,0,0],kind='clear-wrap')

def main():
 ram=v.seed_ram();seen=set();edges=set();records=[];checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=v.prepare(ram,c);fixtures.configure(m,c)
  if c['entry']==v.SLOT_DECODE:s.r[5]=fixtures.info(m,1)
  before=v.effects.State.from_buffer_copy(s);fp=bytes(f);base=v.load(m,0x8c109de8);entry=c['entry'];expected=bytearray(m)
  predicted=None
  if entry==v.SLOT_ROOT:
   for j,index in enumerate((1,7,13,19)):
    at=base+16+j*1596;expected[at&0xffffff:(at&0xffffff)+1536]=bytes(1536)
    fixtures.write(expected,at+0x600,fixtures.predicate(c,index),1);fixtures.write(expected,at+0x601,fixtures.table_value(m,index),1)
    fixtures.write(expected,at+0x602,j,1);fixtures.write(expected,at+0x603,0,1)
    for offset,value in ((0x630,index),(0x634,0),(0x638,0)):fixtures.write(expected,at+offset,value)
   for offset in (4,8,12):fixtures.write(expected,base+offset,0)
  elif entry in (v.SLOT_BATCH,v.SLOT_ONE):
   indices=(1,7,13,19) if entry==v.SLOT_BATCH else (c.get('slot_index',1),)
   for j,index in enumerate(indices):
    at=base+16+j*1596 if entry==v.SLOT_BATCH else fixtures.SCRATCH
    # Batch consumes existing handle words, just as the original does.
    if entry==v.SLOT_BATCH:index=fixtures.read(m,at+0x630)
    fixtures.write(expected,at+0x600,fixtures.predicate(c,index),1);fixtures.write(expected,at+0x601,fixtures.table_value(m,index),1)
  elif entry==v.SLOT_STATUS:predicted=fixtures.predicate(c,1)
  elif entry==v.SLOT_TABLE:predicted=fixtures.table_value(m,c['slot_index'])
  elif entry==v.SLOT_CLEAR and c['kind']!='clear-wrap':expected[fixtures.SCRATCH&0xffffff:(fixtures.SCRATCH&0xffffff)+1536]=bytes(1536)
  elif entry==v.SLOT_DECODE:
   record=c['slot_default_record'];b=record.get('metadata',[0,5,0x10,0]);present=bool(record['flags']&4);predicted=0 if present else 0xffffffff
   if present:
    for offset,value,width in ((0,b[0]+1,2),(2,(b[1]+1)*32,2),(4,b[2]>>4,2),(6,b[3]>>6,1),(7,(b[3]>>5)&1,1)):fixtures.write(expected,fixtures.SCRATCH+offset,value,width)
  r,path,a,s,f=fast.compare(m,s,f,c);seen.update(path)
  for j,pc in enumerate(path):
   instruction=v.ROWS.get(pc,'');delta=2 if instruction.startswith(('bt/s ','bf/s ')) else 1
   if instruction.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+delta<len(path):edges.add((pc,path[j+delta]))
  assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],(i,c,r['status'],r['stop_pc']);n=3
  assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;n+=4
  if predicted is not None:assert s.r[0]==predicted,(i,c,s.r[0],predicted);n+=1
  # Full independent RAM expectation excluding only the actually private stack.
  bottom=before.r[15]-0x100;expected[bottom&0xffffff:before.r[15]&0xffffff]=a[bottom&0xffffff:before.r[15]&0xffffff]
  assert a==expected,(i,c,'independent RAM expectation');n+=1
  assert not any(r['sound_state']) and not r['aperture_events'] and r['pvr_dma_state'][6]==0;n+=3
  checks+=n;records.append(dict(input=dict(c),status=r['status'],r0=s.r[0],steps=len(path),independent_checks=n))
  print(i,hex(entry),c['kind'],r['status'],hex(s.r[0]),flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  wanted=set(body['reachable_addresses']);wanted_edges=set(map(tuple,body['conditional_edges']));actual={hex(a) for a in seen};actual_edges={(hex(a),hex(b)) for a,b in edges}
  coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&actual),static_outcomes=len(wanted_edges),executed_outcomes=len(wanted_edges&actual_edges),missing_rows=sorted(wanted-actual),missing_outcomes=sorted(wanted_edges-actual_edges)))
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_slot_initialize_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Independent entire RAM expectations exclude only private stack; complete original/native machine and ordered boundaries compare without exclusions. Actual original helper/metadata/bit-locator/driver getters execute. Fixed record domains; immutable literal initial-skip outcomes require a separate checked proof. Canonical clear/decoder receive zero extra credit. Not game or hardware proof.')
 (P/'analysis/movie_slot_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('SLOT DIRECT',len(records),checks,coverage)
if __name__=='__main__':main()
