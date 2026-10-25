"""Private native-only pilot; never a substitute for the full comparison."""
import sys,hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'outputs/sonic-adventure'))
import write_watch_cpu_transfer as fast
import verify_movie_console as console
import verify_movie_init_records as init
v=fast.old
base=next(c for c in console.cases() if c['entry']==0x8c0546c0)
c=v.Input(base|dict(cold_filesystem=True,instruction_budget=0x4000000));c.pop('fifo_samples',None);c['fifo_ready_count']=0x40000
ram=v.seed_ram();m,s,f=init.prepare(ram,c)
code=fast.compare_code
line=" status,path,wanted,ml_expected,gb_expected,mh_expected,hidden,live=effects.reference_run(expected,other,ef,c,tracked=tuple(effects.CONNECTED))"
assert code.count(line)==1;code=code.replace(line,'')
marker=' rc=effects.fn(';assert code.count(marker)==1;code=code.replace(marker,' observe=effects.OBSERVE()\n'+marker)
at=code.index(' journal.check()')
code=code[:at]+" return dict(status=rc,stop_pc=hex(s.pc),disc_read_attempts=len(native_reads),filesystem_root=hex(int.from_bytes(actual[0x673414:0x673418],'little')),masaru_sha256=hashlib.sha256(actual[0xd00000:0xd00000+33408]).hexdigest(),native_clear_writes=native_stats.writes,native_clear_code_errors=native_stats.code_errors,native_clear_fpu_errors=native_stats.fpu_errors,sound_state=sound_model.snapshot(sound_memory),sound_ram_sha256=hashlib.sha256(sound_view).hexdigest(),cache_writeback_sequence=native_cache_writebacks,vram_sha256=hashlib.sha256(memory.vram).hexdigest()),actual\n"
ns=dict(fast.compare.__globals__);exec(compile(code,'private_native_cold_pilot','exec'),ns)
report,actual=ns['compare'](m,s,f,c)
source=ROOT/'work/sonic-adventure/extracted/files/SONICADV/MASARU.DRV';report['expected_masaru_sha256']=hashlib.sha256(source.read_bytes()).hexdigest();report['payload_matches']=report['masaru_sha256']==report['expected_masaru_sha256'];report['full_original_comparison']=False
old_path=v.P/'analysis/movie_transfer_start_prefix_verification.json';historical=json.loads(old_path.read_text());old=next(r for r in historical['cases'] if r['input']['cold_filesystem'])
assert report['status']==old['status']==-4 and report['stop_pc']==old['stop_pc']=='0x8c65a6ec'
assert report['disc_read_attempts']==old['disc_read_attempts']==89 and report['payload_matches']
assert report['filesystem_root']==old['filesystem_root_after'] and report['native_clear_writes']==old['clear_native']['writes']==0x200000
assert report['native_clear_code_errors']==report['native_clear_fpu_errors']==0
assert list(report['sound_state'])==old['sound_state'] and report['sound_ram_sha256']==old['sound_ram_sha256'] and report['cache_writeback_sequence']==old['cache_writeback_sequence']
assert report['vram_sha256']==hashlib.sha256(bytes(0x800000)).hexdigest()
files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,v.P/'analysis/movie_cpu_transfer_connection.json',old_path,source]
report.update(native_only=True,independent_checks=12,historical_full_caller_proof_retained=True,new_full_original_caller_comparison=False,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Current native-only earliest caller observation. Default path retains89reads, exact driver bytes, full video-clear count/zero bytes, fullSEGAfill and1043completewritebackaddresses before its genuine wait. Selected results match the preserved prior186-source original/native proof. This is not a fresh full original/native caller comparison and is not counted as one. No IRQ or helper result is supplied.')
(v.P/'analysis/movie_cpu_transfer_native_caller_observation.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('status','stop_pc','disc_read_attempts','payload_matches','native_clear_writes','native_only','new_full_original_caller_comparison')})
