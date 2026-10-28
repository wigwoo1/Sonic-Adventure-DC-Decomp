"""Audit original glyph/string bytes, additive routes, test input seed and preservation."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 mp=P/'analysis/collision_text_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/collision_text_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
 for report in (t,c):
  for p,h in report['sha256'].items():assert digest(p)==h,p;files.append(Path(p))
 assert len(t['routines'])==2 and t['instruction_rows']==315 and len(t['resume_routes'])==2 and t['canonical_reused_entries']==[] and set(t['unregistered_proposals'])=={'0x8c6407b0','0x8c640922'}
 original=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1587,1589)
 assert not any(r['address'] in t['unregistered_proposals'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in registry[:1587])
 for body in t['routines']:
  for r in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
   a=int(r['address'],16);assert original[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(r['bytes'])
 old=(P/'source/sa_collision_render_connected.c').read_text();new=(P/'source/sa_collision_text_connected.c').read_text()
 for body in t['routines']+t['resume_routes']:
  a=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={a}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
  assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_collision_text_'+suffix,'sa_collision_render_'+suffix)
 marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa);before=set(old[oa:ob].split(','));after=set(new[a:b].split(','))
 rp=P/'analysis/collision_text_returns.json';returns=json.loads(rp.read_text());files.append(rp)
 assert len(before)==2292 and len(after)==2294 and after-before=={'0x8c6409d6u','0x8c6409eau'} and returns['new_call_returns']==['0x8c6409d6','0x8c6409ea']
 new=new[:a]+old[oa:ob]+new[b:];assert new==old
 assert (P/'source/collision_text_probe.c').read_text()==(P/'source/collision_render_probe.c').read_text().replace('sa_collision_render_','sa_collision_text_')
 import collision_text_reference as v
 import collision_render_reference as previous
 assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference'] and v.namespace['RETURNS']-previous.namespace['RETURNS']=={0x8c6409d6,0x8c6409ea}
 for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
 import write_watch_collision_text as observer
 expected=(P/'write_watch_collision_render.py').read_text().replace('collision_render_reference','collision_text_reference')
 actual=(P/'write_watch_collision_text.py').read_text();assert actual.startswith(expected)
 marker="effects.LIB.sa_rtc_sample_bus_init(ctypes.byref(memory),view,ctypes.byref(bus),c.get('port_data',0),c.get('control_word',0))"
 assert observer.font_code==observer.purge_code.replace(marker,marker+"\n memory.vertex.qacr[:]=c.get('vertex_qacr',(0,0))")
 assert observer.prior_watched_reference is observer.watched_reference
 for p in (P/'write_watch_collision_text.py',P/'write_watch_collision_render.py'):files.append(p)
 bp=P/'analysis/collision_text_previous_build_manifest.json';builds=json.loads(bp.read_text());files.append(bp);assert len(builds['files'])==2729
 for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
 report=dict(status='original_glyph_and_string_drawing_static_audit',original_rows_byte_checked=315,proposed_new_original_routines=2,new_original_routine_credit=0,new_nonlocal_resume_exports=2,new_nonlocal_routes_qualified=0,prior_return_membership=2292,return_membership=2294,explicit_nonfunction_resume_exports_total=402,qualified_nonlocal_routes_total=71,remaining_unqualified_nonlocal_routes=331,compiled_source_count=248,exact_previous_bridge_plus_two_bodies_and_two_original_call_returns=True,reference_CPU_flat_execution_guards_production_observer_and_ABI_unchanged=True,test_only_external_QACR_input_seed=True,new_source_wait_limits=0,domain_excluded_rows=[],domain_excluded_outcomes=[],preserved_archived_builds=2729,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='All315originalrowsbytechecked;exactprevious247bridgeplus2bodies/2returns/continuations/exportprefix. Existingstrlen/CPU/flatinterpreter/guards/productionobserver/ABIuntouched. NewtestonlysetupseedsdeclaredexternalQACRinputbeforeexecutiononbothsides;allcomparisonlogicunchanged,no suppliedgameoutputs/frames. TextcallerexecutesitsownoriginalQACRwritesfromzero.2729oldbuildspreserved;nogeneration/auditbehaviorcredit.')
 (P/'analysis/collision_text_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('TEXT AUDIT PASS315rows/2294returns/2729oldbuilds;0creditfromaudit')
if __name__=='__main__':main()
