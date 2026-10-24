"""Independent model of declared host disc policies, not Dreamcast firmware."""
import ctypes,shlex
from pathlib import Path
import json

U=ctypes.c_uint32
class Job(ctypes.Structure):
 _fields_=[('handle',U),('state',U),('lba',U),('count',U),('completed',U),('kind',U),('error',ctypes.c_int32),('output',ctypes.c_void_p),('capacity',ctypes.c_uint64)]
class Queue(ctypes.Structure):
 _fields_=[('read',ctypes.c_void_p),('user',ctypes.c_void_p),('file_offset',ctypes.c_uint64),('track_sectors',ctypes.c_uint64),('track_lba',U),('stride',U),('next_handle',U),('cursor',U),('busy',U),('jobs',Job*8)]
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint64,ctypes.POINTER(ctypes.c_uint8),U)

def track_info(root):
 manifest=json.loads((root/'outputs/sonic-adventure/disc_inventory.json').read_text())
 row=next(t for t in reversed([shlex.split(s) for s in manifest['gdi'].splitlines()[1:] if s.strip()]) if t[2]=='4')
 path=Path(manifest['source'])/row[4];offset=int(row[5]);stride=int(row[3]);return path,offset,(path.stat().st_size-offset)//stride,int(row[1]),stride

def snapshot(q,ram,reads):
 base=ctypes.addressof(ram)
 return (q.file_offset,q.track_sectors,q.track_lba,q.stride,q.next_handle,q.cursor,q.busy,tuple((j.handle,j.state,j.lba,j.count,j.completed,j.kind,j.error,None if j.output is None else j.output-base,j.capacity) for j in q.jobs),tuple(reads))

class DiscReference:
 def __init__(self,root,c):
  self.path,self.offset,self.sectors,self.base,self.stride=track_info(root);self.c=c;self.next=1;self.cursor=0;self.busy=0;self.reads=[]
  self.jobs=[[0,0,0,0,0,0,0,None,0] for _ in range(8)]
 def snapshot(self):return (self.offset,self.sectors,self.base,self.stride,self.next,self.cursor,self.busy,tuple(tuple(j) for j in self.jobs),tuple(self.reads))
 def span(self,a,n):
  if a&0xe0000000 not in (0,0x80000000,0xa0000000):return None
  p=(a&0x1fffffff)-0xc000000
  return p if 0<=p<=0x1000000 and n<=0x1000000-p else None
 def submit(self,lba,count,dest,capacity,kind=0):
  if self.busy or not self.base<=lba<=self.base+self.sectors or count>self.base+self.sectors-lba or count*2048>capacity or (count and lba+count-1>0xffffffff) or not 1<=self.next<=0x7fffffff:return 0
  slot=next((i for i,j in enumerate(self.jobs) if not j[1]),None)
  if slot is None:return 0
  h=self.next;self.next+=1;self.jobs[slot]=[h,1,lba,count,0,kind,0,dest,capacity];return h
 def service(self,m,op,a,b):
  def rd(p):return int.from_bytes(m[p:p+4],'little')
  def write(p,values):
   for i,v in enumerate(values):m[p+i*4:p+i*4+4]=(v&0xffffffff).to_bytes(4,'little')
  job=next((j for j in self.jobs if j[1] and j[0]==a),None)
  if op==3:return (-6,0) if self.busy or any(j[1] for j in self.jobs) else (0,0)
  if op==0:
   if a==24 and not b:return 0,self.submit(self.base,0,None,0)
   if a==40:
    p=self.span(b,8)
    if p is None or rd(p+4) or (dest:=self.span(rd(p),16)) is None:return 0,0
    return 0,self.submit(self.base,0,dest,0x1000000-dest,1)
   p=self.span(b,16)
   if a not in (16,17) or p is None:return 0,0
   fad,count,address,reserved=(rd(p+i*4) for i in range(4));dest=self.span(address,count*2048)
   if fad<150 or reserved or dest is None:return 0,0
   return 0,self.submit(fad-150,count,dest,0x1000000-dest)
  if op==4:
   p=self.span(a,8)
   if p is None:return -3,0
   write(p,[0 if self.busy else 2 if self.sectors else 7,0x80 if self.sectors else 0]);return 0,0
  if op==8:
   if self.busy or job is None:return 0,1
   if job[1]==1:job[1]=3
   return 0,0
  if op==2:
   if self.busy:return -6,0
   index=next(((self.cursor+n)%8 for n in range(8) if self.jobs[(self.cursor+n)%8][1]==1),None)
   if index is None:return 0,0
   j=self.jobs[index];self.cursor=(index+1)%8
   if j[5]:m[j[7]:j[7]+16]=b'SA-NATIVE v 1.00\0';j[1]=2;return 0,0
   if j[4]==j[3]:j[1]=2;return 0,0
   pos=self.offset+(j[2]+j[4]-self.base)*self.stride+(16 if self.stride==2352 else 0)
   error=-17 if len(self.reads)==self.c.get('fail_after',-1) else 0;self.reads.append((pos,2048,error))
   if error:j[6]=error;j[1]=4;return 0,0
   with self.path.open('rb') as f:f.seek(pos);data=f.read(2048)
   assert len(data)==2048;m[j[7]+j[4]*2048:j[7]+(j[4]+1)*2048]=data;j[4]+=1
   if j[4]==j[3]:j[1]=2
   return 0,0
  if op!=1:return -8,0
  p=self.span(b,16)
  if p is None:return -3,0
  if self.busy:return 0,4
  raw=0;error=detail=amount=0
  if job:
   state=job[1];amount=(16 if state==2 else 0) if job[5] else job[4]*2048
   raw={1:1,2:2}.get(state,0xffffffff)
   if state not in (1,2):error=0 if state==3 else 8;detail=job[6]
   if state!=1:job[0]=job[1]=0
  write(p,[error,detail,amount,0]);return 0,raw
