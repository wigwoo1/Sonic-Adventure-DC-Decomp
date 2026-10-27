"""Original header I/O/readiness and canonical backend integration."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;EVENT_PARENT=PROJECT/'recover_boot_event.py'
exec(compile(EVENT_PARENT.read_text().split('notes=[];diagnostics=[]')[0],str(EVENT_PARENT),'exec'),globals())
remainder=PROJECT/'source/sa_unsigned_remainder.c';remainder_proof=PROJECT/'analysis/unsigned_remainder_verification.json'
assert hashlib.sha256(remainder.read_bytes()).hexdigest()==json.loads(remainder_proof.read_text())['source_sha256']
division=remainder.read_text();division=division[division.index('static void div1_step'):division.index('SA_STAGE_EXPORT int')]
notes=[];diagnostics=[];excluded={}
groups={'movie-channel-decoder-io':{0x8c6608a4,0x8c660dec},'movie-channel-io-backend':{0x8c646884,0x8c646b36,0x8c646b3c,0x8c646bbc,0x8c646982,0x8c6469c8},'movie-channel-io-status':{0x8c65f3f4,0x8c65f40e,0x8c65f41e},'movie-channel-io-request':{0x8c65f390}}
for folder,included in groups.items():
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\t'):
  assert note['completed']=='true' and not note['error']
  if int(note['address'],16) in included:notes.append(dict(note,folder=folder))
assert len(notes)==12
from channel_io_continuations import expand_function,adapt_decoder,adapt_worker
functions=[];routines=[];resume_routes=[]
for note in notes:
 start=int(note['address'],16);listing=PROJECT/'analysis'/note['folder']/f'{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 body_min=min(int(r['address'],16) for r in rows)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',note['folder']+'/')
 marker='assert len(raw)==int(row[\'length\'])==2 and start<=a<end';assert chosen.count(marker)==1
 chosen=chosen.replace(marker,'assert len(raw)==int(row[\'length\'])==2 and body_min<=a<end')
 marker_control="        assert not text.startswith('f'),text";assert chosen.count(marker_control)==1
 controls="        if w&0xf0ff==0x400a:return f'integer->mach=r[{n}];'\n        if w&0xf0ff==0x401a:return f'*macl=r[{n}];'\n"
 chosen=chosen.replace(marker_control,controls+marker_control)
 exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_movie_channel_io_{start:08x}',len(rows))
 marker='sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
 function=function.replace(marker,marker[:-2]+',uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){\n int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 function=re.sub(r'(r\[\d+\])=rd\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=rd(\2);if(io_status)return io_status;\1=loaded;}',function)
 guards={}
 if guards:
  function=function.replace(' int status;uint32_t target;',' unsigned delay_visits=0;(void)delay_visits;\n int status;uint32_t target;',1)
  for pc,active in guards.items():
   marker=f'L{pc:x}: ';assert function.count(marker)==1
   function=function.replace(marker,f'L{pc:x}:\n#ifdef SA_BOUND_RETAIL_STALLS\n *stop=0x{pc:x}u;if((*sr&1u) && ++delay_visits>=2u)return -4;\n#endif\n ',1)
  record['bounded_research_waits']=[dict(pc=hex(pc),continue_when_t=active,max_continue_tests=2,status=-4) for pc,active in guards.items()]
 record['registration_requires_complete_behavior']=True
 record['discontiguous_before_entry_rows']=[r['address'] for r in rows if int(r['address'],16)<start]
 addresses=sorted(int(r['address'],16) for r in rows);ranges=[]
 for address in addresses:
  if ranges and ranges[-1][1]==address:ranges[-1][1]+=2
  else:ranges.append([address,address+2])
 record['retail_instruction_ranges']=[dict(start=hex(a),end_exclusive=hex(b),sha256=hashlib.sha256(binary[a-0x8c010000:b-0x8c010000]).hexdigest()) for a,b in ranges]
 function,routes=expand_function(function,record,'sa_movie_channel_io');resume_routes.extend(routes)
 functions.append(function);routines.append(record)
header=header.replace(header[header.index('/*'):header.index(' */')+3],'/* Original header/readiness and canonical record backend; resumptions explicit.\n * Static C from byte-checked SH-4 instructions; complete explicit state.\n * Missing callees remain reached boundaries; no callback result is substituted.\n * Context and interrupt entry remain explicit platform state.\n */')
functions=[f.replace('(void)gbr;','(void)gbr;(void)valid_access;(void)r;') for f in functions]
header+='int sa_movie_channel_io_return_allowed(void *,uint32_t);\n'
source=PROJECT/'source/sa_movie_channel_io.c';source.write_text(header+'\n'.join(functions))
decoder_manifest=PROJECT/'analysis/movie_channel_decoder_translation.json';decoder=json.loads(decoder_manifest.read_text())
decoder_source=PROJECT/'source/sa_movie_channel_decoder.c';adapted_decoder=PROJECT/'source/sa_movie_channel_io_decoder.c'
adapted,added=adapt_decoder(decoder_source.read_text(),decoder['routines'],'sa_movie_channel_io_decoder');adapted_decoder.write_text(adapted);resume_routes.extend(added)
worker_manifest=PROJECT/'analysis/movie_channel_worker_translation.json';worker=json.loads(worker_manifest.read_text());worker_source=PROJECT/'source/sa_movie_channel_worker.c';adapted_worker=PROJECT/'source/sa_movie_channel_io_worker.c'
adapted,added=adapt_worker(worker_source.read_text(),next(r for r in worker['routines'] if r['address']=='0x8c647980'),'sa_movie_channel_io_worker');adapted_worker.write_text(adapted);resume_routes.extend(added)
assert len({r['pc'] for r in resume_routes})==len(resume_routes)
import movie_channel_decoder_reference as prior_reference
prior_returns={a+4 for a,t in prior_reference.ROWS.items() if t.startswith(('jsr ','bsr '))};assert prior_returns==prior_reference.namespace['RETURNS']
new_returns=set()
for r in routines:
 for row in csv.DictReader(Path(r['listing']).open(),delimiter='\t'):
  if row['instruction'].lstrip('_').startswith(('jsr ','bsr ')):new_returns.add(int(row['address'],16)+4)
returns=sorted(prior_returns|new_returns);return_manifest=PROJECT/'analysis/movie_channel_io_returns.json'
return_manifest.write_text(json.dumps(dict(prior_model_code_sha256=hashlib.sha256(prior_reference.code.encode()).hexdigest(),prior_rows_sha256=hashlib.sha256(json.dumps(sorted(prior_reference.ROWS.items())).encode()).hexdigest(),prior_return_sites=len(prior_returns),new_call_returns=sorted(map(hex,new_returns)),allowed_call_returns=list(map(hex,returns)),root_entry_pr_also_allowed=True,scope='Exact priorflat-original-model callreturnset plus neworiginalJSR/BSR continuations;notarbitraryPR relaxation. UsedonlyatresumedRTS beforedelayslot. NormalentryPRguardsunchanged;rootPRishostmetadata,notguestrewrite.'),indent=2)+'\n')
files=[Path(__file__),EVENT_PARENT,SUBMIT_PARENT,WAIT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_draw_polygon_lower.py',PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/sh4_integer_state.h',PROJECT/'source/sh4_integer_state.c',PROJECT/'source/sh4_vector_base.c',PROJECT/'source/sh4_vector_base.h',remainder,remainder_proof]+diagnostics+[Path(r['listing']) for r in routines]+[PROJECT/'recover_boot_file.py']+[PROJECT/'ghidra/export_movie_channel_decoder_io.ps1',PROJECT/'ghidra/export_movie_channel_io_backend.ps1',PROJECT/'ghidra/export_movie_channel_io_status.ps1',PROJECT/'ghidra/export_movie_channel_io_request.ps1',PROJECT/'channel_io_continuations.py',decoder_manifest,decoder_source,adapted_decoder,worker_manifest,worker_source,adapted_worker,return_manifest,Path(prior_reference.__file__)]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,resume_routes=resume_routes,adapted_decoder_source=adapted_decoder.relative_to(PROJECT).as_posix(),adapted_worker_source=adapted_worker.relative_to(PROJECT).as_posix(),return_manifest=return_manifest.relative_to(PROJECT).as_posix(),instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,excluded_entries={hex(a):why for a,why in excluded.items()},scope='Twelve originalIO/readiness/backend proposals391rows,including8canonical helpers96rows withzero duplicatecredit. Sharednativecontinuationroutes preserveregisterstate;resumedRTS usesexactflatmodelreturnset/rootentryPRguard. No fakeparser/callbackresults,taskcontext/counterrepair or neworiginalcreditforadaptedolderbodies. Behaviorqualification separate.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/movie_channel_io_translation.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated',len(routines),'file proposals /',report['instruction_rows'],'rows')
