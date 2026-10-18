"""Original clipper SR/product sequences; complete bodies remain proposals."""
import collections,ctypes,hashlib,itertools,json,random,sys
from pathlib import Path
import verify_sh4_integer_state as core
import verify_draw_angle_blend as previous
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
MANIFEST=PROJECT/'analysis/draw_clipping_state_translation.json';manifest=json.loads(MANIFEST.read_text())
DLL=ROOT/'work/sonic-adventure/build/draw_clipping_state.dll';LIB=ctypes.CDLL(str(DLL))
State,Fpu,Extra=previous.State,previous.Fpu,core.Extra
FN=LIB.sa_draw_clipping_state_connected;FN.argtypes=previous.fn.argtypes+[ctypes.POINTER(Extra)];FN.restype=ctypes.c_int
PROBE=LIB.sa_draw_clipping_state_probe;PROBE.argtypes=FN.argtypes+[ctypes.c_uint32];PROBE.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def reference(machine,entry,checkpoint,binary):
 pc=entry;path=[];status=2
 for _ in range(32):
  word=int.from_bytes(binary[pc-0x8c010000:pc-0x8c010000+2],'little');n,m=(word>>8)&15,(word>>4)&15;regs=machine.registers();path.append(pc)
  if word>>12==0xe:machine.write(n,((word&255)-256 if word&128 else word&255)&0xffffffff)
  elif word&0xf00f==0x400d:
   shift=regs[m];count=shift&31
   value=(regs[n]<<count) if shift<2**31 else (regs[n]>>((-shift)&31) if (-shift)&31 else 0)
   machine.write(n,value&0xffffffff)
  elif word&0xf00f==0x6007:machine.write(n,(~regs[m])&0xffffffff)
  elif word&0xf0ff==0x0002:
   if not machine.status&0x40000000:status=-4;break
   machine.write(n,machine.status)
  elif word&0xf00f==0x2009:machine.write(n,regs[n]&regs[m])
  elif word&0xf00f==0x200b:machine.write(n,regs[n]|regs[m])
  elif word&0xf0ff==0x400e:
   if not machine.status&0x40000000:status=-4;break
   # Canonical physical banks do not move; only their visible selection changes.
   machine.status=core.valid_sr(regs[n])
  elif word&0xf00f==0x300d:
   a,b=regs[n],regs[m];a=a if a<2**31 else a-2**32;b=b if b<2**31 else b-2**32
   product=(a*b)%(2**64);machine.lo=product%(2**32);machine.hi=product//2**32
  elif word&0xf0ff==0x001a:machine.write(n,machine.lo)
  else:raise AssertionError((hex(pc),hex(word),'unimplemented scoped reference instruction'))
  if pc==checkpoint:pc+=2;break
  pc+=2
 else:raise AssertionError('scoped sequence did not finish')
 return status,pc,path
def compare(ram,binary,probe,sr,seed,pair=None):
 machine=core.Machine(sr,seed)
 if pair:
  w=int.from_bytes(binary[probe['entry']-0x8c010000:probe['entry']-0x8c010000+2],'little');machine.write((w>>8)&15,pair[0]);machine.write((w>>4)&15,pair[1])
 s=State();s.r[:]=machine.registers();s.sr=machine.status;s.pr=0x8cfed000;s.pc=probe['entry']
 f=Fpu();rng=random.Random(seed^0xf00)
 for b in range(2):f.banks[b][:]=[rng.getrandbits(32) for _ in range(16)]
 f.fpscr=rng.getrandbits(32);f.fpul=rng.getrandbits(32);initial_f=bytes(f)
 extra=Extra();extra.other[:]=machine.banks[1-core.bank(machine.status)];extra.mach=machine.hi
 lo=ctypes.c_uint32(machine.lo);gbr=ctypes.c_uint32(0x8ce10000);initial=(tuple(s.r),s.sr,s.pr,bytes(f),lo.value,extra.mach,tuple(extra.other),gbr.value)
 expected,pc,path=reference(machine,probe['entry'],probe['checkpoint'],binary)
 actual=bytearray(ram);view=previous.RAM_TYPE.from_buffer(actual);memory=previous.Memory();bus=previous.Bus();LIB.sa_vertex_bus_init(ctypes.byref(memory),view,ctypes.byref(bus));events=[]
 @previous.OBSERVE
 def observe(ctx,target,mem,fp,status,regs,pr):events.append((target,tuple(regs[:16]),status[0],pr[0],bytes(fp.contents),lo.value,extra.mach,tuple(extra.other),gbr.value))
 code=PROBE(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),core.P),ctypes.byref(lo),probe['entry'],None,observe,ctypes.byref(bus),ctypes.byref(gbr),ctypes.byref(extra),probe['checkpoint'])
 assert code==expected and s.pc==pc and list(s.r)==machine.registers() and s.sr==machine.status and s.pr==0x8cfed000 and lo.value==machine.lo and extra.mach==machine.hi and list(extra.other)==machine.banks[1-core.bank(machine.status)],(probe,hex(sr),seed,'original sequence complete state',code,expected,hex(s.pc),hex(pc))
 assert events==[(probe['entry'],*initial)],'entry state/extra-state observation'
 assert actual==ram and bytes(f)==initial_f and gbr.value==0x8ce10000 and memory.trace_count==memory.trace_dropped==memory.flushes==memory.prefetches==memory.invalidates==0,'RAM/FPU/GBR/queue preservation'
 return dict(entry=hex(probe['entry']),checkpoint=hex(probe['checkpoint']),owner=hex(probe['owner']),kind=probe['kind'],initial_sr=hex(sr),seed=seed,pair=pair,status=code,stop_pc=hex(pc),instruction_steps=len(path),full_state_match=True,original_model_match=True,bank_mach_and_ordered_entry_match=True,ram_fpu_gbr_queue_preserved=True),path
