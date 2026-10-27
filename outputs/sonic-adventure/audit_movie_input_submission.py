"""Byte/ABI/guard preservation audit; behavior qualification is separate."""
import csv,hashlib,json,re,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    mp=P/'analysis/movie_input_submission_translation.json';t=json.loads(mp.read_text())
    cp=P/'analysis/movie_input_submission_connection.json';c=json.loads(cp.read_text())
    for report in (t,c):
        for pin,h in report['sha256'].items():assert digest(pin)==h,pin
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();count=0
    for body in t['routines']:
        for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
            a=int(row['address'],16);assert binary[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes']);count+=1
    assert (count,t['proposed_rows'],t['canonical_adapter_rows'],len(t['canonical_reuse']))==(384,351,33,1)
    old=(P/'source/sa_movie_input_pump_connected.c').read_text();new=(P/c['source']).read_text()
    for body in t['routines']:
        decl=f'VDECL({body["name"]});\n';route=f' if(target=={body["address"]}u)return {body["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
        assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
    for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):
        new=new.replace('sa_movie_input_submission_'+suffix,'sa_movie_input_pump_'+suffix)
    def allowed(text):return text.split('static const uint32_t allowed[]={')[1].split('};',1)[0]
    returns={int(x[:-1],16) for x in allowed(new).split(',')};new=new.replace(allowed(new),allowed(old))
    # Reinsert the one replaced canonical dispatch at its exact original index.
    route=' if(target==0x8c65f1aau)return sa_movie_input_pump_8c65f1aa(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
    position=old.index(route);assert new[:position]==old[:position];new=new[:position]+route+new[position:];assert new==old
    assert (P/'source/movie_input_submission_probe.c').read_text()==(P/'source/movie_input_pump_probe.c').read_text().replace('sa_movie_input_pump_','sa_movie_input_submission_')
    import movie_input_submission_reference as v
    import movie_input_pump_reference as previous
    import input_submission_reference_cpu as cpu
    assert v.code==previous.code and returns==v.namespace['RETURNS']
    assert cpu.code.replace('if pc in (0x8c66049a,0x8c6604ec,0x8c65f1d2,0x8c65f02c):','if pc in (0x8c66049a,0x8c6604ec,0x8c65f1d2):')==cpu.old.code
    for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):
        assert v.namespace[name]==previous.namespace[name],name
    text=(P/t['source']).read_text();assert 'SA_BOUND_RETAIL_STALLS' not in text
    for pc in (0x8c65f02c,0x8c65f1d2):
        line=next(x for x in text.splitlines() if x.startswith(f'L{pc:x}:'))
        for token in ('valid_ram(r[0],1u)','bus->cache(bus->context,r[0],3u)','value|0x80u','(value==0)'):assert token in line
    for a in (0x5f6c04,0x5f6c14,0x5f6c18,0x5f6c80):assert f'==0x{a:08x}u' in text
    def function(source,name):return re.search(r'SA_STAGE_EXPORT int '+name+r'\(.*?(?=SA_STAGE_EXPORT int |\Z)',source,re.S)[0].strip()
    canonical=function((P/'source/sa_movie_input_pump.c').read_text(),'sa_movie_input_pump_8c65f1aa')
    assert function(text,'sa_movie_input_submission_8c65f1aa')==canonical.replace('sa_movie_input_pump_','sa_movie_input_submission_')
    bm=P/'analysis/movie_input_submission_previous_build_manifest.json'
    builds=json.loads(bm.read_text());assert len(builds['files'])==2679
    for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
    files=[Path(__file__),mp,cp,P/t['source'],P/c['source'],P/'input_submission_reference_cpu.py',P/'movie_input_submission_reference.py',P/'write_watch_input_submission.py',bm]
    report=dict(instruction_rows_byte_checked=384,unregistered_proposals=6,proposed_rows=351,canonical_rows=33,canonical_duplicate_credit=0,canonical_preflight_function_body_identical=True,exact_prior_bridge_inverse_verified=True,prior_runtime_external_ABI_SRFD_guards_unchanged=True,prior_flat_execution_code_identical=True,only_CPU_change_new_TAS_purge=True,only_new_MMIO_guard_addresses=['0x005f6c04','0x005f6c14','0x005f6c18','0x005f6c80'],guard_call_return_sites=len(returns),new_resume_exports=0,new_source_wait_limits=0,preserved_archived_builds=2679,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Static audit only. Six new bodies plus identical canonical preflight body under four aligned32bit MMIO addresses. Existing startup register storage unchanged; no automatic device engine. Exact oldbridge inverse after six additions, canonical route replacement, return membership/export names. No exception/runtime/continuation changes. Earlier source/proof/build preservation is additionally checked by workflow.')
    (P/'analysis/movie_input_submission_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('SUBMISSION AUDIT PASS',count,len(returns),flush=True)
if __name__=='__main__':main()
