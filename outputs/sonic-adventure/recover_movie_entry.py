"""Original MOVIE entry/startup proposals at the observed runtime load address."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
start=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:start]+'''movie_input=PROJECT.parents[1]/'work/sonic-adventure/extracted/files/SONICADV/MOVIE.BIN'
movie_bytes=movie_input.read_bytes();assert hashlib.sha256(movie_bytes).hexdigest()=='84489968b607f6f80a7ad04ad402224ddd951a65e3ae8e18429ecfa17525614b'
notes=[];diagnostics=[];excluded={}
folder='movie-entry';path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
for note in csv.DictReader(path.open(),delimiter='\\t'):
 assert note['completed']=='true' and not note['error'];notes.append(dict(note,folder=folder))
assert len(notes)==2
'''+code[end:]
marker="exec(compile(chosen,str(TEMPLATE),'exec'),globals())";assert code.count(marker)==1
code=code.replace(marker,"chosen=chosen.replace('binary[a-0x8C010000:a-0x8C010000+2]','movie_bytes[a-0x8ceb0000:a-0x8ceb0000+2]').replace('binary[start-0x8C010000:end-0x8C010000]','movie_bytes[start-0x8ceb0000:end-0x8ceb0000]');"+marker)
code=code.replace('binary[a-0x8c010000:b-0x8c010000]','movie_bytes[a-0x8ceb0000:b-0x8ceb0000]')
code=code.replace('sa_boot_file_','sa_movie_entry_').replace('source/sa_boot_file.c','source/sa_movie_entry.c').replace('analysis/boot_file_translation.json','analysis/movie_entry_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent,movie_input,PROJECT/'analysis/movie_runtime_inventory.json',PROJECT/'ghidra/export_movie_entry.ps1']")
exec(compile(code,str(parent),'exec'),globals())
report.update(module='MOVIE.BIN',retail_binary=movie_input.relative_to(PROJECT.parents[1]).as_posix(),retail_binary_sha256=hashlib.sha256(movie_bytes).hexdigest(),load_address='0x8ceb0000',scope='Original raw MOVIE entry thunk and 364-row startup body. Guarded RAM/machine/FPU adapters use the actual EB runtime code/literals. Unrecovered main/module dependencies and waits remain explicit; proposals alone receive no recovery credit.')
for r in report['routines']:r.update(module='MOVIE.BIN',load_address='0x8ceb0000')
(PROJECT/'analysis/movie_entry_translation.json').write_text(json.dumps(report,indent=2)+'\n')
