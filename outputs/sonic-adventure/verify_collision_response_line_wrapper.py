"""Whole043a40 wrapper with exact actual argument packing and real renderer."""
import hashlib,itertools,json
from pathlib import Path
import player_collision_response_fixtures as q
import write_watch_collision_response_arguments as fast
P=q.P;ENTRY=0x8c043a40
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/collision_response_line_wrapper_verification.json';assert not target.exists()
    ram=q.v.seed_ram();records=[];seen=set();checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    for points,mode,rm in itertools.product(((1,2,2,3,4,3),(-1,0,2,2,1,4),(0,1,1,1,2,2)),(0,4),(0,1)):
        m,s,f,cfg=q.prepare(ram,dict(entry=ENTRY,mode=mode,rm=rm))
        bank=(f.fpscr>>21)&1
        for j,x in enumerate(points):f.banks[bank][4+j]=q.bits(x)
        before=type(s).from_buffer_copy(s)
        result,path,m,s,f=fast.compare(m,s,f,cfg)
        require(result['status']==0,(points,mode,rm,result['status'],result['stop_pc']))
        require(result['full_state_match'] and result['ordered_boundaries_match'],'whole state and ordered boundaries')
        require(s.pc==s.pr==before.pr and s.r[15]==before.r[15],'normal return and restored stack')
        calls=[x for x in result['boundaries'] if x['target']=='0x8c63ce24']
        require(len(calls)==1,'one actual renderer call');call=calls[0];actual=call['line_wrapper_input']
        local=(before.r[15]-0x34)&0xffffffff
        require(call['arguments'][:3]==[local+0x20,1,0],'actual descriptor pointer/count/flags')
        require(actual['header']==[local+8,local,0,2],'actual original line descriptor')
        require(actual['points']==[q.bits(x) for x in points],'independent exact six floating arguments')
        require(actual['colours']==[0xffffffff,0xff8888bb],'independent exact original endpoint colours')
        require(int(call['stack'],16)==local,'actual wrapper frame;no supplied frame')
        seen.update(path);records.append(dict(points=points,mode=mode,rm=rm,status=0,steps=len(path),actual_line_arguments=actual,full_state_match=True,ordered_boundaries_match=True))
    t=json.loads((P/'analysis/player_movement_shapes_translation.json').read_text());body=next(x for x in t['routines'] if x['address']==hex(ENTRY))
    missing=set(body['reachable_addresses'])-{hex(a) for a in seen}
    require(not missing and not body['conditional_edges'],('all50rows/no conditional outcomes',sorted(missing)))
    files=[Path(__file__),P/'player_collision_response_fixtures.py',P/'write_watch_collision_response_arguments.py',P/'write_watch_player_response_clip.py',P/'analysis/player_movement_shapes_translation.json',q.v.effects.DLL]
    report=dict(status='complete_original_line_wrapper_arguments_and_real_renderer_match',original_native_comparisons=len(records),checks=checks,cases=records,coverage=dict(address=hex(ENTRY),static_rows=50,executed_rows=50,static_outcomes=0,executed_outcomes=0,missing_rows=[],missing_outcomes=[]),
        supplied_game_callee_outputs=0,supplied_execution_frames=0,new_original_routine_credit=0,overall_completion_percent=10,dll_sha256=sha(q.v.effects.DLL),
        scope='Whole original043a40 entry with declaredFR4..FR9 inputs. The actual renderer boundary is read only after original descriptor/colour/point stores execute. Exact descriptor/frame/six raw point arguments/two literal colours checked independently. Real63ce24 renderer and clipping execute;all12cases return normally. Does not qualify the renderer complete body or arbitrary numerical/device/hardware domains.',sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('LINE WRAPPER COMPLETE',len(records),checks,flush=True)
if __name__=='__main__':main()
