import json,os,subprocess,sys
from pathlib import Path
root=Path(__file__).resolve().parent
banks=['E_0042.MLT','E_00F5.MLT','E_0046.MLT']
reports=[]
for bank in banks:
 env=dict(os.environ,SA_MOUNTED_BANK='SONICADV/'+bank)
 subprocess.run([sys.executable,str(root/'verify_sound_bank_load_mounted_banks.py')],env=env,check=True)
 reports.append(json.loads((root/('analysis/sound_bank_load_mounted_'+bank+'_verification.json')).read_text()))
report={'cases_passed':sum(x['cases_passed'] for x in reports),'banks':reports,'scope':'Three additional original-disc banks; two FPU banks each, fresh native mount and full retail state comparison per case. Modeled sound RAM, BIOS/cache/driver-enable; disabled native debug logger. No audio playback.'}
(root/'analysis/sound_bank_load_mounted_bank_suite_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(report['cases_passed'],'additional mounted bank comparisons passed')
