"""Check original initialization bytes and exact additive bridge/reference changes."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    mp=P/'analysis/collision_setup_translation.json';t=json.loads(mp.read_text())
    cp=P/'analysis/collision_setup_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
    for report in (t,c):
        for name,sha in report['sha256'].items():assert digest(name)==sha,name;files.append(Path(name))
    assert len(t['routines'])==7 and t['instruction_rows']==165 and len(t['resume_routes'])==7
    assert set(t['unregistered_proposals'])=={'0x8c028e60','0x8c04e3de'} and len(t['canonical_reused_entries'])==5
    raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    for body in t['routines']:
        for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
            address=int(row['address'],16);assert raw[address-0x8c010000:address-0x8c010000+2]==bytes.fromhex(row['bytes'])
    old=(P/'source/sa_collision_contact_connected.c').read_text()
    new=(P/'source/sa_collision_setup_connected.c').read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n'
        route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1
        new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):
        new=new.replace('sa_collision_setup_'+suffix,'sa_collision_contact_'+suffix)
    marker='static const uint32_t allowed[]={'
    a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','))
    returns=P/'analysis/collision_setup_returns.json';r=json.loads(returns.read_text());files.append(returns)
    assert len(before)==2326 and len(after)==2333 and after-before=={x+'u' for x in r['new_call_returns']}
    assert new[:a]+old[oa:ob]+new[b:]==old
    assert (P/'source/collision_setup_probe.c').read_text()==(P/'source/collision_contact_probe.c').read_text().replace('sa_collision_contact_','sa_collision_setup_')
    import collision_setup_reference as v
    import collision_contact_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in r['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):
        assert v.namespace[name]==previous.namespace[name],name
    assert (P/'write_watch_collision_setup.py').read_text()==(P/'write_watch_collision_contact.py').read_text().replace('collision_contact','collision_setup')
    bp=P/'analysis/collision_contact_counter_alias_prior_build_manifest.json';builds=json.loads(bp.read_text());files.append(bp)
    assert len(builds['files'])==2733
    for row in builds['files']:
        assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    files.extend([P/'write_watch_collision_setup.py',P/'write_watch_collision_contact.py'])
    report=dict(status='verified_static_additive_initialization_audit',original_rows_byte_checked=165,
        proposed_new_original_routines=2,canonical_reused_entries=5,new_original_routine_credit=0,
        new_nonlocal_resume_exports=7,new_nonlocal_routes_qualified=0,return_membership=2333,
        explicit_nonfunction_resume_exports_total=441,qualified_nonlocal_routes_total=71,
        remaining_unqualified_nonlocal_routes=370,compiled_source_count=250,
        exact_previous_bridge_plus_seven_entries_seven_returns=True,
        reference_CPU_flat_execution_guards_production_observer_and_ABI_unchanged=True,
        test_observer_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2733,
        dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},
        scope='Two original initialization owners and five canonical leaf adapters through existing ABI. '
        'All165rows byte-checked;exact249bridge plus7entries/7returns/7continuation exports. '
        'Old sources,CPU,flat interpreter,guards,production observer,comparison logic and2733builds preserved. '
        'Seven added nonlocal exports are not independently qualified resumes;no behavior/routine credit from audit.')
    (P/'analysis/collision_setup_audit.json').write_text(json.dumps(report,indent=2)+'\n')
    print('SETUP AUDIT PASS:165rows/2333returns/2733oldbuilds;0creditfromaudit')


if __name__=='__main__':main()
