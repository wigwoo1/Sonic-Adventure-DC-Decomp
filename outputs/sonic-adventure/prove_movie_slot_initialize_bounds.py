"""Checked immutable-literal proof for two impossible initial loop skips."""
import csv,hashlib,json,struct
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
 manifest_path=P/'analysis/movie_slot_initialize_translation.json';manifest=json.loads(manifest_path.read_text());files=[Path(__file__),binary,manifest_path]
 def word(a):return struct.unpack_from('<I',raw,a-0x8c010000)[0]
 assertions=[]
 for entry,literal,first,target,skip in ((0x8c109cfe,0x8c109de8,0x8c109d24,0x8c109d54,16),(0x8c108808,0x8c1088f4,0x8c10881a,0x8c10882c,0)):
  body=next(r for r in manifest['routines'] if int(r['address'],16)==entry);listing=Path(body['listing']);files.append(listing)
  rows={int(r['address'],16):r for r in csv.DictReader(listing.open(),delimiter='\t')}
  for a,row in rows.items():assert raw[a-0x8c010000:a-0x8c010000+2].hex()==row['bytes']
  if skip:
   prefix={0x8c109d0a:'mov #0x19,r10',0x8c109d0e:'shll8 r10',0x8c109d10:'mov.l 0x8c109de8,r9',0x8c109d12:'add #-0x10,r10',0x8c109d16:'mov r9,r14',0x8c109d18:'add #0x10,r14',0x8c109d1e:'add r14,r10',0x8c109d22:'cmp/hs r10,r14',first:'bt/s 0x8c109d54'}
  else:
   prefix={0x8c10880c:'mov #0x19,r13',0x8c10880e:'shll8 r13',0x8c108810:'mov.l 0x8c1088f4,r14',0x8c108812:'add #-0x10,r13',0x8c108816:'add r14,r13',0x8c108818:'cmp/hs r13,r14',first:'bt 0x8c10882c'}
  for a,text in prefix.items():assert rows[a]['instruction'].lstrip('_')==text
  base=word(literal);begin=base+skip;end=begin+0x18f0
  assert base==(0x8c7a64dc if skip else 0x8c7a64ec) and begin==0x8c7a64ec and end==0x8c7a7ddc
  assert 0<=begin<end<=0xffffffff and (end-begin)==4*0x63c
  # No guest calls occur before the first comparison; only fixed instruction
  # literals, arithmetic and private stack stores define its operands.
  prefix_addresses=[a for a in rows if entry<=a<first]
  assert not any(rows[a]['instruction'].lstrip('_').startswith(('jsr ','bsr ','jmp ')) for a in prefix_addresses)
  assertions.append(dict(entry=hex(entry),immutable_literal=hex(literal),literal_value=hex(base),initial_cursor=hex(begin),exclusive_end=hex(end),stride=1596,normal_iterations=4,infeasible_conditional_outcome=[hex(first),hex(target)],reason='Unsigned initial cursor is strictly below end; fixed literal/arithmetic, no preceding helper or wrap. Domain excludes code/literal mutation and destructive private-stack aliases.'))
 result=dict(infeasible_rows=[],infeasible_conditional_outcomes=[a['infeasible_conditional_outcome'] for a in assertions],proofs=assertions,qualified_domain='Original immutable literal bytes and disjoint valid private stack/records; no self-modification or arbitrary aliases.',sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_slot_initialize_bounds_proof.json').write_text(json.dumps(result,indent=2)+'\n');print('PROVED2 immutable initial-skip outcomes; no excluded rows')
if __name__=='__main__':main()
