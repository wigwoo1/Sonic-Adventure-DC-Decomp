"""Byte-checked notification selection, work creation and candidate query."""
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
header=header[header.index('#include'):header.index('static void fload')]
header='''/* Notification-related routines, names inferred; generated checked static C.
 * Integer instructions only: all FPU modes preserved, including across calls.
 * Valid original RAM/pointer preconditions. No MMIO or memory exceptions.
 * Host 0 completes (retail results in registers); -2 at reached absent service.
 * Tail disposal preserves PR; service errors retain exact reached partial state.
 */
'''+header
code=ast.get_source_segment(template,next(n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name=='recover'))
changes={
 'main-dispatch-shared-helpers/':'main-dispatch-notification/',
 'or w&0xF0FF==0x400B:':'or w&0xF0FF in (0x400B,0x402B):',
 'else:calls.add(a);todo.append(a+4)':"else:\n                calls.add(a)\n                if w&0xF0FF!=0x402B:todo.append(a+4)",
 "            code+=f'*pr=0x{a+4:08x}u;'+simple(a+2)":
 "            code+=(f'*pr=0x{a+4:08x}u;' if w&0xF0FF!=0x402B else '')+simple(a+2)",
 "            code+='status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'":
 "            code+='if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'\n            if w&0xF0FF==0x402B:code+='return 0;';terminal=True",
 ' if(!call)return -2;\n':'',
 ' (void)fpul;(void)macl;':' (void)fpul;(void)macl;(void)f;',
}
for old,new in changes.items():
 assert code.count(old) in (1,2),(old,code.count(old))
 code=code.replace(old,new)
exec(compile(code,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
for args in ((0x8C109FDE,0x8C10A0EC,'sa_main_dispatch_notification_8c109fde',108),
             (0x8C109E50,0x8C109EAC,'sa_notification_create_8c109e50',46),
             (0x8C64BED8,0x8C64BEFE,'sa_notification_candidate_ready_8c64bed8',19)):
 function,record=recover(*args);functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_notification.c';source.write_text(header+'\n'.join(functions))
files=[BINARY,TRANSLATOR,TEMPLATE,Path(__file__),source]+[Path(r['listing']) for r in records]
report={'source':'source/sa_main_dispatch_notification.c','routines':records,
 'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
 'scope':'All 173 instruction rows and raw reachability checked; native tail disposal retains original PR. Behavioral proof is separate.'}
(PROJECT/'analysis/main_dispatch_notification_translation.json').write_text(json.dumps(report,indent=2)+'\n')
