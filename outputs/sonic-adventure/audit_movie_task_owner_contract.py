"""Bound the original counter observations to examined code and real helpers."""
import csv,hashlib,json,struct
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes();base=0x8c010000
 listing=P/'analysis/movie-record-register/8c668bee_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));by={int(r['address'],16):r for r in rows};assert len(rows)==87
 for r in rows:
  at=int(r['address'],16)-base;assert bytes.fromhex(r['bytes'])==raw[at:at+2]
 assert by[0x8c668c08]['instruction']=='mov r4,r13'
 r13_writes=[r['address'] for r in rows if r['instruction'].lstrip('_').endswith(',r13')];assert r13_writes==['8c668c08','8c668cc4']
 stores=[]
 for r in rows:
  w=int.from_bytes(bytes.fromhex(r['bytes']),'little');n=(w>>8)&15;kind=None
  if w>>12==1:kind=n
  elif w>>12==2 and w&15 in (0,1,2,4,5,6):kind=n
  elif w>>12==0 and w&15 in (4,5,6):kind=n
  elif w==0x4f22:kind=15
  if kind is not None:assert kind in (14,15),(r,kind);stores.append(dict(pc=r['address'],base_register=kind,instruction=r['instruction']))
 assert struct.unpack_from('<h',raw,0x8c668c32-base)[0]==0x208
 assert by[0x8c668c18]['instruction']=='add #0x4,r0' and by[0x8c668c1a]['instruction']=='mov.l @(r0,r13),r3'
 assert struct.unpack_from('<h',raw,0x8c668d70-base)[0]==0x20c
 assert struct.unpack_from('<h',raw,0x8c668d72-base)[0]==0x2694
 hits=[]
 for pc in range(0x8c660000,0x8c669000,2):
  w=struct.unpack_from('<H',raw,pc-base)[0]
  if w>>12==9:
   target=pc+4+(w&255)*2
   if struct.unpack_from('<H',raw,target-base)[0]==0x20c:hits.append(dict(pc=hex(pc),literal=hex(target),reg=(w>>8)&15))
 assert hits==[dict(pc='0x8c668cca',literal='0x8c668d70',reg=0)]
 files=[Path(__file__),binary,listing,P/'analysis/movie-task-admission/references.tsv',P/'analysis/movie-task-admission-lower/8c668d18_instructions.tsv',P/'analysis/movie-task-input-entries/8c64790e_instructions.tsv',P/'analysis/movie-task-input-entries/8c647980_instructions.tsv']
 report=dict(enqueue_rows=87,owner_register='r13',owner_register_writes=r13_writes,enqueue_direct_stores=stores,enqueue_owner_counter_read_pc='0x8c668c1a',enqueue_direct_owner_metadata_stores=0,helper_destinations={'0x8c668c10':'private stack temporary','0x8c668c92':'record+0x18','0x8c668c98':'private stack temporary','0x8c668ca4':'record+0x18'},bounded_movw_20c_scan=dict(start='0x8c660000',end_exclusive='0x8c669000',hits=hits,not_an_exhaustive_global_writer_analysis=True),cleanup_counter_store='0x8c668cd6',successful_owner_teardown_zero_bytes=0x2694,unqualified_task_entries=['0x8c64790e','0x8c647980'],next_worker_shared_target='0x8c647982',sha256={str(p):digest(p) for p in files},scope='Bytechecked bounded static audit:enqueue owner pointer staysr13;direct stores targetrecord(r14) orstack(r15),andfouractualhelper destinations are record/context/stack. Under valid disjoint owner/record/stack domains,this body doesnot increment owner+20c. The limitedMOV.Wliteral scan is evidence forlocalanalysis,notproof noothergamecode writes thefield. Actualinput-channel callersexecution separate. Cleanupdecrement andwholeownerzeroing areoriginalbehavior;countermeaning/upstreamcontractremainundetermined. Workerexports researchonly,notrecoveredcodecredit.')
 (P/'analysis/movie_task_owner_contract_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('OWNER CONTRACT AUDIT PASS;bounded scope;no directenqueue metadata stores')
if __name__=='__main__':main()
