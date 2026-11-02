"""Original buffer ownership and near-plane rejection; no retail vertices exported."""
import json,struct
from pathlib import Path
from model_scene_verified_state import Scene,q,sha,P,W
def main():
 target=P/'analysis/model_vertex_buffer_owner_verification.json';assert not target.exists()
 scene=Scene('model-view-produced');binary=(W/'extracted/1ST_READ.BIN').read_bytes()
 assert binary[0x44772:0x4477e]==bytes.fromhex('28d410e52ed218450b420900')
 assert q.get(scene.m,0x8c054814)==0x8c7ac8e0 and q.get(scene.m,0x8c054830)==0x8c60edb0
 result,path=scene.call(0x8c60edb0,[0x8c7ac8e0,4096]);assert result['status']==0 and len(path)==24
 assert q.get(scene.m,0x8c88f64c)==0x8c7ac8e0 and q.get(scene.m,0x8c88f650)==4096
 saved=scene.save('model-vertex-owner-verified')
 result,path=scene.call(0x8c6215d6,[q.get(scene.m,0x8c044a00)]);assert result['status']==0
 flags=[q.get(scene.m,0x8c7ac8e0+32*i+12) for i in range(11)];assert flags==[1]*11
 assert path.count(0x8c611276)==11 and path.count(0x8c6112b8)==11 and 0x8c6112bc not in path
 assert 0x8c6116ca in path and not result['command_packets']
 near=q.get(scene.m,q.get(scene.m,0x8c611244));near_value=struct.unpack('<f',near.to_bytes(4,'little'))[0]
 assert near_value==3.0
 all_behind=all(struct.unpack('<f',q.get(scene.m,0x8c7ac8e0+32*i+24).to_bytes(4,'little'))[0]<near_value for i in range(11));assert all_behind
 files=[Path(__file__),P/'model_scene_verified_state.py',q.v.effects.DLL,saved/'provenance.json',P/'analysis/model_view_producer_verification.json',P/'analysis/main-dispatch-vertex-workers/8c6111d6_instructions.tsv',P/'analysis/main-dispatch-mesh-services/8c611444_instructions.tsv']
 report=dict(status='verified_original_static_vertex_buffer_owner_and_near_rejection',comparisons=2,phases=scene.records,buffer='0x8c7ac8e0',capacity=4096,arguments_from_original_caller='0x8c054772..0x8c05477c',initializer_entry='0x8c60edb0',initializer_instructions=24,model_instructions=len(path),model_boundary_count=len(result['boundaries']),model_vertices_classified=11,clip_flags=flags,all_vertices_behind_near_plane=all_behind,near_plane=near_value,model_packets=0,buffer_pointer_or_capacity_directly_overridden=False,faulted_state_resumed=False,full_state_and_ordered_boundaries_match=True,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,retail_vertex_data_included=False,existing_292_source_DLL_hash_verified=True,fresh_native_rebuild_required=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('BUFFER OWNER AND NEAR REJECTION VERIFIED',len(path),flags,flush=True)
if __name__=='__main__':main()
