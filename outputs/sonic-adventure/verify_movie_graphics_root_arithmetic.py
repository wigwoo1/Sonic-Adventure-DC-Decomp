"""Independent finite-weight/count/alignment/capacity oracles for the real root.

These focused cases extend qualification evidence. They do not register the
still-partial root or replace any callee result with an expected value.
"""
import hashlib,itertools,json,struct
from pathlib import Path
import write_watch_graphics_init as fast
v=fast.old;P=v.P;ROOT=v.ROOT;ENTRY=v.GRAPHICS_INIT_ENTRY;CTX=0x8c8a330c

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def f32(value):return struct.unpack('<f',struct.pack('<f',value))[0]
def bits(value):return struct.unpack('<I',struct.pack('<f',value))[0]
def number(value):return struct.unpack('<f',struct.pack('<I',value))[0]
def signed(value):return value-(1<<32) if value>>31 else value

def cases():
 for mask in range(32):
  flags=sum(flag for i,flag in enumerate((1,2,0x10,0x20,0x10000)) if mask>>i&1)
  yield dict(kind='root_flags',gfx_config_words=[(4,flags)])
 for flags in (0x100000,0x110033):yield dict(kind='root_alternate_lists',gfx_config_words=[(4,flags)])
 for values in ([0]*5,[20]*5,[30]*5,[150,0,0,0,0],[100,0,0,0,0],[99,1,0,0,0],[0,0,100,0,0],[0,0,0,0,100]):
  yield dict(kind='root_finite_weights',gfx_config_words=[(0x2c+i*4,bits(x)) for i,x in enumerate(values)])
 for low in (1,3,15,31):yield dict(kind='root_vertex_alignment',gfx_config_words=[(0x24,0xacf5fd00+low)])
 for available in (0,8,40,160):yield dict(kind='root_minimum_space',gfx_config_words=[(0x28,available)]+[(0x2c+i*4,bits(0)) for i in range(5)],gfx_global_words=[(0x8c8a2d5c,0x40)])
 yield dict(kind='root_minimum_already_satisfied',gfx_config_words=[(0x28,160)]+[(0x2c+i*4,bits(100 if i==0 else 0)) for i in range(5)],gfx_global_words=[(0x8c8a2d5c,0x40)])
 for available in (0x80000000,0xffffff00,0xfffffff0):yield dict(kind='root_unsigned_capacity',gfx_config_words=[(0x28,available)])
 for reserved in (0,1,7,8,9,0x1ed000,0x1ed100,0x1ed200,0x1ed300,0x1ed3f8,0x1ed400,0x1ed408):yield dict(kind='root_capacity',gfx_config_words=[(0x1c,reserved)])

def counts_oracle(m,config,flags):
 available=v.load(m,config+0x28);pointer=v.load(m,config+0x24)
 if pointer&31:available=(available-(32-(pointer&31)))&0xffffffff;pointer=(pointer+31)&0xffffffe0
 if not flags&0x10:available>>=1
 weights=[number(v.load(m,config+0x2c+i*4)) for i in range(5)];total=0.0
 for weight in weights:assert 0<=weight<=150;total=f32(total+weight)
 if total>100:weights=[f32(f32(w*100)/total) for w in weights]
 as_float=f32(signed(available))
 if available>>31:as_float=f32(as_float+4294967296.0)
 counts=[]
 for weight in weights:
  value=f32(f32(weight*as_float)/100)
  if value>=2147483648:value=f32(value-4294967296.0)
  assert -2147483648<=value<2147483648
  counts.append((int(value)&0xffffffff)&0xfffffff8)
 if v.load(m,0x8c8a2d5c)&0x40 and counts[0]<56:counts[0]=(counts[0]+56)&0xffffffff
 summed=sum(counts)&0xffffffff;maximum=-1;index=None
 for i,value in enumerate(counts):
  if signed(value)>maximum:maximum=signed(value);index=i
 if summed>available:assert index is not None;counts[index]=((counts[index]-(summed-available))&0xffffffff)&0xffffffe0
 return pointer,[((x-16)&0xffffffff) if x else 0 for x in counts]

