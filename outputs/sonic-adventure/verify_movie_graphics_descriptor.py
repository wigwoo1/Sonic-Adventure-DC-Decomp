"""Close original descriptor scanning with flags oracle and checked bound proof."""
import csv,hashlib,itertools,json
from pathlib import Path
import verify_movie_graphics_allocations as fixture
fast=fixture.fast;v=fixture.v;P=v.P;ROOT=v.ROOT;ENTRY=0x8c664ac0
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 mp=P/'analysis/movie_clear_translation.json';translation=json.loads(mp.read_text());body=next(r for r in translation['routines'] if r['address']==hex(ENTRY));listing=Path(body['listing']);rows={int(r['address'],16):r for r in csv.DictReader(listing.open(),delimiter='\t')}
 retail=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';image=retail.read_bytes()
 for address,row in rows.items():assert image[address-0x8c010000:address-0x8c010000+2]==bytes.fromhex(row['bytes'])
 expected=['mov.l 0x8c664b84,r7','mov.l 0x8c664b80,r6','add r6,r7','mov r6,r4','cmp/hs r7,r4','mov r6,r5','bt/s 0x8c664ae6','mov #0x1,r6']
 assert [rows[a]['instruction'].lstrip('_') for a in range(ENTRY,ENTRY+16,2)]==expected
 def literal(address):return int.from_bytes(image[address-0x8c010000:address-0x8c010000+4],'little')
 base,size=literal(0x8c664b80),literal(0x8c664b84);assert base==fixture.POOL and size==fixture.POOL_BYTES and 0<size and base+size<=0xffffffff and base<base+size
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 for free_index,mode,ignored in itertools.product((0,1,7,size//24-1,None),(0,1,4),(0,0xffffffff)):
  c=v.Input(entry=ENTRY,mode=mode,allocation_fixture=dict(exhausted=True),descriptor_free_index=free_index,descriptor_ignored_argument=ignored)
  m,s,f=fixture.prepare(ram,c);s.pc=ENTRY;s.r[4]=ignored
  if free_index is not None:fixture.flags(m,base+24*free_index,0)
  before=v.effects.State.from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m);result=base+24*free_index if free_index is not None else 0
  if result:fixture.flags(wanted,result,1)
  r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['status']==0 and s.r[0]==result and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;checks+=5
  assert v.sample(a,base,size)==v.sample(wanted,base,size) and r['full_state_match'] and r['ordered_boundaries_match'];checks+=3
  assert not r['vram_writes'] and not any(r['sound_state']) and r['runtime_state'] is None;checks+=3
  seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),r0=hex(result),steps=len(path)));print(len(records),free_index,mode,hex(ignored),flush=True)
 wanted_rows=set(map(lambda x:int(x,16),body['reachable_addresses']));wanted_edges=set(map(tuple,body['conditional_edges']));actual_edges={(hex(a),hex(b)) for a,b in edges};missing=wanted_edges-actual_edges
 assert wanted_rows<=seen and missing=={('0x8c664acc','0x8c664ae6')}
 files=[Path(__file__),Path(fixture.__file__),Path(v.__file__),Path(fast.__file__),mp,listing,retail,P/'analysis/movie_graphics_init_connection.json']
 report=dict(routine=body|dict(module='1ST_READ.BIN',source=translation['source'],source_sha256=digest(P/translation['source'])),cases_passed=len(records),independent_checks=checks,cases=records,executed_rows=22,executed_conditional_outcomes=5,proven_infeasible_conditional_outcomes=[['0x8c664acc','0x8c664ae6']],qualified_conditional_outcomes=6,initial_pool_base=hex(base),pool_bytes=size,descriptor_stride=24,initial_bound_no_wrap=True,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Thirty fresh direct original/native descriptor scans: first/second/later/last/none free flags,threeFPmodes andignoredinput variations. Independent entire98304byte pool/first-free/flag-only oracle supplements fullmachine/memory/boundary comparison. Checked immutable entry instruction/literal values prove base<base+size without wrap,so initial BT/S cannot take its empty-range edge. Later loop exhaustion genuinely returns0;no descriptor/pool result is supplied. Mapped aligned boundedpool/immutablecode/no asynchronousflag/register mutation domain;arbitrary ownership/concurrency/hardware remains unqualified.')
 (P/'analysis/movie_graphics_descriptor_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Descriptor cases',len(records),'checks',checks,'22rows/5executed+1infeasible outcomes')
if __name__=='__main__':main()
