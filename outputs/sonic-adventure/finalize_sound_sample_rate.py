import ast,json
from pathlib import Path
p=Path(__file__).resolve().parent
tree=ast.parse((p/'create_sound_work_metadata.py').read_text());specs=next(ast.literal_eval(n.value) for n in tree.body if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='specs' for t in n.targets))
s=(p/'register_sound_source_sizes.py').read_text();start=s.index('for stem,start,end in [');end=s.index('\n',start)
s=s[:start]+'for stem,start,end in '+repr([(name,start,end) for name,start,end,target,lower in specs])+':'+s[end:]
(p/'register_sound_work_metadata.py').write_text(s)
path=p/'analysis/sound_work_sample_rate_get_verification.json';report=json.loads(path.read_text());assert report['cases_passed']==140
report['scope']=report['scope'].replace('independent returned format signed16/channel signed8/sample width4/8/16 as applicable','independent returned 32-bit sample rate')
path.write_text(json.dumps(report,indent=2)+'\n')
path=p/'verify_sound_work_sample_rate_get.py';path.write_text(path.read_text().replace('independent returned format signed16/channel signed8/sample width4/8/16 as applicable','independent returned 32-bit sample rate'))
