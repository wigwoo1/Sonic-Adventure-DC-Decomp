"""Genuine completed-call stack reuse exposes the unchecked child-output contract."""
import csv,hashlib,json
from pathlib import Path
import player_collision_response_fixtures as q
import write_watch_collision_response_contracts as fast
P=q.P;ENTRY=0x8c072946
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/collision_response_caller_contracts.json';assert not target.exists()
    ram=q.v.seed_ram();records=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    def run(label,m,s,f,cfg):
        initial_sp=s.r[15]
        r,path,m,s,f=fast.compare(m,s,f,cfg)
        require(r['full_state_match'] and r['ordered_boundaries_match'],'whole state and ordered boundaries')
        child=[b for b in r['boundaries'] if 'response_output_before_child' in b]
        after=[b for b in r['boundaries'] if 'response_output_after_child' in b]
        require(len(child)==len(after)==1,'one actual child call and one subsequent length read')
        before=child[0]['response_output_before_child'];read=after[0]['response_output_after_child']
        require(int(child[0]['stack'],16)==initial_sp-0x88,'actual caller frame')
        require(before['output']==read['output']==initial_sp-0x28,'actual original caller-owned output pointer')
        require(read['state_words']==read['words'],'original copies the child output to state before length check')
        records.append(dict(label=label,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),before=before,after=read,full_state_match=True,ordered_boundaries_match=True))
        return r,m,s,f
    m,s,f,cfg=q.prepare(ram,dict(entry=ENTRY,flags=1,collision_flags=0x88,velocity=(2,0,0),null_collision_info=True))
    r,m,s,f=run('cold declared null collision information',m,s,f,cfg)
    require(r['status']==-1 and r['stop_pc']=='0x8c639b68','cold zero backing bytes reach retained numeric frontier')
    require(records[-1]['before']['words']==records[-1]['after']['words']==[0,0,0],'rejected child does not initialize cold output')
    m,s,f,cfg=q.prepare(ram,dict(entry=ENTRY,flags=1,collision_flags=0x88,velocity=(2,0,0),shape_position=(1,0,0)))
    root_sp=s.r[15]
    r,m,s,f=run('first real successful call',m,s,f,cfg)
    require(r['status']==0 and s.r[15]==root_sp,'first completed original call restores stack')
    expected=[q.bits(-1),q.bits(0),q.bits(0)]
    require(records[-1]['after']['words']==expected,'first original child writes negativeX direction')
    # Carry the actual completed RAM/CPU/FPU forward. Only ordinary call arguments
    # and actor/collision/motion inputs change; no stack byte is seeded or repaired.
    q.put(m,q.COLLISION+0x18,0);q.vector(m,q.B+4,(2,0,0));q.vector(m,q.C+0x50,(0,1,0))
    q.put(m,q.A+0x18,0);q.put(m,q.B+0x20,0)
    s.pc=cfg['entry']=ENTRY;s.r[4]=q.A;s.r[5]=q.B;s.r[6]=q.C
    r,m,s,f=run('second call reuses real prior stack with null collision information',m,s,f,cfg)
    require(r['status']==0 and s.r[15]==root_sp,'warm rejected-child response can return normally')
    require(records[-1]['before']['collision_info']==0,'second actual child rejects null collision information')
    require(records[-1]['before']['words']==records[-1]['after']['words']==expected,'unchecked output read retains genuine prior-call direction')
    listing=P/'analysis/player-movement-owner-dependencies/8c072946_instructions.tsv'
    rows={int(x['address'],16):x['instruction'].lstrip('_') for x in csv.DictReader(listing.open(),delimiter='\t')}
    require(rows[0x8c0729a0]=='mov r15,r5' and rows[0x8c0729a2]=='add #0x60,r5' and rows[0x8c0729a4]=='jsr @r2','original output argument provenance')
    between=[rows[a] for a in sorted(rows) if 0x8c0729a8<=a<0x8c0729c2]
    require(not any(x.startswith(('bt ','bf ','bt/s ','bf/s ','bra ')) for x in between),'original caller does not branch on child result before copying output')
    files=[Path(__file__),P/'write_watch_collision_response_contracts.py',P/'write_watch_collision_response_arguments.py',P/'player_collision_response_fixtures.py',listing,q.v.effects.DLL]
    report=dict(status='unchecked_child_output_contract_demonstrated_with_real_call_stack_reuse',original_native_comparisons=3,checks=checks,cases=records,supplied_game_callee_outputs=0,supplied_execution_frames=0,stack_bytes_seeded_or_repaired=0,
        caller_frame_bytes=0x88,child_frame_bytes=0x58,output_offset_within_caller_frame=0x60,output_size=12,child_output_initialization_on_rejection=False,caller_checks_child_result_before_output_copy=False,
        retail_validity_invariant_proven=False,response_helper_registered=False,new_original_routine_credit=0,overall_completion_percent=10,dll_sha256=sha(q.v.effects.DLL),
        scope='Cold and genuine two-call carried-state witnesses. The first successful real response call creates the stack output;the next rejected child leaves it unchanged and the caller consumes it. No private stack value,callee result or execution frame is supplied. This proves the observed software contract and why initialization must not be invented in the decompilation. It does not prove invalid collision information occurs in ordinary retail gameplay or globally establish allocation/stack alias exclusion.',sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('CALLER CONTRACT',checks,'checks;genuine prior-call stack reused;response validity invariant remains open',flush=True)
if __name__=='__main__':main()
