"""Connect full original clear plus explicitly separate verification support."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
def main():
 old=P/'source/sa_movie_driver_connected.c';bridge=P/'source/sa_movie_clear_connected.c';bridge.write_text(old.read_text().replace('sa_movie_driver_','sa_movie_clear_'))
 old_build=P/'build_movie_driver.ps1';build=old_build.read_text().replace("'sa_movie_driver.c'","'sa_movie_clear.c','movie_clear_probe.c'").replace('sa_movie_driver_connected.c','sa_movie_clear_connected.c').replace('movie_driver.dll','movie_clear.dll')
 bp=P/'build_movie_clear.ps1';bp.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==165
 files=[Path(__file__),old,old_build,bridge,bp,P/'analysis/movie_clear_translation.json',P/'source/movie_clear_probe.h']+[P/'source'/n for n in names]
 report=dict(compiled_source_count=165,source=bridge.relative_to(P).as_posix(),newly_registered_entries=0,verification_interpreter_is_game_code=False,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 (P/'analysis/movie_clear_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Connected full-clear research DLL: 165 sources, including verification-only probe/interpreter')
if __name__=='__main__':main()
