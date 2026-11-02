"""Read disc directory metadata and locate localization work; copy no assets."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--workspace',type=Path,required=True);a=ap.parse_args();r=a.workspace.resolve();w=r/'work/sonic-adventure';p=r/'outputs/sonic-adventure'
 tool='/root/sa-source-review/build/gdi';disc='/root/sa-source-review/orig/jp-v1.007.gdi'
 command=['wsl.exe','-d','sa-dc-source-review','-u','root','--',tool,'list',disc]
 result=subprocess.run(command,capture_output=True,check=True);log=w/'jp_localization_disc_directory.txt';assert not log.exists();log.write_bytes(result.stdout)
 japanese={}
 for line in result.stdout.decode().splitlines():
  m=re.fullmatch(r'\s*(\d+)\s+(.+)',line);assert m,line
  name=m[2].replace('\\','/').upper();assert name not in japanese;japanese[name]=dict(path=m[2],bytes=int(m[1]))
 usa_path=w/'extracted/disc_inventory.json';usa_raw=json.loads(usa_path.read_text());usa={v['path'].replace('\\','/').upper():v for v in usa_raw['files'] if not v['directory']}
 assert '1ST_READ.BIN' in japanese and '1ST_READ.BIN' in usa
 pattern=re.compile(r'(?:VOICE|EVENT_ADX|FONT|MESSAGE|MSG|SUBTITLE|(?:^|/)EC_|(?:^|/)EG_|(?:^|/)EV|(?:^|/)TITLE|(?:^|/)OPTION|(?:^|/)SUMMARY|(?:^|/)ADVERTISE)',re.I)
 candidates=[]
 for name in sorted(set(japanese)|set(usa)):
  if not pattern.search(name):continue
  left,right=japanese.get(name),usa.get(name)
  candidates.append(dict(path=name,JP_bytes=left['bytes'] if left else None,USA_bytes=right['size'] if right else None,same_path=name in japanese and name in usa,same_size=bool(left and right and left['bytes']==right['size']),content_equivalence_verified=False,localization_equivalence_verified=False))
 report=dict(status='verified_directory_metadata_and_localization_entry_points_not_English_compatibility',
 upstream_commit='25a3d3154dbe657325e97dbe8c5291086f6af5c0',Japanese_disc_files=len(japanese),USA_disc_files=len(usa),common_paths=len(set(japanese)&set(usa)),JP_only_paths=len(set(japanese)-set(usa)),USA_only_paths=len(set(usa)-set(japanese)),
 candidates=candidates,localization_candidate_paths=len(candidates),directory_command=command,directory_log_sha256=sha(log),USA_inventory_sha256=sha(usa_path),
 source_entry_points=['src/event/script.c','src/event/play.c','src/sound/stream.c','src/sound/sound.c','src/draw/text.c'],
 source_findings=['Japanese sound/stream.c declares event_adx.afs with VOICE_FILES=2148. Archive indices require cross-version validation.','Stage hint text and timing are imported from disc data through generated text.inc/values.inc includes. Equal filenames are not evidence that JP and USA offsets or message/voice IDs match.','The debug text renderer writes PowerVR store queues directly; it is not a portable replacement font renderer.'],
 English_text_imported=False,English_audio_imported=False,English_localization_code_integrated=False,retail_assets_copied_or_published=False,
 required_followup=['Index voice AFS archives and compare message/voice ID mappings.','Identify English font/UI resources and decode text using explicit encoding metadata.','Map relevant USA event/stage text tables and timing to the JP source definitions.','Retain assets as user-supplied local inputs and test complete story playback after integration.'],
 script_sha256=sha(Path(__file__)),overall_completion_percent=10,goal_paused=False,playable=False)
 target=p/'analysis/jp_english_localization_inventory.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:v for k,v in report.items() if k not in ['candidates','source_findings','required_followup','directory_command']},indent=2),flush=True)
if __name__=='__main__':main()
