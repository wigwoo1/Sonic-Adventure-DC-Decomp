"""Byte-checked frame command-buffer and actual dependency proposals."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;WAIT_PARENT=PROJECT/'recover_draw_view_registers.py'
exec(compile(WAIT_PARENT.read_text().split("folder='draw-view-registers'")[0],str(WAIT_PARENT),'exec'),globals())
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+'\n if(a==0xffd8000cu && n==4u)return 1;')
folder='boot-device';directory=PROJECT/'analysis'/folder;diagnostics=directory/'diagnostics.tsv'
notes=list(csv.DictReader(diagnostics.open(),delimiter='\t'));assert len(notes)==8 and all(n['completed']=='true' and not n['error'] for n in notes)
functions=[];routines=[]
for note in notes:
 start=int(note['address'],16);listing=directory/f'{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/')
 if start==0x8c652520:
  # The alignment selector's BF enters the adjacent word-copy function without
  # setting PR. Keep that original tail edge and reuse the real native body.
  assert binary[0x8c652528-0x8c010000:0x8c65252a-0x8c010000]==bytes.fromhex('098b')
  marker='for target in (a+4+2*signed(w&255,8),fall):todo.append(target);edges.add((a,target))'
  assert chosen.count(marker)==1
  chosen=chosen.replace(marker,'for target in (a+4+2*signed(w&255,8),fall):\n                edges.add((a,target))\n                if target!=0x8c65253e:todo.append(target)')
 exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_boot_device_{start:08x}',len(rows))
 if start==0x8c652520:
  assert function.endswith('\n}\n') and function.count('goto L8c65253e;')==1
  function=function[:-3]+'\nL8c65253e: if(!call)return -2;return call(ctx,0x8c65253eu,m,f,sr,r,pr);\n}\n'
  record['external_tail_edges']=[['0x8c652528','0x8c65253e']]
 assert function.count('sa_shared_service call,uint32_t *stop){')==1
 function=function.replace('sa_shared_service call,uint32_t *stop){','sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){\n int io_status=0;(void)io_status;(void)bus;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 # A status-bearing bus failure must not commit a destination register.
 # Ordinary RAM guards still run before the instruction's memory operation.
 function=re.sub(r'(r\[\d+\])=rd\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=rd(\2);if(io_status)return io_status;\1=loaded;}',function)
 functions.append(function);record['registration_requires_complete_behavior']=True;routines.append(record)
source=PROJECT/'source/sa_boot_device.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),WAIT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_draw_polygon_lower.py',PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/vertex_bus.h',diagnostics,PROJECT/'ghidra/export_boot_device.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,scope='Eight original frame command-buffer and copy/selection/readiness/callback bodies. Every instruction is byte checked; compilation is not behavior or recovery acceptance.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/boot_device_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated',len(routines),'device proposals /',report['instruction_rows'],'instruction rows')
