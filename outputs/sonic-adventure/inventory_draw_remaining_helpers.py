"""Byte-checked remaining draw helper proposals; analysis only."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
 registered={(r.get('module','1ST_READ.BIN'),int(r['address'],16)):r for r in registry['routines']}
 main_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';overlay_path=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN';binaries={'1ST_READ.BIN':(main_path.read_bytes(),0x8c010000),'AL_MAIN.BIN':(overlay_path.read_bytes(),0xc900000)}
 files=[Path(__file__),registry_path,main_path,overlay_path];records=[]
 for address in (0xc90c384,0xc909d80,0x8c0409c0,0x8c041c92,0x8c0583a0,0x8c638ecc):
  listing=PROJECT/f'analysis/overlay-draw-missing-helpers/{address:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));module='AL_MAIN.BIN' if address<0x10000000 else '1ST_READ.BIN';binary,base=binaries[module];calls=[]
  for row in rows:
   a=int(row['address'],16);assert int(row['length'])==2 and binary[a-base:a-base+2]==bytes.fromhex(row['bytes'])
   if not row['instruction'].lstrip('_').startswith(('jsr','bsr','jmp')):continue
   targets=[]
   for text in row['flows'].strip().split():
    t=int(text,16);target_module='AL_MAIN.BIN' if 0xc900000<=t<0xcb20000 else '1ST_READ.BIN';old=registered.get((target_module,t));targets.append(dict(address=hex(t),module=target_module,registered=bool(old),source=old['source'] if old else None))
   calls.append(dict(site=row['address'],instruction=row['instruction'],static_targets=targets,computed_target_exhaustive=False))
  old=registered.get((module,address));records.append(dict(entry=hex(address),module=module,instruction_rows=len(rows),listing=listing.relative_to(PROJECT).as_posix(),currently_registered=bool(old),registered_source=old['source'] if old else None,transfer_sites=calls));files.append(listing)
 report=dict(routines=records,instruction_rows=sum(r['instruction_rows'] for r in records),newly_recovered_entries=0,reused_routine_credit=0,scope='Existing next draw helper CFG proposals byte-checked against original modules and current registry. Transfers are static Ghidra annotations, not exhaustive runtime/computed-target proof. No parent/helper recovery credit.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/draw_remaining_helpers_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Remaining draw proposals:',report['instruction_rows'],'byte-checked rows in six bodies; zero source credit.')
if __name__=='__main__':main()
