"""Byte-checked static next-callee inventory; never recovered-source credit."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 module=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN';binary=module.read_bytes()
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
 recovered={(r.get('module','1ST_READ.BIN'),int(r['address'],16)):r for r in registry['routines']}
 files=[Path(__file__),module,registry_path];routines=[]
 for entry in (0xc9009d0,0xc9058a0):
  listing=PROJECT/f'analysis/overlay-object-update-callers/{entry:08x}_instructions.tsv';files.append(listing)
  rows=list(csv.DictReader(listing.open(),delimiter='\t'));calls=[]
  for row in rows:
   a=int(row['address'],16);assert int(row['length'])==2 and binary[a-0xc900000:a-0xc900000+2]==bytes.fromhex(row['bytes'])
   if not row['instruction'].lstrip('_').startswith(('jsr','bsr')):continue
   flows=row['flows'].strip().split();assert len(flows)==1,(row,'Static inventory requires exactly one exported target')
   target=int(flows[0],16);target_module='AL_MAIN.BIN' if 0xc900000<=target<0xcb20000 else '1ST_READ.BIN';old=recovered.get((target_module,target))
   calls.append(dict(site=hex(a),target=hex(target),module=target_module,registered=bool(old),source=old['source'] if old else None,verification=old['verification'] if old else None))
  targets=sorted({int(c['target'],16) for c in calls});missing=sorted({int(c['target'],16) for c in calls if not c['registered']})
  routines.append(dict(entry=hex(entry),listing=listing.relative_to(PROJECT).as_posix(),instruction_rows=len(rows),call_sites=calls,unique_targets=list(map(hex,targets)),unrecovered_targets=list(map(hex,missing))))
 report=dict(routines=routines,newly_recovered_entries=0,scope='Static original-export call targets and current registry membership; registered callees still require ABI-compatible integration. No parent body recovery or runtime call-coverage claim.',limitations=['Ghidra flow annotations are static evidence; exported computed-call resolution is not an exhaustive runtime target proof.','Native primitive material families are already complete in the separately tested bounded mesh chain. Natural object drawing/scheduling remains unfinished.'],sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/overlay_draw_dependencies_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps([dict(entry=r['entry'],rows=r['instruction_rows'],call_sites=len(r['call_sites']),unique_targets=len(r['unique_targets']),unrecovered_targets=r['unrecovered_targets']) for r in routines],indent=2))
if __name__=='__main__':main()
