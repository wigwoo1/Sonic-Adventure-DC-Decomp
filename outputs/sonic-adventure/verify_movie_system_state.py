"""Independent settings/byte/pointer oracles with real native providers/callbacks."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_system_state as fast
import verify_movie_packet_flash as fixtures
v=fast.old;P=v.P;ROOT=v.ROOT
QUERY,BYTE,INFO,SUBTRACT,PREDICATE=v.SYSTEM_QUERY,v.SYSTEM_BYTE,v.SYSTEM_INFO,v.SYSTEM_SUBTRACT,v.SYSTEM_PREDICATE
OUT=0x8cda0000
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def signed_byte(value):return value-256 if value&128 else value
def case(entry=QUERY,setting=1,info_byte=0x31,**extra):
 info=bytearray(24);info[11]=info_byte;info[12]=ord('1');info[21]=setting
 payload=bytes(info[16:24])+bytes([255])*52
 base=dict(entry=entry,mode=extra.pop('mode',0),console_info=True,console_flash=True,prime_console=entry==QUERY,full_flash_storage=True,flash_writable=False,runtime_attached=True,info_bytes=list(info),system_arguments=[OUT,0,0,0],system_buffer=0x8cd42000,kind='normal',setting=setting,info_byte=info_byte)
 return base|fixtures.profile(records=[fixtures.record(5,payload)])|extra
def cases():
 for mode,setting in itertools.product((0,1,4),(0,1,0x7f,0x80,0xfe)):yield case(mode=mode,setting=setting)
 for mode,info_byte in itertools.product((0,1,4),(0,0x2f,0x30,0x31,0x39,0x7f,0x80,0xfe,0xff)):yield case(mode=mode,setting=255,info_byte=info_byte)
 for entry,mode in itertools.product((QUERY,BYTE),(0,1,4)):yield case(entry,mode=mode,kind='null-output',system_arguments=[0,0,0,0],prime_console=False)
 for mode in (0,1,4):yield case(mode=mode,kind='null-buffer',system_buffer=0,prime_console=False)
 for mode in (0,1,4):yield case(mode=mode,kind='missing-settings')|fixtures.profile(records=[fixtures.record(6,bytes([0x5a])*60)])
 for mode in (0,1,4):yield case(mode=mode,kind='blank-readonly',flash_patches=[])
 for mode,info_byte in itertools.product((0,1,4),(0,0x31,0x7f,0x80,0xff)):yield case(BYTE,mode=mode,info_byte=info_byte)
 for mode in (0,1,4):yield case(INFO,mode=mode)
 for mode,word in itertools.product((0,1,4),(0,1,0x2f,0x30,0x31,0x7f,0x80,0x7fffffff,0x80000000,0xffffffff)):yield case(SUBTRACT,mode=mode,system_arguments=[word,0x76543210,0x89abcdef,0x13579bdf])
 # Supported guest vectors may point to original game code. Both sides execute
 # the unchanged mode predicate, whose data selects its result. No host callback
 # replaces a game helper or supplies a synthesized return register.
 for entry,mode,hook in itertools.product((BYTE,INFO),(0,1,4),(0,16)):yield case(entry,mode=mode,kind='guest-vector',sysinfo_guest_predicate=hook,prime_console=False)
 for entry in (QUERY,BYTE):yield case(entry,kind='invalid-output',system_arguments=[0x20000000,0,0,0])
 for entry in (BYTE,INFO):yield case(entry,kind='unconfigured-info',console_info=False,prime_console=False)
def main():
 ram=v.seed_ram();seen=set();edges=set();records=[];checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=v.prepare(ram,c);kind=c['kind'];entry=c['entry'];before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
  v.store(m,OUT-4,0xa5a51234);v.store(m,OUT,0x76543210);v.store(m,OUT+4,0x5a5a5678)
  if kind=='unconfigured-info':v.store(m,0x8c0000b0,v.console_model.SYSINFO)
  output=0x76543210;result=0;status=0
  if entry==QUERY:
   if kind=='null-output':result=0xffffffff
   elif kind in ('null-buffer','blank-readonly'):result=0xffffffff;output=0
   elif kind=='missing-settings':output=(1 if c['info_byte']==255 else signed_byte(c['info_byte'])-0x30)&0xffffffff
   elif kind=='invalid-output':status=-3
   else:output=(1 if c['info_byte']==255 else signed_byte(c['info_byte'])-0x30) if c['setting']==255 else signed_byte(c['setting']);output&=0xffffffff
  elif entry==BYTE:
   if kind=='null-output':result=0xffffffff
   elif kind=='invalid-output':status=-3
   elif kind=='unconfigured-info':status=-2;output=0
   elif kind=='guest-vector':result=0xffffffff;output=0
   else:output=signed_byte(c['info_byte'])&0xffffffff
  elif entry==INFO:
   result=0 if kind=='guest-vector' else v.console_model.ADDRESS
   if kind=='unconfigured-info':status=-2
  elif entry==SUBTRACT:result=(before.r[4]-0x30)&0xffffffff
  r,path,a,s,f=fast.compare(m,s,f,c);seen.update(path)
  for j,pc in enumerate(path):
   ins=v.ROWS.get(pc,'');delta=2 if ins.startswith(('bt/s ','bf/s ')) else 1
   if ins.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+delta<len(path):edges.add((pc,path[j+delta]))
  assert r['full_state_match'] and r['ordered_boundaries_match'] and r['status']==status,(i,c,r['status'],r['stop_pc']);n=3
  if status==0:
   assert s.r[0]==result and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]),(i,c,hex(s.r[0]),hex(result),hex(s.pc),hex(s.pr),hex(before.pr),list(s.r[8:]),list(before.r[8:]));n+=4
  elif kind=='invalid-output':
   assert r['stop_pc']==('0x8c5ff3ac' if entry==QUERY else '0x8c5ff408') and s.r[15]==before.r[15]-(12 if entry==QUERY else 8);n+=2
   assert s.pr==(before.pr if entry==QUERY else 0x8c5ff40a);n+=1
  else:assert r['stop_pc']==hex(v.console_model.SYSINFO);n+=1
  assert bytes(f)==fp and v.load(a,OUT)==(output&0xffffffff) and v.load(a,OUT-4)==0xa5a51234 and v.load(a,OUT+4)==0x5a5a5678;n+=4
  assert r['flash_storage_sha256']==hashlib.sha256(fixtures.image(c)).hexdigest() and r['console_inputs_unchanged'];n+=2
  assert not any(r['sound_state']) and r['pvr_dma_state'][6]==0 and not r['aperture_events'];n+=3
  calls=[b['target'] for b in r['boundaries']]
  if entry==QUERY and kind not in ('null-output','null-buffer','invalid-output'):
   packet=next(b for b in r['boundaries'] if b['target']=='0x8c64bddc');assert packet['arguments'][:2]==[0,c['system_buffer']];n+=1
  if kind=='guest-vector':
   assert r['console_snapshot'][5]==0 and calls.count(hex(PREDICATE))==(2 if c['sysinfo_guest_predicate']==0 else 1);n+=2
  if kind in ('null-output','null-buffer'):assert '0x8c64bddc' not in calls and '0x8c5ff494' not in calls;n+=1
  checks+=n;records.append(dict(input=dict(c),status=r['status'],r0=s.r[0],output_word=v.load(a,OUT),steps=len(path),independent_checks=n));print(i,hex(entry),kind,r['status'],hex(s.r[0]),flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  wanted=set(body['reachable_addresses']);wanted_edges=set(map(tuple,body['conditional_edges']));actual={hex(a) for a in seen};actual_edges={(hex(a),hex(b)) for a,b in edges}
  missing=wanted-actual;missing_edges=wanted_edges-actual_edges;assert not missing and not missing_edges,(body['address'],missing,missing_edges)
  coverage.append(dict(address=body['address'],executed_rows=len(wanted),executed_outcomes=len(wanted_edges),missing_rows=[],missing_outcomes=[]))
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),P/'write_watch_buffer_initialize.py',P/'console_service_reference.py',v.MP,P/'analysis/movie_system_state_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,executed_integration_rows=89,executed_integration_outcomes=16,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Originalsettingsquery47rows/bytequery21rows/infolookup13rows allrows/outcomesexecute withactualnativeflash/CRC/consoleproviders. Canonicalsubtract3rows/modepredicate5rows areadapterswithzeroadditionalcredit. Pointerglobal8c67340cisactualimplicitR5packetbuffer,notaboolflag. Independent signedbyte/ASCIIoffset/default1/rawoutput/ABI/neighbor/immutableflashoracles;fulloriginal/nativeCPU/FPU/RAM/VRAM/Sound/platform/flash/runtime/orderedboundaries compare. DefensivehelperbranchesuseexplicitinitialSYSINFOvectorstotheunchangedoriginalmodepredicatewithdeclaredmodeword0or16;originalguestcodeexecutes,notahostreturnstub orclaimofphysicalBIOSfailure. Null/missingrecord/invalidoutput/unconfiguredhostprefixes retained;no fakeunwind. Physicaltiming/resetboot/fullgameunfinished.')
 (P/'analysis/movie_system_state_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('SYSTEM STATE',len(records),checks)
if __name__=='__main__':main()
