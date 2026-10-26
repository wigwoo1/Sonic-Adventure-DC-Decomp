"""Replay the published 2245-case integration in the 201-source DLL, preserving proofs."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports=[]
 for name in ('retained','packet_regression','mode_regression','controls_regression','packing_regression'):
  parent=P/f'verify_movie_midi_commands_{name}.py'
  code=parent.read_text().replace('write_watch_midi_commands','write_watch_graphics_init').replace('movie_midi_commands','movie_graphics_init').replace('200-source','201-source')
  scope=dict(__name__='graphics_retained_suite',__file__=str(Path(__file__)))
  exec(compile(code,str(parent),'exec'),scope);scope['main']();files.append(parent)
  suffix='packet_regression_summary' if name=='packet_regression' else name+'_verification'
  path=P/f'analysis/movie_graphics_init_{suffix}.json';r=json.loads(path.read_text());files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,pinned
  assert r['dll_sha256']==digest(P.parents[1]/'work/sonic-adventure/build/movie_graphics_init.dll'),'Suite must actually execute the new DLL'
  reports.append(dict(suite=str(parent),report=str(path),comparisons=r.get('total_original_native_comparisons',r.get('cases_passed')),dll_sha256=r['dll_sha256']))
 parent=P/'verify_movie_midi_commands.py';code=parent.read_text().replace('import write_watch_midi_commands as fast','import write_watch_graphics_init as fast').replace('movie_midi_commands_connection.json','movie_graphics_init_connection.json').replace('movie_midi_commands_verification.json','movie_graphics_init_midi_regression_verification.json')
 marker="for definition in v.manifest['routines']:";assert code.count(marker)==1
 code=code.replace(marker,"for definition in v.old.manifest['routines']:")
 marker='files=[Path(__file__),';assert code.count(marker)==1;code=code.replace(marker,'files=[Path(__file__),parent_script,')
 scope=dict(__name__='graphics_midi_regression',__file__=str(Path(__file__)),parent_script=parent)
 exec(compile(code,str(parent),'exec'),scope);scope['main']();files.append(parent)
 path=P/'analysis/movie_graphics_init_midi_regression_verification.json';r=json.loads(path.read_text());files.append(path)
 assert r['cases_passed']==258 and r['independent_checks']==5262
 reports.append(dict(suite=str(parent),report=str(path),comparisons=258,dll_sha256=r['dll_sha256']))
 assert sum(x['comparisons'] for x in reports)==2245 and len({x['dll_sha256'] for x in reports})==1
 result=dict(total_original_native_comparisons=2245,suites=reports,dll_sha256=r['dll_sha256'],sha256={str(p):digest(p) for p in files},scope='All 2245 published direct and retained comparisons replay freshly in the 201-source DLL. Parent tests are unchanged except explicit module/export/report/manifest dependency routing. Whole-state and ordered-boundary comparisons and prior independent oracles are retained. Historical canonical proof bytes are not rewritten. Full configured startup is separate; no cold startup replay is implied here.')
 (P/'analysis/movie_graphics_init_all_retained_verification.json').write_text(json.dumps(result,indent=2)+'\n');print('Fresh retained comparisons 2245',flush=True)
if __name__=='__main__':main()
