"""Independent float-colour raster checks and actual native TA event ownership."""
import ctypes,hashlib,json,random,struct
from pathlib import Path
import float_color_pixels_native as native
import pvr_pixels_native as pixels
import pvr_pixels_float_reference as oracle
import frame_events_float_reference as events
import model_float_color_fixtures as scene
import render_completion_native as render
P=Path(__file__).resolve().parent;W=P.parents[1]/'work/sonic-adventure'
def bits(v):return int.from_bytes(struct.pack('<f',v),'little')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def triangle(header=0x808c0010,colour=(1.0,0.25,0.5,0.75)):
 words=[header,7<<29,(1<<29)|(1<<20),0,0,0,0,0]
 for i,(x,y) in enumerate(((1,1),(7,1),(1,7))):words += [0xf0000000 if i==2 else 0xe0000000,bits(x),bits(y),bits(1),*[bits(v) for v in colour]]
 return words
def main():
 target=P/'analysis/float_color_platform_verification.json';assert not target.exists();checks=0;draw_cases=0;event_cases=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 rng=random.Random(0x6111d6)
 for case in range(96):
  width,height=rng.randrange(8,20),rng.randrange(8,20)
  o=pixels.Options(width,height,8,8,case%2,0,0,128);header=0x808c0010|((case%3)*2<<24)|(case%2)*2
  words=triangle(header)
  for i in range(3):
   for channel in range(4):words[8+8*i+4+channel]=bits(rng.choice((0,1,0.5,1/255,0.999,rng.random())))
  vram=bytes(8) if o.textures else None
  rc,colour,depth,stats=pixels.draw(words,vram,o,lib=native.LIB);er,ec,ed,es=oracle.draw(words,vram,o)
  require((rc,colour,depth,stats)==(er,b''.join(struct.pack('<I',v) for v in ec),b''.join(struct.pack('<I',v) for v in ed),es),'complete rational float-colour image/depth/stat match')
  require(rc==0 and stats[3]==0,'untextured float vertices never sample textures');draw_cases+=1
 o=pixels.Options(8,8,8,8,0,0,0,128)
 for bad in (0x7fc00001,0x7f800000,0xff800000,bits(-0.01),bits(1.01)):
  for channel in range(4):
   words=triangle()+triangle()[8:];words[-8+4+channel]=bad
   initial=[0xff123456]*64;depth=[9]*64
   rc,col,dep,stats=pixels.draw(words,None,o,initial,depth,lib=native.LIB)
   require(rc==-3 and col==struct.pack('<64I',*initial) and dep==struct.pack('<64I',*depth),'invalid late float channel fails before any image mutation');draw_cases+=1
 for word in (0x808c0018,0x808c0050,0x808c0090,0x808c0014,0x808c0030):
  rc,col,dep,stats=pixels.draw(triangle(word),None,o,lib=native.LIB)
  require(rc==-2,'unsupported float texture/volume/offset/previous-intensity remains rejected');draw_cases+=1
 for fmt in (0,2):
  for gouraud in (0,2):
   words=triangle(0x808c0000|(fmt<<4)|gouraud)
   words[4:8]=[bits(v) for v in (1,0.5,0.75,0.25)]
   for i in range(3):words[8+8*i+6]=bits(0.5) if fmt==2 else 0xff4080c0
   require(pixels.draw(words,None,o,lib=native.LIB)==pixels.draw(words,None,o),'previous packed and intensity profile bytes/stats preserved');draw_cases+=1
 # The actual frame memory and new event C are used; no game results are supplied.
 game=scene.v.effects.LIB;Bus=render.Bus;EventState=events.state_type(Bus);events.declare(game,Bus,EventState)
 @render.READ
 def read(ctx,a,n,out):out[0]=0;return 0
 @render.WRITE
 def write(ctx,a,value,n):return 0
 @render.CACHE
 def cache(ctx,a,op):return 0
 parent=Bus(None,read,write,cache)
 for header in (0x808c0010,0x808c0012,0x828c0010,0x848c0010,0x80800000,0x80800020,0x808c0018,0x808c0050,0x808c0090,0x858c0010):
  memory=scene.fast.namespace['RtcMemory']();memory.frame.packet_capacity=64;state=EventState();out=Bus();pending=native.U()
  require(game.sa_frame_events_bus_init(ctypes.byref(state),ctypes.byref(memory.frame),ctypes.byref(parent),ctypes.byref(out),ctypes.byref(pending),None,1)==0,'event bus initializes')
  reference=events.Reference();packets=[];words=triangle(header);parts=[words[i:i+8] for i in range(0,len(words),8)]+[[0]*8]
  for index,w in enumerate(parts):
   memory.frame.packets[index].words[:]=w;memory.frame.packet_count=index+1;packets.append((0x10000000,0,w))
   rc=out.cache(out.context,0xe0000000,1);er=reference.consume(packets)
   require(rc==er and events.snapshot(state)==reference.snapshot() and pending.value==reference.pending,'complete event state and pending flags match independently')
   if rc:break
  else:require(state.ends==1 and state.vertices==3 and pending.value=={0:128,2:512,4:2097152}[(header>>24)&7], 'float strip produces the actual corresponding list event only after EOL')
  event_cases+=1
 files=[Path(__file__),P/'analysis/model_float_color_extension.json',P/'frame_events_float_reference.py',P/'pvr_pixels_float_reference.py',native.DLL,scene.v.effects.DLL]
 report=dict(status='float_ARGB32_platform_profile_verified',checks=checks,raster_cases=draw_cases,event_sequences=event_cases,random_raster_cases=96,late_invalid_channel_cases=20,previous_packed_intensity_regressions=4,complete_pixel_depth_stats_and_event_state_compared=True,unsupported_formats_still_rejected=True,texture_samples_in_new_untextured_profile=0,hardware_precision_qualified=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('FLOAT PLATFORM VERIFIED',checks,draw_cases,event_cases,flush=True)
if __name__=='__main__':main()
