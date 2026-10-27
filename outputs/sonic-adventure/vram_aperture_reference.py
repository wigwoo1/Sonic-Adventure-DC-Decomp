"""Independent byte/word address mapping for the added 64-bit VRAM aperture.

Primary evidence: Flycast core/hw/pvr/pvr_mem.cpp (pvr_map32), read alongside
core/hw/sh4/modules/dmac.cpp. No emulator implementation is imported or called.
Earlier backend storage is deliberately bank-linear, not Flycast's layout.
"""
import ctypes
U=ctypes.c_uint32
class Event(ctypes.Structure):_fields_=[('address',U),('mapped',U),('value',U),('operation',U)]

def inside(a):return not a&3 and 0x04000000<=a&0x1fffffff<0x04800000
def mapping(a):
 offset=a&0x7fffff
 return 0xa5000000|((offset&4)<<20)|((offset&0x7ffff8)>>1)
def snapshot(s):return (s.count,s.dropped,s.attempts,s.signature)
def state_type(Bus):
 class State(ctypes.Structure):_fields_=[('parent',Bus),('trace',ctypes.POINTER(Event)),('capacity',U),('count',U),('dropped',U),('attempts',U),('signature',U)]
 return State
def declare(lib,Bus,State):
 fn=lib.sa_vram_aperture_bus_init;fn.argtypes=[ctypes.POINTER(State),ctypes.POINTER(Bus),ctypes.POINTER(Bus),ctypes.POINTER(Event),U];fn.restype=ctypes.c_int

class Reference:
 def __init__(self,capacity):self.capacity=capacity;self.events=[];self.attempts=0;self.signature=2166136261
 def record(self,a,m,value,op):
  for word in (a,m,value,op):self.signature=((self.signature^word)*16777619)&0xffffffff
  self.events.append((a,m,value,op))
 def snapshot(self):return (min(len(self.events),self.capacity),max(0,len(self.events)-self.capacity),self.attempts,self.signature)
