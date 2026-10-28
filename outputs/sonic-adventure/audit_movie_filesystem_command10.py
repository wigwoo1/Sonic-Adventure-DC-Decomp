"""Bytechecked command10 record-update bridge; unchanged execution, guards and ABI."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    mp=P/'analysis/movie_filesystem_command10_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_filesystem_command10_connection.json';c=json.loads(cp.read_text())
    for report in (t,c):
        for pin,h in report['sha256'].items():assert digest(pin)==h,pin
    assert len(t['routines'])==1 and t['instruction_rows']==115 and len(t['resume_routes'])==10
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();checked=0;paths=[]
    for body in t['routines']:
        paths.append(Path(body['listing']))
        for row in csv.DictReader(paths[-1].open(),delimiter='\t'):
            at=int(row['address'],16)-0x8c010000;assert binary[at:at+2]==bytes.fromhex(row['bytes']);checked+=1
    assert checked==115
    old=(P/'source/sa_movie_filesystem_command9_connected.c').read_text();new=(P/c['source']).read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movie_filesystem_command10_'+suffix,'sa_movie_filesystem_command9_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));returns=json.loads((P/'analysis/movie_filesystem_command10_returns.json').read_text())
    assert len(before)==2114 and len(after)==2124 and after-before=={x+'u' for x in returns['new_call_returns']} and before<=after
    new=new[:a]+old[oa:ob]+new[b:];assert new==old
    assert (P/'source/movie_filesystem_command10_probe.c').read_text()==(P/'source/movie_filesystem_command9_probe.c').read_text().replace('sa_movie_filesystem_command9_','sa_movie_filesystem_command10_')
    import movie_filesystem_command10_reference as v
    import movie_filesystem_command9_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in returns['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert (P/'write_watch_filesystem_command10.py').read_text()==(P/'write_watch_filesystem_command9.py').read_text().replace('movie_filesystem_command9_reference','movie_filesystem_command10_reference')
    assert (P/'movie_filesystem_command10_session.py').read_text()==(P/'movie_filesystem_command9_session.py').read_text().replace('movie_filesystem_command9_task_fixtures','movie_filesystem_command10_task_fixtures')
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    bm=P/'analysis/movie_filesystem_command10_previous_build_manifest.json';builds=json.loads(bm.read_text());assert len(builds['files'])==2703
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1555];assert len(registry)==1555
    policy=0x8c66d3a8;assert int.from_bytes(binary[policy-0x8c010000:policy-0x8c010000+4],'little')==0
    hits=[];at=0
    while True:
        at=binary.find(policy.to_bytes(4,'little'),at)
        if at<0:break
        hits.append(at+0x8c010000);at+=1
    loads=[]
    for off in range(0,len(binary)-2,2):
        word=int.from_bytes(binary[off:off+2],'little');pc=off+0x8c010000
        if word&0xf000==0xd000 and (((pc+4)&~3)+(word&255)*4) in hits:loads.append(pc)
    assert hits==[0x8c64a354] and loads==[0x8c64a2f2]
    files=[Path(__file__),mp,cp,P/t['source'],P/c['source'],P/'movie_filesystem_command10_reference.py',P/'movie_filesystem_command10_session.py',P/'write_watch_filesystem_command10.py',bm,P/'source/retail_return_delay_stubs.c']+paths
    report=dict(instruction_rows_byte_checked=115,new_unregistered_translated_rows=115,canonical_adapter_rows=0,canonical_duplicate_credit=0,prior_return_membership=2114,return_membership=2124,new_resume_exports=10,explicit_nonfunction_exports_total=232,exact_previous_bridge_plus_one_body_ten_routes_returns=True,reference_CPU_flat_execution_and_observer_identical=True,runtime_ABI_SRFD_exception_and_guard_policy_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2703,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='115 original command10 record-update rowsbytechecked. Exactprevious235bridge exceptonebody/tenactualreturns/exports/prefix. CPU/flatcode/guards/observer/rawsession/ABI unchanged;nonewwaitlimits. Staticaudit alone givesno routine/routecredit. Prior source/proofs/2703builds preserved. Policybranchdefault0;nonzero acquisition unproved.')
    report.update(policy_retail_initial_word=0,policy_exact_main_image_literals=list(map(hex,hits)),policy_candidate_pc_relative_loads=list(map(hex,loads)),policy_writer_inventory_complete=False,nondefault_policy_natural_acquisition_proved=False)
    (P/'analysis/movie_filesystem_command10_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('COMMAND10 AUDIT PASS115rows2124returns2703oldbuilds',flush=True)
if __name__=='__main__':main()
