"""Qualify one fresh warm caller return; retain older cold evidence separately."""
import ast,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 # Reuse the unchanged earlier startup semantic oracles. Only profile/entry
 # completion, source stage and expected observed boundary count differ.
 parent=P/'verify_movie_record_register_startup.py';tree=ast.parse(parent.read_text());main=next(n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name=='main');construction=[]
 for node in main.body:
  if isinstance(node,ast.Assign) and any(isinstance(target,ast.Name) and target.id=='ns' for target in node.targets):break
  construction.append(node)
 scope={'P':P};exec(compile(ast.Module(body=construction,type_ignores=[]),str(parent),'exec'),scope);code=scope['code'].replace('movie_record_register','movie_record_address')
 assert code.count("for profile in ('warm','cold'):")==1;code=code.replace("for profile in ('warm','cold'):","for profile in ('warm',):")
 code=code.replace("r['status']==-2 and r['stop_pc']=='0x8c601874'","r['status']==0 and r['stop_pc']=='0x8cfed000'").replace("r['boundary_comparisons']==4695","r['boundary_comparisons']==4702")
 code=code.replace("==['warm','cold']","==['warm']")
 ns={'__name__':'address_warm_startup_oracles','__file__':str(Path(__file__))};exec(compile(code,str(P/'verify_movie_midi_commands_startup.py'),'exec'),ns);ns['main']()
 target=P/'analysis/movie_record_address_startup_verification.json';report=json.loads(target.read_text());path=ROOT/'work/sonic-adventure/movie_record_address_startup_warm_capture.json';capture=json.loads(path.read_text());r=capture['record']
 previous=ROOT/'work/sonic-adventure/movie_record_register_startup_warm_capture.json';old=json.loads(previous.read_text())['record']['boundaries'];assert r['boundaries'][:len(old)]==old;report['independent_checks']+=1
 assert r['packet_native_calls']=={'0x8c601874':4,'0x8c601f5a':4};report['independent_checks']+=1
 calls=[b for b in r['boundaries'] if b['target']=='0x8c601874'];helpers=[b for b in r['boundaries'] if b['target']=='0x8c601f5a']
 assert [b['arguments'][0] for b in calls]==[0,6,12,18] and [b['arguments'] for b in calls]==[b['arguments'] for b in helpers];report['independent_checks']+=2
 assert r['status']==0 and r['stop_pc']=='0x8cfed000' and r['uninterrupted_declared_entry']=='0x8c0546c0';report['independent_checks']+=3
 report.pop('actual_next_frontier',None)
 report.update(compiled_source_count=212,completed_entry='0x8c0546c0',declared_return_pc='0x8cfed000',fresh_cold_profile=False,prior_warm_boundary_prefix=len(old),current_warm_boundaries=4702,next_task='Inspect earlier startup callers; this declared function returns with no missing dependency.',scope='One fresh212-source uninterrupted configured0546c0 warm startup returns normally to its declared caller. Whole original/native machine and ordered boundaries match, including preserved old211 prefix and four pointer lookups at indices0/6/12/18. Earlier Sound/flash/SDK/MIDI/VRAM semantic oracles retained. Previous211 full cold proof is unchanged historical evidence, not rerun or relabeled. Not full game/reset/physical boot/gameplay proof.')
 files=[Path(__file__),parent,P/'verify_movie_midi_commands_startup.py',P/'capture_movie_record_address_startup.py',path,previous];report['sha256'].update({str(p):digest(p) for p in files});target.write_text(json.dumps(report,indent=2)+'\n');print('ADDRESS WARM COMPLETE',report['independent_checks'],'checks;0546c0returned;4702boundaries')
if __name__=='__main__':main()
