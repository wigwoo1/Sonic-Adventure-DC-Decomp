"""Use two-sided unique Japanese audio identities to propose cross-region IDs.

The USA English index is a candidate only until language-bank/cue use is checked.
No archive replacement or audio playback is performed here.
"""
import argparse,collections,hashlib,json
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def unique_pairs(left,right):
 a=collections.defaultdict(list);b=collections.defaultdict(list)
 for row in left:
  if row['bytes']:a[row['sha256']].append(row['index'])
 for row in right:
  if row['bytes']:b[row['sha256']].append(row['index'])
 return sorted((ids[0],b[h][0],h) for h,ids in a.items() if len(ids)==1 and len(b.get(h,[]))==1)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--workspace',type=Path,required=True);a=ap.parse_args();p=a.workspace.resolve()/'outputs/sonic-adventure'
 path=p/'analysis/jp_english_voice_archive_inventory.json';inventory=json.loads(path.read_text());archives=inventory['archives'];left=archives['JP_Japanese'];right=archives['USA_Japanese'];english=archives['USA_English']
 # Adversarial tool checks: content identity, not numerical index, selects the
 # pair; duplicates on either side and empty entries provide no unique anchor.
 def row(i,h,n=1):return dict(index=i,sha256=h,bytes=n)
 assert unique_pairs([row(0,'a'),row(1,'b')],[row(0,'b'),row(1,'a')])==[(0,1,'a'),(1,0,'b')]
 assert unique_pairs([row(0,'a'),row(1,'a')],[row(0,'a')])==[]
 assert unique_pairs([row(0,'a')],[row(0,'a'),row(1,'a')])==[]
 assert unique_pairs([row(0,'a')],[row(0,'b')])==[]
 assert unique_pairs([row(0,'a',0)],[row(0,'a',0)])==[]
 for archive in archives.values():assert sha(Path(archive['path']))==archive['sha256']
 pairs=unique_pairs(left['entries'],right['entries']);candidates=[]
 with Path(left['path']).open('rb') as jp,Path(right['path']).open('rb') as us:
  for i,j,h in pairs:
   l=left['entries'][i];r=right['entries'][j];assert j<len(english['entries'])
   jp.seek(l['offset']);us.seek(r['offset']);one=jp.read(l['bytes']);two=us.read(r['bytes'])
   assert one==two and hashlib.sha256(one).hexdigest()==h
   candidates.append(dict(JP_archive_index=i,USA_Japanese_archive_index=j,Japanese_payload_sha256=h,Japanese_payload_bytes=len(one),Japanese_payloads_independently_equal=True,proposed_USA_English_archive_index=j,USA_English_payload_sha256=english['entries'][j]['sha256'],English_ID_binding_verified=False,event_cue_or_timing_verified=False))
 used={x['JP_archive_index'] for x in candidates};unmapped=[row['index'] for row in left['entries'] if row['index'] not in used]
 report=dict(status='verified_Japanese_clip_identity_map_with_unverified_English_bank_candidates',
 Japanese_original_entries=left['entry_count'],USA_Japanese_entries=right['entry_count'],USA_English_entries=english['entry_count'],
 unique_payload_identity_pairs=len(candidates),same_index_pairs=sum(c['JP_archive_index']==c['USA_Japanese_archive_index'] for c in candidates),moved_index_pairs=sum(c['JP_archive_index']!=c['USA_Japanese_archive_index'] for c in candidates),JP_indices_without_unique_identity_anchor=unmapped,
 source_declared_load_capacity=2148,actual_JP_entries_within_source_capacity=left['entry_count']<=2148,
 metadata_interpretation_correction='VOICE_FILES is the maximum ADXF_LoadPartition count, not an assertion that the archive has exactly 2148 entries. The observed 2088 entries fit that limit.',
 candidates=candidates,independent_byte_pair_checks=len(candidates),adversarial_matcher_checks=5,
 English_voice_ID_mapping_verified=False,English_audio_playback_verified=False,English_localization_implemented=False,archive_replacement_written=False,retail_data_published=False,
 source_inventory_sha256=sha(path),script_sha256=sha(Path(__file__)),
 next_task='Verify USA language-bank selection and cue IDs before using candidate English indices. Resolve duplicate/unmatched clips through event/source context and validate dialogue timing.',overall_completion_percent=10,goal_paused=False,playable=False)
 target=p/'analysis/jp_english_voice_candidates.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
 print('Verified',len(candidates),'unique Japanese clip identities;',report['moved_index_pairs'],'move index;',len(unmapped),'JP entries have no unique anchor. English binding remains unverified.',flush=True)
if __name__=='__main__':main()
