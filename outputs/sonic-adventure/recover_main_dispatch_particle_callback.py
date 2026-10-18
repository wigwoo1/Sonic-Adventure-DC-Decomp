"""Recover particle callbacks/shared tail and draw/state wrappers from retail bytes."""
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
header='''/* Particle callback/update/draw/state wrappers; names inferred.
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
header+='''SA_STAGE_EXPORT int sa_particle_work_update_8c0448ba(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_shared_service);
'''
code=ast.get_source_segment(template,next(n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name=='recover'))
changes={
 'main-dispatch-shared-helpers/':'main-dispatch-particle-callback/',
 'w&0xF0FF==0x400B:':'w&0xF0FF in (0x400B,0x402B):',
 '            if w==0xB:continue':'            if w==0xB:continue\n            if w&0xF0FF==0x402B:calls.add(a);continue',
 "            code+='status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'":
 "            code+='if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;'",
 '        elif w>>12==0xB or w&0xF0FF==0x400B:':
 "        elif w&0xF0FF==0x402B:\n            code=f'target=r[{w>>8&15}];'+simple(a+2)+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'\n            slots.add(a+2);terminal=True\n        elif w>>12==0xB or w&0xF0FF==0x400B:",
 ' if(!call)return -2;\n':'',
 ' (void)fpul;(void)macl;':' (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;',
 ' int status;uint32_t target;':' int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);',
 '            if w>>12==0xA:todo.append(a+4+2*signed(w&4095,12))':
 '            if w>>12==0xA:\n                target=a+4+2*signed(w&4095,12)\n                if target in rows:todo.append(target)\n                else:calls.add(a)',
 "            code=simple(a+2)+f'goto L{a+4+2*signed(w&4095,12):x};'":
 "            target=a+4+2*signed(w&4095,12)\n            code=simple(a+2)+(f'goto L{target:x};' if target in rows else f'if(!call)return -2;return call(ctx,0x{target:x}u,m,f,sr,r,pr);')",
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
        if text.startswith('fldi1 '):return f'if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][{n}]=0x3f800000u;'
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
for args in ((0x8C04481A,0x8C0448B4,'sa_particle_primary_8c04481a',27),
             (0x8C0448B4,0x8C0449C2,'sa_particle_secondary_8c0448b4',135),
             (0x8C6215D6,0x8C6215FA,'sa_particle_draw_wrapper_8c6215d6',18),
             (0x8C605DAC,0x8C605E0A,'sa_particle_render_state_save_8c605dac',47),
             (0x8C605E0A,0x8C605E68,'sa_particle_render_state_restore_8c605e0a',47)):
 function,record=recover(*args)
 if args[0]==0x8C0448B4:
  function=function.replace('SA_STAGE_EXPORT int sa_particle_secondary_8c0448b4','static int secondary_engine')
  function=function.replace('void *ctx,sa_shared_service call){','void *ctx,sa_shared_service call,uint32_t entry){')
  function=function.replace(' goto L8c0448b4;',' if(entry==0x8c0448bau)goto L8c0448ba;goto L8c0448b4;',1)
  for name,entry in (('sa_particle_secondary_8c0448b4',0x8C0448B4),('sa_particle_work_update_8c0448ba',0x8C0448BA)):
   function+=f'''\nSA_STAGE_EXPORT int {name}(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,sa_shared_service call){{
 return secondary_engine(m,f,sr,r,pr,fpul,macl,ctx,call,0x{entry:x}u);
}}\n'''
  record['shared_internal_entry']='0x8c0448ba'
 if args[0]==0x8C04481A:
  record['literal_pool_gap']=['0x8c04483c','0x8c0448a0']
  old='if(!call)return -2;return call(ctx,0x8c0448bau,m,f,sr,r,pr);'
  assert function.count(old)==1
  function=function.replace(old,'return sa_particle_work_update_8c0448ba(m,f,sr,r,pr,fpul,macl,ctx,call);')
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_particle_callback.c';source.write_text(header+'\n'.join(functions))
files=[BINARY,TRANSLATOR,TEMPLATE,Path(__file__),source]+[Path(r['listing']) for r in records]
translation_listing=PROJECT/'analysis/main-dispatch-particle-callback/8c63a804_instructions.tsv'
translation_source=PROJECT/'source/model_matrix_translate.c'
translation_baseline=PROJECT/'analysis/model_matrix_translate_verification.json'
old_translation=json.loads(translation_baseline.read_text())
assert hashlib.sha256(translation_source.read_bytes()).hexdigest()==old_translation['source_sha256']
translation_rows=list(csv.DictReader(translation_listing.open(),delimiter='\t'));assert len(translation_rows)==56
for row in translation_rows:
 a=int(row['address'],16);assert binary[a-0x8C010000:a-0x8C010000+2]==bytes.fromhex(row['bytes'])
files.extend((translation_listing,translation_source,translation_baseline))
report={'source':'source/sa_main_dispatch_particle_callback.c','routines':records,
 'existing_translation_dependency':{'address':'0x8c63a804','end_exclusive':'0x8c63a874','name':'sa_model_matrix_translate',
 'source':'source/model_matrix_translate.c','source_sha256':hashlib.sha256(translation_source.read_bytes()).hexdigest(),
 'instruction_rows':56,'listing':str(translation_listing),'prior_verification':'analysis/model_matrix_translate_verification.json',
 'retail_span_sha256':hashlib.sha256(binary[0x8C63A804-0x8C010000:0x8C63A874-0x8C010000]).hexdigest()},
 'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
 'scope':'274 unique original rows checked. Primary skips a 100-byte literal gap and tail-branches to shared secondary entry 8c0448ba, emitted once. Five entries; native behavior/drawing proof separate.'}
(PROJECT/'analysis/main_dispatch_particle_callback_translation.json').write_text(json.dumps(report,indent=2)+'\n')
