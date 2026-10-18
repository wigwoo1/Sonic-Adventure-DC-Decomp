import json
from pathlib import Path
p=Path(__file__).resolve().parent;path=p/'analysis/native_audio_full_file_verification.json';r=json.loads(path.read_text())
assert r['cases_passed']==2 and all(a['total_samples']==156653 and a['full_file_published'] for a in r['assets'])
note=' Controlled caller requests normally256 samples or contiguous output space,then512 for the final tail when remaining samples fit an empty ring; native completion trims padding. Variable input-window update sizes are independently derived. The failed earlier exact-tail request diagnostic is retained as an investigation artifact,not passing evidence.'
if note not in r['scope']:r['scope']+=note
path.write_text(json.dumps(r,indent=2)+'\n')
print([{k:a[k] for k in ['path','bank','total_samples','streamed_updates','full_file_published']} for a in r['assets']])
