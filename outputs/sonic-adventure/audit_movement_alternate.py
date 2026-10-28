"""Byte-backed additive alternate owner; unchanged guest execution and archived builds."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    mp=P/'analysis/movement_alternate_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movement_alternate_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
    for report in (t,c):
        for name,h in report['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    assert len(t['routines'])==1 and t['instruction_rows']==202 and len(t['resume_routes'])==10
    assert t['unregistered_proposals']==['0x8c075860'] and not t['canonical_reused_entries']
    raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    for row in csv.DictReader(Path(t['routines'][0]['listing']).open(),delimiter='\t'):
        a=int(row['address'],16);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes'])
    old=(P/'source/sa_movement_response_connected.c').read_text();new=(P/'source/sa_movement_alternate_connected.c').read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n'
        route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movement_alternate_'+suffix,'sa_movement_response_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));returns=P/'analysis/movement_alternate_returns.json';r=json.loads(returns.read_text());files.append(returns)
    assert len(before)==2382 and len(after)==2392 and after-before=={x+'u' for x in r['new_call_returns']}
    assert new[:a]+old[oa:ob]+new[b:]==old
    assert (P/'source/movement_alternate_probe.c').read_text()==(P/'source/movement_response_probe.c').read_text().replace('sa_movement_response_','sa_movement_alternate_')
    import movement_alternate_reference as v
    import movement_response_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in r['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert v.HOST_IMPORT_RECURSION_FLOOR==4096 and v._host_sys.getrecursionlimit()>=4096
    assert (P/'write_watch_movement_alternate.py').read_text()==(P/'write_watch_movement_response.py').read_text().replace('movement_response','movement_alternate')
    bp=P/'analysis/movement_alternate_previous_build_manifest.json';builds=json.loads(bp.read_text());files.append(bp);assert len(builds['files'])==2741
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    files.extend([P/'movement_alternate_reference.py',P/'write_watch_movement_alternate.py',P/'write_watch_movement_response.py'])
    report=dict(status='verified_static_additive_alternate_response_audit',original_rows_byte_checked=202,proposed_new_original_routines=1,new_original_routine_credit=0,canonical_duplicate_credit=0,
      new_nonlocal_resume_exports=10,new_nonlocal_routes_qualified=0,return_membership=2392,explicit_nonfunction_resume_exports_total=500,qualified_nonlocal_routes_total=71,remaining_unqualified_nonlocal_routes=429,compiled_source_count=254,
      exact_previous_bridge_plus_one_entry_ten_returns=True,reference_CPU_flat_execution_guards_production_observer_and_ABI_unchanged=True,
      host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,test_observer_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2741,
      dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='202bytechecked originalrows;exact253bridge plus1entry/10actualreturns/10continuationexports. OldguestCPU/interpreter/guards/ABI/observer unchanged. HostPythonimportstartup floor raisedforfinitehistoricalmodulechain;guestlimitsunchanged. No behaviorcreditfromaudit;newresumesnotindependentlyqualified.')
    (P/'analysis/movement_alternate_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('ALTERNATE AUDIT PASS202rows/2392returns/2741oldbuilds;hostimports4096;guestABIunchanged')

if __name__=='__main__':main()
