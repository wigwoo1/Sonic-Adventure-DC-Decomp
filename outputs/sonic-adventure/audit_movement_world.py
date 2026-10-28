"""Original-byte proof, exact additive integration, unchanged reference and build preservation."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    mp=P/'analysis/movement_world_translation.json';t=json.loads(mp.read_text())
    cp=P/'analysis/movement_world_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
    for report in (t,c):
        for name,sha in report['sha256'].items():assert digest(name)==sha,name;files.append(Path(name))
    assert len(t['routines'])==3 and t['instruction_rows']==542 and len(t['resume_routes'])==13
    assert set(t['unregistered_proposals'])=={'0x8c028bfe','0x8c0522c0','0x8c63a8e0'} and not t['canonical_reused_entries']
    raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    for body in t['routines']:
        for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
            address=int(row['address'],16);assert raw[address-0x8c010000:address-0x8c010000+2]==bytes.fromhex(row['bytes'])
    old=(P/'source/sa_movement_vectors_connected.c').read_text();new=(P/'source/sa_movement_world_connected.c').read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n'
        route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1
        new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):
        new=new.replace('sa_movement_world_'+suffix,'sa_movement_vectors_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','))
    returns=P/'analysis/movement_world_returns.json';r=json.loads(returns.read_text());files.append(returns)
    assert len(before)==2350 and len(after)==2363 and after-before=={x+'u' for x in r['new_call_returns']}
    assert new[:a]+old[oa:ob]+new[b:]==old
    assert (P/'source/movement_world_probe.c').read_text()==(P/'source/movement_vectors_probe.c').read_text().replace('sa_movement_vectors_','sa_movement_world_')
    import movement_world_reference as v
    import movement_vectors_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in r['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):
        assert v.namespace[name]==previous.namespace[name],name
    assert (P/'write_watch_movement_world.py').read_text()==(P/'write_watch_movement_vectors.py').read_text().replace('movement_vectors','movement_world')
    bp=P/'analysis/movement_world_previous_build_manifest.json';builds=json.loads(bp.read_text());files.append(bp)
    assert len(builds['files'])==2737
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    files.extend([P/'write_watch_movement_world.py',P/'write_watch_movement_vectors.py'])
    report=dict(status='verified_static_additive_world_query_audit',original_rows_byte_checked=542,proposed_new_original_routines=3,canonical_duplicate_credit=0,new_original_routine_credit=0,
      new_nonlocal_resume_exports=13,new_nonlocal_routes_qualified=0,return_membership=2363,explicit_nonfunction_resume_exports_total=471,qualified_nonlocal_routes_total=71,remaining_unqualified_nonlocal_routes=400,compiled_source_count=252,
      exact_previous_bridge_plus_three_entries_thirteen_returns=True,reference_CPU_flat_execution_guards_production_observer_and_ABI_unchanged=True,test_observer_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2737,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original world/nearby selection and identity loader.542bytechecked rows;exact251bridge plus3entries/13returns/13continuationexports. No behavior credit from this audit. Old source/proof/builds retained;newcontinuations not independently qualified.')
    (P/'analysis/movement_world_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('WORLD AUDIT PASS542rows/2363returns/2737oldbuilds;0creditfromaudit')

if __name__=='__main__':main()
