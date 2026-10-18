"""Byte-check the next object-effect dispatcher, eight callees and three services."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
 registered={(r.get('module','1ST_READ.BIN'),int(r['address'],16)):r for r in registry['routines']}
 binary_paths={'AL_MAIN.BIN':ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN','1ST_READ.BIN':ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'}
 binaries={m:(p.read_bytes(),0xc900000 if m=='AL_MAIN.BIN' else 0x8c010000) for m,p in binary_paths.items()}
 files=[Path(__file__),registry_path,PROJECT/'ghidra/export_draw_object_effects.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+list(binary_paths.values());records=[];unresolved=set();notes=[]
 for module,folder,count in (('AL_MAIN.BIN','draw-object-effects',9),('1ST_READ.BIN','draw-object-effects-main',3)):
  directory=PROJECT/'analysis'/folder;diagnostics=directory/'diagnostics.tsv';group=list(csv.DictReader(diagnostics.open(),delimiter='\t'))
  assert len(group)==count and all(r['completed']=='true' and not r['error'] for r in group)
  notes.extend(dict(r,module=module,directory=directory) for r in group);files.append(diagnostics)
 for note in notes:
  module=note['module'];binary,base=binaries[module];directory=note['directory']
  address=int(note['address'],16);listing=directory/f'{address:08x}_instructions.tsv';draft=directory/f'{address:08x}_draft.c';rows=list(csv.DictReader(listing.open(),delimiter='\t'));calls=[]
  assert len(rows)*2==int(note['body_bytes'])
  for row in rows:
   a=int(row['address'],16);assert int(row['length'])==2 and binary[a-base:a-base+2]==bytes.fromhex(row['bytes'])
   if not row['instruction'].lstrip('_').startswith(('jsr ','bsr ','jmp ')):continue
   targets=[]
   for text in row['flows'].strip().split():
    target=int(text,16);target_module='AL_MAIN.BIN' if 0xc900000<=target<0xc900000+len(binaries['AL_MAIN.BIN'][0]) else '1ST_READ.BIN';old=registered.get((target_module,target));targets.append(dict(address=hex(target),module=target_module,registered=bool(old),source=old['source'] if old else None))
    if old is None:unresolved.add((target_module,target))
   calls.append(dict(site=row['address'],instruction=row['instruction'],static_targets=targets,computed_target_exhaustive=False))
  old=registered.get((module,address));records.append(dict(entry=hex(address),module=module,instruction_rows=len(rows),listing=listing.relative_to(PROJECT).as_posix(),draft=draft.relative_to(PROJECT).as_posix(),currently_registered=bool(old),registered_source=old['source'] if old else None,transfer_sites=calls));files.extend((listing,draft))
 report=dict(routines=records,instruction_rows=sum(r['instruction_rows'] for r in records),unregistered_static_targets=[dict(module=m,address=hex(a)) for m,a in sorted(unresolved)],newly_recovered_entries=0,reused_routine_credit=0,parent_recovery_credit=0,scope='Read-only exports of existing AL_MAIN function boundaries for dispatcher 0c909d80/eight selected services, plus three main-executable dependencies. Every listed instruction byte checked against its original module. Drafts/static transfer annotations only: no buildable-source, complete computed-target or behavior-verification credit.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/draw_object_effects_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Next object-effect proposals:',len(records),'bodies /',report['instruction_rows'],'byte-checked rows;',len(unresolved),'unregistered static targets; zero recovery credit.')
if __name__=='__main__':main()
