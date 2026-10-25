"""Fresh complete two-MiB varying-data copy; save results before validation."""
import hashlib,json,time
from pathlib import Path
import write_watch_cpu_transfer as fast
v=fast.old;P=v.P;ROOT=v.ROOT

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 c=v.Input(dict(entry=v.COPY,mode=4,length=0x200000,pattern_bytes=0x200000,fifo_ready_count=0x40000,instruction_budget=0x4000000,progress=True));m,s,f=v.prepare(v.seed_ram(),c);before=v.effects.State.from_buffer_copy(s);source=v.sample(m,before.r[4],c['length'])
 print('START complete varying-data CPU copy',flush=True);started=time.perf_counter();r,path,a,s,f=fast.compare(m,s,f,c)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_cpu_transfer_connection.json',P/'analysis/movie_transfer_start_qualification.json']
 keep=('input','status','stop_pc','steps','boundary_comparisons','full_state_match','ordered_boundaries_match','sound_state','sound_ram_sha256','sound_trace_sha256','sound_trace_events','sound_trace_dropped','write_watch','fifo_cursor')
 report=dict(record={k:r[k] for k in keep},source_bytes=len(source),source_sha256=hashlib.sha256(source).hexdigest(),source_unchanged=v.sample(a,before.r[4],c['length'])==source,guest_r0=hex(s.r[0]),original_sp=hex(before.r[15]),final_sp=hex(s.r[15]),original_pr=hex(before.pr),final_pr=hex(s.pr),word_copy_calls=[b for b in r['boundaries'] if b['target']=='0x8c10d770'],elapsed_seconds=time.perf_counter()-started,full_original_native_comparison=True,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files})
 (ROOT/'work/sonic-adventure/movie_cpu_transfer_full_capture.json').write_text(json.dumps(report,indent=2)+'\n');print('CAPTURED complete CPU copy',r['status'],r['stop_pc'],r['sound_state'],flush=True)
if __name__=='__main__':main()
