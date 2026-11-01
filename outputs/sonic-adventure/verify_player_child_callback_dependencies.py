"""Verify canonical original helper adapters; award no duplicate routine credit."""
import hashlib,json
from pathlib import Path
import player_child_callback_fixtures as q
P=q.P
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/player_child_callback_dependencies_verification.json';assert not target.exists()
    ram=q.v.seed_ram();seen=set();records=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    for entry,value in [(0x8c042dc4,0)]+[(0x8c04f698,x) for x in (0,0x1234,0x8000,0xffff)]:
        m,s,f,cfg=q.prepare(ram,dict(entry=entry,fallback_kind=value));s.r[0]=0xaabbccdd
        before=type(s).from_buffer_copy(s);before_f=bytes(f)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(result['status']==0,(entry,value,result['status'],result['stop_pc']))
        require(result['full_state_match'] and result['ordered_boundaries_match'],'whole state and ordered boundaries')
        require(s.pc==s.pr==before.pr and s.r[15]==before.r[15],'normal original return and stack')
        require(s.r[0]==value,'independent original zero/unsigned16 return')
        changed={0,2} if entry==0x8c04f698 else {0}
        require(all(s.r[i]==before.r[i] for i in range(16) if i not in changed),'other integer registers unchanged')
        require(s.sr==before.sr and bytes(f)==before_f,'integer helper preserves SR and floating state')
        if entry==0x8c04f698:require(s.r[2]==q.FALLBACK_KIND and q.get(m,q.FALLBACK_KIND,2)==value,'actual global pointer and unchanged input')
        seen.update(path);records.append(dict(entry=hex(entry),input=value,status=0,return_value=s.r[0],steps=len(path)))
    t=json.loads((P/'analysis/player_child_callback_dependencies_translation.json').read_text());coverage=[]
    require(t['canonical_reused_entries']==['0x8c042dc4','0x8c04f698'],'canonical original identities retained')
    for b in t['routines']:
        missing=set(b['reachable_addresses'])-{hex(a) for a in seen}
        require(not missing and not b['conditional_edges'],('whole canonical helper',b['address'],sorted(missing)))
        coverage.append(dict(address=b['address'],static_rows=b['instruction_rows'],executed_rows=b['instruction_rows'],static_outcomes=0,executed_outcomes=0,missing_rows=[],missing_outcomes=[]))
    files=[Path(__file__),P/'player_child_callback_fixtures.py',P/'analysis/player_child_callback_dependencies_translation.json',P/'analysis/player_child_callback_dependencies_connection.json',q.v.effects.DLL]
    report=dict(status='both_original_canonical_helpers_match_in_new_uniform_interface',original_native_comparisons=len(records),checks=checks,cases=records,coverage_by_routine=coverage,canonical_reused_entries=t['canonical_reused_entries'],new_original_routine_credit=0,canonical_duplicate_credit=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,overall_completion_percent=10,dll_sha256=sha(q.v.effects.DLL),scope='Exact original2-row return-zero and4-row unsigned16getter bodies run through the new interface. Zero,nonzero and unsigned-high-bit return values checked with preserved registers/SR/FPU. Existing qualified originals are reused,not registered again;uniform adapter names are not extra original routine credit.',sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('CANONICAL CHILD DEPENDENCIES',len(records),checks,'checks;all6rows;zero duplicate credit',flush=True)
if __name__=='__main__':main()
