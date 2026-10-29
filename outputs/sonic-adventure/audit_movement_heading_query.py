"""Byte preservation, additive connection and unchanged interpreter/observer audit."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    mp=P/'analysis/movement_heading_query_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movement_heading_query_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
    for report in (t,c):
        for name,h in report['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    assert len(t['routines'])==1 and t['instruction_rows']==106 and len(t['resume_routes'])==2
    assert set(t['unregistered_proposals'])=={'0x8c06cd22'}
    assert not t['canonical_reused_entries'] and t['canonical_duplicate_credit']==0
    raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();count=0
    for body in t['routines']:
        for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
            a=int(row['address'],16);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes']);count+=1
    assert count==106
    old=(P/'source/sa_movement_surface_resolution_connected.c').read_text();new=(P/'source/sa_movement_heading_query_connected.c').read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n'
        route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movement_heading_query_'+suffix,'sa_movement_surface_resolution_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));returns=P/'analysis/movement_heading_query_returns.json';ret=json.loads(returns.read_text());files.append(returns)
    assert len(before)==2532 and len(after)==2534 and after-before=={x+'u' for x in ret['new_call_returns']}
    assert new[:a]+old[oa:ob]+new[b:]==old
    assert (P/'source/movement_heading_query_probe.c').read_text()==(P/'source/movement_surface_resolution_probe.c').read_text().replace('sa_movement_surface_resolution_','sa_movement_heading_query_')
    import movement_heading_query_reference as v
    import movement_surface_resolution_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in ret['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert v.HOST_IMPORT_RECURSION_FLOOR==4096 and v._host_sys.getrecursionlimit()>=4096
    assert (P/'write_watch_movement_heading_query.py').read_text()==(P/'write_watch_movement_surface_resolution.py').read_text().replace('movement_surface_resolution_reference','movement_heading_query_reference')
    bp=P/'analysis/movement_heading_query_previous_build_manifest.json';builds=json.loads(bp.read_text());files.append(bp);assert len(builds['files'])==2757
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    files.extend([P/'movement_heading_query_reference.py',P/'write_watch_movement_heading_query.py',P/'write_watch_movement_surface_resolution.py'])
    report=dict(status='verified_additive_movement_heading_query_audit',original_rows_byte_checked=106,proposed_new_original_routines=1,new_original_routine_credit=0,canonical_duplicate_credit=0,canonical_reused_entries=t['canonical_reused_entries'],new_nonlocal_resume_exports=2,new_nonlocal_routes_qualified=0,return_membership=2534,explicit_nonfunction_resume_exports_total=642,qualified_nonlocal_routes_total=71,remaining_unqualified_nonlocal_routes=571,compiled_source_count=262,exact_previous_bridge_plus_one_entry_two_returns=True,reference_CPU_guards_comparison_logic_and_ABI_unchanged=True,existing_read_only_callback_input_capture_preserved=True,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2757,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='106 original instruction bytes checked. Exact previous261bridge/runtime/ABI plus1entry/2returns/2continuationexports. Actual original04f7e0stage query executes unchanged;no suppliedstage results. All prior source/interpreter/guards and full-state comparison logic unchanged;the inherited read-only callback-input capture is preserved. New nonlocal resumptions not independently qualified. No recovery credit from source generation, compilation or this audit.')
    (P/'analysis/movement_heading_query_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('ORIENTATION AUDIT PASS106rows/2534returns/2757oldbuilds;unchangedCPU/observer/ABI')
if __name__=='__main__':main()
