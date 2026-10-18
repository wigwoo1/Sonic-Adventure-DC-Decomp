import json
from pathlib import Path
p=Path(__file__).resolve().parent;path=p/'analysis/initialized_stop_verification.json';report=json.loads(path.read_text())
assert report['cases_passed']==218 and report['feed_updates_compared']==218
report['scope']=report['scope'].replace('work state1 becomes0','work state byte1 becomes0')
path.write_text(json.dumps(report,indent=2)+'\n')
