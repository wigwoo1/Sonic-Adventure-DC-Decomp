"""Recover the stored context callback; re-inventory existing native scheduler bodies."""
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
header='''/* Stored context callback thunk; descriptive name inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * Integer instructions, no FPU-mode restriction. Valid RAM/table preconditions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
'''+header[header.index('#include'):header.index('static uint32_t sx')]
code=ast.get_source_segment(template,next(n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name=='recover'))
changes={
 'main-dispatch-shared-helpers/':'main-dispatch-context-callback/',
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
for args in ((0x8C01B358,0x8C01B35E,'sa_main_dispatch_context_callback_8c01b358',3),):
 function,record=recover(*args);functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_context_callback.c';source.write_text(header+'\n'.join(functions))
dependencies=[]
for start,end,name,count,path in ((0x8C098786,0x8C09878C,'sa_task_remove_request',3,'source/sa_task_remove_request.c'),
                                 (0x8C09878C,0x8C0987CE,'sa_task_update_list',33,'source/sa_task_update_list.c')):
 _,record=recover(start,end,name,count)
 assert name in (PROJECT/path).read_text()
 record.update(source=path,source_sha256=hashlib.sha256((PROJECT/path).read_bytes()).hexdigest())
 dependencies.append(record)
files=[BINARY,TRANSLATOR,TEMPLATE,Path(__file__),source]+[Path(r['listing']) for r in records+dependencies]
files.extend(PROJECT/r['source'] for r in dependencies)
report={'source':'source/sa_main_dispatch_context_callback.c','routines':records,'existing_dependencies':dependencies,
 'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
 'boundary_evidence':{'entry':'0x8c01b358','end_exclusive':'0x8c01b35e','prior_indexed_function':'0x8c01b308',
                      'prior_indexed_body_bytes':80,'tail_target_literal':'0x8c01b3bc','tail_target':'0x8c098786',
                      'old_function_index_contains_entry':False,'read_only_seed_and_export':'ghidra/export_main_dispatch_context_callback.ps1'},
 'scope':'Three new thunk rows plus 36 reused scheduler/removal-request rows byte/CFG checked; no additional function credit for existing helpers. Tail JMP preserves PR. Native lifecycle proof is separate.'}
index=PROJECT/'analysis/ghidra-all/function_index.tsv'
assert not any(line.split('\t')[0]=='8c01b358' for line in index.read_text().splitlines())
assert int.from_bytes(binary[0x8C01B3BC-0x8C010000:0x8C01B3C0-0x8C010000],'little')==0x8C098786
report['sha256'][str(index)]=hashlib.sha256(index.read_bytes()).hexdigest()
(PROJECT/'analysis/main_dispatch_context_callback_translation.json').write_text(json.dumps(report,indent=2)+'\n')
