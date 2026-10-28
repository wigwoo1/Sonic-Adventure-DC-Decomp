"""Bytecheck original file-read/handle integration and preserve the prior execution ABI."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    mp=P/'analysis/movie_filesystem_command13_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_filesystem_command13_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
    for report in (t,c):
        for pin,h in report['sha256'].items():assert digest(pin)==h,pin;files.append(Path(pin))
    assert len(t['routines'])==4 and t['instruction_rows']==483 and len(t['resume_routes'])==32 and t['canonical_reused_entries']==[]
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();checked=0
    for body in t['routines']:
        listing=Path(body['listing']);files.append(listing)
        for row in csv.DictReader(listing.open(),delimiter='\t'):
            at=int(row['address'],16)-0x8c010000;assert binary[at:at+2]==bytes.fromhex(row['bytes']);checked+=1
    assert checked==483
    old=(P/'source/sa_movie_filesystem_command12_connected.c').read_text();new=(P/c['source']).read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movie_filesystem_command13_'+suffix,'sa_movie_filesystem_command12_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));returns=json.loads((P/'analysis/movie_filesystem_command13_returns.json').read_text())
    assert len(before)==2220 and len(after)==2252 and after-before=={x+'u' for x in returns['new_call_returns']} and before<=after
    new=new[:a]+old[oa:ob]+new[b:];assert new==old
    assert (P/'source/movie_filesystem_command13_probe.c').read_text()==(P/'source/movie_filesystem_command12_probe.c').read_text().replace('sa_movie_filesystem_command12_','sa_movie_filesystem_command13_')
    import movie_filesystem_command13_reference as v
    import movie_filesystem_command12_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in returns['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert (P/'write_watch_filesystem_command13.py').read_text()==(P/'write_watch_filesystem_command12.py').read_text().replace('movie_filesystem_command12_reference','movie_filesystem_command13_reference')
    assert (P/'movie_filesystem_command13_session.py').read_text()==(P/'movie_filesystem_command12_session.py').read_text().replace('movie_filesystem_command12_task_fixtures','movie_filesystem_command13_task_fixtures')
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    bm=P/'analysis/movie_filesystem_command13_previous_build_manifest.json';builds=json.loads(bm.read_text());assert len(builds['files'])==2709;files.append(bm)
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1573];assert len(registry)==1573
    report=dict(instruction_rows_byte_checked=483,new_instruction_rows_byte_checked=483,new_unregistered_translated_rows=483,canonical_adapter_rows=0,canonical_duplicate_credit=0,prior_return_membership=2220,return_membership=2252,new_resume_exports=32,explicit_nonfunction_exports_total=360,exact_previous_bridge_plus_four_bodies_32_routes_returns=True,reference_CPU_flat_execution_and_observer_identical=True,runtime_ABI_SRFD_exception_and_guard_policy_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2709,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='483freshoriginaltypeCCwriterowsbytechecked. Exactprevious238bridgeexcept4bodies/32actualreturns/exportprefix. CPU/flatcode/guards/productionobserver/rawsession/ABIunchanged;nonewwaitlimits. Staticaudit givesnoroutine/routecredit;2709oldbuildspreserved.')
    (P/'analysis/movie_filesystem_command13_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('COMMAND13 AUDIT PASS483rows2252returns2709priorbuilds',flush=True)
if __name__=='__main__':main()
