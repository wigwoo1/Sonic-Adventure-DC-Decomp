"""Bounded retail cache-helper control flow. Literal jumps and two explicitly decoded P2 transitions only; not execution proof."""
from pathlib import Path
import json,struct
root=Path(__file__).resolve().parents[3];out=root/'outputs/sonic-adventure';b=(root/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
def word(a):return struct.unpack_from('<H',b,a-0x8c010000)[0]
for stem,start,end,alias_pc,alias_target in [('invalidate',0x8c5fc0d8,0x8c5fc336,0x8c5fc162,0x8c5fc16c),('purge',0x8c5fc336,0x8c5fc58a,0x8c5fc3b6,0x8c5fc3c0)]:
 todo=[start];seen=set();jumps={}
 while todo:
  a=todo.pop()
  if a in seen:continue
  assert start<=a<end,(stem,hex(a));seen.add(a);w=word(a)
  if w==0xb:seen.add(a+2);continue
  if w&0xf0ff==0x402b:
   if a==alias_pc:
    assert word(a-6)==0xc703 and word(a-4)==0xd102 and word(a-2)==0x210b
    literal=((a-4+4)&~3)+8;mask=struct.unpack_from('<I',b,literal-0x8c010000)[0]
    origin=((a-6+4)&~3)+12;assert origin==alias_target and mask==0xa0000000
    target=origin|mask
   else:
    reg=w>>8&15;prior=word(a-2);assert prior>>12==13 and prior>>8&15==reg,(hex(a),hex(prior))
    literal=((a-2+4)&~3)+4*(prior&255);target=struct.unpack_from('<I',b,literal-0x8c010000)[0]
   jumps[a]=target;seen.add(a+2);todo.append(0x8c000000|(target&0xffffff));continue
  if w>>12==10:
   d=w&4095;d=d-4096 if d&2048 else d;seen.add(a+2);todo.append(a+4+2*d);continue
  if w>>8 in (0x89,0x8b,0x8d,0x8f):
   d=w&255;d=d-256 if d&128 else d;todo.append(a+4+2*d)
   if w>>8 in (0x8d,0x8f):seen.add(a+2);todo.append(a+4)
   else:todo.append(a+2)
   continue
  todo.append(a+2)
 (out/'analysis'/f'file_cache_{stem}_reachable_addresses.json').write_text(json.dumps(sorted(seen),indent=2)+'\n')
 (out/'analysis'/f'file_cache_{stem}_literal_jumps.json').write_text(json.dumps({hex(a):hex(t) for a,t in jumps.items()},indent=2)+'\n')
 print(stem,len(seen),'reachable instruction addresses')
