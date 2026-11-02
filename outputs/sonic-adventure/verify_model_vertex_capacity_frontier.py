"""Explain a normal but empty retail model call without changing its limit."""
import bisect,csv,hashlib,json,pickle
from pathlib import Path
import verify_model_view_producer as scene
P=Path(__file__).resolve().parent;W=scene.W;q=scene.q
def main():
 target=P/'analysis/model_vertex_capacity_frontier.json';assert not target.exists()
 proof=P/'analysis/model_view_producer_verification.json';r=json.loads(proof.read_text())
 for name,h in r['sha256'].items():assert scene.sha(name)==h,name
 folder=W/'model-view-produced';provenance=json.loads((folder/'provenance.json').read_text());raw=(folder/'normal_public_return.pkl').read_bytes()
 assert hashlib.sha256(raw).hexdigest()==provenance['checkpoint_sha256'] and not provenance['faulted_state_saved']
 m,s,f,c=pickle.loads(raw);m=bytearray(m);s=q.v.effects.State.from_buffer_copy(s);f=q.v.effects.Fpu.from_buffer_copy(f)
 assert s.pc==s.pr==0x8cfed000
 root=q.get(m,0x8c044a00);mesh=q.get(m,root+4);count=q.get(m,mesh+8)
 slot=q.get(m,0x8c620a20);limit=q.get(m,slot)
 assert count==11 and limit==0
 s.pc=c['entry']=0x8c6215d6;s.r[4]=root
 result,path,after,ss,ff=q.fast.compare(m,s,f,c)
 assert result['status']==0 and result['full_state_match'] and result['ordered_boundaries_match']
 assert {0x8c6209c4,0x8c6209c8,0x8c6209ca,0x8c620f0a}<=set(path)
 assert 0x8c6209ce not in path and not result['command_packets']
 assert q.get(after,slot)==limit
 binary=(W/'extracted/1ST_READ.BIN').read_bytes();needle=slot.to_bytes(4,'little')
 literals=[0x8c010000+i for i in range(0,len(binary)-3,4) if binary[i:i+4]==needle];literal_set=set(literals)
 index=list(csv.DictReader((P/'analysis/ghidra-all/function_index.tsv').open(),delimiter='\t'));starts=[int(row['address'],16) for row in index]
 refs=[]
 for offset in range(0,len(binary)-1,2):
  word=int.from_bytes(binary[offset:offset+2],'little')
  if word>>12!=0xd:continue
  pc=0x8c010000+offset;literal=((pc+4)&~3)+(word&255)*4
  if literal not in literal_set:continue
  guess=index[bisect.bisect_right(starts,pc)-1]
  refs.append(dict(instruction=hex(pc),register=(word>>8)&15,literal=hex(literal),preceding_indexed_function=guess['address'],enclosing_function_confirmed=False))
 report=dict(status='verified_original_vertex_capacity_rejects_retail_model_before_projection',model_root=hex(root),mesh=hex(mesh),mesh_vertex_count=count,capacity_literal='0x8c620a20',capacity_slot=hex(slot),capacity=limit,capacity_directly_overridden=False,comparison='signed mesh vertex count11 > original initialized capacity0',original_early_exit_instructions=['0x8c6209c4','0x8c6209c8','0x8c6209ca','0x8c620f0a'],vertex_filter_reached=False,model_call_status=result['status'],original_instructions=len(path),ordered_boundaries=len(result['boundaries']),full_state_and_ordered_boundaries_match=True,submitted_packets=0,potential_capacity_producer_references=refs,failed_state_resumed=False,actual_model_rasterization_qualified=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,next_task='Identify and execute the original vertex-capacity/buffer owner and its allocation path. Keep the limit and buffer contents unchanged until produced by original code.',sha256={str(f):scene.sha(f) for f in (Path(__file__),proof,folder/'provenance.json',q.v.effects.DLL,P/'analysis/main-dispatch-mesh/8c6209a8_instructions.tsv')})
 target.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='sha256'},indent=2),flush=True)
if __name__=='__main__':main()
