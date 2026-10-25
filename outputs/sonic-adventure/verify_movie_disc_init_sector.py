"""Retained descriptor-failure/finite-wait controls for the original sector wrapper."""
import hashlib,json
from pathlib import Path
import verify_movie_disc_init as v
P=v.P

def main():
 ram=v.driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();base=next(c for c in v.cases() if c['entry']==v.SECTOR);records=[];coverage=set();edges=set();checks=0
 for mode in ('null','busy','free'):
  c=base|dict(sector=45016,descriptor_profile=mode);m,s,f=v.prepare(ram,c)
  m[v.DST&0xffffff:(v.DST&0xffffff)+84]=bytes(84);v.store(m,v.DST,v.CTX);v.store(m,v.CTX+12,0x8c66d240);v.store(m,v.CTX+0x3c,0x8c66c4b4)
  if mode=='null':v.store(m,v.CTX+0x14,0)
  if mode=='busy':v.store(m,v.CTX+0x1c,v.DST)
  before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=v.compare(m,s,f,c);r['guest_r0']=hex(s.r[0]);coverage.update(path)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  if mode!='free':
   assert r['status']==0 and s.r[0]==0xfffffff3 and s.r[15]==before.r[15] and s.pr==before.pr;checks+=4
   assert not r['disc_read_attempts'] and not r['disc_snapshot'][5] and r['disc_snapshot'][4]==1;checks+=3
  else:assert r['status']==-4 and r['stop_pc']=='0x8c600f7e';checks+=2
  records.append(r)
 body=set(v.PER_BODY[v.SECTOR]);files=[Path(__file__),Path(v.__file__),P/'disc_init_platform_reference.py',P/'analysis/movie_disc_init_translation.json',P/'analysis/movie_disc_init_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,covered_addresses=[hex(a) for a in sorted(coverage&body)],covered_edges=[[hex(a),hex(b)] for a,b in sorted(edges) if a in body],cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='The original null/owned descriptor paths return guest -13 without queuing transport work. A separate free-descriptor control retains the existing finite read-wait stop. It does not substitute a descriptor, firmware status or successful sector result. Combine explicit own-body coverage with actual initializer reads, not with claims of complete library qualification.')
 (P/'analysis/movie_disc_init_sector_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Sector-wrapper controls:',len(records),'cases /',checks,'independent checks;',len(coverage&body),'own rows')
if __name__=='__main__':main()
