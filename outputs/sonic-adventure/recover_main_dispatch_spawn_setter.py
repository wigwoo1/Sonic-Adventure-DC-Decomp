"""Recover the state gate's spawn and callback-assignment helpers from bytes."""
import ast,csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
TRANSLATOR=PROJECT/'ghidra/translate_tikal_helper.py';TEMPLATE=PROJECT/'recover_main_dispatch_shared_helpers.py'
binary=BINARY.read_bytes()
assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
simple_node=next(n for n in ast.parse(TRANSLATOR.read_text()).body if isinstance(n,ast.FunctionDef) and n.name=='simple')
def signed(v,b):return v-(1<<b) if v&(1<<(b-1)) else v
template=TEMPLATE.read_text();tree=ast.parse(template)
header=ast.literal_eval(next(n.value for n in tree.body if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='HEADER' for t in n.targets)))
header='''/* State-gate spawn/callback setter; descriptive names inferred.
 * Static C from checked retail bytes. Integer instructions, no FPU mode guard.
 * Original guest-memory addressing/stack/SR/callee ordering is preserved.
 * Host 0 completes (guest results in registers); missing service -2 at reached
 * boundary. Requires valid original pointer/table preconditions; no MMIO.
 */
'''+header[header.index('#include'):header.index('static void fload')]
code=ast.get_source_segment(template,next(n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name=='recover'))
changes={
 'main-dispatch-shared-helpers/':'main-dispatch-spawn-setter/',
 "            code+='status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'":
 "            code+='if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'",
 ' if(!call)return -2;\n':'',
 ' (void)fpul;(void)macl;':' (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;',
 ' int status;uint32_t target;':' int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);',
}
for old,new in changes.items():
 assert code.count(old)==1,old
 code=code.replace(old,new)
exec(compile(code,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
for args in ((0x8C07B320,0x8C07B346,'sa_main_dispatch_spawn_8c07b320',19),
             (0x8C07B2B8,0x8C07B2DC,'sa_main_dispatch_callback_set_8c07b2b8',18)):
 function,record=recover(*args);functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_spawn_setter.c';source.write_text(header+'\n'.join(functions))
files=[BINARY,TRANSLATOR,TEMPLATE,Path(__file__),source]+[Path(r['listing']) for r in records]
report={'source':'source/sa_main_dispatch_spawn_setter.c','routines':records,
 'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
 'scope':'All 37 retail instruction rows and raw reachability checked. Behavioral and native caller proof are separate; stored callbacks are not executed by these helpers.'}
(PROJECT/'analysis/main_dispatch_spawn_setter_translation.json').write_text(json.dumps(report,indent=2)+'\n')
