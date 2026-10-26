"""Re-execute all original177 mode/SDK cases in the199-source integration."""
import json
from pathlib import Path
def main():
 P=Path(__file__).resolve().parent
 parent_script=P/'verify_movie_audio_mode.py'
 mode_manifest_path=P/'analysis/movie_audio_mode_translation.json'
 mode_manifest=json.loads(mode_manifest_path.read_text())
 code=parent_script.read_text()
 code=code.replace('import write_watch_audio_mode as fast','import write_watch_midi_packet as fast')
 code=code.replace("for definition in v.manifest['routines']:","for definition in mode_manifest['routines']:")
 code=code.replace('movie_audio_mode_connection.json','movie_midi_packet_connection.json')
 code=code.replace('movie_audio_mode_verification.json','movie_midi_packet_mode_regression_verification.json')
 code=code.replace('files=[Path(__file__),','files=[Path(__file__),parent_script,mode_manifest_path,')
 scope=dict(__name__='controls_mode_regression',__file__=__file__,parent_script=parent_script,mode_manifest_path=mode_manifest_path,mode_manifest=mode_manifest)
 exec(compile(code,str(parent_script),'exec'),scope);scope['main']()
if __name__=='__main__':main()
