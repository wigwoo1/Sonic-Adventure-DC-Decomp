"""Exercise immutable jobs through the adapter, including Off with no VRAM."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
file=P/'verify_pixel_jobs.py';source=file.read_text()
assert source.count('None,0,pixels')==3
source=source.replace('import pixel_jobs_native as j','import pixel_banked_native as j').replace("target=P/'analysis/pixel_jobs_verification.json'","target=P/'analysis/banked_pixel_jobs_unit_verification.json'").replace('LIB.sa_pixel_job_execute(','LIB.sa_banked_pixel_job_execute(').replace('None,0,pixels','None,0,None,0,pixels')
exec(compile(source,str(file),'exec'),dict(__file__=str(file),__name__='__main__'))
target=P/'analysis/banked_pixel_jobs_unit_verification.json';report=json.loads(target.read_text())
report.update(adapter_Off_without_VRAM_or_scratch_verified=True,transformed_test_sha256=hashlib.sha256(source.encode()).hexdigest())
for f in (Path(__file__),P/'pixel_banked_native.py',P/'source/banked_pixel_jobs.c',P/'source/banked_pixel_jobs.h',P/'build_banked_pixel_jobs.ps1'):report['sha256'][str(f)]=hashlib.sha256(f.read_bytes()).hexdigest()
target.write_text(json.dumps(report,indent=2)+'\n')
