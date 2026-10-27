"""Real API pump frontier and actual diagnostic/formatter/callback execution."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_channel_request_fixtures as fx
v=fx.v;P=v.P;put=fx.put

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.seed_ram();records=[];checks=0;seen=set();edges=set();smoke='--smoke' in sys.argv
 def note(kind,data,r,path,n):
  nonlocal checks
  checks+=n;seen.update(path)
  for j,pc in enumerate(path[:-1]):
   if v.ROWS.get(pc,'').startswith(('bt ','bf ')):edges.add((hex(pc),hex(path[j+1])))
  records.append(dict(kind=kind,input=data,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
 for entry,unit,pump,busy,(mode,sr) in itertools.product((0x8c646982,0x8c6469c8),(1,20),(0,1),(0,0xff),fx.CONFIGS):
  c,m,s,f=fx.setup(ram,entry,[unit,0x89abcdef,0xa5a5a5a5,0x12345678],mode,sr);fx.configure(m,unit,busy=busy,order=mode%2)
  put(m,s.r[15],0x13579bdf);backend=0x8c660494 if entry==0x8c646982 else 0x8c6604e6
  pump_offset=int.from_bytes(v.sample(m,0x8c6469a8,2),'little',signed=True);put(m,fx.CTX+pump_offset,pump)
  before=type(s).from_buffer_copy(s);fp=bytes(f);expected,result,opcode,words=fx.expected_packet(m,s,backend,0x13579bdf)
  r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
  assert r['full_state_match'] and r['ordered_boundaries_match'] and r['status']==(-2 if pump else 0)
  assert v.sample(a,fx.CTX,0x2000)==v.sample(expected,fx.CTX,0x2000) and v.sample(a,fx.FRAME,512)==v.sample(expected,fx.FRAME,512);n+=2
  assert a[fx.LOCK&0xffffff]==0 and r['cache_purge_sequence']==[fx.LOCK];n+=2
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'] and r['final_macl']==44*unit;n+=4
  if pump:
   assert r['stop_pc']=='0x8c646654' and s.pr==(0x8c64699e if entry==0x8c646982 else 0x8c6469ea);n+=2
   assert s.r[15]==before.r[15]-8 and v.load(a,s.r[15])==result;n+=2
  else:
   assert s.r[0]==result and s.r[15]==before.r[15] and s.pc==s.pr==before.pr;n+=3
  note('api_pump_frontier',dict(entry=hex(entry),unit=unit,pump=pump,busy=busy,mode=mode,sr=hex(sr),words=words),r,path,n)
  if smoke:break
 print('REQUEST API',len(records),flush=True)
 for entry,callback,(mode,sr) in itertools.product((0x8c660494,0x8c6604e6),(0,0x8c05180c,0x8cfef000),fx.CONFIGS):
  c,m,s,f=fx.setup(ram,entry,[1,2,0,0],mode,sr);fx.configure(m,1,lock=1)
  offset=int.from_bytes(v.sample(m,0x8c65f3c8,2),'little',signed=True);put(m,fx.CTX+offset,1);put(m,v.load(m,0x8c65f3f0),callback)
  before=type(s).from_buffer_copy(s);fp=bytes(f);message=v.load(m,0x8c660614);text=bytes(m[message&0xffffff:]).split(b'\0',1)[0];out=v.load(m,0x8c65f3e8)
  assert b'%' not in text and len(text)==37
  r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3;unknown=callback==0x8cfef000
  assert r['status']==(-2 if unknown else 0) and r['full_state_match'] and r['ordered_boundaries_match']
  assert v.sample(a,out,len(text)+1)==text+b'\0' and v.sample(a,fx.FRAME,512)==v.sample(m,fx.FRAME,512);n+=2
  assert a[fx.LOCK&0xffffff]==0x81 and r['cache_purge_sequence']==[fx.LOCK];n+=2
  targets=[b['target'] for b in r['boundaries']];assert '0x8c10d400' in targets and '0x8c10dbc4' in targets and ('0x8c65f810' not in targets);n+=3
  assert (hex(callback) in targets)==bool(callback);n+=1
  if callback:
   event=next(b for b in r['boundaries'] if b['target']==hex(callback));assert event['arguments'][0]==out and event['pr']=='0x8c65f38a';n+=2
  if unknown:assert r['stop_pc']==hex(callback) and s.r[15]==before.r[15]-16;n+=2
  else:assert s.r[0]==0xfffffffe and s.r[15]==before.r[15] and s.pr==s.pc==before.pr;n+=3
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'];n+=3
  note('actual_busy_diagnostic',dict(entry=hex(entry),callback=hex(callback),mode=mode,sr=hex(sr),text_bytes=len(text)),r,path,n)
  if smoke:break
 print('REQUEST DIAGNOSTICS',len(records),flush=True)
 if smoke:return
 files=[Path(__file__),Path(fx.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_channel_request_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,executed_rows=sorted(map(hex,seen)),executed_edges=sorted(edges),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual646982/6469c8 APIs,extra stackargument,successfulpackets/busy-device,mode/SR matrix andboth realpumpbranches. Enabledpump stopsatexactmissing646654 afteroriginalpacket/unlock/save;nofakepump result. Busylock diagnosis executesactualcanonicalreport/formatter/textcopy andnull/NOP/unknownnotification. Independent37byteoriginalmessage/output/lock/serial/stack/packet oracles pluswholemachine/device/boundaries. Unalignedstack branch/physicalcontroller/pump internals unqualified;canonicalreport zero extra credit.')
 (P/'analysis/movie_channel_request_api_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL REQUEST API PASS',len(records),checks)
if __name__=='__main__':main()
