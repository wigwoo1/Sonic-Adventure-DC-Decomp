"""Qualify new own bodies only after complete measured coverage and full startup."""
import csv,hashlib,json,struct
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
REUSED={'0x8c6536ec':'sa_engine_render_buffer_dispatch','0x8c6524d2':'sa_driver_dispatch_8c6524d2','0x8c6652c0':'sa_payload_metadata_8c6652c0','0x8c665466':'sa_linked_list_push_8c665466'}

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 names=['translation','connection','format_verification','size_edges_verification','allocation_verification','fixed_verification','cache_verification','edges_verification','failure_verification','retained_verification','startup_verification'];reports={};files=[Path(__file__)]
 for name in names:
  path=P/f'analysis/movie_render_buffer_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,(name,pinned);files.append(Path(pinned))
 t=reports['translation'];dll=digest(ROOT/'work/sonic-adventure/build/movie_render_buffer.dll');assert reports['connection']['compiled_source_count']==203 and len(t['routines'])==19
 direct_names=['format','size_edges','allocation','fixed','cache','edges','failure'];direct=[reports[n+'_verification'] for n in direct_names]
 assert [(r['cases_passed'],r['independent_checks']) for r in direct]==[(216,4104),(28,532),(142,2280),(200,3200),(80,1296),(57,912),(4,40)]
 assert sum(r['cases_passed'] for r in direct)==727 and sum(r['independent_checks'] for r in direct)==12364
 retained=reports['retained_verification'];startup=reports['startup_verification'];assert (retained['total_original_native_comparisons'],retained['independent_checks'])==(231,4103)
 assert len(startup['cases'])==2 and startup['actual_next_frontier']=='0x8c65393a' and startup['independent_checks']==164
 assert all(r['dll_sha256']==dll for r in direct+[retained,startup])
 # Checked local proof of the one constant-bound branch, independent of input regs.
 body=next(r for r in t['routines'] if r['address']=='0x8c6653e8');listing={int(x['address'],16):x['instruction'].lstrip('_') for x in csv.DictReader(Path(body['listing']).open(),delimiter='\t')}
 expected={0x8c6653e8:'mov.l 0x8c665430,r7',0x8c6653ea:'mov.l 0x8c66542c,r6',0x8c6653ec:'add r6,r7',0x8c6653ee:'mov r6,r4',0x8c6653f0:'cmp/hs r7,r4',0x8c6653f2:'mov r6,r5',0x8c6653f4:'bt/s 0x8c66540e',0x8c6653f6:'mov #0x1,r6'}
 assert all(listing[a]==ins for a,ins in expected.items())
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes();files.append(binary);read=lambda a:struct.unpack_from('<I',raw,a-0x8c010000)[0]
 base,length=read(0x8c66542c),read(0x8c665430);assert (base,length)==(0x8c8c03e4,0x13000)
 end=(base+length)&0xffffffff;assert base<end and end==0x8c8d33e4
 dead=('0x8c6653f4','0x8c66540e');assert list(dead) in body['conditional_edges']
 proof=dict(address='0x8c6653e8',edge=list(dead),instruction_sequence=[dict(address=hex(a),instruction=x) for a,x in expected.items()],literal_base=hex(base),literal_length=hex(length),computed_end=hex(end),unsigned_comparison=False,reason='Before any input-dependent operation or write: r7=length, r6=base, r7=base+length without wrap, r4=base, CMP/HS r7,r4 is false. MOV r6,r5 does not change SR.T; BT/S cannot take its true edge. Delay MOV changes r6 only after the saved condition. Checked retail literals immutable; caller registers and mapped pool contents cannot alter this entry condition.')
 captures=[]
 for profile in ('warm','cold'):
  path=ROOT/f'work/sonic-adventure/movie_render_buffer_startup_{profile}_capture.json';capture=json.loads(path.read_text());assert capture['full_original_native_comparison'] and capture['dll_sha256']==dll;captures.append(capture);files.append(path)
 coverage=[]
 for routine in t['routines']:
  missing=set(routine['reachable_addresses']);missing_edges=set(map(tuple,routine['conditional_edges']))
  for r in direct[:-1]:
   c=next(x for x in r['coverage_by_routine'] if x['address']==routine['address']);missing&=set(c['missing_rows']);missing_edges&=set(map(tuple,c['missing_outcomes']))
  for capture in captures:missing-=set(capture['executed_rows']);missing_edges-=set(map(tuple,capture['executed_edges']))
  proved={dead} if routine['address']=='0x8c6653e8' else set();assert not missing and missing_edges==proved,(routine['address'],sorted(missing),sorted(missing_edges))
  coverage.append(dict(address=routine['address'],executed_rows=len(routine['reachable_addresses']),executed_conditional_outcomes=len(routine['conditional_edges'])-len(proved),proved_infeasible_outcomes=[list(x) for x in proved],missing_rows=[],missing_outcomes=[]))
 routines=[r|dict(kind='original_routine',module='1ST_READ.BIN',source=t['source'],source_sha256=digest(P/t['source'])) for r in t['routines'] if r['address'] not in REUSED]
 assert len(routines)==15 and sum(r['instruction_rows'] for r in routines)==1491 and sum(len(r['conditional_edges']) for r in routines)==370
 prior_path=P/'analysis/movie_graphics_handoff_qualification.json';prior=json.loads(prior_path.read_text());files.append(prior_path)
 for path,h in prior['sha256'].items():assert digest(path)==h,(prior_path,path)
 assert prior['total_evidence_original_native_comparisons']==3873
 limitations=[
 '15new original ownbodies1491instructions/370conditionaloutcomes:all1491rows and369outcomes execute;one cachepool entry-boundoutcome is impossible bycheckedretail instruction/literal proof. Four earlier canonical routines receive no duplicate credit. All19integration bodies1652rows execute;393of394outcomes execute plusoneproveddead.',
 '727freshdirect original/native comparisons12364independentchecks coverdescriptor formats/validation/sizes/mips,firstfit/bankmaxima,alignment/fixedrange splitting/exhaustion/rollback,cacheclasses/slotsearch/newallocation/failurecleanup,listlinks,negative size/wrap and4actualbad-descriptor failureprefixes. CompleteCPU/FPU/RAM/VRAM/Sound/device/orderedboundaries compare. Finitecoherent mappedaligned metadata/disjointprivate stack. Unsupported cachewidth usesordinary untouchedzero local RAM fixture;notclaimeduniversal arbitraryuninitializedstack behavior. Cycles,destructivealiases,physicalexceptions/concurrency remainunfinished.',
 'Fixedallocator oraclereused firstfit bankflagbits andfailed after16cases whileoriginal/native matched. Originalcheckedliterals establishfixedflags0800/1000 andalignmentflags0200/0400,distinctfromfirstfit0020/0040. Correctedoracle thenall200cases pass. NativegameC/DLL neverchangedforthisoraclecorrection;failed evidence retainedprivately andnot counted.',
 '231fresh retained comparisons4103checks replayoldroot/constructor/accounting oracles. Earlierproofbytes preserved;earlier3873evidence comparisonsnotallrerun. Accountingretainedcoverage describesnewintegrationmanifest (no newrenderbodycoverage expected);directnewrendercoverage isseparatelymeasured. Twofreshconfigured uninterrupted0546c0startups retainfull8MiBclear/two2MiBSoundfills/two33376bytetransfers/originalIRQ/RTE/REQUESTreturns/all1065282Soundevents0drops/39SDKrecords/32MIDIsends;exact202boundaryprefixmatches. Real32768bytebank1allocation completesandparentreaches65393a. No checkpoint/helperreturn/priming/shortenedclear/copy/historiccoldsubstitution.',
 'Prior202component source/proof/builds and1433canonical rows preserved. New203bridge adds19routes andrenamesexports;probeexactprefixrenaming retains17/18/19/20argument APIs. StaticC,no runtimeopcode decoding innewbodies. Strictunbounded syntax/generator andpreservation workflow checked separately beforepublication.',
 'Graphicsroot656940remains partialat twooutcomes(cf4->d06,e90->f38). FullgamePC60/120/240FPS8K,Android/stereoscopic3DS,N64/PS1,SUPIR HDtextures/percent/render/aspectsettings unfinished. TextureOff disables texturesandusesvertexcolors. No pixels,audibleoutput,physicalIRQtiming,resetboot,gameplayorplayableport claim. GoalACTIVE10%weighted milestones,notmeasuredcodecoverage.'
 ]
 q=dict(routines=routines,canonical_reuse=[dict(address=a,canonical_name=n,additional_recovery_credit=0) for a,n in REUSED.items()],newly_registered_entries=15,newly_qualified_rows=1491,executed_rows=1491,executed_conditional_outcomes=369,proved_infeasible_conditional_outcomes=1,qualified_conditional_outcomes=370,coverage_by_routine=coverage,local_infeasibility_proof=proof,compiled_source_count=203,new_direct_original_native_comparisons=727,new_direct_independent_checks=12364,fresh_retained_original_native_comparisons=231,fresh_retained_independent_checks=4103,fresh_full_startup_comparisons=2,fresh_startup_independent_checks=164,fresh_original_native_comparisons=960,fresh_independent_checks=16631,retained_202_source_original_native_comparisons=3873,total_evidence_original_native_comparisons=4833,actual_next_frontier='0x8c65393a',remaining_unqualified_graphics_root_outcomes=prior['remaining_unqualified_graphics_root_outcomes'],reused_additional_credit=0,dll_sha256=dll,limitations=limitations,sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_render_buffer_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('Qualified15renderbodies1491rows/370outcomes,960freshcomparisons,next65393a')

if __name__=='__main__':main()
