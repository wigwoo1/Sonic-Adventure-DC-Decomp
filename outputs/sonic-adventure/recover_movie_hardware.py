"""Original control, copy and synchronization family; no substituted results."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_boot_file.py';code=parent.read_text()
marker=";header+='\\n'+division.replace('div1_step','div_step')";assert code.count(marker)==1;code=code.replace(marker,'')
start=code.index('notes=[];diagnostics=[];excluded=');end=code.index('functions=[];routines=[]')
code=code[:start]+'''notes=[];diagnostics=[];excluded={}
for folder in ('movie-hardware-control','movie-hardware-services','movie-hardware-lower','movie-hardware-hal','movie-hardware-channel'):
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\t'):
  assert note['completed']=='true' and not note['error'];notes.append(dict(note,folder=folder))
assert len(notes)==12
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+chr(10)+' if(n==4u && (a&0x1fffffffu)==0x005f688cu)return 1;'+chr(10)+' if((n==1u||n==2u||n==4u) && !(a&(n-1u)) && (a&0x1fffffffu)>=0x00702c00u && (a&0x1fffffffu)+n<=0x00702c04u)return 1;')
'''+code[end:]
code=code.replace('sa_boot_file_','sa_movie_hardware_').replace('source/sa_boot_file.c','source/sa_movie_hardware.c').replace('analysis/boot_file_translation.json','analysis/movie_hardware_translation.json')
code=code.replace("list((PROJECT/'ghidra').glob('export_boot_file*.ps1'))","[parent]+[PROJECT/'ghidra'/name for name in ('export_movie_hardware_control.ps1','export_movie_hardware_services.ps1','export_movie_hardware_lower.ps1','export_movie_hardware_hal.ps1','export_movie_hardware_channel.ps1')]")
marker=' functions.append(function);routines.append(record)';assert code.count(marker)==1
code=code.replace(marker," function=re.sub(r'(r\\[\\d+\\])=sx\\(([^;]+)\\);if\\(io_status\\)return io_status;',r'{uint32_t loaded=sx(\\2);if(io_status)return io_status;\\1=loaded;}',function)\n"+marker)
exec(compile(code,str(parent),'exec'),globals())
