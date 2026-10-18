"""Recover cancellation and its restore dependency from checked retail bytes."""
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
header='''/* Cancellation and saved-context restore; descriptive names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * Integer instructions, no FPU-mode restriction. Valid RAM/table preconditions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
'''+header[header.index('#include'):header.index('static void fload')]
code=ast.get_source_segment(template,next(n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name=='recover'))
changes={
 'main-dispatch-shared-helpers/':'main-dispatch-cancellation/',
 'w&0xF0FF==0x400B:':'w&0xF0FF in (0x400B,0x402B):',
 '            if w==0xB:continue':'            if w==0xB:continue\n            if w&0xF0FF==0x402B:calls.add(a);continue',
 "            code+='status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'":
 "            code+='if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'",
 '        elif w>>12==0xB or w&0xF0FF==0x400B:':
 "        elif w&0xF0FF==0x402B:\n            code=f'target=r[{w>>8&15}];'+simple(a+2)+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'\n            slots.add(a+2);terminal=True\n        elif w>>12==0xB or w&0xF0FF==0x400B:",
 ' if(!call)return -2;\n':'',
 ' (void)fpul;(void)macl;':' (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;',
 ' int status;uint32_t target;':' int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);',
}
# The generic CFG and emitter both contain the JSR pattern. Change only the CFG
# occurrence; the emitter gets a separate JMP arm with no PR write.
for old,new in changes.items():
 if old=='w&0xF0FF==0x400B:':
  assert code.count(old)==2
  code=code.replace(old,new,1)
 else:
  assert code.count(old)==1,(old,code.count(old))
  code=code.replace(old,new)
exec(compile(code,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
for args in ((0x8C01AE60,0x8C01AF06,'sa_main_dispatch_cancel_8c01ae60',83),
             (0x8C01AAE0,0x8C01ABDC,'sa_main_dispatch_context_restore_8c01aae0',126)):
 function,record=recover(*args);functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_cancellation.c';source.write_text(header+'\n'.join(functions))
files=[BINARY,TRANSLATOR,TEMPLATE,Path(__file__),source]+[Path(r['listing']) for r in records]
report={'source':'source/sa_main_dispatch_cancellation.c','routines':records,
 'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
 'scope':'All 209 retail instruction rows and raw reachability checked. Restore tail-JMP preserves PR. Native behavior/caller proof is separate; stored callbacks are not executed here.'}
(PROJECT/'analysis/main_dispatch_cancellation_translation.json').write_text(json.dumps(report,indent=2)+'\n')
