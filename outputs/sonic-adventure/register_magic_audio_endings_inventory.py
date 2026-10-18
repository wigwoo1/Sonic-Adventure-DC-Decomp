import json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/magic_audio_endings_inventory.json';e=json.loads((p/report).read_text());assert e['original_files']==109
summary={k:v for k,v in e.items() if k!='assets'};summary['report']=report
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['magic_audio_endings_inventory']=summary;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';s=path.read_text();marker='Original magic audio end inventory:'
if marker not in s:path.write_text(s+'\n'+marker+' inventory_magic_audio_endings.py / '+report+'. '+e['scope']+' Observed108 terminal words8001 at header-declared ends;SSRACING.ADX declares an end beyond file size;104 partial final pairs require sample trimming. Overall project10%.\n')
