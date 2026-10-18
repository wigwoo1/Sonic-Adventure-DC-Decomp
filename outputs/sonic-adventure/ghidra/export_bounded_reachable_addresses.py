"""Export reachable SH-4 instruction addresses within explicit function bounds.
Calls remain external; branch targets/delay slots are retained. This does not
identify functions or validate opcode semantics. Verify against retail execution.
"""
import json,struct,sys
from pathlib import Path
root=Path(__file__).resolve().parents[3]
binary=(root/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
start,end=int(sys.argv[1],16),int(sys.argv[2],16);todo=[start];seen=set()
while todo:
 a=todo.pop()
 if a in seen:continue
 if not start<=a<end or a&1:raise ValueError(f'Control flow outside explicit bounds: {a:08x}')
 seen.add(a);w=struct.unpack_from('<H',binary,a-0x8c010000)[0]
 if w==0x000b:
  seen.add(a+2);continue
 if w>>12==10:
  seen.add(a+2);d=w&4095;d=d-4096 if d&2048 else d;todo.append(a+4+2*d);continue
 if w>>8 in (0x89,0x8b,0x8d,0x8f):
  d=w&255;d=d-256 if d&128 else d;todo.append(a+4+2*d)
  if w>>8 in (0x8d,0x8f):seen.add(a+2);todo.append(a+4)
  else:todo.append(a+2)
  continue
 if w&0xf0ff==0x402b:raise ValueError(f'Indirect jump at {a:08x} requires explicit analysis')
 todo.append(a+2)
for a in seen:
 if not start<=a<end:raise ValueError('Delay slot outside bounds')
(root/'outputs/sonic-adventure/analysis'/sys.argv[3]).write_text(json.dumps(sorted(seen),indent=2)+'\n')
print(len(seen),'reachable instruction addresses')
