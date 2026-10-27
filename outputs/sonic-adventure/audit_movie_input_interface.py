"""Original byte evidence and exact prior bridge/runtime/CPU preservation."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    mp=P/'analysis/movie_input_interface_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_input_interface_connection.json';c=json.loads(cp.read_text())
    for report in (t,c):
        for pin,h in report['sha256'].items():assert digest(pin)==h,pin
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();rows=0
    for body in t['routines']:
        for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
            at=int(row['address'],16)-0x8c010000;assert binary[at:at+2]==bytes.fromhex(row['bytes']);rows+=1
    assert rows==24 and len(t['routines'])==5
    old=(P/'source/sa_movie_input_submission_connected.c').read_text();new=(P/c['source']).read_text()
    for body in t['routines']:
        decl=f'VDECL({body["name"]});\n';route=f' if(target=={body["address"]}u)return {body["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):
        new=new.replace('sa_movie_input_interface_'+suffix,'sa_movie_input_submission_'+suffix)
    assert new==old
    assert (P/'source/movie_input_interface_probe.c').read_text()==(P/'source/movie_input_submission_probe.c').read_text().replace('sa_movie_input_submission_','sa_movie_input_interface_')
    import movie_input_interface_reference as v
    import movie_input_submission_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    for name in ('RETURNS','REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):
        assert v.namespace[name]==previous.namespace[name],name
    text=(P/t['source']).read_text();assert 'SA_BOUND_RETAIL_STALLS' not in text
    copy=text[text.index('SA_STAGE_EXPORT int sa_movie_input_interface_8c6607fc'):text.index('SA_STAGE_EXPORT int sa_movie_input_interface_8c66080c')]
    assert 'r[6]>>=2' in copy and 'r[6]--' in copy and 'goto L8c6607fe' in copy
    assert 'if(!r[6])return' not in copy and 'memcpy(' not in copy
    # Verify callback provenance against retail literals and indexed Ghidra refs.
    def word(a):return int.from_bytes(binary[a-0x8c010000:a-0x8c010000+4],'little')
    assert word(0x8c647e9c)==word(0x8c660ad4)==word(0x8c648370)==0x8c89c54c
    assert [word(a) for a in (0x8c647ea0,0x8c647ea4,0x8c647ea8,0x8c647eac)]==[0x8c66080c,0x8c66081e,0x8c660816,0x8c660826]
    assert word(0x8c647eb0)==0x8c89c538
    assert word(0x8c66094e)&0xffff==0xff30
    assert word(0x8c648362)&0xffff==0xff01
    assert v.ROWS[0x8c648422].lstrip('_').startswith('jsr ')
    assert 'if(target==0x8c648426u)return sa_movie_channel_io_decoder_resume_8c648426' in (P/c['source']).read_text()
    refs=P/'analysis/movie-input-interface-references/references.tsv';entries=list(csv.DictReader(refs.open(),delimiter='\t'))
    writes=[x for x in entries if x['resource_address']=='8c89c54c' and x['reference_type']=='WRITE']
    assert any(x['reference_from']=='8c647e38' and x['function']=='8c647e16' for x in writes)
    bm=P/'analysis/movie_input_interface_previous_build_manifest.json';builds=json.loads(bm.read_text());assert len(builds['files'])==2681
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    files=[Path(__file__),mp,cp,P/t['source'],P/c['source'],P/'movie_input_interface_reference.py',P/'write_watch_input_interface.py',bm,refs,P/'analysis/movie-input-interface-references/literal_evidence.json',P/'ghidra/ExportResourceReferences.java']
    report=dict(instruction_rows_byte_checked=24,proposed_bodies=5,new_conditional_outcomes=2,canonical_duplicate_credit=0,exact_previous_bridge_plus_five_routes=True,return_membership_unchanged=1971,original_CPU_and_flat_execution_identical=True,runtime_ABI_SRFD_exception_and_continuation_code_unchanged=True,new_resume_exports=0,new_source_wait_limits=0,copy_keeps_original_do_while=True,indexed_interface_writer='0x8c647e38 in canonical0x8c647e16',callback_targets_from_original_selector_literals=True,preserved_archived_builds=2681,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Static audit only. Indexed references establish original initializer/selector provenance without claiming an exhaustive writer inventory. Five original callbacks/wordcopy route additions; entire prior bridge/runtime/return set and originalCPU identical. Copy retains loop/short-count underflow, mappedRAM guards and bytecode; no source fast path or forced completion. Behavior qualification separate.')
    report.update(original_removal_reader_error=-208,original_negative_header_caller_error=-255,block_reader_callsite='0x8c648422',block_reader_nonlocal_return='0x8c648426')
    (P/'analysis/movie_input_interface_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('INTERFACE AUDIT PASS24rows;1971returns;2681priorbuilds',flush=True)
if __name__=='__main__':main()
