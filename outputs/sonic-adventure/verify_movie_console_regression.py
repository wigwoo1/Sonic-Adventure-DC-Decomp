"""Replay the previous direct controls and earlier full-clear startup unchanged."""
import hashlib,json
from pathlib import Path
import verify_movie_console as v
import verify_movie_disc_init as boot_v
P=v.P;ROOT=v.ROOT
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 old_bridge=P/'source/sa_movie_initialize_connected.c';new_bridge=P/'source/sa_movie_console_connected.c'
 text=new_bridge.read_text().replace('#include "native_console_service.h"\n','').replace('struct sa_console_service *console;','')
 routes=[line+'\n' for line in text.splitlines() if line.startswith(' if(target==SA_NATIVE_SYSINFO_TARGET')];assert len(routes)==1;text=text.replace(routes[0],'')
 guard=' if(firmware==SA_NATIVE_SYSINFO_TARGET||firmware==SA_NATIVE_FLASH_TARGET){s->pc=entry;return -3;}\n';assert text.count(guard)==1;text=text.replace(guard,'')
 text=text.replace('struct sa_disc_queue *disc,uint32_t firmware,struct sa_console_service *console){','struct sa_disc_queue *disc,uint32_t firmware){').replace('0,disc,firmware,console};','0,disc,firmware};').replace('disc,firmware,console);','disc,firmware);')
 text=text.replace('sa_movie_console_connected','sa_movie_initialize_connected').replace('sa_movie_console_initialized_connected','sa_movie_initialize_initialized_connected')
 assert text==old_bridge.read_text()
 prior_path=P/'analysis/movie_initialize_verification.json';prior=json.loads(prior_path.read_text());cases=list(v.old.cases());assert len(cases)==len(prior['cases'])==68
 ram=v.old.old.driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();checks=0;records=[]
 for c,saved in zip(cases,prior['cases']):
  m,s,f=v.prepare(ram,c);record,path,a,s,f=v.compare(m,s,f,c)
  record.update(guest_r0_at_stop=hex(s.r[0]),system_info_vector=hex(v.load(m,0x8c0000b0)),flash_vector=hex(v.load(m,0x8c0000b8)))
  for key,value in saved.items():assert json.loads(json.dumps(record[key]))==value,key;checks+=1
  assert tuple(record['console_snapshot'])==(0,)*8 and record['console_inputs_unchanged'];checks+=2
  records.append(dict(entry=hex(c['entry']),status=record['status'],stop_pc=record['stop_pc']))
 warm_path=P/'analysis/movie_disc_init_verification.json';warm=next(c for c in json.loads(warm_path.read_text())['cases'] if c['input']['entry']==0x8c010180 and not c['input'].get('cold_filesystem'))
 c=next(c for c in boot_v.cases() if c['entry']==0x8c010180 and not c.get('cold_filesystem'));m,s,f=boot_v.prepare(ram,c);record,path,a,s,f=v.compare(m,s,f,c)
 assert record['status']==0 and hex(s.r[0])==warm['guest_r0']=='0xfffffff8' and record['stop_pc']=='0x8cfed000';checks+=3
 for key in ('steps','boundaries','boundary_comparisons','disc_reads','disc_snapshot','clear_native','clear_reference','vram_sha256','clock_reads'):assert json.loads(json.dumps(record[key]))==warm[key],key;checks+=1
 assert record['clear_native']['writes']==0x200000 and tuple(record['console_snapshot'])==(0,)*8;checks+=2
 files=[Path(__file__),Path(v.__file__),prior_path,warm_path,old_bridge,new_bridge,P/'analysis/movie_console_connection.json']
 report=dict(previous_direct_cases_replayed=68,retained_warm_startup_comparisons=1,total_comparisons=69,saved_record_checks=checks,original_bridge_preserved_except_explicit_console_interface=True,warm_result=record,warm_guest_r0=hex(s.r[0]),cases=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='New native console backend with unconfigured inputs replays all prior direct controls and every earlier warm full-clear write. Previous direct saved records match field-for-field, apart from newly added console metadata. All old game routes preserve their bytes. This is not a replay of the separate long cold benchmark or complete accumulated suite.')
 (P/'analysis/movie_console_regression_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Matched 68 prior direct records and retained full-clear startup;',checks,'saved-record checks')
if __name__=='__main__':main()
