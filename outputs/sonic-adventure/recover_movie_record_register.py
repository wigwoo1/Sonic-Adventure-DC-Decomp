"""Recover original record registration and reached canonical dependencies."""
from pathlib import Path
import re
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
export=P/'ghidra/export_movie_record_register.ps1'
included={int(s,16) for s in re.findall(r"'(8c[0-9a-f]{6})'",export.read_text())};assert len(included)>=20
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
begin=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
extension='''# The callback's exported body includes a shared indirect tail. Keep the
# original15 rows together and guard the actual register target at runtime.
marker='            if w&0xF0FF==0x402B:calls.add(a);continue'
assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,'            if w&0xF0FF==0x402B:\\n                if a==0x8c010f36:todo.append(0x8c602de0)\\n                else:calls.add(a)\\n                continue')
marker="            code=f'target=r[{w>>8&15}];'+simple(a+2)+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'"
assert BASE_CHOSEN.count(marker)==1
replacement="            code=f'target=r[{w>>8&15}];'+simple(a+2)+(f'if(target==0x8c602de0u)goto L8c602de0;' if a==0x8c010f36 else '')+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'"
BASE_CHOSEN=BASE_CHOSEN.replace(marker,replacement)
marker="        assert not text.startswith('f'),text"
assert BASE_CHOSEN.count(marker)==1
controls="""        # Context-save control registers; bit-preserving reads, no writes.
        if w&0xf0ff==0x000a:return f'r[{n}]=integer->mach;'
        if w&0xf0ff==0x001a:return f'r[{n}]=*macl;'
        if w&0xf0ff==0x0012:return f'r[{n}]=*gbr;'
        if w&0xf0ff==0x006a:return f'if(*sr&0x8000u)return -4;r[{n}]=f->fpscr;'
        if w&0xf0ff==0x005a:return f'if(*sr&0x8000u)return -4;r[{n}]=*fpul;'
"""
BASE_CHOSEN=BASE_CHOSEN.replace(marker,controls+marker)
'''
code=code[:begin]+extension+f'''notes=[];diagnostics=[];excluded={{}}
included={included!r}
folder='movie-record-register';path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
for note in csv.DictReader(path.open(),delimiter='\\t'):
 assert note['completed']=='true' and not note['error'] and int(note['address'],16) in included
 notes.append(dict(note,folder=folder))
assert len(notes)==len(included)
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_record_register_').replace('source/sa_boot_file.c','source/sa_movie_record_register.c').replace('analysis/boot_file_translation.json','analysis/movie_record_register_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,PROJECT/'ghidra/export_movie_record_register.ps1']")
marker="source=PROJECT/'source/sa_movie_record_register.c';source.write_text(header+'\\n'.join(functions))";assert code.count(marker)==1
code=code.replace(marker,"header=header.replace('Geometry projection, sampling and view proposals; explicit GBR/queue state.','Original record registration/callback/work-pool proposals; explicit complete state.')\n"+marker)
code=code.replace('Original file/driver/lookup/metadata/read/close and existing math/string/platform-tail adapters. Source proposals are not recovery credit. Firmware services, uncached cache bodies, asynchronous completion and driver initialization remain explicit frontiers; no file result is substituted.','Original record registration/callback/work-pool proposals. Canonical adapters receive zero duplicate credit. Actual helpers must execute; full behavior qualification is separate, with privileged SR/error prefixes explicit. No game helper return substitution.')
exec(compile(code,str(parent),'exec'),globals())
