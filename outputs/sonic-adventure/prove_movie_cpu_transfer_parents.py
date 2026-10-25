"""Constant-return callee proofs; never force the callers' impossible edges."""
import csv,hashlib,json
from pathlib import Path
import write_watch_cpu_transfer as fast
v=fast.old;P=v.P;ROOT=v.ROOT

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 retail=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=retail.read_bytes();files=[Path(__file__),retail,Path(v.__file__),Path(fast.__file__),P/'analysis/movie_cpu_transfer_connection.json'];bodies={};checks=0
 for address,folder in ((0x8c65a5cc,'movie-request-fields'),(0x8c10f9f4,'movie-queue-hal'),(0x8c666c2c,'movie-queue-family'),(0x8c666d10,'movie-queue-family')):
  path=P/f'analysis/{folder}/{address:08x}_instructions.tsv';body={int(r['address'],16):r for r in csv.DictReader(path.open(),delimiter='\t')};bodies[address]=body;files.append(path)
  for pc,r in body.items():assert raw[pc-0x8c010000:pc-0x8c010000+2]==bytes.fromhex(r['bytes']);checks+=1
 def text(entry,pc):return bodies[entry][pc]['instruction'].lstrip('_')
 def literal(a):return int.from_bytes(raw[a-0x8c010000:a-0x8c010000+4],'little')
 setter=bodies[0x8c65a5cc]
 assert len(setter)==3 and [text(0x8c65a5cc,a) for a in sorted(setter)]==['mov.l r5,@(0x4,r4)','rts','mov #0x0,r0'];checks+=2
 config=bodies[0x8c10f9f4];returns=[a for a in config if text(0x8c10f9f4,a)=='rts'];assert returns==[0x8c10fa4e];checks+=1
 assert text(0x8c10f9f4,0x8c10fa46)=='mov #0x0,r3' and text(0x8c10f9f4,0x8c10fa50)=='mov r3,r0';checks+=2
 assert [text(0x8c10f9f4,a) for a in (0x8c10fa48,0x8c10fa4a,0x8c10fa4c)]==['mov.l @(0x10,r15),r2','mov.l r2,@(0x20,r4)','mov.l r3,@(0x30,r4)'];checks+=1
 assert not any(text(0x8c10f9f4,a).startswith(('jsr ','bsr ','jmp ')) for a in config);checks+=1
 assert not any(set(r['flows'].split())&{'8c10fa48','8c10fa4a','8c10fa4c','8c10fa4e'} for r in config.values());checks+=1
 assert literal(0x8c666cfc)==literal(0x8c666e3c)==0x8c65a5cc and literal(0x8c666e38)==0x8c10f9f4;checks+=3
 expected={0x8c666c2c:{0x8c666c7c:'mov.l 0x8c666cfc,r3',0x8c666c86:'jsr @r3',0x8c666c88:'mov r12,r4',0x8c666c8a:'mov r0,r14',0x8c666c8c:'tst r14,r14',0x8c666c8e:'bf 0x8c666cb4'},0x8c666d10:{0x8c666d84:'mov.l 0x8c666e38,r1',0x8c666d8a:'jsr @r1',0x8c666d8e:'add #0x14,r15',0x8c666d90:'mov r0,r3',0x8c666d92:'tst r3,r3',0x8c666d94:'bt/s 0x8c666d9c',0x8c666d96:'mov.l r0,@r11',0x8c666d98:'bsr 0x8c666e06',0x8c666d9a:'nop',0x8c666d9c:'mov.l 0x8c666e3c,r2',0x8c666da0:'jsr @r2',0x8c666da2:'mov r13,r4',0x8c666da4:'mov r0,r14',0x8c666da6:'tst r14,r14',0x8c666da8:'bf 0x8c666dd2'}}
 for entry,rows in expected.items():
  for pc,instruction in rows.items():assert text(entry,pc)==instruction,(hex(pc),text(entry,pc));checks+=1
 # Normal game returns are distinct from negative host guard statuses, which
 # propagate immediately before any caller post-return test. No return is supplied.
 cases=[];ram=v.seed_ram()
 for mode in (0,1,4):
  c=v.Input(dict(entry=v.REQUEST,mode=mode,sdk_mode=1,fifo_ready_count=0x40000));m,s,f=v.prepare(ram,c);r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['status']==-4 and r['stop_pc']=='0x8c65a6ec' and path.count(0x8c65a6ec)==2;checks+=3
  assert v.load(a,v.RECORD+24)==0 and v.load(a,v.RECORD+4)==2 and not any(r['sound_state']);checks+=3
  cases.append({k:r[k] for k in ('input','status','stop_pc','steps','boundary_comparisons','full_state_match','ordered_boundaries_match','sound_state','cache_writeback_sequence','write_watch')})
 for source,length in ((v.SRC+1,64),(v.SRC,65)):
  c=v.Input(dict(entry=v.SUBMIT,mode=0,source=source,length=length,fifo_ready_count=0x40000));m,s,f=v.prepare(ram,c);r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['status']==0 and s.r[0]==0x7fffffff and not any(r['sound_state']);checks+=3
  assert v.load(a,v.RECORD+4)==2 and v.load(a,v.RECORD+24)==0;checks+=2
  assert v.load(a,v.ROOT_CONTEXT+0x190)==v.RECORD;checks+=1
  cases.append({k:r[k] for k in ('input','status','stop_pc','steps','boundary_comparisons','full_state_match','ordered_boundaries_match','sound_state','cache_writeback_sequence','write_watch')})
 files.extend((P/'analysis/movie_cpu_transfer_verification.json',P/'source/sa_movie_queue_family.c',P/'source/sa_movie_request_fields.c'))
 report=dict(cases_passed=5,independent_checks=checks,unreachable_rows=['0x8c666d98','0x8c666d9a'],unreachable_outcomes=[['0x8c666c8e','0x8c666cb4'],['0x8c666d94','0x8c666d98'],['0x8c666da8','0x8c666dd2']],constant_return_helpers={'0x8c65a5cc':{'normal_r0':0,'reason':'Only three instructions; sole RTS delay slot explicitly loads zero into R0.'},'0x8c10f9f4':{'normal_r0':0,'reason':'Sole RTS copies R3 to R0 in its delay slot. FA46 sets R3 to zero; the three intervening instructions leave R3 unchanged. No internal call/computed jump or branch into that tail can bypass the zero assignment.'}},assumptions=['Original code/literals immutable; callers enter the original function starts. Valid initialized disjoint record/descriptor/root/stack storage and inherited CPU/software-platform domain.','A fault/unsupported access returns a host stop before the caller continuation. No fabricated game-helper result, patched branch or invented hardware interrupt is used.','The original setter/config leaf normal-return values are zero for every data input that reaches their RTS, rather than only the tested scalar samples.','The larger REQUEST parent remains partial at actual completion; three current-build controls preserve its genuine wait and unchanged zero completion. Two valid parent calls retain the actual alignment error with state2/incomplete/current record unchanged.'],cases=cases,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original byte/literal and constant-return proofs close three impossible caller outcomes and two dead rows. Actual compiled callbacks/copies execute in separate full-state comparisons; no impossible edge is forced. Five current-build wait/error controls retain genuine outcomes; synchronous and channel-start parent qualification remains a separate coverage decision.')
 (P/'analysis/movie_cpu_transfer_parent_proof.json').write_text(json.dumps(report,indent=2)+'\n');print('Parent proof:',checks,'checks;5actualwait/errorcontrols;2deadrows/3deadoutcomes')
if __name__=='__main__':main()
