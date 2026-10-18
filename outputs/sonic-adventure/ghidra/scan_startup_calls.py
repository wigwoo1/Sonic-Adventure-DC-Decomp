"""Locate SH-4 call candidates; decoded executable data can produce false positives."""
import hashlib,json,struct
from pathlib import Path
root=Path(__file__).resolve().parents[3]
b=(root/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();base=0x8c010000
out=root/'outputs/sonic-adventure/analysis'
targets={0x8c6510a4:'engine_initialize',0x8c10d5a8:'transfer_initialize',0x8c65acc0:'sound_initialize',0x8c600038:'file_initialize'}
rows=[]
for o in range(0,len(b)-32,2):
 w=struct.unpack_from('<H',b,o)[0];pc=base+o
 if w>>12==11:
  d=w&4095;d=d-4096 if d&2048 else d;t=(pc+4+2*d)&0xffffffff
  if t in targets:rows.append(dict(target=hex(t),name=targets[t],kind='bsr',call=hex(pc),validated=False))
 if w>>12!=13:continue
 reg=(w>>8)&15;lit=((pc+4)&~3)+4*(w&255);lo=lit-base
 if not 0<=lo<=len(b)-4:continue
 t=struct.unpack_from('<I',b,lo)[0]
 if t not in targets:continue
 for delta in range(2,32,2):
  nw=struct.unpack_from('<H',b,o+delta)[0]
  if nw==0x400b+(reg<<8):
   rows.append(dict(target=hex(t),name=targets[t],kind='mov.l/jsr',load=hex(pc),literal=hex(lit),call=hex(pc+delta),validated=False));break
  # A register write can invalidate this candidate; retain as unvalidated evidence.
report={'executable_sha256':hashlib.sha256(b).hexdigest(),'scope':'Static binary call candidates, not proof of executable code or startup ordering. Requires instruction/function boundary and intervening register-write validation.','candidates':rows}
(out/'startup_call_candidates.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(rows,indent=2))
