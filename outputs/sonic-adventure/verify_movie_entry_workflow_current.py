"""Run the frozen entry workflow with its original explicit export inputs."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
parent=P/'verify_movie_entry_workflow.py';code=parent.read_text()
# Later research exports are unrelated inputs to the historical entry proof.
# Keep its original file/report intact while verifying its actual six scripts.
marker="ps_scripts=list((P/'ghidra').glob('export_movie_*.ps1'))";assert code.count(marker)==1
names=('export_movie_entry.ps1','export_movie_main_prefix.ps1','export_movie_main_setup.ps1','export_movie_main_services.ps1','export_movie_main_allocation.ps1','export_movie_main_firmware.ps1')
code=code.replace(marker,"ps_scripts=[P/'ghidra'/name for name in "+repr(names)+']')
marker='analysis/movie_entry_workflow_verification.json';assert code.count(marker)==1
code=code.replace(marker,'analysis/movie_entry_workflow_current_verification.json')
exec(compile(code,str(parent),'exec'),dict(__file__=str(parent),__name__='__main__'))
path=P/'analysis/movie_entry_workflow_current_verification.json';report=json.loads(path.read_text());report['sha256'][str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest();report['historical_workflow_preserved']=True;report['explicit_historical_exports']=list(names);path.write_text(json.dumps(report,indent=2)+'\n')
