"""Observe the retained startup heap at its actual allocation boundary."""
import hashlib,json,sys,types
from pathlib import Path
import verify_movie_render_setup as base
P=base.P;ROOT=base.ROOT
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def compare(m,s,f,c):
 result=base.compare(m,s,f,c);record,path,a,s,f=result
 assert record['status']==-2 and record['stop_pc']=='0x8c6021d2'
 literal=base.load(a,0x8c602284)
 def valid(address):return not address&3 and 0x0c000000<=address&0x1fffffff<=0x0cfffff8
 assert valid(literal)
 cursor=base.load(a,literal);nodes=[];seen=set();at=cursor
 while valid(at) and at not in seen and len(nodes)<16:
  seen.add(at);nxt=base.load(a,at);units=base.load(a,at+4)
  nodes.append(dict(address=hex(at),next=hex(nxt),units=units));at=nxt
 files=[Path(__file__),P/'verify_movie_render_config_prefix.py',Path(base.__file__),P/'analysis/movie_render_setup_qualification.json']
 report=dict(stop_pc=record['stop_pc'],allocator_entry='0x8c6021d6',request_bytes=s.r[4],root_literal_pc='0x8c602284',root_word=hex(literal),cursor=hex(cursor),nodes=nodes,
             next_unexamined=hex(at),closed_to_cursor=at==cursor and bool(nodes),invalid_cursor=not valid(cursor),
             full_state_comparison_passed=True,dll_sha256=digest(base.effects.DLL),sha256={str(p):digest(p) for p in files},
             scope='Read-only observation of one retained initialized startup profile paused at its actual unconnected allocator thunk. Existing runtime/driver/context fixtures are unchanged; no heap root, free-list node or allocation result is inserted. At most sixteen nodes are inspected without executing allocation.')
 (P/'analysis/movie_heap_state_probe.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('root_word','cursor','request_bytes','nodes','invalid_cursor')},flush=True)
 return result
v=types.SimpleNamespace(**base.__dict__);v.compare=compare
parent=P/'verify_movie_render_config_prefix.py';code=parent.read_text().replace('import verify_movie_render_config as v','')
if '--smoke' not in sys.argv:sys.argv.append('--smoke')
exec(compile(code,str(parent),'exec'),globals())
