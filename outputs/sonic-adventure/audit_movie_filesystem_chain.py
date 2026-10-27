"""Byte audit and exact inverse of new dispatch/continuation additions."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    mp=P/'analysis/movie_filesystem_chain_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_filesystem_chain_connection.json';c=json.loads(cp.read_text())
    for report in (t,c):
        for pin,h in report['sha256'].items():assert digest(pin)==h,pin
    assert len(t['routines'])==2 and t['instruction_rows']==84 and len(t['resume_routes'])==3
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();checked=0;paths=[]
    for body in t['routines']:
        paths.append(Path(body['listing']))
        for row in csv.DictReader(paths[-1].open(),delimiter='\t'):
            at=int(row['address'],16)-0x8c010000;assert binary[at:at+2]==bytes.fromhex(row['bytes']);checked+=1
    assert checked==84
    old=(P/'source/sa_movie_filesystem_command_connected.c').read_text();new=(P/c['source']).read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movie_filesystem_chain_'+suffix,'sa_movie_filesystem_command_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));assert len(before)==2002 and len(after)==2005 and after-before=={x+'u' for x in json.loads((P/'analysis/movie_filesystem_chain_returns.json').read_text())['new_call_returns']} and before<=after
    new=new[:a]+old[oa:ob]+new[b:];assert new==old
    assert (P/'source/movie_filesystem_chain_probe.c').read_text()==(P/'source/movie_filesystem_command_probe.c').read_text().replace('sa_movie_filesystem_command_','sa_movie_filesystem_chain_')
    import movie_filesystem_chain_reference as v
    import movie_filesystem_command_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in json.loads((P/'analysis/movie_filesystem_chain_returns.json').read_text())['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    bm=P/'analysis/movie_filesystem_chain_previous_build_manifest.json';builds=json.loads(bm.read_text());assert len(builds['files'])==2691
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    canonical_sources=[P/'source/sa_indexed_stride_value_8c64b274.c']
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1526]
    assert {r['address'] for r in registry if r['address'] in ('0x8c64b274',)}=={'0x8c64b274'}
    files=canonical_sources+[Path(__file__),mp,cp,P/t['source'],P/c['source'],P/'movie_filesystem_chain_reference.py',P/'movie_filesystem_chain_session.py',P/'write_watch_filesystem_command.py',bm]+paths
    report=dict(instruction_rows_byte_checked=84,new_unregistered_translated_rows=72,canonical_adapter_rows=12,canonical_duplicate_credit=0,prior_return_membership=2002,return_membership=2005,new_resume_exports=3,explicit_nonfunction_exports_total=113,exact_previous_bridge_plus_two_bodies_three_resume_routes_and_actual_returns=True,original_CPU_and_flat_execution_identical=True,runtime_ABI_SRFD_exception_and_guard_policy_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2691,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Static audit only;behavior qualification separate. Original source/guard/runtime preserved;two adapters/actual continuations and JSRreturns only. Nonfunction continuations receive no routine credit.')
    (P/'analysis/movie_filesystem_chain_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('CHAIN AUDIT PASS84rows2005returns2691oldbuilds',flush=True)
if __name__=='__main__':main()
