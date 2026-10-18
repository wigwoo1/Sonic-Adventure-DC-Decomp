"""Byte-checked sound request and slot-selection recovery; symbols inferred."""
import ast,csv,hashlib,json,re
from pathlib import Path

PROJECT=Path(__file__).resolve().parent
ROOT=PROJECT.parents[1]
BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
TRANSLATOR=PROJECT/'ghidra/translate_tikal_helper.py'
TEMPLATE=PROJECT/'recover_main_dispatch_shared_helpers.py'
binary=BINARY.read_bytes()
assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
simple_node=next(n for n in ast.parse(TRANSLATOR.read_text()).body if isinstance(n,ast.FunctionDef) and n.name=='simple')
def signed(v,b):return v-(1<<b) if v&(1<<(b-1)) else v
template=TEMPLATE.read_text();tree=ast.parse(template)
header=ast.literal_eval(next(n.value for n in tree.body if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='HEADER' for t in n.targets)))
header=header[header.index('#include'):]
header=header[:header.index('static uint32_t sx')]+header[header.index('static void wr'):]
header=header[:header.index('static void fload')]+header[header.index('static void fstore'):]
header='''/* Retail sound-request/slot-selection control flow, generated from checked
 * bytes. Names are inferred. No runtime opcode interpreter or audio output.
 * Valid original 16 MiB RAM/pointer preconditions; no MMIO/CPU exceptions.
 * FLDI0 is guarded only when reached with PR=1; FMOV stores obey FR/SZ.
 * Host 0 completes (retail result in R0), -1 unsupported reached FLDI0,
 * -2 missing service at its call boundary. Errors retain partial state.
 */
'''+header
code=ast.get_source_segment(template,next(n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name=='recover'))
changes={
 'main-dispatch-shared-helpers/':'main-dispatch-sound/',
 "        assert not text.startswith('f'),text":
 "        if text=='fldi0 fr4':return 'if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;'\n        assert not text.startswith('f'),text",
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
for args in ((0x8C093F02,0x8C0940DA,'sa_sound_slot_select_8c093f02',193),
             (0x8C0940DA,0x8C09413A,'sa_sound_request_8c0940da',48)):
 function,record=recover(*args);functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_sound.c';source.write_text(header+'\n'.join(functions))
files=[BINARY,TRANSLATOR,TEMPLATE,Path(__file__),source]+[Path(r['listing']) for r in records]
report={'source':'source/sa_main_dispatch_sound.c','routines':records,
 'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
 'scope':'Raw-byte comparison and static reachability account for 241 rows; selector has no external calls. Behavioral proof is separate; descriptive sound semantics are inferred.'}
(PROJECT/'analysis/main_dispatch_sound_translation.json').write_text(json.dumps(report,indent=2)+'\n')
