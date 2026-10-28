"""Audit original glyph/string bytes, additive routes, test input seed and preservation."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 mp=P/'analysis/collision_contact_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/collision_contact_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
 for report in (t,c):
  for p,h in report['sha256'].items():assert digest(p)==h,p;files.append(Path(p))
 assert len(t['routines'])==1 and t['instruction_rows']==1121 and len(t['resume_routes'])==32 and t['canonical_reused_entries']==[] and set(t['unregistered_proposals'])=={'0x8c029b00'}
 original=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1589
 assert not any(r['address'] in t['unregistered_proposals'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry[:1589])
 for body in t['routines']:
  for r in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
   a=int(r['address'],16);assert original[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(r['bytes'])
 old=(P/'source/sa_collision_text_connected.c').read_text();new=(P/'source/sa_collision_contact_connected.c').read_text()
 for body in t['routines']+t['resume_routes']:
  a=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={a}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
  assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_collision_contact_'+suffix,'sa_collision_text_'+suffix)
 marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa);before=set(old[oa:ob].split(','));after=set(new[a:b].split(','))
 rp=P/'analysis/collision_contact_returns.json';returns=json.loads(rp.read_text());files.append(rp)
 assert len(before)==2294 and len(after)==2326 and after-before=={x+'u' for x in returns['new_call_returns']} and len(returns['new_call_returns'])==32
 new=new[:a]+old[oa:ob]+new[b:];assert new==old
 assert (P/'source/collision_contact_probe.c').read_text()==(P/'source/collision_text_probe.c').read_text().replace('sa_collision_text_','sa_collision_contact_')
 import collision_contact_reference as v
 import collision_text_reference as previous
 assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference'] and v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in returns['new_call_returns']}
 for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
 import write_watch_collision_contact as observer
 expected=(P/'write_watch_collision_text.py').read_text().replace('collision_text_reference','collision_contact_reference')
 actual=(P/'write_watch_collision_contact.py').read_text();assert actual.startswith(expected)
 marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8]),sr=sr[0]))"
 extra="\n  if target in (0x8c10d008,0x8c10d058,0x8c10d0f8):\n   bank=(fp.contents.fpscr>>21)&1;boundaries[-1]['angle_input']=dict(fr4=int(fp.contents.banks[bank][4]),fr5=int(fp.contents.banks[bank][5]),fpscr=int(fp.contents.fpscr))"
 assert observer.font_code.count(marker)==1 and observer.angle_input_code==observer.font_code.replace(marker,marker+extra)
 for p in (P/'write_watch_collision_contact.py',P/'write_watch_collision_text.py'):files.append(p)
 bp=P/'analysis/collision_contact_previous_build_manifest.json';builds=json.loads(bp.read_text());files.append(bp);assert len(builds['files'])==2731
 for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
 report=dict(status='complete_original_collision_caller_static_audit_only',original_rows_byte_checked=1121,proposed_new_original_routines=1,new_original_routine_credit=0,new_nonlocal_resume_exports=32,new_nonlocal_routes_qualified=0,prior_return_membership=2294,return_membership=2326,explicit_nonfunction_resume_exports_total=434,qualified_nonlocal_routes_total=71,remaining_unqualified_nonlocal_routes=363,compiled_source_count=249,exact_previous_bridge_plus_one_body_and_thirtytwo_original_call_returns=True,reference_CPU_flat_execution_guards_production_observer_and_ABI_unchanged=True,test_only_external_QACR_input_seed_inherited=True,test_only_readonly_actual_angle_input_capture=True,new_source_wait_limits=0,domain_excluded_rows=[],domain_excluded_outcomes=[],preserved_archived_builds=2731,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='All1121originalrowsbytechecked;exactprevious248bridgeplus1body/32returns/continuations/exportprefix. Existingcallees/CPU/flatinterpreter/guards/productionobserver/ABIuntouched. OldfontQACRinputsetupinherited;newtest-onlyreadonlyFPargumentcapturedoesnotmodifyexecution/results/comparisons.2731oldbuildspreserved. No routine/behaviorcreditfromaudit;remainingoutcomesretained.')
 (P/'analysis/collision_contact_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('CONTACT AUDIT PASS1121rows/2326returns/2731oldbuilds;0creditfromaudit')
if __name__=='__main__':main()
