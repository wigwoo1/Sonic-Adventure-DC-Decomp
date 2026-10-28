"""Audit canonical inverse adapters, exact bridge extension and old artifact preservation."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 mp=P/'analysis/collision_inverse_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/collision_inverse_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
 for report in (t,c):
  for p,h in report['sha256'].items():assert digest(p)==h,p;files.append(Path(p))
 assert len(t['routines'])==2 and t['instruction_rows']==1191 and len(t['resume_routes'])==1 and t['canonical_reused_entries']==['0x8c6390b0','0x8c64f3ec'] and not t['unregistered_proposals']
 original=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1586
 for body in t['routines']:
  row=next(r for r in registry if r['address']==body['address'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN');assert row['name']==body['name'] and row['retail_span_sha256']==body['retail_span_sha256'] and body['canonical_registered_reuse']
  for r in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
   a=int(r['address'],16);assert original[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(r['bytes'])
 old=(P/'source/sa_collision_normalize_connected.c').read_text();new=(P/'source/sa_collision_inverse_connected.c').read_text()
 for body in t['routines']+t['resume_routes']:
  a=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={a}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
  assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_collision_inverse_'+suffix,'sa_collision_normalize_'+suffix)
 marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa);before=set(old[oa:ob].split(','));after=set(new[a:b].split(','))
 rp=P/'analysis/collision_inverse_returns.json';returns=json.loads(rp.read_text());files.append(rp);assert len(before)==2271 and len(after)==2272 and after-before=={'0x8c6390bau'} and returns['new_call_returns']==['0x8c6390ba']
 new=new[:a]+old[oa:ob]+new[b:];assert new==old
 assert (P/'source/collision_inverse_probe.c').read_text()==(P/'source/collision_normalize_probe.c').read_text().replace('sa_collision_normalize_','sa_collision_inverse_')
 import collision_inverse_reference as v
 import collision_normalize_reference as previous
 assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference'] and v.namespace['RETURNS']-previous.namespace['RETURNS']=={0x8c6390ba}
 for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
 assert (P/'write_watch_collision_inverse.py').read_text()==(P/'write_watch_collision_angle.py').read_text().replace('collision_angle_reference','collision_inverse_reference')
 bp=P/'analysis/collision_inverse_previous_build_manifest.json';builds=json.loads(bp.read_text());files.append(bp);assert len(builds['files'])==2725
 for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
 report=dict(status='canonical_inverse_adapters_audited_no_new_routine_credit',original_rows_byte_checked=1191,canonical_adapter_rows=1191,new_original_routine_credit=0,proposed_canonical_direct_entries=2,new_nonlocal_resume_exports=1,new_nonlocal_routes_qualified=0,prior_return_membership=2271,return_membership=2272,explicit_nonfunction_resume_exports_total=380,qualified_nonlocal_routes_total=71,remaining_unqualified_nonlocal_routes=309,compiled_source_count=246,exact_previous_bridge_plus_two_bodies_and_one_original_call_return=True,reference_CPU_flat_execution_guards_observer_and_ABI_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2725,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='1191canonical original rows bytechecked and registry names/spans verified. Exactprevious245bridge plus2adapters/1actualreturn/continuation/exportprefix. Currentnumeric/busABI andorderedcall boundaries preserved;no newroutine/nonlocalroutequalification fromaudit.2725oldbuilds preserved;oldcanonical source/proofs untouched.')
 (P/'analysis/collision_inverse_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('INVERSE AUDIT PASS1191rows/2272returns/2725oldbuilds;0newroutinecredit')
if __name__=='__main__':main()
