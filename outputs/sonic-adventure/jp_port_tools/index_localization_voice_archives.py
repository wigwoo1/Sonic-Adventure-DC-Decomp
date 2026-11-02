"""Index supplied AFS inputs privately; publish hashes/counts, never audio bytes."""
import argparse,hashlib,json,struct,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def index(path):
 size=path.stat().st_size;rows=[]
 with path.open('rb') as f:
  header=f.read(8);assert header[:4]==b'AFS\0',path
  count=struct.unpack_from('<I',header,4)[0];assert 0<count<=100000 and 8+count*8<=size
  table=f.read(count*8);assert len(table)==count*8
  for n in range(count):
   offset,length=struct.unpack_from('<II',table,n*8);assert offset+length<=size,(path,n)
   assert not length or offset>=8+count*8,(path,n)
   f.seek(offset);payload=f.read(length);assert len(payload)==length
   rows.append(dict(index=n,offset=offset,bytes=length,sha256=hashlib.sha256(payload).hexdigest()))
 return dict(path=str(path),bytes=size,sha256=sha(path),entry_count=count,entries=rows)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--workspace',type=Path,required=True);a=ap.parse_args();r=a.workspace.resolve();w=r/'work/sonic-adventure';p=r/'outputs/sonic-adventure'
 dest=w/'jp-localization-inputs';assert not dest.exists();dest.mkdir()
 linux='/mnt/'+dest.drive[0].lower()+dest.as_posix()[2:]
 command=['wsl.exe','-d','sa-dc-source-review','-u','root','--','/root/sa-source-review/build/gdi','extract','/root/sa-source-review/orig/jp-v1.007.gdi',linux,'SONICADV/EVENT_ADX.AFS']
 completed=subprocess.run(command,capture_output=True,check=True);(dest/'extraction_log.txt').write_bytes(completed.stdout+completed.stderr)
 jp=dest/'SONICADV/EVENT_ADX.AFS';us_j=w/'extracted/files/SONICADV/EVENT_ADX.AFS';us_e=w/'extracted/files/SONICADV/EVENT_ADX_US.AFS'
 originals=json.loads((w/'extracted/disc_inventory.json').read_text())
 for name,path in [('SONICADV/EVENT_ADX.AFS',us_j),('SONICADV/EVENT_ADX_US.AFS',us_e)]:
  row=next(x for x in originals['files'] if x['path'].upper()==name);assert sha(path)==row['sha256'] and path.stat().st_size==row['size']
 metadata=json.loads((p/'analysis/jp_english_localization_inventory.json').read_text());row=next(x for x in metadata['candidates'] if x['path']=='SONICADV/EVENT_ADX.AFS');assert jp.stat().st_size==row['JP_bytes']
 archives={'JP_Japanese':index(jp),'USA_Japanese':index(us_j),'USA_English':index(us_e)}
 jp_rows=archives['JP_Japanese']['entries'];us_rows=archives['USA_Japanese']['entries'];en_rows=archives['USA_English']['entries']
 pairs=[dict(index=i,JP_bytes=jp_rows[i]['bytes'],USA_Japanese_bytes=us_rows[i]['bytes'],same_nonempty_payload=bool(jp_rows[i]['bytes'] and jp_rows[i]['sha256']==us_rows[i]['sha256'])) for i in range(min(len(jp_rows),len(us_rows)))]
 report=dict(status='verified_voice_archive_structure_and_payload_hashes_not_semantic_ID_equivalence',archives=archives,JP_source_declared_voice_files=2148,JP_archive_count_matches_source=len(jp_rows)==2148,JP_USA_Japanese_shared_index_count=len(pairs),JP_USA_Japanese_same_nonempty_payload_at_same_index=sum(x['same_nonempty_payload'] for x in pairs),JP_USA_Japanese_index_comparison=pairs,English_voice_ID_mapping_verified=False,English_audio_playback_verified=False,English_localization_implemented=False,retail_data_published=False,retail_data_extracted_privately=True,script_sha256=sha(Path(__file__)),directory_inventory_sha256=sha(p/'analysis/jp_english_localization_inventory.json'),overall_completion_percent=10,goal_paused=False,playable=False)
 target=p/'analysis/jp_english_voice_archive_inventory.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(dict(status=report['status'],archive_counts={k:v['entry_count'] for k,v in archives.items()},same_nonempty_Japanese_payloads_at_same_index=report['JP_USA_Japanese_same_nonempty_payload_at_same_index'],English_voice_ID_mapping_verified=False),indent=2),flush=True)
if __name__=='__main__':main()
