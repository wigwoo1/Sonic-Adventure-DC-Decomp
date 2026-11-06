"""Actual native System/sound/joint fixtures shared by RNA qualification cases."""
import ctypes as C
from verify_native_audio import bind
from verify_native_aica import Options as AicaOptions
from verify_native_game_sound import Options as SoundOptions,Info as SoundInfo
from verify_native_system import Options as SystemOptions
from verify_native_device_init import Options as DeviceOptions
from verify_native_input_backend import Source,Sample
from verify_native_sound_clock import Options as ClockOptions
from verify_native_adx_joints import Chunk
P,I,U,Q,S=C.c_void_p,C.c_int32,C.c_uint32,C.c_uint64,C.c_size_t
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('initializations','shutdowns','service_calls')]+[(n,U) for n in ('active','stopping','live_renderers','pending_transfers','status','clock_error')]

class Runtime:
    def __init__(self,b,track,check,timed=False):
        self.b=b;self.check=check;self.keep=[];self.live=False;self.prereqs=False;self.timed=timed
        self.frames=C.CDLL(b['frames_DLL']);self.present=C.CDLL(b['present_DLL']);self.system=C.CDLL(b['System_DLL'])
        self.lib=C.CDLL(b['game_sound_DLL']);self.core=C.CDLL(str(__import__('pathlib').Path(b['game_sound_DLL']).parent/'jp_native_aica.dll'))
        self.g2=C.CDLL(str(__import__('pathlib').Path(b['game_sound_DLL']).parent/'jp_native_g2_audio.dll'))
        self.device=C.CDLL(b['device_DLL']);self.audio=C.CDLL(str(__import__('pathlib').Path(b['game_sound_DLL']).parent/'jp_native_audio.dll'))
        @Source
        def source(ctx,samples):
            C.memset(samples,0,4*C.sizeof(Sample));samples[0]=Sample(1,0,0,0,0,0,0,0);return 1
        self.keep.append(source)
        for name,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
            check(bind(self.frames,name,I,*([S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S]))(*args)==0,'actual render prerequisite '+name)
        self.prereqs=True;path=C.create_unicode_buffer(str(track.resolve()));self.keep.append(path)
        opts=SystemOptions(96,0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(path),2352,3,5000,0,C.cast(source,P).value,None,None,None)
        check(bind(self.system,'jp_system_configure',I,P)(C.byref(opts))==0 and bind(self.system,'jp_system_initialize',I,I,I,I)(1,0,1)==0,'real System/GDFS/wait/input')
        self.live=True
        sound=SoundOptions(56,0,2<<20,5000,AicaOptions(32,1 if timed else 0,16384,40,5000,1,1,0),None)
        if timed:
            clock=ClockOptions(56,441,8192,5,None,None,0,None,None)
            check(bind(self.system,'jp_system_start_sound_clock',I,P,P)(C.byref(sound),C.byref(clock))==0,'actual delegated timed sound owner')
        else:
            check(bind(self.lib,'jp_sound_configure',I,P)(C.byref(sound))==0 and bind(self.lib,'jp_sound_initialize',I)()==0,'original game startup and real supplied firmware')
            for _ in range(100):
                check(bind(self.lib,'jp_sound_service',I)()==0,'actual startup SDK service');self.render(441)
                q=self.peer('peer_commands',8);w=self.peer('peer_observe',20)
                if not any(q[:3]) and not w[7] and not w[8] and self.memory(w[14],1)==b'\0':break
            else:raise AssertionError('real startup did not drain')
        bind(self.lib,'SJRBF_Init',None)()
        check(bind(self.lib,'jp_rna_initialize',I)()==0,'RNA shares the live actual sound owner')
    def peer(self,name,n):
        kind=U if name=='peer_mwsnd_counts' else Q
        out=(kind*n)();self.check(bind(self.lib,name,I,P)(out)==0,'actual '+name);return list(out)
    def info(self):
        out=Info();self.check(bind(self.lib,'jp_rna_snapshot',I,P)(C.byref(out))==0,'actual RNA owner snapshot');return out
    def state(self,handle):
        out=(Q*41)();self.check(bind(self.lib,'peer_rna_state',I,P,P)(handle,out)==0,'actual complete middleware record');return list(out)
    def memory(self,at,n):
        out=C.create_string_buffer(n);self.check(bind(self.core,'jp_aica_read',I,U,P,U)(at,out,n)==0,'actual AICA RAM read');return out.raw
    def render(self,n):
        out=C.create_string_buffer(n*4);self.check(bind(self.lib,'jp_sound_render',I,U,P,Q)(n,out,len(out))==0,'actual shared firmware renders PCM');return out.raw
    def service(self):self.check(bind(self.lib,'jp_rna_service',I)()==0,'actual shared SDK/RNA service')
    def create(self,joints):
        array=(P*len(joints))(*joints);self.keep.append(array)
        handle=bind(self.lib,'mwRnaCreate',P,P,I)(array,len(joints))
        return handle
    def close(self):
        if self.live:
            if not self.timed:self.check(bind(self.lib,'jp_sound_shutdown',I,U)(5000)==0,'actual sound cleanup retires RNA before SDK/G2/core')
            self.check(bind(self.system,'jp_system_shutdown',I)()==0,'actual System delegated/global shutdown');self.live=False
            info=self.info();self.check(not info.active and not info.live_renderers and not info.pending_transfers,'no middleware records or copies survive global shutdown')
            self.check(bind(self.lib,'peer_sndbuf_used',I)()==0,'all actual sound-buffer handles released')
            self.check(self.peer('peer_mwsnd_counts',3)[0]==0,'all actual middleware sound ports released')
            out=SoundInfo();self.check(bind(self.lib,'jp_sound_snapshot',I,P)(C.byref(out))==0 and not out.resources,'all underlying sound resources released')
        if self.prereqs:
            bind(self.frames,'jp_frames_shutdown',I)();bind(self.frames,'jp_render_state_shutdown',None)();bind(self.frames,'jp_textures_shutdown',None)();bind(self.frames,'jp_queue_shutdown',None)();self.prereqs=False

