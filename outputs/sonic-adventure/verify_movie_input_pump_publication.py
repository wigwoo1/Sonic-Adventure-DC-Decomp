"""Independent display-register publication and reached sample-depletion effects."""
import hashlib,itertools,json
from pathlib import Path
import movie_input_pump_fixtures as fx
v=fx.v;P=v.P;put=fx.put

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.seed_ram();records=[];checks=0;seen=set();edges=set()
 for publish,selection,mode,count in itertools.product((0,1),(0,2),(0,4),(0,0xffffffff)):
  c,m,s,f=fx.setup(ram,0x8c646654,[0,0,0,0],mode,0x400000a1);fx.configure(m,active=publish);put(m,fx.CTX+0xfb0,1);put(m,0x8c893e4c,0x8c04fe40);put(m,v.load(m,0x8c04fe8c),0,2)
  selected=v.load(m,0x8c646758);put(m,selected,selection);counter=v.load(m,0x8c64676c);put(m,counter,count)
  latched=v.load(m,0x8c646760);dirty=v.load(m,0x8c646768);put(m,latched,0x76543210);put(m,dirty,0)
  c['render_seed']={0x5f8040:0x13579bdf,0x5f80e8:0x40,0x5f80dc:0x12345678};before=type(s).from_buffer_copy(s);fp=bytes(f)
  r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  emitted=bool(publish and selection)
  assert v.load(a,selected)==selection and v.load(a,latched)==(0x13579bdf if emitted else 0x76543210) and v.load(a,dirty)==int(emitted) and v.load(a,counter)==(count+int(emitted))&0xffffffff;n+=4
  assert s.r[0]==0 and s.r[15]==before.r[15] and s.pr==s.pc==before.pr and list(s.r)[8:15]==list(before.r)[8:15];n+=4
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'] and a[fx.LOCK&0xffffff]==0 and v.load(a,fx.CTX+0xfc0)==1;n+=5
  checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((hex(pc),hex(path[j+distance])))
  records.append(dict(publish=publish,selection=selection,mode=mode,counter=count,emitted=emitted,status=r['status'],independent_checks=n))
 for mode in (0,4):
  c,m,s,f=fx.setup(ram,0x8c646654,[0,0,0,0],mode,0x400000a1);fx.configure(m);c['scan_samples']=[];before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
  assert r['status']==-4 and r['stop_pc']=='0x8c6466a4' and r['full_state_match'] and r['ordered_boundaries_match']
  assert a[fx.LOCK&0xffffff]==0x80 and v.load(a,fx.CTX+0xfc0)==0 and s.r[15]==before.r[15]-40 and r['cache_purge_sequence']==[fx.LOCK];n+=4
  assert not any(b['target']=='0x8c65f1ec' for b in r['boundaries']) and bytes(f)==fp and r['final_mach']==c['mach'];n+=3
  checks+=n;seen.update(path);records.append(dict(mode=mode,kind='display_sample_depleted',status=r['status'],stop_pc=r['stop_pc'],independent_checks=n))
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(fx.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_input_pump_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actualfalse game-modepredicate callback,declaredpreviousselection andpublicationflag;independentdisplay-latch/dirty/counter/overflow/stack/preservedstate expectations pluscompletebus/model match. Emptydisplay-sample stream stopsatreachedread withheldlock/stack/status effectsunchanged,notfakedtimingornewsourcewaitlimit. Notphysicaldisplaytiming proof.')
 (P/'analysis/movie_input_pump_publication_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INPUT PUMP PUBLICATION PASS',len(records),checks)
if __name__=='__main__':main()
