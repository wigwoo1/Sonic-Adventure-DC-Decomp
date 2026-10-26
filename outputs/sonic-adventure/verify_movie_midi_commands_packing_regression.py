"""Replay all432 original MIDI packing cases in the200-source integration."""
import json
from pathlib import Path
def main():
 P=Path(__file__).resolve().parent
 parent_script=P/'verify_movie_midi_packet.py'
 packet_manifest_path=P/'analysis/movie_midi_packet_translation.json'
 packet_manifest=json.loads(packet_manifest_path.read_text())
 code=parent_script.read_text()
 code=code.replace('import write_watch_midi_packet as fast','import write_watch_midi_commands as fast')
 code=code.replace("definition=v.manifest['routines'][0];","definition=packet_manifest['routines'][0];")
 code=code.replace('movie_midi_packet_connection.json','movie_midi_commands_connection.json')
 code=code.replace('movie_midi_packet_verification.json','movie_midi_commands_packing_regression_verification.json')
 code=code.replace('files=[Path(__file__),','files=[Path(__file__),parent_script,packet_manifest_path,')
 scope=dict(__name__='midi_packing_regression',__file__=__file__,parent_script=parent_script,packet_manifest_path=packet_manifest_path,packet_manifest=packet_manifest)
 exec(compile(code,str(parent_script),'exec'),scope);scope['main']()
if __name__=='__main__':main()
