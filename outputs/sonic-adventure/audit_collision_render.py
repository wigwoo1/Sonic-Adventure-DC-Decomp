"""Audit byte identity, exact bridge extension, predicate proof and preserved builds."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 mp=P/'analysis/collision_render_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/collision_render_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
 for report in (t,c):
  for p,h in report['sha256'].items():assert digest(p)==h,p;files.append(Path(p))
 assert len(t['routines'])==1 and t['instruction_rows']==332 and len(t['resume_routes'])==20 and t['canonical_reused_entries']==[] and t['unregistered_proposals']==['0x8c63d66c']
 original=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1586,1587)
 assert not any(r['address']=='0x8c63d66c' and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry[:1586])
 for body in t['routines']:
  for r in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
   a=int(r['address'],16);assert original[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(r['bytes'])
 old=(P/'source/sa_collision_inverse_connected.c').read_text();new=(P/'source/sa_collision_render_connected.c').read_text()
 for body in t['routines']+t['resume_routes']:
  a=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={a}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
  assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_collision_render_'+suffix,'sa_collision_inverse_'+suffix)
 marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa);before=set(old[oa:ob].split(','));after=set(new[a:b].split(','))
 rp=P/'analysis/collision_render_returns.json';returns=json.loads(rp.read_text());files.append(rp)
 assert len(before)==2272 and len(after)==2292 and after-before=={x+'u' for x in returns['new_call_returns']} and len(returns['new_call_returns'])==20
 new=new[:a]+old[oa:ob]+new[b:];assert new==old
 assert (P/'source/collision_render_probe.c').read_text()==(P/'source/collision_inverse_probe.c').read_text().replace('sa_collision_inverse_','sa_collision_render_')
 import collision_render_reference as v
 import collision_inverse_reference as previous
 assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference'] and v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in returns['new_call_returns']}
 for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
 assert (P/'write_watch_collision_render.py').read_text()==(P/'write_watch_collision_inverse.py').read_text().replace('collision_inverse_reference','collision_render_reference')
 bp=P/'analysis/collision_render_previous_build_manifest.json';builds=json.loads(bp.read_text());files.append(bp);assert len(builds['files'])==2727
 for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
 pp=P/'analysis/collision_render_predicate_proof.json';proof=json.loads(pp.read_text());files.append(pp)
 for pin,h in proof['sha256'].items():assert digest(pin)==h;files.append(Path(pin))
 assert proof['infeasible_outcomes']==[['0x8c63d6e0','0x8c63d6f2']] and not proof['domain_excluded_outcomes']
 report=dict(status='original_triangle_renderer_wrapper_static_audit',original_rows_byte_checked=332,proposed_new_original_routines=1,new_original_routine_credit=0,new_nonlocal_resume_exports=20,new_nonlocal_routes_qualified=0,prior_return_membership=2272,return_membership=2292,explicit_nonfunction_resume_exports_total=400,qualified_nonlocal_routes_total=71,remaining_unqualified_nonlocal_routes=329,compiled_source_count=247,exact_previous_bridge_plus_one_body_and_twenty_original_call_returns=True,reference_CPU_flat_execution_guards_observer_and_ABI_unchanged=True,new_source_wait_limits=0,proven_infeasible_outcomes=proof['infeasible_outcomes'],domain_excluded_rows=[],domain_excluded_outcomes=[],preserved_archived_builds=2727,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='All332originalrows bytechecked. Exactprevious246bridge plus1body/20actualreturns/continuations/exportprefix. Existingcallee implementations,CPU/flatinterpreter/guards/productionobserver/ABI untouched. 2727oldbuilds preserved. Staticpredicateproof accounts foroneinfeasible normalentry outcome withoutremoving anyvalidinput ororiginalbranch. No behaviorcredit fromaudit alone.')
 (P/'analysis/collision_render_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('RENDER AUDIT PASS332rows/2292returns/2727oldbuilds;0newroutinecreditfromaudit')
if __name__=='__main__':main()
