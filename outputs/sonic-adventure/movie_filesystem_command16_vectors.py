"""Declared ABI vectors and proof collection for file metadata helpers."""
import hashlib,json
from pathlib import Path
from movie_filesystem_command16_session import Session,v,tasks
P=v.P;RETURN=0x8cfed000;A=0x8cd90000;B=0x8cda0000;OUT=0x8cdb0000
PROFILES=((0,0),(1,0x400000a1),(4,0x600000a1),(7,0x400080a1))
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def put(m,a,value,n=4):m[a&0xffffff:(a&0xffffff)+n]=(value&((1<<(n*8))-1)).to_bytes(n,'little')
def raw(m,a,data):m[a&0xffffff:(a&0xffffff)+len(data)]=data
def fresh(base,entry,args,mode=0,sr=0):
    m=bytearray(base.m);s=type(base.s).from_buffer_copy(base.s);f=type(base.f).from_buffer_copy(base.f);c=v.Input(dict(base.c));c.update(entry=entry,mode=mode,sr=sr)
    for i in range(15):s.r[i]=(0x12345678^((i+1)*0x01030507))&0xffffffff
    for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
    s.r[15]=0x8cf00000;s.pc=entry;s.pr=RETURN;s.sr=sr;f.fpscr=0x40001|(mode<<19)
    for bank in range(2):
        for reg in range(16):f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
    return m,s,f,c
def decoded(before,entry,endian):
    out=bytearray(before);order='big' if endian else 'little';out[4:8]=int(entry[0]).to_bytes(4,'little');out[8:10]=int.from_bytes(entry[26:28],order).to_bytes(2,'little');out[10]=entry[1];out[12:16]=int.from_bytes(entry[2:4],order).to_bytes(4,'little');out[16:28]=entry[4:16];out[28]=0;out[32:40]=entry[16:24];out[40:44]=int.from_bytes(entry[24:26],order).to_bytes(4,'little');return out
def name(data):return data[:12].split(b'\0',1)[0]
def timestamp(data):
    bcd=lambda x:(x>>4)*10+(x&15)
    return ((bcd(data[0])*100+bcd(data[1]))&65535).to_bytes(2,'little')+bytes(bcd(x)&255 for x in data[2:7])+bytes(((1,2,3,4,5,6,0,255)[data[7]&7],))
class Evidence:
    def __init__(self):self.records=[];self.seen=set();self.edges=set();self.checks=0
    def require(self,ok,label):assert ok,label;self.checks+=1
    def compare(self,m,s,f,c,label):
        before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=tasks.fast.compare(m,s,f,c)
        self.require(r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],(label,'original/native state and boundaries'))
        self.require(s.pc==s.pr==before.pr and s.r[15]==before.r[15],(label,'return/stack'))
        self.require(bytes(f)==fp,(label,'all physical FPU banks'))
        self.require(list(s.r)[8:]==list(before.r)[8:],(label,'callee saved GPRs'))
        self.seen.update(path)
        for j,pc in enumerate(path):
            t=v.ROWS.get(pc,'');d=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+d<len(path):self.edges.add((hex(pc),hex(path[j+d])))
        self.records.append(dict(input=label,entry=hex(c['entry']),result=hex(s.r[0]),steps=len(path),boundaries=[b['target'] for b in r['boundaries']]))
        return r,a,s,f
    def report(self,path,addresses,files,scope):
        coverage=[]
        for body in v.ANALYSIS_ROUTINES:
            if int(body['address'],16) not in addresses:continue
            missing=set(body['reachable_addresses'])-{hex(pc) for pc in self.seen};outcomes=set(map(tuple,body['conditional_edges']))-self.edges
            coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(outcomes),missing_rows=sorted(missing),missing_outcomes=sorted(outcomes)))
        files=[Path(__file__),Path(v.__file__),v.MP,P/'analysis/movie_filesystem_command16_connection.json',P/'movie_filesystem_command16_session.py',P/'movie_filesystem_command16_task_fixtures.py',P/'write_watch_filesystem_command16.py',v.effects.DLL]+files
        report=dict(original_native_comparisons=len(self.records),independent_checks=self.checks,cases=self.records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope=scope)
        (P/'analysis'/path).write_text(json.dumps(report,indent=2)+'\n');print(path,'PASS',len(self.records),self.checks,flush=True)
