"""Byte audit and exact inverse of new dispatch/continuation additions."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    mp=P/'analysis/movie_filesystem_directory_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_filesystem_directory_connection.json';c=json.loads(cp.read_text())
    for report in (t,c):
        for pin,h in report['sha256'].items():assert digest(pin)==h,pin
    assert len(t['routines'])==2 and t['instruction_rows']==71 and len(t['resume_routes'])==2
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();checked=0;paths=[]
    for body in t['routines']:
        paths.append(Path(body['listing']))
        for row in csv.DictReader(paths[-1].open(),delimiter='\t'):
            at=int(row['address'],16)-0x8c010000;assert binary[at:at+2]==bytes.fromhex(row['bytes']);checked+=1
    assert checked==71
    old=(P/'source/sa_movie_filesystem_root_connected.c').read_text();new=(P/c['source']).read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movie_filesystem_directory_'+suffix,'sa_movie_filesystem_root_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));assert len(before)==1977 and len(after)==1979 and after-before=={'0x8c64a9e0u','0x8c64a9f0u'} and before<=after
    new=new[:a]+old[oa:ob]+new[b:];assert new==old
    assert (P/'source/movie_filesystem_directory_probe.c').read_text()==(P/'source/movie_filesystem_root_probe.c').read_text().replace('sa_movie_filesystem_root_','sa_movie_filesystem_directory_')
    import movie_filesystem_directory_reference as v
    import movie_filesystem_root_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={0x8c64a9e0,0x8c64a9f0}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    bm=P/'analysis/movie_filesystem_directory_previous_build_manifest.json';builds=json.loads(bm.read_text());assert len(builds['files'])==2687
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    files=[Path(__file__),mp,cp,P/t['source'],P/c['source'],P/'movie_filesystem_directory_reference.py',P/'movie_filesystem_directory_session.py',P/'write_watch_filesystem_directory.py',bm]+paths
    report=dict(instruction_rows_byte_checked=71,new_translated_rows=71,prior_return_membership=1977,return_membership=1979,new_resume_exports=2,explicit_nonfunction_exports_total=87,exact_previous_bridge_plus_two_bodies_two_resume_routes_and_actual_returns=True,original_CPU_and_flat_execution_identical=True,runtime_ABI_SRFD_exception_and_guard_policy_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2687,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Static audit only;behavior qualification separate. Original source/guard/runtime preserved;new routes and actual JSRreturns only. Nonfunction continuations receive no routine credit.')
    (P/'analysis/movie_filesystem_directory_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('DIRECTORY AUDIT PASS71rows1979returns2687oldbuilds',flush=True)
if __name__=='__main__':main()
