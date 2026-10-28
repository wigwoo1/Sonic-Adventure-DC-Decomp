"""Byte preservation, additive connection and unchanged interpreter/observer audit."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    mp=P/'analysis/movement_projection_response_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movement_projection_response_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
    for report in (t,c):
        for name,h in report['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    assert len(t['routines'])==6 and t['instruction_rows']==1005 and len(t['resume_routes'])==30
    assert set(t['unregistered_proposals'])=={'0x8c075a40','0x8c64016c','0x8c650614','0x8c640080','0x8c63a9dc'}
    assert t['canonical_reused_entries']==['0x8c055d5a'] and t['canonical_duplicate_credit']==0
    raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();count=0
    for body in t['routines']:
        for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
            a=int(row['address'],16);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes']);count+=1
    assert count==1005
    old=(P/'source/sa_movement_model_response_connected.c').read_text();new=(P/'source/sa_movement_projection_response_connected.c').read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n'
        route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movement_projection_response_'+suffix,'sa_movement_model_response_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));returns=P/'analysis/movement_projection_response_returns.json';ret=json.loads(returns.read_text());files.append(returns)
    assert len(before)==2458 and len(after)==2488 and after-before=={x+'u' for x in ret['new_call_returns']}
    assert new[:a]+old[oa:ob]+new[b:]==old
    assert (P/'source/movement_projection_response_probe.c').read_text()==(P/'source/movement_model_response_probe.c').read_text().replace('sa_movement_model_response_','sa_movement_projection_response_')
    import movement_projection_response_reference as v
    import movement_model_response_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in ret['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert v.HOST_IMPORT_RECURSION_FLOOR==4096 and v._host_sys.getrecursionlimit()>=4096
    assert (P/'write_watch_movement_projection_response.py').read_text()==(P/'write_watch_movement_model_response.py').read_text().replace('movement_model_response','movement_projection_response')
    bp=P/'analysis/movement_projection_response_previous_build_manifest.json';builds=json.loads(bp.read_text());files.append(bp);assert len(builds['files'])==2749
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    old_source=P/'source/sa_short_absolute_difference_8c055d5a.c';old_proof=P/'analysis/short_absolute_difference_8c055d5a_verification.json'
    assert digest(old_source)==json.loads(old_proof.read_text())['source_sha256'];files.extend([old_source,old_proof])
    files.extend([P/'movement_projection_response_reference.py',P/'write_watch_movement_projection_response.py',P/'write_watch_movement_model_response.py'])
    report=dict(status='verified_additive_movement_projection_response_audit',original_rows_byte_checked=1005,proposed_new_original_routines=5,new_original_routine_credit=0,canonical_duplicate_credit=0,canonical_reused_entries=t['canonical_reused_entries'],new_nonlocal_resume_exports=30,new_nonlocal_routes_qualified=0,return_membership=2488,explicit_nonfunction_resume_exports_total=596,qualified_nonlocal_routes_total=71,remaining_unqualified_nonlocal_routes=525,compiled_source_count=258,exact_previous_bridge_plus_six_entries_thirty_returns=True,reference_CPU_guards_observer_and_ABI_unchanged=True,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2749,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='1005 original instruction bytes checked. Exact previous257bridge/runtime/ABI plus6entries/30returns/30continuationexports. Canonical055d5a preserved and reused with zero duplicate credit. All prior source/interpreter/observer/guards unchanged. New nonlocal resumptions not independently qualified. No recovery credit from source generation, compilation or this audit.')
    (P/'analysis/movement_projection_response_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('PROJECTION AUDIT PASS1005rows/2488returns/2749oldbuilds;unchangedCPU/observer/ABI')
if __name__=='__main__':main()
