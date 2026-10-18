"""Confirm private stop-publication copies preserve all original guest logic."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 connection_path=PROJECT/'analysis/draw_motion_mesh_connection.json';connection=json.loads(connection_path.read_text())
 for p,h in connection['sha256'].items():assert digest(p)==h,p
 files=[Path(__file__),connection_path];records=[];prefix='#include "sa_draw_motion_stop.h"\n'
 for text in connection['sha256']:
  copy=Path(text)
  if not copy.name.startswith('sa_motion_reuse_') or copy.suffix!='.c':continue
  original=copy.with_name(copy.name.removeprefix('sa_motion_reuse_'));body=copy.read_text();assert body.startswith(prefix)
  stripped,count=re.subn(r'sa_motion_publish_stop\(ctx,0x[0-9a-f]+u\); ','',body[len(prefix):])
  assert count and stripped==original.read_text(),str(copy)
  records.append(dict(original=original.relative_to(PROJECT).as_posix(),copy=copy.relative_to(PROJECT).as_posix(),published_emitted_sites=count,original_guest_logic_unchanged=True));files.extend((original,copy))
 assert len(records)==connection['reused_stop_publication_copies']==61
 for name in ('sa_draw_motion_mesh_connected.c','sa_draw_motion_mesh_lower_connected.c'):
  source=PROJECT/'source'/name;assert 'struct context {uint32_t *published_stop;' in source.read_text();files.append(source)
 report=dict(copies_verified=len(records),published_emitted_sites=sum(r['published_emitted_sites'] for r in records),all_original_guest_logic_unchanged=True,newly_recovered_entries=0,reused_routine_credit=0,files=records,sha256={str(p):digest(p) for p in files},scope='Removing only the private publication include/calls exactly reproduces every original raw C source. The common context prefix is checked in both bridges. This audits host reporting metadata, not a new guest implementation or universal hardware exception/guard-PC behavior.')
 (PROJECT/'analysis/draw_motion_reuse_publication_audit.json').write_text(json.dumps(report,indent=2)+'\n')
 print('61 reporting copies preserve original guest logic;',report['published_emitted_sites'],'emitted sites; zero new routine credit.')
if __name__=='__main__':main()
