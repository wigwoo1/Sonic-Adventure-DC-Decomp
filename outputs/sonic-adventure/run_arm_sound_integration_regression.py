"""Focused existing ARM regression/registration chain after core integration."""
import json,re,subprocess,sys,time
from pathlib import Path
p=Path(__file__).resolve().parent
lines=(p/'build_and_verify.ps1').read_text().splitlines()
tasks=[]
for line in lines:
    m=re.search(r"^& \$python \(Join-Path \$PSScriptRoot '(.*arm_sound.*\.py)'\)(.*)$",line)
    if m and m[1] not in ('prepare_arm_sound_drivers.py','verify_arm_sound_driver_inventory.py','register_arm_sound_driver_inventory.py'):
        tasks.append([m[1]]+m[2].split())
results=[]
if '--resume' in sys.argv:
    previous=json.loads((p/'analysis/arm_sound_integration_regression_run.json').read_text())
    for row in previous['results']:
        if row['exit_code']!=0:break
        assert row['task']==tasks[len(results)],'Changed task sequence cannot be resumed'
        results.append(row)
    print('Retaining',len(results),'completed stages; resuming remaining tasks',flush=True)
for task in tasks[len(results):]:
    print('RUN',*task,flush=True);start=time.monotonic()
    result=subprocess.run([sys.executable,*task],cwd=p,capture_output=True,text=True)
    print(result.stdout,result.stderr,flush=True)
    results.append({'task':task,'exit_code':result.returncode,'seconds':round(time.monotonic()-start,2),'stdout':result.stdout,'stderr':result.stderr})
    (p/'analysis/arm_sound_integration_regression_run.json').write_text(json.dumps({'complete':False,'results':results},indent=2)+'\n')
    if result.returncode:sys.exit(result.returncode)
(p/'analysis/arm_sound_integration_regression_run.json').write_text(json.dumps({'complete':True,'results':results},indent=2)+'\n')
print('Completed',len(results),'focused ARM verification/registration stages',flush=True)
