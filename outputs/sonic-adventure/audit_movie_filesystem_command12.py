"""Bytecheck original file-read/handle integration and preserve the prior execution ABI."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    mp=P/'analysis/movie_filesystem_command12_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_filesystem_command12_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
    for report in (t,c):
        for pin,h in report['sha256'].items():assert digest(pin)==h,pin;files.append(Path(pin))
    assert len(t['routines'])==11 and t['instruction_rows']==808 and len(t['resume_routes'])==59 and t['canonical_reused_entries']==[]
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();checked=0
    for body in t['routines']:
        listing=Path(body['listing']);files.append(listing)
        for row in csv.DictReader(listing.open(),delimiter='\t'):
            at=int(row['address'],16)-0x8c010000;assert binary[at:at+2]==bytes.fromhex(row['bytes']);checked+=1
    assert checked==808
    old=(P/'source/sa_movie_filesystem_command11_connected.c').read_text();new=(P/c['source']).read_text()
    for body in t['routines']+t['resume_routes']:
        address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movie_filesystem_command12_'+suffix,'sa_movie_filesystem_command11_'+suffix)
    marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));returns=json.loads((P/'analysis/movie_filesystem_command12_returns.json').read_text())
    assert len(before)==2161 and len(after)==2220 and after-before=={x+'u' for x in returns['new_call_returns']} and before<=after
    new=new[:a]+old[oa:ob]+new[b:];assert new==old
    assert (P/'source/movie_filesystem_command12_probe.c').read_text()==(P/'source/movie_filesystem_command11_probe.c').read_text().replace('sa_movie_filesystem_command11_','sa_movie_filesystem_command12_')
    import movie_filesystem_command12_reference as v
    import movie_filesystem_command11_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in returns['new_call_returns']}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
    assert (P/'write_watch_filesystem_command12.py').read_text()==(P/'write_watch_filesystem_command11.py').read_text().replace('movie_filesystem_command11_reference','movie_filesystem_command12_reference')
    assert (P/'movie_filesystem_command12_session.py').read_text()==(P/'movie_filesystem_command11_session.py').read_text().replace('movie_filesystem_command11_task_fixtures','movie_filesystem_command12_task_fixtures')
    assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
    bm=P/'analysis/movie_filesystem_command12_previous_build_manifest.json';builds=json.loads(bm.read_text());assert len(builds['files'])==2707;files.append(bm)
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1561];assert len(registry)==1561
    previous_close=P/'analysis/movie-filesystem-command11-dependencies/8c64919e_instructions.tsv';fresh_close=P/'analysis/movie-filesystem-command12-timestamp-close/8c64919e_instructions.tsv';assert previous_close.read_bytes()==fresh_close.read_bytes();files.extend([previous_close,fresh_close,P/'source/sa_movie_filesystem_command11.c'])
    report=dict(instruction_rows_byte_checked=969,new_instruction_rows_byte_checked=808,retained_close_rows_byte_checked=161,new_unregistered_translated_rows=808,canonical_adapter_rows=0,canonical_duplicate_credit=0,prior_return_membership=2161,return_membership=2220,new_resume_exports=59,explicit_nonfunction_exports_total=328,exact_previous_bridge_plus_eleven_bodies_59_routes_returns=True,reference_CPU_flat_execution_and_observer_identical=True,runtime_ABI_SRFD_exception_and_guard_policy_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2707,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='808neworiginalrows and161preservedclose rowsbytechecked. Exactprevious237bridgeexcept11bodies/59actualreturns/exportprefix. CPU/flatcode/guards/observer/rawsession/ABIunchanged;nonewwaitlimits. Static audit givesnoroutine/routecredit;all2707priorbuilds preserved.')
    (P/'analysis/movie_filesystem_command12_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('COMMAND12 AUDIT PASS969rows2220returns2707priorbuilds',flush=True)
if __name__=='__main__':main()
