"""Static byte/bridge/guard preservation audit, separate from behavior evidence."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 mp=P/'analysis/movie_channel_request_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_channel_request_connection.json';c=json.loads(cp.read_text())
 for report in (t,c):
  for path,value in report['sha256'].items():assert digest(path)==value,path
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();rows=0
 for body in t['routines']:
  for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
   address=int(row['address'],16);raw=bytes.fromhex(row['bytes']);assert binary[address-0x8c010000:address-0x8c010000+2]==raw
   rows+=1
 assert rows==125 and {r['address']:r['instruction_rows'] for r in t['routines']}=={'0x8c660494':22,'0x8c6604e6':25,'0x8c6604c0':19,'0x8c660518':23,'0x8c65f348':36}
 old=(P/'source/sa_movie_channel_io_connected.c').read_text();new=(P/c['source']).read_text()
 for body in t['routines']:
  decl=f'VDECL({body["name"]});\n';route=f' if(target=={body["address"]}u)return {body["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
  assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movie_channel_request_'+suffix,'sa_movie_channel_io_'+suffix)
 def allowed(text):return text.split('static const uint32_t allowed[]={')[1].split('};',1)[0]
 new=new.replace(allowed(new),allowed(old));assert new==old
 assert (P/'source/movie_channel_request_probe.c').read_text()==(P/'source/movie_channel_io_probe.c').read_text().replace('sa_movie_channel_io_','sa_movie_channel_request_')
 import movie_channel_request_reference as v
 import movie_channel_io_reference as previous
 import channel_request_reference_cpu as cpu
 assert v.code==previous.code
 assert v.namespace['RETURNS']==previous.namespace['RETURNS']|{int(x,16) for x in json.loads((P/t['return_manifest']).read_text())['new_call_returns']}
 actual={int(x[:-1],16) for x in allowed((P/c['source']).read_text()).split(',')};assert actual==v.namespace['RETURNS']
 marker='        elif word & 0xF0FF == 0x401B:'
 extra="\n            if pc in (0x8c66049a,0x8c6604ec):\n                assert cache_purge is not None,'TAS needs explicit coherent purge service'\n                cache_purge(regs[n])"
 assert cpu.code.replace(marker+extra,marker)==cpu.old.code
 text=(P/t['source']).read_text();assert 'SA_BOUND_RETAIL_STALLS' not in text
 for pc in (0x8c66049a,0x8c6604ec):
  line=next(x for x in text.splitlines() if x.startswith(f'L{pc:x}:'))
  for token in ('valid_ram(r[0],1u)','bus->cache(bus->context,r[0],3u)','value|0x80u','(value==0)'):assert token in line
  assert line.index('bus->cache(')<line.index('uint32_t value=')<line.index('wr(m,')<line.index('*sr=')
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=next(r for r in registry if r['address']=='0x8c65f348' and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN')
 assert canonical['source']=='source/sa_input_device_report_callback_8c65f348.c'
 previous_builds=P/'analysis/movie_channel_request_previous_build_manifest.json';old_build=json.loads(previous_builds.read_text())
 assert len(old_build['files'])==2675
 for row in old_build['files']:assert digest(ROOT/row['path'])==row['sha256'],row['path']
 # The canonical callback is adapted, not counted as a newly recovered routine.
 files=[Path(__file__),mp,cp,P/t['source'],P/c['source'],P/'channel_request_reference_cpu.py',P/'movie_channel_request_reference.py',P/'write_watch_channel_request.py',P/'source/sa_input_device_report_callback_8c65f348.c',previous_builds]
 report=dict(instruction_rows_byte_checked=125,new_request_rows=89,canonical_diagnostic_rows=36,canonical_duplicate_credit=0,exact_prior_bridge_plus_five_routes=True,prior_runtime_ABI_guards_unchanged=True,prior_flat_execution_code_identical=True,only_CPU_change_two_TAS_purge_events=True,guard_call_return_sites=len(actual),new_nonlocal_continuation_exports=0,new_source_wait_limits=0,preserved_archived_builds=2675,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Staticproofonly,notbehaviorcredit. Original125rows/89newrequests+36canonical;mandatorycoherentpurge beforebyteatomicguestoperation;oldbridge/runtime/ABI/SRFD/continuationsbytepreservedapartfromfiveactualroutes/exportprefix/newcallreturnmembership. TwooriginalCPU TASpurgeeventextensionsonly;allotherexecutioncodeidentical. Cache/TLB/physicalhardware/hostconcurrentRAM unqualified.')
 (P/'analysis/movie_channel_request_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL REQUEST AUDIT PASS125rows;',len(actual),'returns;2675 priorbuilds preserved')
if __name__=='__main__':main()
