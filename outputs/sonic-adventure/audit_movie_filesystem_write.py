"""Original-byte audit, exact bridge inverse and observation-only CPU delta."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    mp=P/'analysis/movie_filesystem_write_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_filesystem_write_connection.json';c=json.loads(cp.read_text())
    for report in (t,c):
        for pin,h in report['sha256'].items():assert digest(pin)==h,pin
    assert len(t['routines'])==8 and t['instruction_rows']==451 and len(t['resume_routes'])==31
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();checked=0;paths=[]
    for body in t['routines']:
        paths.append(Path(body['listing']))
        for row in csv.DictReader(paths[-1].open(),delimiter='\t'):
            at=int(row['address'],16)-0x8c010000;assert binary[at:at+2]==bytes.fromhex(row['bytes']);checked+=1
    assert checked==451
    old=(P/'source/sa_movie_filesystem_chain_connected.c').read_text();new=(P/c['source']).read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movie_filesystem_write_'+suffix,'sa_movie_filesystem_chain_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));returns=json.loads((P/'analysis/movie_filesystem_write_returns.json').read_text())
    assert len(before)==2005 and len(after)==2036 and after-before=={x+'u' for x in returns['new_call_returns']} and before<=after
    new=new[:a]+old[oa:ob]+new[b:];assert new==old
    assert (P/'source/movie_filesystem_write_probe.c').read_text()==(P/'source/movie_filesystem_chain_probe.c').read_text().replace('sa_movie_filesystem_chain_','sa_movie_filesystem_write_')
    import movie_filesystem_write_reference as v
    import movie_filesystem_chain_reference as previous
    import filesystem_write_reference_cpu as cpu
    import input_submission_reference_cpu as old_cpu
    assert v.code==previous.code and cpu.code.replace(cpu.replacement,cpu.marker)==old_cpu.code
    assert v.namespace['reference'] is cpu.reference
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in returns['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    bm=P/'analysis/movie_filesystem_write_previous_build_manifest.json';builds=json.loads(bm.read_text());assert len(builds['files'])==2693
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    files=[Path(__file__),mp,cp,P/t['source'],P/c['source'],P/'movie_filesystem_write_reference.py',P/'movie_filesystem_write_session.py',P/'write_watch_filesystem_write.py',P/'filesystem_write_reference_cpu.py',bm]+paths
    report=dict(instruction_rows_byte_checked=451,new_unregistered_translated_rows=451,canonical_duplicate_credit=0,prior_return_membership=2005,return_membership=2036,new_resume_exports=31,explicit_nonfunction_exports_total=144,exact_previous_bridge_plus_eight_bodies_31_resume_routes_and_actual_returns=True,flat_execution_identical=True,reference_CPU_exact_inverse_except_two_TAS_event_observations=True,new_TAS_event_sites=['0x8c66054c','0x8c6605b6'],runtime_ABI_SRFD_exception_and_guard_policy_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2693,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Static audit only;behavior qualification separate. CPU change extends mandatory cache-purge observation at two bytecheckedTAS sites only;integer/memory/TASexecution unchanged. Readonly requestbuilder input snapshots do not supplyexecutionoutputs. Fullmemory/state/orderedjournaling checks remainstrict. Nonfunction continuations receive no routine credit.')
    (P/'analysis/movie_filesystem_write_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('WRITE AUDIT PASS451rows2036returns2693oldbuilds',flush=True)
if __name__=='__main__':main()