def reused_regressions(ram):
 import verify_draw_vector_blend as vector
 results=[]
 for module in (vector,previous):
  saved_fn,saved_lib=module.fn,module.LIB
  try:
   extra=Extra();extra.mach=0x89abcdef;extra.other[:]=[0x10304050+i for i in range(8)];initial=bytes(extra)
   module.fn=lambda *args:FN(*args,ctypes.byref(extra));module.LIB=LIB
   for mode,sr in itertools.product((0,4),(0x400083f3,0x600083f3)):
    c=dict(entry=module.ENTRY,mode=mode,semantic=True);m,s,f=module.prepare(ram,c);s.sr=sr;r,path,b=module.compare(m,s,f,c)
    assert r['status']==0 and bytes(extra)==initial
    results.append(dict(entry=hex(module.ENTRY),mode=mode,initial_sr=hex(sr),status=0,full_state_match=True,extra_state_preserved=True,independent_checks=r['independent_checks']))
  finally:module.fn,module.LIB=saved_fn,saved_lib
 return results
def probe_guards(ram):
 p=manifest['test_only_probe_entries'][0];actual=bytearray(ram);view=previous.RAM_TYPE.from_buffer(actual)
 s=State();s.r[:]=list(range(16));s.sr=0x600083f3;s.pr=0x8cfed000;f=Fpu();extra=Extra();extra.mach=0xdeadbeef;extra.other[:]=list(range(8))
 lo=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(0x8ce10000);bus=previous.Bus();mem=previous.Memory();LIB.sa_vertex_bus_init(ctypes.byref(mem),view,ctypes.byref(bus))
 shared=(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),core.P),ctypes.byref(lo),p['entry'],None,previous.OBSERVE(),ctypes.byref(bus),ctypes.byref(gb))
 before=(tuple(s.r),s.sr,s.pr,bytes(f),bytes(extra),lo.value,gb.value)
 for call,expected in ((lambda:FN(*shared,ctypes.byref(extra)),-2),(lambda:PROBE(*shared,ctypes.byref(extra),0xdeadbeef),-3),(lambda:FN(*shared,None),-3)):
  code=call();assert code==expected and s.pc==p['entry']
  assert before==(tuple(s.r),s.sr,s.pr,bytes(f),bytes(extra),lo.value,gb.value) and actual==ram
 assert mem.trace_count==mem.flushes==0
 return 3
def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_clipping_state_connection.json',PROJECT/'analysis/sh4_integer_state_verification.json']
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert digest(f)==h,f
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(binary)]=binary
 records=[];coverage=set();rng=random.Random(0x300d400e)
 pairs=[(0,0),(0xffffffff,0xffffffff),(0x80000000,0x80000000),(0x80000000,0xffffffff),(0x7fffffff,0x7fffffff),(0x7fffffff,0x80000000),(1,0x7fffffff),(0xfffffffe,0x80000001)]+[(rng.getrandbits(32),rng.getrandbits(32)) for _ in range(32)]
 for probe in manifest['test_only_probe_entries']:
  for sr in (0,0x20000000,0x400083f3,0x600083f3):
   for seed,pair in enumerate(pairs if probe['kind']=='signed_product' else [None]*8):
    record,path=compare(ram,binary,probe,sr,seed,pair);records.append(record);coverage.update(path)
 regressions=reused_regressions(ram)
 guard_cases=probe_guards(ram)
 connection=json.loads((PROJECT/'analysis/draw_clipping_state_connection.json').read_text())
 sr_sources=[str(Path(p).name) for p in connection['sha256'] if p.endswith('.c') and ('/* ldc ' in Path(p).read_text()) and ',SR */' in Path(p).read_text()]
 assert sr_sources==['sa_draw_clipping_state.c'],sr_sources
 own={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 assert len(records)==2816 and len(coverage)==98 and len(regressions)==8
 files=[Path(__file__),PROJECT/'verify_sh4_integer_state.py',PROJECT/'verify_draw_vector_blend.py',PROJECT/'verify_draw_angle_blend.py']+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(sequence_cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),original_sequence_rows_covered=len(coverage),full_proposal_static_rows=len(own),full_proposal_unexecuted_rows=len(own-coverage),complete_game_entries_verified=[],newly_recovered_game_entries=0,probe_sequences=len(manifest['test_only_probe_entries']),sr_sequences=8,signed_product_sequences=16,reused_helper_regressions=regressions,probe_api_guard_cases=guard_cases,compiled_sources_with_commented_ldc_sr=sr_sources,scope='The same compiled full-body proposals execute 24 explicitly mapped interior sequences to a scoped status-2 checkpoint: original eight SR bank-mask sequences and sixteen DMULS/STS pairs, with full canonical-bank/MACH/MACL/register/PC/FPU/RAM/GBR and entry-state comparisons. Eight existing vector/integer-angle regressions use the extended production API with independent arithmetic checks and unchanged extra state. Three probe-API guard cases prevent production interior entry, mismatched checkpoint entry and missing extra state. The source-comment LDC audit is scoped, not a complete proof about arbitrary SR writes. This is not whole-clipper/caller/geometry, interrupt, exception-vector, timing or hardware proof; no game entries registered.',source_sha256=digest(PROJECT/manifest['source']),dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},cases=records)
 (PROJECT/'analysis/draw_clipping_state_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('sequence_cases_passed','status_counts','original_sequence_rows_covered','full_proposal_static_rows','full_proposal_unexecuted_rows','complete_game_entries_verified','probe_sequences','dll_sha256')},indent=2))
if __name__=='__main__':main()
