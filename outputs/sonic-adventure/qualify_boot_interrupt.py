"""Qualify full instruction coverage, explicit special submission, and dead edges."""
import csv,hashlib,itertools,json
from pathlib import Path
import verify_boot_interrupt as v

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 proof_path=v.PROJECT/'analysis/boot_interrupt_verification.json';proof=json.loads(proof_path.read_text())
 for path,h in proof['sha256'].items():assert digest(path)==h,path
 assert proof['cases_passed']==581 and proof['status_counts']=={'0':558,'-4':7,'-3':4,'-1':12}
 assert digest(v.effects.DLL)==proof['dll_sha256']
 records=[];coverage=set();edges=set();ram=v.previous.previous.seed_ram()
 for mode,slot,header in itertools.product((0,4),(0,3),(0,1)):
  c=dict(entry=v.SUBMIT,mode=mode,slot=slot,extra_header=header,renderer=1,lengths=[32]*5,command_ticks=8,timeout_b=4,submission_flags=32)
  m,s,f=v.prepare(ram,c);v.store(m,0x8c8a316c,32)
  before=v.effects.State.from_buffer_copy(s);fp=v.effects.Fpu.from_buffer_copy(f)
  record,path,actual,s,f=v.compare(m,s,f,c);assert record['status']==0
  record['independent_checks']=v.semantic(m,actual,before,fp,s,f,record,c)
  assert len(record['command_packets'])==1 and record['command_packets'][0][2][0]==v.load(m,v.load(m,0x8c641fdc))
  record['independent_checks']+=1;records.append(record);coverage.update(path)
  for j,a in enumerate(path):
   if a not in v.OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little');distance=2 if w>>8 in (0x8d,0x8f) else 1
   if w>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path):edges.add((a,path[j+distance]))
 # These outcomes are impossible in the qualified immutable-code/literal and
 # distinct RAM/stack/MMIO domain. Keep them explicit, never mark them executed.
 select=v.PER_BODY[v.SELECT];flip=v.PER_BODY[v.FLIP]
 assert select[0x8c651576]=='mov #0x1,r4'
 assert [(a,t) for a,t in select.items() if t.endswith(',r4')]==[(0x8c651576,'mov #0x1,r4'),(0x8c6515be,'tst r4,r4')]
 assert select[0x8c6515be]=='tst r4,r4' and select[0x8c6515c0]=='bt 0x8c6515dc'
 assert not any(t.startswith(('jsr ','bsr ')) for t in select.values())
 assert select[0x8c651582]=='mov #0x7,r11' and select[0x8c6515a8]=='mov.l r11,@(r0,r12)'
 assert select[0x8c651600]=='mov.l 0x8c651670,r0' and select[0x8c651602]=='mov.l @(r0,r12),r0' and select[0x8c651604]=='cmp/eq #-0x3,r0'
 assert v.load(ram,0x8c651670)==0x8c8a3098
 # Between storing seven and reloading the same slot, the only other writes
 # target the four immutable framebuffer/control MMIO pointers.
 stores=[(a,t) for a,t in select.items() if 0x8c6515a8<a<0x8c651602 and t.split(',')[-1].startswith('@')]
 assert [a for a,t in stores]==[0x8c6515cc,0x8c6515da,0x8c6515f4,0x8c6515fe]
 assert all(v.load(ram,a)&0x1fffffff>=0x005f8000 and v.load(ram,a)&0x1fffffff<0x005fa000 for a in (0x8c65166c,))
 assert flip[0x8c651966]=='mov #0x1,r14' and flip[0x8c651a14]=='tst r14,r14'
 assert [(a,t) for a,t in flip.items() if t.endswith(',r14') and a<0x8c651a16]==[(0x8c651966,'mov #0x1,r14'),(0x8c651a14,'tst r14,r14')]
 # The one reached layout callee saves/restores R14 on its sole successful exit.
 layout=v.PER_BODY[v.LAYOUT]
 assert layout[min(layout)]=='mov.l r14,@-r15'
 assert len([t for t in layout.values() if t=='mov.l @r15+,r14'])==1
 excluded={v.SELECT:{(0x8c6515c0,0x8c6515dc),(0x8c651606,0x8c651628)},v.FLIP:{(0x8c651a16,0x8c651ac0)}}
 rows=[]
 for old in proof['coverage_by_routine']:
  row=dict(old);a=int(row['address'],16);missing=set(int(x,16) for x in old['missing_rows'])-coverage
  missing_edges={tuple(int(x,16) for x in e) for e in old['missing_edges']}-edges
  assert not missing and missing_edges==excluded.get(a,set()),(hex(a),missing,missing_edges)
  row.update(covered_rows=row['static_rows'],missing_rows=[],conditional_edges_covered=len(next(r for r in v.manifest['routines'] if int(r['address'],16)==a)['conditional_edges'])-len(missing_edges),missing_edges=[],structurally_unreachable_edges=[[hex(x),hex(y)] for x,y in sorted(missing_edges)])
  rows.append(row)
 inputs=[Path(__file__),proof_path,v.MANIFEST,v.PROJECT/'analysis/boot_interrupt_connection.json',v.PROJECT/'analysis/boot_interrupt_bus_verification.json',v.PROJECT/'analysis/boot_interrupt_prefix_verification.json']
 for p in list(inputs[1:]):
  obj=json.loads(p.read_text())
  for path,h in obj.get('sha256',{}).items():assert digest(path)==h,path
  inputs.extend(Path(path) for path in obj.get('sha256',{}))
 report=dict(source=proof['source'],source_sha256=proof['source_sha256'],routines=proof['routines'],cases_passed=589,status_counts={'0':566,'-4':7,'-3':4,'-1':12},independent_checks=proof['independent_checks']+sum(r['independent_checks'] for r in records),coverage_by_routine=rows,backend_independent_checks=66,prefix_comparisons=16,wait_rows_covered=60,boot_rows_covered=16,next_dependency='0x8c601e2a',external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=proof['dll_sha256'],sha256={str(p):digest(p) for p in inputs},structural_scope='Three excluded outcomes remain explicitly unexecuted: SELECT R4 is constant one; SELECT reloads its own just-written seven rather than minus three; FLIP R14 remains one across the layout callee that saves/restores it. Immutable literals/code, distinct valid stack/RAM/MMIO and synchronous software entry apply. No concurrent external RAM writer is modeled.',scope=proof['scope'],limitations=proof['limitations'],special_submission_cases=records)
 (v.PROJECT/'analysis/boot_interrupt_qualification.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Qualified 13 bodies / 2180 rows; 589 comparisons; three explicitly unreachable outcomes')

if __name__=='__main__':main()
