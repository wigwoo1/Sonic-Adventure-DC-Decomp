"""Uninterrupted original wait/frame/submission/IRQ/input and outer-boot checks."""
import hashlib,itertools,json
from pathlib import Path
import verify_boot_input as v
import verify_boot_wait as wait

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();boot_rows=set()
 for entry,mode,renderer,variant in itertools.product((wait.WAIT,wait.boot.BOOT),(0,4),(1,2),(0,1,2)):
  c=dict(entry=entry,mode=mode,renderer=renderer,gate=int(variant!=0),pending=int(variant==1),active=True,command_ticks=8,timeout_b=4,ticks=[0,0,20000000]+list(range(20000001,20000300)))
  original,_,_=v.previous.previous.prepare(ram,dict(c));m,s,f=v.prepare(ram,c)
  m[0x8a3380:0x8a3400]=original[0x8a3380:0x8a3400]
  # Explicit original-layout active-controller and frame-buffer tables. Their
  # natural initialization is a separate unresolved boot requirement.
  v.store(m,v.load(m,0x8c60521c),v.CHANNEL)
  v.store(m,v.load(m,0x8c6051d8),wait.NOP if variant==2 else 0)
  record,path,a,s,f=v.compare(m,s,f,c)
  assert record['status']==(0 if entry==wait.WAIT else -2) and record['stop_pc']==('0x8cfed000' if entry==wait.WAIT else '0x8c605234'),(c,record['status'],record['stop_pc'])
  targets={int(b['target'],16) for b in record['boundaries']};assert {v.previous.previous.previous.FRAME,v.previous.SUBMIT,v.previous.IRQ,v.UPDATE}<=targets
  for i,literal in enumerate((0x8c605220,0x8c605224,0x8c605228,0x8c60522c,0x8c605230)):assert v.load(a,v.load(m,literal))==v.load(a,v.CHANNEL+i*4)
  assert v.load(a,v.load(m,0x8c605218))==(0xffffffff-record['clock_reads'][-1][1])&0xffffffff
  assert v.load(a,v.load(m,0x8c60520c))==(v.load(m,v.load(m,0x8c60520c))+1)&0xffffffff
  record['independent_checks']=8;records.append(record);coverage.update(set(path)&set(wait.PER_BODY[wait.WAIT]));boot_rows.update(set(path)&set(wait.boot.geometry.PER_BODY[wait.boot.BOOT]))
  for j,pc in enumerate(path):
   if pc not in wait.PER_BODY[wait.WAIT]:continue
   w=int.from_bytes(ram[pc&0xffffff:(pc&0xffffff)+2],'little');distance=2 if w>>8 in (0x8d,0x8f) else 1
   if w>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path):edges.add((pc,path[j+distance]))
  print(len(records),'input wait/boot sequences passed',flush=True)
 routine=next(r for r in wait.manifest['routines'] if int(r['address'],16)==wait.WAIT);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
 row=dict(address=hex(wait.WAIT),static_rows=len(wait.PER_BODY[wait.WAIT]),covered_rows=len(coverage),missing_rows=[hex(a) for a in sorted(set(wait.PER_BODY[wait.WAIT])-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)])
 assert not row['missing_rows'] and not row['missing_edges'],row
 source=v.PROJECT/'source/sa_boot_wait.c';files=[Path(__file__),v.PROJECT/'verify_boot_input.py',v.MANIFEST,v.PROJECT/'analysis/boot_input_connection.json',v.PROJECT/'analysis/boot_wait_translation.json',source]
 for p in list(files[2:5]):
  obj=json.loads(p.read_text())
  for path,h in obj.get('sha256',{}).items():assert digest(path)==h,path
  files.extend(Path(path) for path in obj.get('sha256',{}))
 report=dict(source=source.relative_to(v.PROJECT).as_posix(),source_sha256=digest(source),routines=[routine],cases_passed=len(records),status_counts={'0':12,'-2':12},coverage_by_routine=[row],boot_rows_covered=len(boot_rows),independent_checks=192,external_game_service_fixtures=0,newly_registered_entries=0,next_dependency='0x8c605234',dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Twelve complete wait returns and twelve original outer-boot prefixes through actual frame, submission, synchronous IRQ callbacks, 24-slot input update, clock and five-word controller snapshot. All final/reached RAM/CPU/FPU/banks/MACH/MACL/GBR and software device state match. No game-service result fixtures.',limitations=['Initial driver/frame/active-controller tables and finite timer samples are explicit runtime fixtures, not evidence of natural startup.','IRQ delivery, controller hardware/Maple transport, DMA/GPU execution, physical timing and visible gameplay remain unproved.','The outer boot caller remains partial at 8c605234 and receives zero complete-body credit. Full decompilation, ports/demakes and graphics/texture settings remain unfinished.'],cases=records)
 (v.PROJECT/'analysis/boot_input_sequence_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('cases_passed','status_counts','coverage_by_routine','boot_rows_covered','independent_checks','next_dependency')})

if __name__=='__main__':main()