def semantic(m,a,before,s,f,r,c,path):
 assert r['full_state_match'] and r['ordered_boundaries_match'];checks=2
 config=before.r[4];flags=v.load(m,config+4)
 edges=set()
 for j,pc in enumerate(path):
  ins=v.ROWS.get(pc,'');distance=2 if ins.startswith(('bt/s ','bf/s ')) else 1
  if ins.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
 alternate=v.load(m,0x8c656a98);assert alternate==0x10000
 if not flags&alternate:
  pointer,counts=counts_oracle(m,config,flags)
  assert v.load(a,0x8c8a318c)==pointer
  assert [v.load(a,CTX+i*4) for i in range(1,6)]==counts;checks+=6
 if r['status']:
  assert c['kind']=='root_unsigned_capacity' and r['status']==-3 and r['stop_pc']=='0x8c652544'
  # Predict the first invalid header destination from the real pointer ABI.
  # Large unsigned counts can wrap a later list address outside mapped memory;
  # they do not necessarily produce the null pointer of the separate mode test.
  assert flags&0x10;fmt=v.load(m,config+0x5c);used=0;destinations=[]
  for j,size in enumerate(counts):
   if fmt==32+j:continue
   if not size:continue
   destination=(pointer+used*4)&0xffffffff;destinations.append(destination);used=((used+size+23)&0xffffffff)&0xfffffff8
   physical=destination&0x1fffffff
   if not (0x0c000000<=physical<0x0d000000 or 0x04000000<=physical<0x06000000):break
  assert destinations and r['boundaries'][-1]['target']=='0x8c65253e' and r['boundaries'][-1]['arguments'][0]==destinations[-1] and s.r[4]==destinations[-1]
  assert 0x8c656e4c not in path and v.load(a,CTX+0x58)==v.load(m,CTX+0x58);checks+=6
  return checks,edges
 assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]);checks+=3
 mode=v.load(m,0x8c8a2d74);video=v.load(m,0x8c8a2d78);width=640 if mode==1 or video&0x10 else 320
 height=((v.load(m,config+20)+31)&0xffffffff)&0xffffffe0 if flags&2 else 240 if mode!=1 and (not video&0x20 or not video&1 and video&0x2c==0x20) else 480
 index=int.from_bytes(v.sample(m,0x8c656a80,2),'little');index=v.load(m,0x8c8a2d7c) if index==255 else index
 stride=v.load(m,v.load(m,0x8c665d68)+4*index)>>3;display=(width*height*stride)&0xffffffff
 assert [v.load(a,CTX+offset) for offset in (0x48,0x4c,0x50,0x60)]==[width,height,stride,display];checks+=4
 reserved=(v.load(m,config+28)+7)&0xfffffff8
 original_total=v.load(m,0x8c6631f8);assert original_total==0x800000
 texture=min(((original_total-4*reserved-2*display)&0xffffffff)>>1,0x200000)
 assert v.load(a,CTX+0x5c)==reserved and v.load(a,CTX+0x58)==texture;checks+=2
 option=[b for b in r['boundaries'] if b['target']=='0x8c665a80'];assert option[-1]['arguments'][:3]==[0x8c671d98,0x8c671da4,0];checks+=1
 calls=[b['target'] for b in r['boundaries']]
 if (0x8c656e78,0x8c656e7a) in edges:
  assert texture<v.load(a,CTX+0x64) and s.r[0]==3 and '0x8c6634a0' not in calls and '0x8c657760' not in calls;checks+=4
 else:
  assert texture>=v.load(a,CTX+0x64);checks+=1
  texture_calls=[b for b in r['boundaries'] if b['target']=='0x8c6634a0'];assert texture_calls and texture_calls[0]['arguments'][0]==texture;checks+=1
 assert not any(r['sound_state']) and r['runtime_state'] is None;checks+=2
 return checks,edges

def main():
 ram=v.seed_ram();seen=set();edges=set();records=[];checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(dict(entry=ENTRY,mode=0,gfx_observed_fpscr=True)|data);m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s)
  r,path,a,s,f=fast.compare(m,s,f,c);n,new_edges=semantic(m,a,before,s,f,r,c,path);checks+=n;seen.update(path);edges.update(new_edges)
  records.append(dict(input=dict(c),status=r['status'],r0=s.r[0],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n,capacity_failure=(0x8c656e78,0x8c656e7a) in new_edges));print(i,c['kind'],s.r[0],flush=True)
 body=next(b for b in v.manifest['routines'] if int(b['address'],16)==ENTRY);wanted=set(body['reachable_addresses']);want_edges=set(map(tuple,body['conditional_edges']));actual={hex(x) for x in seen};actual_edges={(hex(a),hex(b)) for a,b in edges}
 coverage=dict(address=body['address'],static_rows=len(wanted),covered_rows=len(wanted&actual),missing_rows=sorted(wanted-actual),conditional_outcomes=len(want_edges),covered_outcomes=len(want_edges&actual_edges),missing_outcomes=[list(e) for e in sorted(want_edges-actual_edges)])
 assert any(x['capacity_failure'] for x in records) and any(not x['capacity_failure'] for x in records)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,v.GRAPHICS_CONFIG_FIXTURE_PATH,P/'analysis/movie_graphics_init_connection.json',P/'source/sa_movie_graphics_init.c',P/'source/sa_movie_graphics_init_connected.c',P/'source/sa_movie_driver.c',ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage=coverage,native_source_changed=False,additional_recovery_credit=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Focused full-entry root comparisons with independent finite nonnegative binary32 normalization/count/rounding/minimum/unsigned/underflow/vertex-alignment and display/texture-capacity oracles. Real insufficient-capacity branch returns3 before texture allocation or mode dispatch; huge unsigned list sizes retain their actual -3 unmapped-copy stop, independently computed invalid destination and earlier counts. Full CPU/FPU/RAM/VRAM/Sound/device/ordered boundaries compare; actual callees execute. Explicit observed raw configuration inputs; no checkpoint/helper returns. Root/constructor remain partial; no registration/full-startup/rendering/physical timing claim. Other arithmetic domains, remaining root edges and constructor allocation invariants need further work.')
 (P/'analysis/movie_graphics_root_arithmetic_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Root arithmetic',len(records),checks,coverage)
if __name__=='__main__':main()