class Joints:
    """One aligned caller-owned channel group; SJD requires contiguous rings."""
    def __init__(self,rt,nch,samples=1024):
        self.rt=rt;self.bytes=samples*2;self.extra=64;self.stride=self.bytes+self.extra;self.nch=nch
        self.backing=C.create_string_buffer(self.stride*nch+128);C.memset(self.backing,0xa5,len(self.backing))
        self.base=(C.addressof(self.backing)+63)&~31
        self.handles=[bind(rt.lib,'SJRBF_Create',P,P,I,I)(self.base+ch*self.stride,self.bytes,self.extra) for ch in range(nch)]
        rt.check(all(self.handles) and self.base>0xffffffff,'actual aligned native ring handles and contiguous decoder channels')
    def count(self,ch,line):return bind(self.rt.lib,'SJRBF_GetNumData',I,P,I)(self.handles[ch],line)
    def fill(self,ch,data):
        ck=Chunk();bind(self.rt.lib,'SJRBF_GetChunk',None,P,I,I,P)(self.handles[ch],0,len(data),C.byref(ck))
        self.rt.check(0<=ck.len<=len(data),'actual producer free chunk bounded')
        if ck.len:C.memmove(ck.data,data,ck.len)
        bind(self.rt.lib,'SJRBF_PutChunk',None,P,I,P)(self.handles[ch],1,C.byref(ck));return ck.len
    def guards(self):
        self.rt.check(C.string_at(self.base-16,16)==b'\xa5'*16 and C.string_at(self.base+self.stride*self.nch,16)==b'\xa5'*16,'whole native channel-group outer guards')
    def retired(self):
        for ch in range(self.nch):self.rt.check(self.count(ch,0)+self.count(ch,1)==self.bytes,'all borrowed input chunks returned before owner retirement')
        self.guards()
    def close(self):
        for h in self.handles:bind(self.rt.lib,'SJRBF_Destroy',None,P)(h)
