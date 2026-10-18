"""Recover the dispatcher prefix state gate from checked original instructions."""
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
header='''/* Dispatcher prefix gate 8c0afb10, inferred name; static C generated from
 * checked retail bytes. R4=object and R6=work (the draft signature is unreliable).
 * Host 0 completes; guest result is R0. Missing/rejected service returns -2 at
 * its reached call boundary; service/scalar errors preserve partial state.
 * FMOV obeys FR/SZ. PR=1 is rejected only at reached FMUL/FNEG, not entry.
 * Valid original RAM/pointers required; MMIO/exception delivery excluded.
 */
'''+header[header.index('#include'):]
header=header.replace('#include "fpu_cleanup_transfer.h"','#include "fpu_cleanup_transfer.h"\n#include "sh4_multiply_extended.h"')
code=ast.get_source_segment(template,next(n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name=='recover'))
changes={
 'main-dispatch-shared-helpers/':'main-dispatch-interrupt-gate/',
 "        assert not text.startswith('f'),text":
 """        if text=='fsts FPUL,fr3':return 'f->bank[(f->fpscr>>21)&1u][3]=*fpul;'
        if text=='fmul fr3,fr2':return '{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}'
        if text=='fneg fr3':return 'if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;'
        assert not text.startswith('f'),text""",
 "            code+='status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'":
 "            code+='if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'",
 ' if(!call)return -2;\n':'',
}
for old,new in changes.items():
 assert code.count(old)==1,old
 code=code.replace(old,new)
exec(compile(code,str(TEMPLATE),'exec'),globals())
function,record=recover(0x8C0AFB10,0x8C0AFC98,'sa_main_dispatch_prefix_gate_8c0afb10',153)
source=PROJECT/'source/sa_main_dispatch_prefix_gate.c';source.write_text(header+function)
files=[BINARY,TRANSLATOR,TEMPLATE,Path(__file__),source,Path(record['listing'])]
report={**record,'source':'source/sa_main_dispatch_prefix_gate.c',
 'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
 'scope':'All 153 original instruction rows checked and statically accounted for, preserving 22 conditional outcomes and seven indirect call sites. Behavioral evidence is separate.'}
(PROJECT/'analysis/main_dispatch_prefix_gate_translation.json').write_text(json.dumps(report,indent=2)+'\n')
