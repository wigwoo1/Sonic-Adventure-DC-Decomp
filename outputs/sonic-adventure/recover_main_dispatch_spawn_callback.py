"""Recover spawn callback and its transform/particle helpers from retail bytes."""
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
header='''/* Spawn callback and transform/particle helpers; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
'''+header[header.index('#include'):]
header=header.replace('#include "fpu_cleanup_transfer.h"', '''#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
#include "sh4_multiply_extended.h"
#include "sh4_float_i32.h"
#include "sh4_compare_bits.h"
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);''')
header+='''static void fcopy(struct sa_fpu_banks *f,unsigned n,unsigned s){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){unsigned nb=bank^(n&1u),sb=bank^(s&1u);n&=~1u;s&=~1u;
  uint32_t a=f->bank[sb][s],b=f->bank[sb][s+1];f->bank[nb][n]=a;f->bank[nb][n+1]=b;
 }else f->bank[bank][n]=f->bank[bank][s];
}
'''
code=ast.get_source_segment(template,next(n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name=='recover'))
changes={
 'main-dispatch-shared-helpers/':'main-dispatch-spawn-callback/',
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
begin=code.index('    def simple(a):')
end=code.index('    out=[];slots=set()',begin)
code=code[:begin]+'''    def simple(a):
        w,text=rows[a];n,m=(w>>8)&15,(w>>4)&15
        if text.startswith('fmov'):
            form=w&15
            if form==6:return f'fload(m,f,{n}u,r[0]+r[{m}]);'
            if form==8:return f'fload(m,f,{n}u,r[{m}]);'
            if form==9:return f'fload(m,f,{n}u,r[{m}]);r[{m}]+=(f->fpscr&0x100000u)?8u:4u;'
            if form==7:return f'fstore(m,f,{m}u,r[0]+r[{n}]);'
            if form==10:return f'fstore(m,f,{m}u,r[{n}]);'
            if form==11:return f'r[{n}]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,{m}u,r[{n}]);'
            if form==12:return f'fcopy(f,{n}u,{m}u);'
            raise AssertionError((hex(a),text))
        if text.startswith('fldi0 '):return f'if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][{n}]=0;'
        if text.startswith('fsts '):return f'f->bank[(f->fpscr>>21)&1u][{n}]=*fpul;'
        if text.startswith('float '):return f'status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][{n}]);if(status)return status;'
        if text.startswith('ftrc '):return f'status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][{n}],&f->fpscr,fpul);if(status)return status;'
        if text.startswith(('fadd ','fsub ','fmul ')):
            fn='sa_sh4_multiply_extended' if text.startswith('fmul ') else 'sa_sh4_add_sub'
            extra='' if text.startswith('fmul ') else ',1' if text.startswith('fsub ') else ',0'
            return f'{{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status={fn}(&fr[{n}],&fr[{m}],&f->fpscr,&fr[{n}]{extra});if(status)return status;}}'
        if text.startswith('fcmp/gt '):return f'status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][{n}],f->bank[(f->fpscr>>21)&1u][{m}],&f->fpscr,sr,1);if(status)return status;'
        assert not text.startswith('f'),text
        return namespace['simple'](a)
''' + code[end:]
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
for args in ((0x8C044680,0x8C0446A2,'sa_main_dispatch_spawn_callback_8c044680',17),
             (0x8C0446A2,0x8C04474A,'sa_spawn_work_transform_8c0446a2',84),
             (0x8C04474A,0x8C04481A,'sa_spawn_particle_emit_8c04474a',104)):
 function,record=recover(*args);functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_spawn_callback.c';source.write_text(header+'\n'.join(functions))
files=[BINARY,TRANSLATOR,TEMPLATE,Path(__file__),source]+[Path(r['listing']) for r in records]
report={'source':'source/sa_main_dispatch_spawn_callback.c','routines':records,
 'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
 'scope':'All 205 original instruction rows byte/CFG checked; FR/SZ-aware FMOV, reached numerical guards and explicit game-call boundaries. Native behavior/caller/lifecycle proof is separate.'}
(PROJECT/'analysis/main_dispatch_spawn_callback_translation.json').write_text(json.dumps(report,indent=2)+'\n')
