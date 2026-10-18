"""Static recovery of the object morph and its complete word-copy dispatcher."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
GENERATOR=PROJECT/'recover_main_dispatch_particle_callback.py'
prefix=GENERATOR.read_text().split('functions=[];records=[]')[0]
prefix=prefix.replace("BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'", "BINARY=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN'")
prefix=prefix.replace("assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'", "assert hashlib.sha256(binary).hexdigest()==json.loads((PROJECT/'analysis/overlay_face_model_selector_verification.json').read_text())['sha256'][str(BINARY)]")
prefix=prefix.replace('main-dispatch-particle-callback/','overlay-initializer-dependencies/')
exec(compile(prefix,str(GENERATOR),'exec'),globals())
header=header.replace('Particle callback/update/draw/state wrappers; names inferred.', 'AL_MAIN model morph and word-copy dispatcher; names inferred.')
header=header.replace('#include "sh4_add_sub.h"','#include "sh4_add_sub.h"\n#include "sh4_fmac.h"\n#include "sh4_divide.h"')
header=header.replace('SA_STAGE_EXPORT int sa_particle_work_update_8c0448ba(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_shared_service);\n','')
header+='''static int valid_ram(uint32_t a,unsigned n){
 return (a&0x1f000000u)==0x0c000000u && (a&0xffffffu)<=0x1000000u-n;
}
'''

def memory_guard(w):
 n,m,low,top=(w>>8)&15,(w>>4)&15,w&15,w>>12
 address=None;size='4u'
 if top==15:
  size='((f->fpscr&0x100000u)?8u:4u)'
  if low==6:address=f'r[0]+r[{m}]'
  elif low in (8,9):address=f'r[{m}]'
  elif low==7:address=f'r[0]+r[{n}]'
  elif low==10:address=f'r[{n}]'
  elif low==11:address=f'r[{n}]-{size}'
 elif top in (1,5):address=f'r[{n if top==1 else m}]+{low*4}u'
 elif top==6 and low in (0,1,2,4,5,6):
  size=f'{1<<(low%4)}u';address=f'r[{m}]'
 elif top==2 and low in (0,1,2,4,5,6):
  size=f'{1<<(low%4)}u';address=f'r[{n}]'+(f'-{size}' if low>=4 else '')
 elif top==0 and low in (4,5,6,12,13,14):
  size=f'{1<<(low%4)}u';address=f'r[0]+r[{n if low<8 else m}]'
 elif w>>8 in (0x80,0x81,0x84,0x85):
  size=f'{1<<(w>>8&1)}u';address=f'r[{m}]+{low*(1<<(w>>8&1))}u'
 elif w==0x4f22:address='r[15]-4u'
 elif w==0x4f26:address='r[15]'
 return '' if address is None else f'if(!valid_ram({address},{size}))return -3;'

code=code.replace('    def simple(a):','    def raw_simple(a):')
marker="        assert not text.startswith('f'),text"
assert code.count(marker)==1
code=code.replace(marker,"""        if text.startswith('fneg '):return f'if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][{n}]^=0x80000000u;'
        if text.startswith('fcmp/eq '):return f'status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][{n}],f->bank[(f->fpscr>>21)&1u][{m}],&f->fpscr,sr,0);if(status)return status;'
        if text.startswith('fmac '):return f'{{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[{m}],&fr[{n}],&f->fpscr,&fr[{n}]);if(status)return status;}}'
        if text.startswith('fdiv '):return f'{{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[{n}],&fr[{m}],&f->fpscr,&fr[{n}]);if(status)return status;}}'
"""+marker)
code=code.replace('    out=[];slots=set()',"""    def simple(a):
        return f'*stop=0x{a:x}u;'+memory_guard(rows[a][0])+raw_simple(a)
    out=[];slots=set()""")
code=code.replace('void *ctx,sa_shared_service call){','void *ctx,sa_shared_service call,uint32_t *stop){')
code=code.replace(' (void)fpul;', ' uint32_t entry_pr=*pr;\n (void)entry_pr;\n (void)fpul;',1)
code=code.replace("code=simple(a+2)+'return 0;'", "code=f'*stop=0x{a:x}u;if(*pr!=entry_pr)return -3;'+simple(a+2)+'return 0;'")
overlay_code=code.replace('0x8C010000','0x0C900000')
exec(compile(overlay_code,str(TEMPLATE),'exec'),globals())
function,blend_record=recover(0xc904e80,0xc905548,'sa_overlay_model_blend_0c904e80',813)
blend_record.update(module='AL_MAIN.BIN',load_address='0x0c900000',memory_guards='Every dynamic RAM read/write is checked before instruction effects; stop PC is published. RTS checks entry PR. This is a bounded host guard, not hardware exception dispatch.')

BINARY=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=BINARY.read_bytes()
assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
COPY_JUMPS={0x8c10ce32-length for length in range(0,68,4)}
copy_code=code.replace('overlay-initializer-dependencies/','overlay-model-blend-copy/')
copy_code=copy_code.replace('if w&0xF0FF==0x402B:calls.add(a);continue','if w&0xF0FF==0x402B:todo.extend(COPY_JUMPS);continue')
old="code=f'target=r[{w>>8&15}];'+simple(a+2)+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'"
new="code=f'target=r[{w>>8&15}];'+simple(a+2)+'switch(target){'+''.join(f'case 0x{v:x}u:goto L{v:x};' for v in sorted(COPY_JUMPS))+f'default:*stop=0x{a:x}u;return -3;}}'"
assert copy_code.count(old)==1;copy_code=copy_code.replace(old,new)
exec(compile(copy_code,str(TEMPLATE),'exec'),globals())
copy,copy_record=recover(0x8c10cddc,0x8c10ce9c,'sa_word_copy_dispatch_8c10cddc',58)
copy_record.update(module='1ST_READ.BIN',load_address='0x8c010000',internal_jump_targets=[hex(a) for a in sorted(COPY_JUMPS)],memory_guards='Checked original RAM transfers and entry-PR return; unsupported computed jump stops at JMP. Short helper lengths 0..64 require aligned table indices; bulk loop preserves original reads and copies.')
source=PROJECT/'source/sa_overlay_model_blend.c';source.write_text(header+function+'\n'+copy)
files=[Path(__file__),GENERATOR,TRANSLATOR,TEMPLATE,ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN',BINARY,source,PROJECT/'ghidra/ExpandOverlayBlendCopyBody.java',PROJECT/'ghidra/export_overlay_model_blend_copy.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java',PROJECT/'recover_main_dispatch_shared_helpers.py']+[Path(r['listing']) for r in (blend_record,copy_record)]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=[blend_record,copy_record],sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='871 byte-checked rows: complete 813-row AL_MAIN morph and expanded 58-row main word-copy dispatcher, including 17 short-copy targets. Static C, original branch/delay/transfer effects and numerical backend; memory/FPU boundary guards publish partial state. Behavior proof separate.')
(PROJECT/'analysis/overlay_model_blend_translation.json').write_text(json.dumps(report,indent=2)+'\n')
