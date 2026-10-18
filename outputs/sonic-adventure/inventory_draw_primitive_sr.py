"""Byte-check the actual SR masks in pending primitive clippers; static evidence."""
import csv,hashlib,json,random
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
ENTRIES=(0x8c037ee0,0x8c037ff4,0x8c038516,0x8c038802)
PREFIX=(0xe120,0xe018,0x410d,0x6117,0x0002,0x2019)
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';data=binary.read_bytes();files=[Path(__file__),binary,PROJECT/'analysis/sh4_clipping_architecture_reference.json'];records=[];seen=set();checks=0
 rng=random.Random(0x20000000);samples=[0,1,0x20000000,0x40000000,0x60000000,0x700083f3,0xffffffff]+[rng.getrandbits(32) for _ in range(256)]
 for entry in ENTRIES:
  listing=PROJECT/'analysis/draw-primitive-frontier'/f'{entry:08x}_instructions.tsv';files.append(listing)
  rows=list(csv.DictReader(listing.open(),delimiter='\t'));words={int(r['address'],16):int.from_bytes(bytes.fromhex(r['bytes']),'little') for r in rows}
  for pc,w in words.items():
   if w!=0x400e or pc in seen:continue
   seen.add(pc);setting=words.get(pc-2)==0x201b;pattern=PREFIX+((0x6117,0x201b) if setting else ());start=pc-2*len(pattern)
   actual=tuple(words.get(start+2*i) for i in range(len(pattern)));assert actual==pattern,(hex(pc),actual)
   for a in range(start,pc+2,2):assert data[a-0x8c010000:a-0x8c010000+2]==words[a].to_bytes(2,'little')
   mask=(0x20<<0x18)&0xffffffff;assert mask==0x20000000
   for sr in samples:
    result=(sr&(~mask&0xffffffff))|(mask if setting else 0)
    assert result==((sr|mask) if setting else (sr&~mask)) and (result&~mask)==(sr&~mask)
    if sr&0x40000000:assert ((result>>29)&1)==int(setting)
    checks+=1
   records.append(dict(pc=hex(pc),entry_export=hex(entry),pattern_start=hex(start),pattern_words=[hex(x) for x in pattern]+['0x400e'],sr_mask=hex(mask),operation='set_RB' if setting else 'clear_RB',md_preserved=True,privileged_execution_required=True,register_bank_effect='When MD=1, set selects integer bank 1 and clear selects integer bank 0. R8-R15 are common; R0-R7 require both banks.',unprivileged_execution='Privilege exception is outside this static mask calculation; no successful MD=0 LDC execution is claimed.',native_recovery_credit=0))
 assert len(records)==8 and sum(r['operation']=='clear_RB' for r in records)==3
 report=dict(sr_transitions=sorted(records,key=lambda r:int(r['pc'],16)),clear_rb_sites=3,set_rb_sites=5,mask_derivation_checks=checks,mach_required_separately=True,newly_recovered_entries=0,scope='Exact local instruction patterns and bytes prove that pending clippers clear/set SR.RB (bit 29), preserving MD. These are real bank changes under privileged execution, not arbitrary SR loads. Static mask arithmetic only: no complete CFG, runtime integer-bank/privilege implementation, clipping or hardware proof.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/draw_primitive_sr_transitions.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Byte-checked SR.RB transitions:',len(records),'sites, 3 clear / 5 set;',checks,'mask arithmetic checks; zero native credit.')
if __name__=='__main__':main()
