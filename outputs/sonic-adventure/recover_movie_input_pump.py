"""Original header I/O/readiness and canonical backend integration."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;EVENT_PARENT=PROJECT/'recover_boot_event.py'
exec(compile(EVENT_PARENT.read_text().split('notes=[];diagnostics=[]')[0],str(EVENT_PARENT),'exec'),globals())
remainder=PROJECT/'source/sa_unsigned_remainder.c';remainder_proof=PROJECT/'analysis/unsigned_remainder_verification.json'
assert hashlib.sha256(remainder.read_bytes()).hexdigest()==json.loads(remainder_proof.read_text())['source_sha256']
division=remainder.read_text();division=division[division.index('static void div1_step'):division.index('SA_STAGE_EXPORT int')]
notes=[];diagnostics=[];excluded={}
groups={'movie-input-pump-canonical':{0x8c646654,0x8c65f1aa,0x8c65f202,0x8c65f1ec,0x8c64658a,0x8c65fd4e,0x8c65fe14,0x8c65ff12,0x8c65ff5a,0x8c65ffb8,0x8c66005c,0x8c66018e,0x8c668b24}}
for folder,included in groups.items():
 path=PROJECT/'analysis'/folder/'diagnostics.tsv';diagnostics.append(path)
 for note in csv.DictReader(path.open(),delimiter='\t'):
  assert note['completed']=='true' and not note['error']
  if int(note['address'],16) in included:notes.append(dict(note,folder=folder))
assert len(notes)==13
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
 tas="        if w&0xf0ff==0x401b:return f'if(!valid_ram(r[{n}],1u))return -3;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[{n}],3u);if(status)return status;{{uint32_t value=rd(m,r[{n}],1);wr(m,r[{n}],value|0x80u,1);*sr=(*sr&~1u)|(value==0);}}'\n"
 chosen=chosen.replace(marker_control,tas+marker_control)
 exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_movie_input_pump_{start:08x}',len(rows))
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
 # No new nonlocal continuation is claimed below these synchronous backends.
 functions.append(function);routines.append(record)
header=header.replace(header[header.index('/*'):header.index(' */')+3],'/* Canonical input pump, response decoding and status publication integration.\n * TAS is one uninterrupted guest operation on coherent RAM, with explicit purge.\n * Single guest executor only; host multithreading/cache/TLB hardware unqualified.\n */')
functions=[f.replace('(void)gbr;','(void)gbr;(void)valid_access;(void)r;') for f in functions]
source=PROJECT/'source/sa_movie_input_pump.c';source.write_text(header+'\n'.join(functions))
import movie_channel_request_reference as previous
prior_returns=previous.namespace['RETURNS'];new_returns={int(row['address'],16)+4 for r in routines for row in csv.DictReader(Path(r['listing']).open(),delimiter='\t') if row['instruction'].lstrip('_').startswith(('jsr ','bsr '))}
return_manifest=PROJECT/'analysis/movie_input_pump_returns.json'
return_manifest.write_text(json.dumps(dict(prior_return_sites=len(prior_returns),new_call_returns=sorted(map(hex,new_returns)),allowed_call_returns=sorted(map(hex,prior_returns|new_returns))),indent=2)+'\n')
files=[Path(__file__),EVENT_PARENT,SUBMIT_PARENT,WAIT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_draw_polygon_lower.py',PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/sh4_integer_state.h',PROJECT/'source/sh4_integer_state.c',PROJECT/'source/sh4_vector_base.c',PROJECT/'source/sh4_vector_base.h',remainder,remainder_proof]+diagnostics+[Path(r['listing']) for r in routines]+[return_manifest,Path(previous.__file__),PROJECT/'ghidra/export_movie_input_pump_canonical.ps1']
registry=json.loads((PROJECT/'analysis/recovered_routines.json').read_text())['routines'][:1493];assert len(registry)==1493;canonical=[];proposed=[]
for routine in routines:
 row=next((r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==routine['address']),None)
 if row is None:proposed.append(routine);continue
 a=int(row['address'],16);b=int(row['end_exclusive'],16);assert hashlib.sha256(binary[a-0x8c010000:b-0x8c010000]).hexdigest()==row['retail_span_sha256'];canonical.append(row);files.extend((PROJECT/row['source'],PROJECT/row['verification']))
assert len(canonical)==6 and len(proposed)==7
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,return_manifest=return_manifest.relative_to(PROJECT).as_posix(),instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,canonical_duplicate_credit=0,canonical_reuse=canonical,skipped_already_connected=['0x8c6461ae','0x8c65f50c'],scope='Thirteen already registered canonical input-pump/response adapters, zero new routine credit. Existing status publisher6461ae/getter65f50c routes retained. Byte-checked static source, explicit TAS byte/T and purge, no substitute request/diagnostic/completion result. Existing source and proofs retained. Behavior qualification separate; single guest executor coherent-RAM domain only; no host-concurrent atomicity/TLB/cache-hardware claim.',architecture_reference=dict(url='https://www.st.com/resource/ja/user_manual/cd00147165-sh-4-32-bit-cpu-core-architecture-stmicroelectronics.pdf',pdf_page=474,opcode='0100nnnn00011011',tas='Purge then read byte; T=old==0; store old|128 without guest instruction interruption.'),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
report.update(unregistered_response_proposals=[r['address'] for r in proposed],canonical_rows=sum(r['instruction_rows'] for r in routines if any(c['address']==r['address'] for c in canonical)),proposed_rows=sum(r['instruction_rows'] for r in proposed),scope='Six registered canonical adapters and seven existing unregistered response proposals; zero credit on generation. Actual13bodies1035bytechecked rows, existing publisher6461ae/getter65f50c routes retained. Canonical6receivezero duplicatecredit;unregistered7requirefreshbehavior/coverage qualification. ExplicitTASpurge/coherentRAM single guestexecutor domain;no physicalresponse/hostthread/TLB claim.')
(PROJECT/'analysis/movie_input_pump_translation.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated',len(routines),'pump/response adapters /',report['instruction_rows'],'rows,6canonical/7proposed')
