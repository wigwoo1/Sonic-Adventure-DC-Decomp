"""Explicit coherent SDK/response inputs; originals own publication and completion."""
import types
import movie_channel_request_fixtures as previous
import movie_channel_request_task_fixtures as task_previous
import write_watch_input_pump as fast
v=fast.old;put=previous.put;CTX=previous.CTX;FRAME=previous.FRAME;LOCK=previous.LOCK;CONFIGS=previous.CONFIGS
FLAG_OBJECT=0x8cc7d000;INFO_OUT=0x8cc60000
_setup=types.FunctionType(previous.setup.__code__,dict(previous.setup.__globals__,v=v,fast=fast),'setup',previous.setup.__defaults__)
def setup(*args,**kwargs):
 c,m,s,f=_setup(*args,**kwargs)
 c['scan_samples']=[0x1122]
 return c,m,s,f
configure_packet=previous.configure
call=types.FunctionType(task_previous.call.__code__,dict(task_previous.call.__globals__,v=v,fast=fast),'call',task_previous.call.__defaults__)

def configure(m,context=CTX,status=0,device_flags=0,lock=0,active=0):
 previous.configure(m,1,lock=lock,context=context)
 put(m,context+0x10f4,FLAG_OBJECT);put(m,FLAG_OBJECT+0x18,device_flags)
 put(m,context+0xfc0,status);put(m,context+0xfb0,0);put(m,context+0xfbc,active)
 put(m,0x8c893e48,0);put(m,0x8c893e4c,0);put(m,0x8c893e18,INFO_OUT)
 m[0x893e50:0x893e50+48*12]=bytes(48*12)
 # Context's per-port presence bytes and status table start empty. Raw RX bytes
 # are device-side fixtures; no completed token or game result is fabricated.
 m[(context+0xf90)&0xffffff:(context+0xf90+24)&0xffffff]=bytes(24)
 return dict(context=context,flag_object=FLAG_OBJECT,lock=LOCK)

def seed_response(m,unit,status=8,present=1,command=1,token=0,order=0,buffer_bank=0,context=CTX,port=0):
 record=context+0x28+unit*44
 put(m,context+0x10c4,order);put(m,context+0x10b4,buffer_bank)
 pointer=v.load(m,context+0xfe8+(1-buffer_bank)*96+unit*4)
 for off,value in ((0,present),(4,9),(7,command),(12,1),(13,1)):put(m,record+off,value,1)
 put(m,record+0x1c,token);put(m,record+0x20,0)
 header=bytes([status,0x20,1,3]);m[pointer&0xffffff:(pointer&0xffffff)+4]=header[::-1] if order else header
 m[(pointer+4)&0xffffff:(pointer&0xffffff)+260]=bytes(range(256))
 return dict(record=record,response=pointer,unit=unit,port=port,status=status,token=token)
