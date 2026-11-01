"""Admit the reached 32-bit external interrupt-status word to the existing bus."""
import ast,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def write(name,text):
 p=P/name;assert not p.exists();p.write_text(text);return p
prior=P/'source/sa_renderer_input_tick.c';text=prior.read_text()
marker=' if(valid_ram(a,n))return 1;';assert text.count(marker)==1
text=text.replace(marker,marker+'\n if(n==4u && !(a&3u) && (a&0x1fffffffu)==0x005f6908u)return 1;')
source=write('source/sa_renderer_input_tick_mmio.c',text)
recipe=write('build_renderer_input_tick_mmio.ps1',(P/'build_renderer_input_tick.ps1').read_text().replace("'sa_renderer_input_tick.c'","'sa_renderer_input_tick_mmio.c'").replace('renderer_input_tick.dll','renderer_input_tick_mmio.dll'))
generated=[source,recipe,write('renderer_input_tick_mmio_reference.py',(P/'renderer_input_tick_reference.py').read_text().replace('build/renderer_input_tick.dll','build/renderer_input_tick_mmio.dll'))]
for suffix in ('_base','_state','_capture',''):
 generated.append(write('write_watch_renderer_input_tick_mmio'+suffix+'.py',(P/('write_watch_renderer_input_tick'+suffix+'.py')).read_text().replace('write_watch_renderer_input_tick','write_watch_renderer_input_tick_mmio').replace('renderer_input_tick_reference','renderer_input_tick_mmio_reference')))
generated.append(write('renderer_input_tick_mmio_fixtures.py',(P/'renderer_input_tick_fixtures.py').read_text().replace('write_watch_renderer_input_tick','write_watch_renderer_input_tick_mmio').replace('renderer_input_tick_reference','renderer_input_tick_mmio_reference')))
test=(P/'verify_renderer_input_tick.py').read_text().replace('import renderer_input_tick_fixtures as q','import renderer_input_tick_mmio_fixtures as q').replace('renderer_input_tick_verification.json','renderer_input_tick_mmio_verification.json').replace('build/renderer_input_tick.dll','build/renderer_input_tick_mmio.dll')
test=test.replace('r,path,a,after,fp=q.fast.compare(m,s,f,c)','before_f=bytes(f)\n  r,path,a,after,fp=q.fast.compare(m,s,f,c)').replace('assert bytes(fp)==bytes(f)','assert bytes(fp)==before_f')
test=test.replace("P/'renderer_input_tick_reference.py'","P/'renderer_input_tick_mmio_reference.py'").replace("P/'renderer_input_tick_fixtures.py'","P/'renderer_input_tick_mmio_fixtures.py'").replace("P/'write_watch_renderer_input_tick.py'","P/'write_watch_renderer_input_tick_mmio.py'")
generated.append(write('verify_renderer_input_tick_mmio.py',test))
for p in generated:
 if p.suffix=='.py':ast.parse(p.read_text(),filename=str(p))
files=[Path(__file__),prior,P/'source/startup_platform_bus.c',P/'analysis/renderer-input-irq-tail/8c6463ac_instructions.tsv',*generated]
report=dict(status='reached_external_status_register_admitted_pending_qualification',allowed_physical_register='0x005f6908',width=4,alignment=4,old_candidate_preserved=True,guest_instruction_bodies_unchanged=True,wait_guards_changed=False,comparison_checks_unchanged=True,peripheral_results_supplied=False,new_original_routine_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
target=P/'analysis/renderer_input_tick_mmio_connection.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('Added reached 32-bit MMIO register guard; all old candidates preserved',flush=True)
