"""Byte-backed identification of the real texture resource/loader dependency."""
import csv,hashlib,json,struct
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_child_particle_texture_frontier.json';assert not target.exists()
 retail=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=retail.read_bytes()
 assert sha(retail)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 def u32(a):return struct.unpack_from('<I',raw,a-0x8c010000)[0]
 def string(a):return raw[a-0x8c010000:].split(b'\0',1)[0].decode('ascii')
 verify=P/'analysis/player_child_particles_verification.json';v=json.loads(verify.read_text())
 assert len(v['visible_texture_frontiers'])==8 and all(x['stop_pc']=='0x8c606a9e' and x['partial_execution_discarded'] for x in v['visible_texture_frontiers'])
 texture_list=u32(0x8c0b4008);frames=u32(0x8c0b400c);names=u32(texture_list);count=u32(texture_list+4);index=struct.unpack_from('<H',raw,frames+0x10-0x8c010000)[0]
 assert (texture_list,frames,names,count,index)==(0x8c1c3068,0x8c1c67dc,0x8c1c29f0,138,132)
 assert u32(names+12*index+8)==0
 assert u32(0x8c1c245c)==u32(0x8c1c2494)==texture_list
 assert string(u32(0x8c1c2458))==string(u32(0x8c1c2490))=='EFF_REGULAR'
 assert u32(0x8c099190)==0x8c1c24a8 and u32(0x8c1c24a8)==0x8c1c2450
 inventory=P/'analysis/prs_inventory.json';prs=next(x for x in json.loads(inventory.read_text()) if x['source']=='SONICADV/EFF_REGULAR.PRS')
 asset=R/'work/sonic-adventure/extracted/files/SONICADV/EFF_REGULAR.PRS';assert asset.stat().st_size==prs['compressed_bytes']==124234 and sha(asset)==prs['compressed_sha256']
 header=bytes.fromhex(prs['header_hex']);assert header[:4]==b'PVMH' and int.from_bytes(header[10:12],'little')==count
 folder=P/'analysis/player-child-particle-loader';files=[Path(__file__),retail,verify,inventory,asset,P/'ghidra/export_player_child_particle_loader.ps1',folder/'diagnostics.tsv'];bodies=[];total=0
 for a in (0x8c0990de,0x8c099750,0x8c0997bc):
  listing=folder/f'{a:08x}_instructions.tsv';draft=folder/f'{a:08x}_draft.c';rows=list(csv.DictReader(listing.open(),delimiter='\t'));calls=[]
  for row in rows:
   pc=int(row['address'],16);data=bytes.fromhex(row['bytes']);assert len(data)==2 and raw[pc-0x8c010000:pc-0x8c010000+2]==data
   if row['instruction'].lstrip('_').startswith(('jsr ','bsr ')):
    calls.append(dict(pc=hex(pc),instruction=row['instruction'],static_targets=row['flows'].split()))
  bodies.append(dict(address=hex(a),original_rows=len(rows),calls=calls,listing=listing.relative_to(P).as_posix(),draft=draft.relative_to(P).as_posix(),native_implementation_added=False));files.extend([listing,draft]);total+=len(rows)
 assert total==229
 assert any('8c0997bc' in call['static_targets'] for body in bodies if body['address']=='0x8c099750' for call in body['calls'])
 helper_literals={hex(a):hex(u32(a)) for a in (0x8c0998cc,0x8c0998d4,0x8c0998d8,0x8c0998dc,0x8c0998e4,0x8c0998e8,0x8c0998f0,0x8c0998f4)}
 result=dict(status='actual_visible_texture_frontier_and_retail_resource_loader_identified',runtime_witnesses=8,matched_stop_pc='0x8c606a9e',texture_list=hex(texture_list),name_array=hex(names),texture_count=count,frame_array=hex(frames),frame_texture_index=index,initial_texture_descriptor=0,resource='SONICADV/EFF_REGULAR.PRS',compressed_bytes=prs['compressed_bytes'],compressed_sha256=prs['compressed_sha256'],historical_decoded_bytes=prs['decoded_bytes'],historical_decoded_sha256=prs['decoded_sha256'],historical_decoded_header_kind='PVMH',decoded_texture_count=count,resource_batch_entry='0x8c0990de',next_loader_entry='0x8c099750',file_decode_loader='0x8c0997bc',helper_literals=helper_literals,exported_original_loader_rows=total,loader_bodies=bodies,new_native_loader_implementations=0,new_original_routine_credit=0,retail_assets_included_in_publication=False,overall_completion_percent=10,
  scope='Current local compressed asset andoriginal executable bytes are verified;decoded metadata is preserved historical inventory evidence. Three full original loader listings/drafts are exported andbyte-checked,not generated orqualified native implementations. Visible draw stillrequires original texture-loader integration;no descriptor is hand-filled.',sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('TEXTURE FRONTIER VERIFIED;EFF_REGULAR138textures;229loader rows byte-checked;next099750',flush=True)
if __name__=='__main__':main()
