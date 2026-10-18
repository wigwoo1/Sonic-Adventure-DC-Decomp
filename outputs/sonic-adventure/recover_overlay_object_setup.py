"""Static recovery of AL_MAIN's object setup, clone and preorder walkers."""
import ast,csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
BINARY=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN'
TRANSLATOR=PROJECT/'ghidra/translate_tikal_helper.py';TEMPLATE=PROJECT/'recover_main_dispatch_shared_helpers.py'
binary=BINARY.read_bytes()
expected=json.loads((PROJECT/'analysis/overlay_face_model_selector_verification.json').read_text())['sha256'][str(BINARY)]
assert hashlib.sha256(binary).hexdigest()==expected
simple_node=next(n for n in ast.parse(TRANSLATOR.read_text()).body if isinstance(n,ast.FunctionDef) and n.name=='simple')
def signed(v,b):return v-(1<<b) if v&(1<<(b-1)) else v
text=TEMPLATE.read_text();tree=ast.parse(text)
header=ast.literal_eval(next(n.value for n in tree.body if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='HEADER' for t in n.targets)))
header=header.replace('recover_main_dispatch_shared_helpers.py','recover_overlay_object_setup.py').replace('All callees remain explicit services.','Native selector integration and other explicit callees use the bridge.')
code=ast.get_source_segment(text,next(n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name=='recover'))
code=code.replace('main-dispatch-shared-helpers/','overlay-object-setup/').replace('0x8C010000','0x0C900000')
code=code.replace('            if w>>12==0xA:todo.append(a+4+2*signed(w&4095,12))','            if w>>12==0xA:\n                target=a+4+2*signed(w&4095,12)\n                if target in rows:todo.append(target)\n                else:calls.add(a)')
old="            code=simple(a+2)+f'goto L{a+4+2*signed(w&4095,12):x};'"
new="            target=a+4+2*signed(w&4095,12)\n            code=simple(a+2)+(f'goto L{target:x};' if target in rows else f'if(!call)return -2;return call(ctx,0x{target:x}u,m,f,sr,r,pr);')"
assert code.count(old)==1;code=code.replace(old,new)
code=code.replace("code+='status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'","code+='if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'")
code=code.replace(' if(!call)return -2;\n','')
code=code.replace(' (void)fpul;(void)macl;',' (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)sr;').replace(' int status;uint32_t target;',' int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);')
code=code.replace("        if text.startswith('fmov'):","""        if a in (0x0c9044b4,0x0c9044f2,0x0c904534):
            reg=10 if a==0x0c904534 else 13
            return f'if((r[{reg}]&0x1f000000u)!=0x0c000000u)return -3;'+namespace['simple'](a)
        if text.startswith('fldi0'):
            return 'if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;'
        if text.startswith('fmov'):""")
exec(compile(code,str(TEMPLATE),'exec'),globals())
header=header.replace('These routines use integer instructions and bank/width-aware FMOV only;', 'These routines use integer instructions, guarded FLDI0 and raw FMOV;').replace('no floating-point arithmetic or PR=0 restriction is introduced.', 'PR=1 stops at the original FLDI0 before its effects.')
code=code.replace('(void)sr;', '(void)sr;(void)fload;')
exec(compile(code,str(TEMPLATE),'exec'),globals())
functions=[];routines=[]
for start,end,count,label in ((0x0c9044a0,0x0c9044e2,33,'preorder_walk'),(0x0c9044e2,0x0c90451c,29,'root_walk'),(0x0c90451c,0x0c904776,273,'clone'),(0x0c904d22,0x0c904e26,130,'setup')):
 function,record=recover(start,end,f'sa_overlay_object_{label}_{start:08x}',count)
 functions.append(function);routines.append(record)
source=PROJECT/'source/sa_overlay_object_setup.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),BINARY,TRANSLATOR,TEMPLATE,source,PROJECT/'ghidra/export_overlay_object_setup.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(module='AL_MAIN.BIN',load_address='0x0c900000',source=source.relative_to(PROJECT).as_posix(),routines=routines,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='465 byte-checked instruction rows, complete CFG, static native C. Guards stop before invalid source node reads and PR=1 FLDI0. Clone intentionally retains original shallow substreams and allocation-failure behavior. Native behavior verified separately.')
(PROJECT/'analysis/overlay_object_setup_translation.json').write_text(json.dumps(report,indent=2)+'\n')
