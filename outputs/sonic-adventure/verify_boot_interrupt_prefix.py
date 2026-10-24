"""Actual wait/outer-boot prefixes through original IRQ callbacks to input frontier."""
import hashlib,itertools,json
from pathlib import Path
import verify_boot_interrupt as v

def main():
 wait=v.previous.previous.previous;boot=wait.boot;ram=v.previous.previous.seed_ram();records=[];coverage={wait.WAIT:set(),boot.BOOT:set()}
 for entry,mode,renderer,flags in itertools.product((wait.WAIT,boot.BOOT),(0,4),(1,2),(0,32)):
  c=dict(entry=entry,mode=mode,renderer=renderer,flags=flags,gate=1,pending=1,active=True,command_ticks=8,timeout_b=4,ticks=[0,0,20000000]+list(range(20000001,20000300)))
  original,_,_=v.previous.prepare(ram,dict(c));m,s,f=v.prepare(ram,c)
  # Preserve the existing, explicitly initialized FRAME runtime-table fixture.
  # The interrupt verifier clears neighboring BSS for its direct tests.
  m[0x8a3380:0x8a3400]=original[0x8a3380:0x8a3400]
  record,path,actual,s,f=v.compare(m,s,f,c)
  assert record['status']==-2 and record['stop_pc']=='0x8c601e2a',record
  targets={int(b['target'],16) for b in record['boundaries']}
  assert {v.previous.previous.FRAME,v.SUBMIT,v.IRQ,0x8c601e2a}<=targets
  assert v.load(actual,0x8c8a30c0)==0
  for a in coverage:coverage[a].update(set(path)&set(wait.PER_BODY[wait.WAIT] if a==wait.WAIT else boot.geometry.PER_BODY[a]))
  record['independent_checks']=3;records.append(record)
 files=[Path(__file__),v.PROJECT/'verify_boot_interrupt.py',v.PROJECT/'analysis/boot_interrupt_translation.json',v.PROJECT/'analysis/boot_interrupt_connection.json']
 report=dict(cases_passed=len(records),independent_checks=len(records)*3,wait_rows_covered=len(coverage[wait.WAIT]),boot_rows_covered=len(coverage[boot.BOOT]),newly_registered_entries=0,frontier='0x8c601e2a',external_game_service_fixtures=0,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Sixteen full-state direct wait/outer-boot comparisons through actual frame/submission/IRQ code to the unresolved input boundary. Peripheral timer/register/VRAM state is compared throughout each uninterrupted call. Runtime tables and CPU/device initial states are explicit fixtures; natural startup and asynchronous interrupt delivery remain unproved.',cases=records)
 (v.PROJECT/'analysis/boot_interrupt_prefix_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print({k:report[k] for k in ('cases_passed','independent_checks','wait_rows_covered','boot_rows_covered','frontier')})

if __name__=='__main__':main()
