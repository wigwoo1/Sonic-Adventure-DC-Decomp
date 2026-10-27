"""Original-byte evidence and exact previous bridge/runtime preservation."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def rows(path):return list(csv.DictReader(Path(path).open(),delimiter='\t'))
def main():
    mp=P/'analysis/movie_filesystem_root_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_filesystem_root_connection.json';c=json.loads(cp.read_text())
    for report in (t,c):
        for pin,h in report['sha256'].items():assert digest(pin)==h,pin
    assert len(t['routines'])==1 and t['instruction_rows']==68 and not t['resume_routes'] and not c['resume_routes']
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    previous_table=P/'analysis/movie_filesystem_state_translation.json';table=json.loads(previous_table.read_text());table=next(r for r in table['routines'] if r['address']=='0x8c64ae62')
    previous_status=P/'analysis/movie_countdown_translation.json';status=json.loads(previous_status.read_text());status=next(r for r in status['routines'] if r['address']=='0x8c660dac')
    paths=[Path(t['routines'][0]['listing']),P/'analysis/movie-filesystem-root/8c660dac_instructions.tsv',P/'analysis/movie-filesystem-root/8c64850a_instructions.tsv',Path(table['listing'])]
    checked=0
    for path in paths:
        for row in rows(path):
            at=int(row['address'],16)-0x8c010000;assert binary[at:at+2]==bytes.fromhex(row['bytes']);checked+=1
    assert checked==165
    normalize=lambda path:[(r['address'],r['bytes'],r['instruction'].lstrip('_')) for r in rows(path)]
    assert normalize(paths[1])==normalize(status['listing'])
    old=(P/'source/sa_movie_filesystem_state_connected.c').read_text();new=(P/c['source']).read_text()
    body=t['routines'][0];decl=f'VDECL({body["name"]});\n';route=f' if(target=={body["address"]}u)return {body["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
    assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):
        new=new.replace('sa_movie_filesystem_root_'+suffix,'sa_movie_filesystem_state_'+suffix)
    marker='static const uint32_t allowed[]={'
    a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
    old_returns=set(old[oa:ob].split(','));new_returns=set(new[a:b].split(','))
    assert len(old_returns)==1974 and len(new_returns)==1977 and new_returns-old_returns=={'0x8c64a4d4u','0x8c64a4deu','0x8c64a4e8u'} and old_returns<=new_returns
    new=new[:a]+old[oa:ob]+new[b:];assert new==old
    assert (P/'source/movie_filesystem_root_probe.c').read_text()==(P/'source/movie_filesystem_state_probe.c').read_text().replace('sa_movie_filesystem_state_','sa_movie_filesystem_root_')
    import movie_filesystem_root_reference as v
    import movie_filesystem_state_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={0x8c64a4d4,0x8c64a4de,0x8c64a4e8}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):
        assert v.namespace[name]==previous.namespace[name],name
    source=P/t['source'];assert 'SA_BOUND_RETAIL_STALLS' not in source.read_text() and '_resume_' not in source.read_text()
    def word(a,n=4):return int.from_bytes(binary[a-0x8c010000:a-0x8c010000+n],'little')
    assert [word(a) for a in (0x8c64a54c,0x8c64a550,0x8c64a554)]==[0x8c660dac,0x8c64850a,0x8c660dec]
    assert word(0x8c64a548)==0x8c89c550 and word(0x8c64a558)==word(0x8c64b2e8)==0x8c89cf90
    assert word(0x8c64a53a,2)==0xff03 and word(0x8c64a540,2)==0xff01 and word(0x8c64b2ec)==0xffff0000
    root_rows={r['address']:r['instruction'].lstrip('_') for r in rows(paths[0])}
    assert root_rows['8c64a502']=='mov #0x10,r7' and root_rows['8c64a50a']=='cmp/eq #0x55,r0'
    bridge=(P/c['source']).read_text()
    assert 'if(target==0x8c660dacu)return sa_movie_countdown_8c660dac' in bridge
    assert 'if(target==0x8c64ae62u)return sa_movie_filesystem_state_8c64ae62' in bridge
    # Fresh cleanup body has no call sites. Status/readiness only call existing
    # synchronous record/info/busy getters; no new scheduling source is added.
    assert not any(r['instruction'].lstrip('_').startswith(('jsr ','bsr ')) for r in rows(paths[2]))
    bm=P/'analysis/movie_filesystem_root_previous_build_manifest.json';builds=json.loads(bm.read_text());assert len(builds['files'])==2685
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    files=[Path(__file__),mp,cp,source,P/c['source'],P/'movie_filesystem_root_reference.py',P/'write_watch_filesystem_root.py',P/'movie_filesystem_root_session.py',P/'movie_input_interface_session.py',previous_table,previous_status,P/'source/sa_movie_filesystem_state.c',P/'source/sa_movie_countdown.c',P/'source/sa_movie_channel_decoder.c',P/'source/sa_movie_channel_io_decoder.c',bm]+paths
    report=dict(instruction_rows_byte_checked=165,new_translated_rows=68,reused_unregistered_rows=76,registered_cleanup_rows_reused=21,registered_cleanup_duplicate_credit=0,exact_previous_bridge_plus_one_body_and_three_return_sites=True,prior_return_membership=1974,return_membership=1977,new_resume_exports=0,existing_nonfunction_exports_unchanged=85,existing_status_and_table_routes_unchanged=True,original_CPU_and_flat_execution_identical=True,runtime_ABI_SRFD_exception_and_guard_policy_unchanged=True,new_source_wait_limits=0,root_signature_bytes=16,root_signature_value=85,preserved_archived_builds=2685,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Static audit only.68new root rows plus32existing unregistered status rows,44existing unregistered table rows and21registered cleanup rows bytechecked. New source/route only root;existing status/table/cleanup implementations preserved. Exact previous bridge exceptbody/prefix/threeactualJSRreturns,no new continuations/waitbounds. Raw metadata-word observation extension supplies device inputs only;original constructors/parser/frames unchanged. Behavior qualification separate;physical acquisition/game unfinished.')
    (P/'analysis/movie_filesystem_root_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('ROOT AUDIT PASS165bytecheckedrows;1977returns;2685oldbuilds',flush=True)
if __name__=='__main__':main()
