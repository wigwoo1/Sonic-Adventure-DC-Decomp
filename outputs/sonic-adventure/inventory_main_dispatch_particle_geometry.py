"""Byte/flow inventory for the next rendering dependency; no recovery credit."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
DIRECTORY=PROJECT/'analysis/main-dispatch-particle-geometry'

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def signed(v,b):return v-(1<<b) if v&(1<<(b-1)) else v

def main():
 binary=BINARY.read_bytes()
 assert digest(BINARY)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 records=[];inputs=[BINARY,Path(__file__),PROJECT/'ghidra/export_main_dispatch_particle_geometry.ps1',DIRECTORY/'diagnostics.tsv']
 for start,end,count in ((0x8C620F2C,0x8C62113C,258),(0x8C6215FA,0x8C621614,13),(0x8C62113C,0x8C621574,514)):
  listing=DIRECTORY/f'{start:08x}_instructions.tsv';inputs.append(listing)
  rows={};text={}
  for r in csv.DictReader(listing.open(),delimiter='\t'):
   a=int(r['address'],16);raw=bytes.fromhex(r['bytes'])
   assert a not in rows and start<=a<end and len(raw)==int(r['length'])==2
   assert binary[a-0x8C010000:a-0x8C010000+2]==raw
   rows[a]=int.from_bytes(raw,'little');text[a]=r['instruction'].lstrip('_')
  assert len(rows)==count
  seen=set();todo=[start];edges=set();calls=[];literals={}
  while todo:
   a=todo.pop()
   if a in seen:continue
   assert a in rows,hex(a)
   w=rows[a];seen.add(a)
   if w==0xB or w>>12 in (0xA,0xB) or w&0xF0FF==0x400B:
    assert a+2 in rows;seen.add(a+2)
    if w==0xB:continue
    if w>>12==0xA:todo.append(a+4+2*signed(w&4095,12))
    else:
     calls.append(dict(site=hex(a),target=hex(a+4+2*signed(w&4095,12)) if w>>12==0xB else None,
                       indirect_register=None if w>>12==0xB else f'r{w>>8&15}'))
     todo.append(a+4)
   elif w>>8 in (0x89,0x8B,0x8D,0x8F):
    fall=a+4 if w>>8 in (0x8D,0x8F) else a+2
    if fall==a+4:assert a+2 in rows;seen.add(a+2)
    for target in (a+4+2*signed(w&255,8),fall):todo.append(target);edges.add((a,target))
   else:todo.append(a+2)
  assert seen==set(rows)
  # Decode PC-relative literals directly, including instructions in delay slots.
  for a,w in rows.items():
   if w>>12 not in (9,13):continue
   n=2 if w>>12==9 else 4
   target=a+4+(w&255)*2 if n==2 else ((a+4)&~3)+(w&255)*4
   value=int.from_bytes(binary[target-0x8C010000:target-0x8C010000+n],'little')
   literals[hex(target)]=dict(bytes=n,value=hex(value))
  gaps=[];g=start
  while g<end:
   if g in rows:g+=2;continue
   first=g
   while g<end and g not in rows:g+=2
   gaps.append([hex(first),hex(g)])
  records.append(dict(address=hex(start),end_exclusive=hex(end),instruction_rows=count,
                      instruction_bytes=count*2,retail_span_sha256=hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest(),
                      non_instruction_gaps=gaps,conditional_outcomes=len(edges),call_sites=sorted(calls,key=lambda c:c['site']),
                      pc_relative_literals=literals,fmac_rows=sum(t.startswith('fmac ') for t in text.values()),
                      fmul_rows=sum(t.startswith('fmul ') for t in text.values()),
                      frchg_rows=sum(t=='frchg' for t in text.values()),listing=listing.relative_to(PROJECT).as_posix()))
 assert records[0]['non_instruction_gaps']==[['0x8c62110c','0x8c621118']]
 assert records[0]['call_sites']==[dict(site='0x8c621126',target='0x8c6215fa',indirect_register=None)]
 assert records[1]['call_sites']==[dict(site='0x8c621604',target='0x8c62113c',indirect_register=None)]
 report=dict(routines=records,original_instruction_rows_checked=sum(r['instruction_rows'] for r in records),
             newly_registered_routines=0,native_behavior_verified=False,
             next_milestones=['Translate 8c620f2c and R8-preserving wrapper 8c6215fa using existing scalar FMAC helper.',
                              'Compare full original/native state, renderer globals, both FPU banks, reached guards and nested-call failures.',
                              'Then recover recursive model traversal 8c62113c and its matrix/model dependencies.'],
             scope='Exact original-byte and CFG inventory only; Ghidra drafts are analysis aids, not verified C.',
             limitations=['The new geometry source is not yet translated, compiled, behavior-verified or registered.',
                          'Indirect call targets and recursion/data structure semantics require separate recovery; this is not a pixel-rendering or frame-driver proof.'],
             sha256={str(p):digest(p) for p in inputs})
 path=PROJECT/'analysis/main_dispatch_particle_geometry_inventory.json';path.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('original_instruction_rows_checked','newly_registered_routines','native_behavior_verified')}))
 for r in records:print(r['address'],r['instruction_rows'],'rows;',r['conditional_outcomes'],'conditional outcomes;',len(r['call_sites']),'call sites')

if __name__=='__main__':main()
