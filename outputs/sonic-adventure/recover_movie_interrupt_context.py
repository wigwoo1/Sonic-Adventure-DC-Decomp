"""Original vector fragments and interrupt save/restore bodies, with explicit SSR/SPC."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent

def exception_generator(chosen):
 old='if w==0xB or w>>12';assert chosen.count(old)==1;chosen=chosen.replace(old,'if w in (0xB,0x2B) or w>>12')
 old='if w==0xB:continue';assert chosen.count(old)==1;chosen=chosen.replace(old,'if w in (0xB,0x2B):continue')
 old='        elif w==0xB:';assert chosen.count(old)==1
 chosen=chosen.replace(old,"        elif w==0x2B:\n            assert rows[a+2][0]==9,'This original RTE delay slot must remain NOP'\n            code=f'*stop=0x{a:x}u;status=sa_sh4_rte_begin(exception,sr,r,integer,&target,0);if(status)return status;'+simple(a+2)+'exception->returned=1;*stop=target;return 2;'\n            slots.add(a+2);terminal=True\n"+old)
 marker="        assert not text.startswith('f'),text";assert chosen.count(marker)==1
 extra=r'''        if w==0xfbfd:return 'f->fpscr^=0x200000u;'
        if w&0xf0ff==0x406a:return f'f->fpscr=r[{n}]&0x3fffffu;'
        if text.startswith(('stc.l ','sts.l ')):
            reg=(w>>8)&15;part=text.split()[1].split(',')[0].upper()
            values={'GBR':'*gbr','VBR':'*vbr','MACH':'integer->mach','MACL':'*macl','PR':'*pr','FPUL':'*fpul','FPSCR':'f->fpscr'}
            if part in ('SSR','SPC','SGR'):load=f'status=sa_sh4_exception_stc(exception,SA_EXCEPTION_{part},*sr,&value,{delay}u);if(status)return status;'
            elif part=='SR':load=f'status=sa_sh4_stc_sr(*sr,&value,{delay}u);if(status)return status;'
            elif part=='VBR':load=f'status=sa_sh4_stc_vbr(*vbr,*sr,&value,{delay}u);if(status)return status;'
            else:load=f'value={values[part]};'
            return '{uint32_t value;'+load+f'if(!valid_access(r[{reg}]-4u,4u))return -3;r[{reg}]-=4;wr(m,r[{reg}],value,4);'+'}'
        if text.startswith(('ldc.l ','lds.l ')):
            reg=(w>>8)&15;part=text.split(',')[-1].upper()
            privileged=part in ('SR','VBR','SSR','SPC')
            prefix=f'if(!(*sr&0x40000000u))return {-5 if delay else -4};' if privileged else ''
            code=prefix+f'if(!valid_access(r[{reg}],4u))return -3;'+'{uint32_t value=rd(m,r['+str(reg)+'],4);'
            if part in ('SSR','SPC'):code+=f'status=sa_sh4_exception_ldc(exception,SA_EXCEPTION_{part},value,*sr,{delay}u);if(status)return status;'
            elif part=='SR':code+=f'status=sa_sh4_ldc_sr(value,sr,r,integer,{delay}u);if(status)return status;'
            elif part=='FPSCR':code+='f->fpscr=value&0x3fffffu;'
            else:code+=dict(GBR='*gbr',VBR='*vbr',MACH='integer->mach',MACL='*macl',PR='*pr',FPUL='*fpul')[part]+'=value;'
            return code+f'r[{reg}]+=4u;'+'}'
        if w&0xf0ff in (0x0032,0x0042,0x003a):
            part={0x0032:'SSR',0x0042:'SPC',0x003a:'SGR'}[w&0xf0ff]
            return f'status=sa_sh4_exception_stc(exception,SA_EXCEPTION_{part},*sr,&r[{n}],{delay}u);if(status)return status;'
        if w&0xf0ff in (0x403e,0x404e):
            part='SSR' if w&0xf0ff==0x403e else 'SPC'
            return f'status=sa_sh4_exception_ldc(exception,SA_EXCEPTION_{part},r[{n}],*sr,{delay}u);if(status)return status;'
'''
 return chosen.replace(marker,extra+marker)

parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
a=code.index('notes=[];diagnostics=[];excluded=');b=code.index('functions=[];routines=[]')
code=code[:a]+'''notes=[];diagnostics=[];excluded={}
path=PROJECT/'analysis/movie-interrupt-context/diagnostics.tsv';diagnostics.append(path)
for note in csv.DictReader(path.open(),delimiter='\\t'):
 assert note['completed']=='true' and not note['error'];notes.append(dict(note,folder='movie-interrupt-context'))
path=PROJECT/'analysis/movie-interrupt-context/template_diagnostics.tsv';diagnostics.append(path)
for note in csv.DictReader(path.open(),delimiter='\\t'):
 notes.append(dict(address=note['address'],folder='movie-interrupt-context',kind='copied_vector_fragment'))
assert len(notes)==5
header=header.replace('#include "sh4_integer_state.h"','#include "sh4_integer_state.h"\\n#include "sh4_exception_state.h"')
header=header.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original vector fragments and interrupt context save/restore; explicit SSR/SPC state.')
header=header.replace('Host 0 completes; guest results remain in registers. Missing service -2','Host 0 completes; internal status2 propagates an actual RTE unwind. Missing service -2')
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+'\\n if(n==4u && (a==0xff000020u||a==0xff000024u||a==0xff000028u))return 1;\\n if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f6800u && (a&0x1fffffffu)<0x005f7d00u)return 1;')
'''+code[b:]
code=code.replace('sa_boot_file_','sa_movie_interrupt_context_').replace('source/sa_boot_file.c','source/sa_movie_interrupt_context.c').replace('analysis/boot_file_translation.json','analysis/movie_interrupt_context_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[PROJECT/'ghidra/export_movie_interrupt_context.ps1',PROJECT/'ghidra/ExportInterruptTemplates.java',PROJECT/'source/sh4_exception_state.h',PROJECT/'source/sh4_exception_state.c']")
marker=" exec(compile(chosen,str(TEMPLATE),'exec'),globals())";assert code.count(marker)==1
code=code.replace(marker," chosen=exception_generator(chosen)\n"+marker)
marker=' functions.append(function);routines.append(record)';assert code.count(marker)==1
extra=r''' function=function.replace('const uint32_t *vbr){','uint32_t *vbr,struct sa_sh4_exception_state *exception){')
 function=function.replace(' int io_status=0;',' if(!exception)return -2;\n int io_status=0;',1)
 # Keep the same delayed-load failure behavior as the later ABI adapters.
 function=re.sub(r'(r\[\d+\])=sx\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=sx(\2);if(io_status)return io_status;\1=loaded;}',function)
 function=function.replace('(void)entry_pr;','(void)r;(void)entry_pr;')
 for row in rows:
  pc=int(row['address'],16);word=int.from_bytes(bytes.fromhex(row['bytes']),'little')
  if word>>12==0xf:
   marker=f'L{pc:x}: ';assert function.count(marker)==1
   function=function.replace(marker,marker+f'*stop=0x{pc:x}u;if(*sr&0x8000u)return -4;')
 if note.get('kind')=='copied_vector_fragment':
  function=function.replace('struct sa_sh4_exception_state *exception){','struct sa_sh4_exception_state *exception,uint32_t origin){')
  source_start=start
  for row in rows:
   pc=int(row['address'],16);text=row['instruction'].lstrip('_')
   if text.startswith(('mov.l 0x','mov.w 0x','mova 0x')):
    address=int(re.search(r'0x([0-9a-f]+)',text)[1],16)
    assert start<=address<start+52
    function=function.replace(f'0x{address:x}u',f'(origin+0x{address-start:x}u)')
   function=function.replace(f'*stop=0x{pc:x}u',f'*stop=origin+0x{pc-start:x}u')
  record['kind']='copied_vector_fragment';record['template_end_exclusive']=hex(start+52)
  record['relocatable_entry']=True;record['additional_recovery_credit']=0
 else:record['kind']='original_interrupt_context'
'''
code=code.replace(marker,extra+marker)
exec(compile(code,str(parent),'exec'),globals())
report['scope']='Byte-checked original copied vector fragments and complete interrupt save/restore/return-helper proposals. Explicit SSR/SPC/SGR state, original NOP delay slots and internal RTE unwind token; no guest result or completion supplied. Native behavior/qualification separate.'
report['sha256'][str(P/'recover_movie_interrupt_context.py')]=hashlib.sha256((P/'recover_movie_interrupt_context.py').read_bytes()).hexdigest()
(P/'analysis/movie_interrupt_context_translation.json').write_text(json.dumps(report,indent=2)+'\n')
