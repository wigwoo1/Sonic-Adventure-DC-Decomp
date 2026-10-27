"""Bytechecked deletion-family bridge; unchanged execution, guards and ABI."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    mp=P/'analysis/movie_filesystem_delete_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_filesystem_delete_connection.json';c=json.loads(cp.read_text())
    for report in (t,c):
        for pin,h in report['sha256'].items():assert digest(pin)==h,pin
    assert len(t['routines'])==5 and t['instruction_rows']==202 and len(t['resume_routes'])==14
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();checked=0;paths=[]
    for body in t['routines']:
        paths.append(Path(body['listing']))
        for row in csv.DictReader(paths[-1].open(),delimiter='\t'):
            at=int(row['address'],16)-0x8c010000;assert binary[at:at+2]==bytes.fromhex(row['bytes']);checked+=1
    assert checked==202
    old=(P/'source/sa_movie_filesystem_write_connected.c').read_text();new=(P/c['source']).read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movie_filesystem_delete_'+suffix,'sa_movie_filesystem_write_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));returns=json.loads((P/'analysis/movie_filesystem_delete_returns.json').read_text())
    assert len(before)==2036 and len(after)==2050 and after-before=={x+'u' for x in returns['new_call_returns']} and before<=after
    new=new[:a]+old[oa:ob]+new[b:];assert new==old
    assert (P/'source/movie_filesystem_delete_probe.c').read_text()==(P/'source/movie_filesystem_write_probe.c').read_text().replace('sa_movie_filesystem_write_','sa_movie_filesystem_delete_')
    import movie_filesystem_delete_reference as v
    import movie_filesystem_write_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in returns['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert (P/'write_watch_filesystem_delete.py').read_text()==(P/'write_watch_filesystem_write.py').read_text().replace('movie_filesystem_write_reference','movie_filesystem_delete_reference')
    assert (P/'movie_filesystem_delete_session.py').read_text()==(P/'movie_filesystem_write_session.py').read_text().replace('movie_filesystem_write_task_fixtures','movie_filesystem_delete_task_fixtures')
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    bm=P/'analysis/movie_filesystem_delete_previous_build_manifest.json';builds=json.loads(bm.read_text());assert len(builds['files'])==2695
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1535];assert len(registry)==1535 and sum(r['address']=='0x8c64b2dc' for r in registry)==1
    files=[Path(__file__),mp,cp,P/t['source'],P/c['source'],P/'movie_filesystem_delete_reference.py',P/'movie_filesystem_delete_session.py',P/'write_watch_filesystem_delete.py',bm,P/'source/retail_return_delay_stubs.c']+paths
    report=dict(instruction_rows_byte_checked=202,new_unregistered_translated_rows=200,canonical_adapter_rows=2,canonical_duplicate_credit=0,prior_return_membership=2036,return_membership=2050,new_resume_exports=14,explicit_nonfunction_exports_total=158,exact_previous_bridge_plus_five_bodies_fourteen_routes_returns=True,reference_CPU_flat_execution_and_observer_identical=True,runtime_ABI_SRFD_exception_and_guard_policy_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2695,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Static audit only.202originalrows=200proposed+2registeredzero-returnadapter. Exactprevious231bridge except5bodies/14actualreturnroutes/exportprefix;CPU/flatcode/guards/observer/rawinputsession unchanged. Allnonfunctionroutesunqualified fornewnonlocalentry;no routinecreditforroutes/canonicalreuse. Originalsource/proofs/builds preserved.')
    (P/'analysis/movie_filesystem_delete_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('DELETE AUDIT PASS202rows2050returns2695oldbuilds',flush=True)
if __name__=='__main__':main()
