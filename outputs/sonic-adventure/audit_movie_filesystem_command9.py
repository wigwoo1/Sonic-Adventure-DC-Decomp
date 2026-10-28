"""Bytechecked command9 relocation bridge; unchanged execution, guards and ABI."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    mp=P/'analysis/movie_filesystem_command9_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_filesystem_command9_connection.json';c=json.loads(cp.read_text())
    for report in (t,c):
        for pin,h in report['sha256'].items():assert digest(pin)==h,pin
    assert len(t['routines'])==8 and t['instruction_rows']==489 and len(t['resume_routes'])==30
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();checked=0;paths=[]
    for body in t['routines']:
        paths.append(Path(body['listing']))
        for row in csv.DictReader(paths[-1].open(),delimiter='\t'):
            at=int(row['address'],16)-0x8c010000;assert binary[at:at+2]==bytes.fromhex(row['bytes']);checked+=1
    assert checked==489
    old=(P/'source/sa_movie_filesystem_command8_connected.c').read_text();new=(P/c['source']).read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movie_filesystem_command9_'+suffix,'sa_movie_filesystem_command8_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));returns=json.loads((P/'analysis/movie_filesystem_command9_returns.json').read_text())
    assert len(before)==2084 and len(after)==2114 and after-before=={x+'u' for x in returns['new_call_returns']} and before<=after
    new=new[:a]+old[oa:ob]+new[b:];assert new==old
    assert (P/'source/movie_filesystem_command9_probe.c').read_text()==(P/'source/movie_filesystem_command8_probe.c').read_text().replace('sa_movie_filesystem_command8_','sa_movie_filesystem_command9_')
    import movie_filesystem_command9_reference as v
    import movie_filesystem_command8_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in returns['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert (P/'write_watch_filesystem_command9.py').read_text()==(P/'write_watch_filesystem_command8.py').read_text().replace('movie_filesystem_command8_reference','movie_filesystem_command9_reference')
    assert (P/'movie_filesystem_command9_session.py').read_text()==(P/'movie_filesystem_command8_session.py').read_text().replace('movie_filesystem_command8_task_fixtures','movie_filesystem_command9_task_fixtures')
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    bm=P/'analysis/movie_filesystem_command9_previous_build_manifest.json';builds=json.loads(bm.read_text());assert len(builds['files'])==2701
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1547];assert len(registry)==1547
    files=[Path(__file__),mp,cp,P/t['source'],P/c['source'],P/'movie_filesystem_command9_reference.py',P/'movie_filesystem_command9_session.py',P/'write_watch_filesystem_command9.py',bm,P/'source/retail_return_delay_stubs.c']+paths
    report=dict(instruction_rows_byte_checked=489,new_unregistered_translated_rows=489,canonical_adapter_rows=0,canonical_duplicate_credit=0,prior_return_membership=2084,return_membership=2114,new_resume_exports=30,explicit_nonfunction_exports_total=222,exact_previous_bridge_plus_eight_bodies_thirty_routes_returns=True,reference_CPU_flat_execution_and_observer_identical=True,runtime_ABI_SRFD_exception_and_guard_policy_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2701,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='489 original relocation-family rows bytechecked. Exact previous234-source bridge except eightbodies,30actualreturn routes andexportprefix. CPU/flatcode/guards/observer/rawinputsession/ABI unchanged;nonewwaitlimits. Staticaudit alone givesnoqualification/routecredit. Prior source/proofs and2701builds preserved.')
    (P/'analysis/movie_filesystem_command9_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('COMMAND9 AUDIT PASS489rows2114returns2701oldbuilds',flush=True)
if __name__=='__main__':main()
