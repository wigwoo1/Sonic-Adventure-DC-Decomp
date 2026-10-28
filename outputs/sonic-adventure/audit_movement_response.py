"""Byte-backed additive response/math integration; unchanged ABI/reference and old artifacts."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    mp=P/'analysis/movement_response_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movement_response_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
    for report in (t,c):
        for name,sha in report['sha256'].items():assert digest(name)==sha,name;files.append(Path(name))
    assert len(t['routines'])==5 and t['instruction_rows']==378 and len(t['resume_routes'])==19
    assert set(t['unregistered_proposals'])=={'0x8c075416','0x8c055dac','0x8c638eac','0x8c639b50'} and t['canonical_reused_entries']==['0x8c63a970']
    raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    for body in t['routines']:
        for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
            a=int(row['address'],16);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes'])
    old=(P/'source/sa_movement_world_connected.c').read_text();new=(P/'source/sa_movement_response_connected.c').read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n'
        route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movement_response_'+suffix,'sa_movement_world_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));returns=P/'analysis/movement_response_returns.json';r=json.loads(returns.read_text());files.append(returns)
    assert len(before)==2363 and len(after)==2382 and after-before=={x+'u' for x in r['new_call_returns']}
    assert new[:a]+old[oa:ob]+new[b:]==old
    assert (P/'source/movement_response_probe.c').read_text()==(P/'source/movement_world_probe.c').read_text().replace('sa_movement_world_','sa_movement_response_')
    import movement_response_reference as v
    import movement_world_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in r['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert (P/'write_watch_movement_response.py').read_text()==(P/'write_watch_movement_world.py').read_text().replace('movement_world','movement_response')
    bp=P/'analysis/movement_response_previous_build_manifest.json';builds=json.loads(bp.read_text());files.append(bp);assert len(builds['files'])==2739
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    files.extend([P/'write_watch_movement_response.py',P/'write_watch_movement_world.py'])
    report=dict(status='verified_static_additive_contact_response_audit',original_rows_byte_checked=378,proposed_new_original_routines=4,canonical_duplicate_credit=0,new_original_routine_credit=0,
      new_nonlocal_resume_exports=19,new_nonlocal_routes_qualified=0,return_membership=2382,explicit_nonfunction_resume_exports_total=490,qualified_nonlocal_routes_total=71,remaining_unqualified_nonlocal_routes=419,compiled_source_count=253,
      exact_previous_bridge_plus_five_entries_nineteen_returns=True,reference_CPU_flat_execution_guards_production_observer_and_ABI_unchanged=True,test_observer_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2739,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original response and four actualmathhelpers,onecanonicalcosineadapter.378bytechecked rows;exact252bridge plus5entries/19actualreturns/19continuationexports. No behaviorcreditfromaudit. Oldsource/proof/builds retained;newcontinuationsnotindependentlyqualified.')
    (P/'analysis/movement_response_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('RESPONSE AUDIT PASS378rows/2382returns/2739oldbuilds;0creditfromaudit')

if __name__=='__main__':main()
