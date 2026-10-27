"""Qualify only five fully covered original own bodies; keep dispatcher partial."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
OWN={'0x8c65393a','0x8c654520','0x8c6544a0','0x8c651000','0x8c650e40'}
REUSED={'0x8c6534e0','0x8c65344e','0x8c65253e','0x8c65254e'}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 names=('movie_buffer_initialize_translation','movie_buffer_initialize_connection','movie_buffer_initialize_verification','movie_buffer_interrupt_verification','movie_buffer_initialize_retained_verification','movie_buffer_initialize_startup_verification','vram_aperture_bus_verification','pvr_dma_bus_verification','pvr_error_status_verification');reports={};files=[Path(__file__)]
 for name in names:
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports[names[0]];connection=reports[names[1]];direct,irq,retained,startup=[reports[n] for n in names[2:6]];dll=digest(ROOT/'work/sonic-adventure/build/movie_buffer_initialize.dll')
 assert len(t['routines'])==10 and connection['compiled_source_count']==206 and t['instruction_rows']==635
 assert [(r['cases_passed'],r['independent_checks']) for r in (direct,irq)]==[(78,1793),(23,334)]
 assert (retained['total_original_native_comparisons'],retained['independent_checks'])==(229,4077) and len(startup['cases'])==2 and startup['actual_next_frontier']=='0x8c60edb0'
 assert all(r['dll_sha256']==dll for r in (direct,irq,retained,startup))
 coverage=[]
 for body in t['routines']:
  missing=set(body['reachable_addresses']);missing_edges=set(map(tuple,body['conditional_edges']))
  for report in (direct,irq):
   c=next(r for r in report['coverage_by_routine'] if r['address']==body['address']);missing&=set(c['missing_rows']);missing_edges&=set(map(tuple,c['missing_outcomes']))
  if body['address'] in OWN:assert not missing and not missing_edges,body['address']
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=[list(e) for e in sorted(missing_edges)],new_original_recovery_credit=int(body['address'] in OWN)))
 routines=[r|dict(kind='original_routine',module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in t['routines'] if r['address'] in OWN]
 assert len(routines)==5 and sum(r['instruction_rows'] for r in routines)==252 and sum(len(r['conditional_edges']) for r in routines)==26
 prior_path=P/'analysis/movie_render_buffer_qualification.json';prior=json.loads(prior_path.read_text());files.append(prior_path)
 for path,h in prior['sha256'].items():assert digest(path)==h,(prior_path,path)
 assert prior['total_evidence_original_native_comparisons']==4833
 fresh=78+23+229+2;checks=1793+334+4077+startup['independent_checks']
 limitations=[
 'Fiveoriginalownbodies252rows26conditionaloutcomes all execute in finite declaredfixtures. Fourcanonicaladapters receive no duplicatecredit. Broadgraphicsdispatcher652210 ispartial92/259rows16/50outcomes andunregistered;transferbranchonly verified,notallgraphics/error/softwarecallbacksemantics. Graphicsroot656940stillpartialcf4->d06/e90->f38.',
 '101freshdirect original/native comparisons2127independentchecks;229retainedroot/constructor/accounting comparisons4077checks;twofreshfullconfigured0546c0startup comparisons. CompleteCPU/FPU/RAM/VRAM/Sound/platform/flash/runtime/orderedboundaries match. Fivebodyqualificationusesactualexecutedcoverage,notdraftcount. Prior4833evidence comparisonsretained,notallrerun. TwohistoricalnegativeA400outputcases excluded:the platformnowsupportstheaperture butoldaccountingadapters retain theirnarrower accessguard. Thoseadapteroutputsremainunsupported,notfaked asphysicalinvalidaddresses.',
 'AuthoredVRAM64bitaperture inverseinterleaving,untimedCH2textureDMA,andISTERRwrite-one-clearpolicy areplatformsupport,zerooriginalrecoverycredit. DMAcopiesactualRAMbeforehardwarecompletion;original652210ACK/650e40retirement/RTE/651000waitrelease executes. No gamequeuecompletion orhelperresultinjection. Errorstatusrawaccessestrackedseparatelyfromoldgenericparentbank;oldproofbytesretained.',
 'Platformcontrolscoverfullaperturealignedwordbijection2097152words/1612nativecalls,41DMAcases and3errorstatusaliascases4884checks. Untimedfirst8MiBVRAMtextureports11/13,modes0/1;no polygon/YUV,wrappedout-of-rangeDMA,physicaltiming. ImmediatecompletionmaycoalescebackloggedIRQevents;onlythedeclaredone-job/two-separatesynchronousjob lifecycleisclaimed.',
 'Directtestsusecoherentmappedalignedqueues/descriptors/privategueststack;notarbitrarycorruptcycles/concurrentmutation/destructivealiases. Zero-lengthCPUcopyunderflowisnotclaimedvalid. Busywaitresearchguardstops-4withoutfakingcompletion;strictunboundedsourcecheckedseparately.',
 'Twofreshstartup profiles retainfull8MiBclear/twoSoundfills/two33376Btransfers/G2IRQ/RTE/REQUEST/1065282Soundevents0drops/39SDKrecords1092B/32MIDIsends. Actual32768BCPUupload throughA400viewcompletes,nextfrontier60edb0. Explicitinitialplatform/flashconfiguration,notresetboot/guestcheckpoint/priming/shortenedcopy/historiccoldsubstitution.',
 'FullgamePC60/120/240FPS8K,Android/stereoscopic3DS,N64/PS1,SUPIR HDtextures/texturepercent/renderresolution/aspectsettings unfinished. TextureOff disables samplingandusesvertexcolors. No pixels/audibleoutput/gameplay/playableportclaim. GoalACTIVE10%weighted milestones,notmeasuredcodecoverage.'
 ]
 q=dict(routines=routines,newly_registered_entries=5,newly_qualified_rows=252,executed_rows=252,executed_conditional_outcomes=26,qualified_conditional_outcomes=26,coverage_by_routine=coverage,canonical_reuse=[dict(address=a,additional_recovery_credit=0) for a in sorted(REUSED)],partial_unregistered_dispatcher='0x8c652210',compiled_source_count=206,new_direct_original_native_comparisons=101,new_direct_independent_checks=2127,fresh_retained_original_native_comparisons=229,fresh_retained_independent_checks=4077,fresh_full_startup_comparisons=2,fresh_startup_independent_checks=startup['independent_checks'],fresh_original_native_comparisons=fresh,fresh_independent_checks=checks,retained_203_source_original_native_comparisons=4833,total_evidence_original_native_comparisons=4833+fresh,actual_next_frontier='0x8c60edb0',remaining_unqualified_graphics_root_outcomes=prior['remaining_unqualified_graphics_root_outcomes'],reused_additional_credit=0,dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_buffer_initialize_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('QUALIFIED5/252/26',fresh,checks)
if __name__=='__main__':main()
