"""Audit additive movement/model integration and preserve prior source/build ABI."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    mp=P/'analysis/movement_model_response_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movement_model_response_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
    for report in (t,c):
        for name,h in report['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    assert len(t['routines'])==3 and t['instruction_rows']==906 and len(t['resume_routes'])==66
    assert set(t['unregistered_proposals'])=={'0x8c0742d4','0x8c036f12','0x8c036bc0'} and not t['canonical_reused_entries']
    raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();count=0
    for body in t['routines']:
        for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
            a=int(row['address'],16);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes']);count+=1
    assert count==906
    old=(P/'source/sa_movement_mode_query_connected.c').read_text();new=(P/'source/sa_movement_model_response_connected.c').read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n'
        route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movement_model_response_'+suffix,'sa_movement_mode_query_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));returns=P/'analysis/movement_model_response_returns.json';ret=json.loads(returns.read_text());files.append(returns)
    assert len(before)==2392 and len(after)==2458 and after-before=={x+'u' for x in ret['new_call_returns']}
    assert new[:a]+old[oa:ob]+new[b:]==old
    assert (P/'source/movement_model_response_probe.c').read_text()==(P/'source/movement_mode_query_probe.c').read_text().replace('sa_movement_mode_query_','sa_movement_model_response_')
    import movement_model_response_reference as v
    import movement_mode_query_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in ret['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert v.HOST_IMPORT_RECURSION_FLOOR==4096 and v._host_sys.getrecursionlimit()>=4096
    assert (P/'write_watch_movement_model_response.py').read_text()==(P/'write_watch_movement_mode_query.py').read_text().replace('movement_mode_query','movement_model_response')
    bp=P/'analysis/movement_model_response_previous_build_manifest.json';builds=json.loads(bp.read_text());files.append(bp);assert len(builds['files'])==2747
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    files.extend([P/'movement_model_response_reference.py',P/'write_watch_movement_model_response.py',P/'write_watch_movement_mode_query.py'])
    report=dict(status='verified_additive_movement_model_response_audit',original_rows_byte_checked=906,proposed_new_original_routines=3,new_original_routine_credit=0,canonical_duplicate_credit=0,
      new_nonlocal_resume_exports=66,new_nonlocal_routes_qualified=0,return_membership=2458,explicit_nonfunction_resume_exports_total=566,qualified_nonlocal_routes_total=71,remaining_unqualified_nonlocal_routes=495,
      compiled_source_count=257,exact_previous_bridge_plus_three_entries_sixty_six_returns=True,reference_CPU_guards_observer_and_ABI_unchanged=True,
      host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2747,
      dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='906 original instruction bytes checked. Exact previous256bridge/runtime/ABI plus3entries/66returns/66continuationexports. All oldsource/guestCPU/interpreter/observer/guards unchanged;no newwaitbounds. Newnonlocalresumesnotindependentlyqualified;no routinecreditfromaudit.')
    (P/'analysis/movement_model_response_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('MODEL RESPONSE AUDIT PASS906rows/2458returns/2747oldbuilds;unchangedCPU/observer/ABI')

if __name__=='__main__':main()
