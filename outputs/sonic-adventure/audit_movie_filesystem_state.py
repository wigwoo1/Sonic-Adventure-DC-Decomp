"""Check original bytes and exact preservation of the previous runtime/bridge."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    mp=P/'analysis/movie_filesystem_state_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_filesystem_state_connection.json';c=json.loads(cp.read_text())
    for report in (t,c):
        for pin,h in report['sha256'].items():assert digest(pin)==h,pin
    raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();rows=0
    for body in t['routines']:
        for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
            at=int(row['address'],16)-0x8c010000;assert raw[at:at+2]==bytes.fromhex(row['bytes']);rows+=1
    assert rows==146 and len(t['routines'])==3 and len(t['resume_routes'])==3
    old=(P/'source/sa_movie_input_interface_connected.c').read_text();new=(P/c['source']).read_text()
    for body,key in [(r,'address') for r in t['routines']]+[(r,'pc') for r in t['resume_routes']]:
        decl=f'VDECL({body["name"]});\n';route=f' if(target=={body[key]}u)return {body["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):
        new=new.replace('sa_movie_filesystem_state_'+suffix,'sa_movie_input_interface_'+suffix)
    marker='static const uint32_t allowed[]={'
    start=new.index(marker)+len(marker);end=new.index('};',start);old_start=old.index(marker)+len(marker);old_end=old.index('};',old_start)
    old_returns=set(old[old_start:old_end].split(','));new_returns=set(new[start:end].split(','))
    assert new_returns-old_returns=={'0x8c647646u','0x8c647694u','0x8c64ae9au'} and old_returns<=new_returns
    assert len(new_returns)==1974 and len(old_returns)==1971
    new=new[:start]+old[old_start:old_end]+new[end:];assert new==old
    assert (P/'source/movie_filesystem_state_probe.c').read_text()==(P/'source/movie_input_interface_probe.c').read_text().replace('sa_movie_input_interface_','sa_movie_filesystem_state_')
    import movie_filesystem_state_reference as v
    import movie_input_interface_reference as previous
    assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
    assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={0x8c647646,0x8c647694,0x8c64ae9a}
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):
        assert v.namespace[name]==previous.namespace[name],name
    source=P/t['source'];assert 'SA_BOUND_RETAIL_STALLS' not in source.read_text()
    def word(a,n=4):return int.from_bytes(raw[a-0x8c010000:a-0x8c010000+n],'little')
    assert [word(a) for a in (0x8c64aec0,0x8c6476d0)]==[0x8c89c550]*2
    assert word(0x8c64aec4)==0x8c647610 and word(0x8c6476d8)==0x8c660a7e and word(0x8c6476e0)==0x8c647bde
    assert word(0x8c6476c6,2)==0xff30 and word(0x8c6476c8,2)==0xff01 and word(0x8c6476e4)==0xffff0000
    assert word(0x8c660bee,2)==251 and word(0x8c660c04)==0xffff0000
    bm=P/'analysis/movie_filesystem_state_previous_build_manifest.json';builds=json.loads(bm.read_text());assert len(builds['files'])==2683
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    files=[Path(__file__),mp,cp,source,P/c['source'],P/'movie_filesystem_state_reference.py',P/'write_watch_filesystem_state.py',P/'channel_io_continuations.py',bm]
    report=dict(instruction_rows_byte_checked=146,proposed_bodies=3,proposed_conditional_outcomes=20,canonical_duplicate_credit=0,exact_previous_bridge_plus_three_bodies_and_three_resume_routes=True,prior_return_membership=1971,return_membership=1974,new_return_sites=['0x8c647646','0x8c647694','0x8c64ae9a'],original_CPU_and_flat_execution_identical=True,runtime_ABI_SRFD_exception_and_continuation_policy_unchanged=True,new_resume_exports=3,new_source_wait_limits=0,preserved_archived_builds=2683,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Static evidence only. Three original filesystem read bodies and three nonfunction continuations proposed; no behavioral credit from generation/audit. Exact previous bridge/runtime except declared routes/exportprefix and actual original callreturn sites. Downstream block reader and canonical progress callback remain real. No supplied task frames,completed IDs,decoded parser or callback results. Existing guards and unbounded retry semantics retained; finite behavior qualification separate.')
    (P/'analysis/movie_filesystem_state_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('FILESYSTEM AUDIT PASS146rows;1974returns;2683priorbuilds',flush=True)
if __name__=='__main__':main()
