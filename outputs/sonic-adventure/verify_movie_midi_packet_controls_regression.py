"""Replay the244 original command-control cases in the199-source integration."""
import json
from pathlib import Path
def main():
 P=Path(__file__).resolve().parent
 parent_script=P/'verify_movie_audio_controls.py'
 controls_manifest_path=P/'analysis/movie_audio_controls_translation.json'
 controls_manifest=json.loads(controls_manifest_path.read_text())
 code=parent_script.read_text()
 code=code.replace('import write_watch_audio_controls as fast','import write_watch_midi_packet as fast')
 code=code.replace("for definition in v.manifest['routines']:","for definition in controls_manifest['routines']:")
 code=code.replace('movie_audio_controls_connection.json','movie_midi_packet_connection.json')
 code=code.replace('movie_audio_controls_verification.json','movie_midi_packet_controls_regression_verification.json')
 code=code.replace('files=[Path(__file__),','files=[Path(__file__),parent_script,controls_manifest_path,')
 scope=dict(__name__='midi_controls_regression',__file__=__file__,parent_script=parent_script,controls_manifest_path=controls_manifest_path,controls_manifest=controls_manifest)
 exec(compile(code,str(parent_script),'exec'),scope);scope['main']()
if __name__=='__main__':main()
