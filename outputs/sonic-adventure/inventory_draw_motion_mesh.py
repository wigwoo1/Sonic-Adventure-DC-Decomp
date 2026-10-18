"""Byte-check motion mesh exports/registry; no recovered-source credit."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 binary_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=binary_path.read_bytes();registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
 registered={int(r['address'],16):r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'}
 files=[Path(__file__),binary_path,registry_path,PROJECT/'ghidra/export_draw_motion_mesh.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java'];routines=[]
 for entry in (0x8c621574,0x8c617c34):
  old=registered.get(entry)
  listing=PROJECT/f'analysis/draw-motion-mesh/{entry:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));calls=[]
  for row in rows:
   a=int(row['address'],16);assert int(row['length'])==2 and binary[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes'])
   if not row['instruction'].lstrip('_').startswith(('jmp','jsr','bsr')):continue
   flows=row['flows'].strip().split();targets=[int(t,16) for t in flows]
   calls.append(dict(site=row['address'],instruction=row['instruction'],static_targets=[dict(address=hex(t),already_registered=t in registered,source=registered[t]['source'] if t in registered else None) for t in targets]))
  routines.append(dict(entry=hex(entry),instruction_rows=len(rows),listing=listing.relative_to(PROJECT).as_posix(),currently_registered=bool(old),registered_source=old['source'] if old else None,transfers=calls,boundary_status='This inventory byte-checks the export and registry membership only; consult the separate recovered-source proof for bounded behavior.' if old else 'Read-only Ghidra proposal; computed transfers, complete code/data ownership, native source and runtime behavior still to verify.'));files.append(listing)
 assert [r['instruction_rows'] for r in routines]==[19,259]
 report=dict(routines=routines,instruction_rows=278,newly_recovered_entries=0,reused_routine_credit=0,function_index_modified=False,scope='Byte-checked motion mesh instruction inventory and current registry membership. Distinct 8c617c34 preprocessing calls the existing native mesh wrapper 8c6215ba; wrapper alone is not a substitute for that preprocessing. This inventory itself grants no recovery credit.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/draw_motion_mesh_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Motion mesh inventory: 278 byte-checked rows;',sum(r['currently_registered'] for r in routines),'registered entries; this inventory grants zero recovery credit.')
if __name__=='__main__':main()
