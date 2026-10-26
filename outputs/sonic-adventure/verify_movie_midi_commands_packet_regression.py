"""Replay all423 prior packet cases against the new200-source integration."""
import hashlib,json,types
from pathlib import Path
P=Path(__file__).resolve().parent

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 parent=P/'verify_movie_packet_flash.py';code=parent.read_text()
 code=code.replace('import write_watch_packet_flash as fast','import write_watch_midi_commands as fast')
 code=code.replace("for definition in v.manifest['routines']:","for definition in packet_manifest['routines']:")
 code=code.replace('movie_packet_flash_verification.json','movie_midi_commands_packet_regression_verification.json').replace('movie_packet_flash_connection.json','movie_midi_commands_connection.json')
 marker='files=[Path(__file__),';assert code.count(marker)==1
 code=code.replace(marker,'files=[Path(__file__),Path(parent_script),')
 manifest_path=P/'analysis/movie_packet_startup_translation.json'
 scope=dict(__name__='audio_packet_regression',__file__=str(Path(__file__)),parent_script=str(parent),packet_manifest=json.loads(manifest_path.read_text()))
 exec(compile(code,str(parent),'exec'),scope);scope['main']()
 direct=P/'analysis/movie_midi_commands_packet_regression_verification.json';r=json.loads(direct.read_text());assert r['cases_passed']==360
 prior=types.SimpleNamespace(**scope)
 parent2=P/'verify_movie_packet_flash_errors.py';code2=parent2.read_text().replace('import verify_movie_packet_flash as base','base=prior_packet_suite')
 code2=code2.replace('movie_packet_flash_errors_verification.json','movie_midi_commands_packet_errors_regression_verification.json').replace('movie_packet_flash_connection.json','movie_midi_commands_connection.json')
 assert code2.count(marker)==1;code2=code2.replace(marker,'files=[Path(__file__),Path(parent_script),')
 scope2=dict(__name__='audio_packet_error_regression',__file__=str(Path(__file__)),parent_script=str(parent2),prior_packet_suite=prior)
 exec(compile(code2,str(parent2),'exec'),scope2);scope2['main']()
 errors=P/'analysis/movie_midi_commands_packet_errors_regression_verification.json';e=json.loads(errors.read_text());assert e['cases_passed']==63 and e['dll_sha256']==r['dll_sha256']
 files=[Path(__file__),parent,parent2,manifest_path,direct,errors]
 for report in (r,e):
  for path,h in report['sha256'].items():assert digest(path)==h,path;files.append(Path(path))
 report=dict(total_original_native_comparisons=423,prior_packet_cases=360,prior_error_cases=63,independent_checks=r['independent_checks']+e['independent_checks'],dll_sha256=r['dll_sha256'],sha256={str(p):digest(p) for p in files},scope='All423previous original/native packet/settings/word-copy/CRC/compaction/failure cases execute freshly against200-source integration. Parent scripts unchanged except explicit module/report/dependency names and original24-body coverage manifest. Original/canonical proof bytes remain intact; no older startup capture is substituted.')
 (P/'analysis/movie_midi_commands_packet_regression_summary.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Fresh packet regression comparisons',423)
if __name__=='__main__':main()
