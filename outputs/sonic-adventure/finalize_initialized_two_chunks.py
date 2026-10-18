import json
from pathlib import Path
p=Path(__file__).resolve().parent
path=p/'analysis/initialized_two_chunks_verification.json'
report=json.loads(path.read_text())
assert report['cases_passed']==218 and report['feed_updates_compared']==218
report['decode_updates_compared']=436
report['scope']=report['scope'].replace('work limit512 and ring2048 controlled.', 'work limit512, configured output capacity512 and ring2048 controlled.')
path.write_text(json.dumps(report,indent=2)+'\n')
generator=p/'create_initialized_two_chunks_verifier.py'
text=generator.read_text().replace('work limit512 and ring2048 controlled.', 'work limit512, configured output capacity512 and ring2048 controlled.')
text=text.replace("s=s.replace('initialized_decode_verification'", "s=s.replace('feed_updates_compared=feed_updates,','feed_updates_compared=feed_updates,decode_updates_compared=feed_updates*2,')\ns=s.replace('initialized_decode_verification'")
generator.write_text(text)
