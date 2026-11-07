"""Diagnostic ownership for real event/weld rendering, not a player callback."""
import ctypes as C
import hashlib
import struct
from pathlib import Path
from native_packet_test_support import bind
from verify_native_action_poses import Action
from verify_native_character_tree import Node
from verify_native_model_preflight import Model
from verify_native_motion_store import View

P, I, U, F, Q = C.c_void_p, C.c_int32, C.c_uint32, C.c_float, C.c_uint64

class Weld(C.Structure):
    _fields_ = [('base',P),('object_a',P),('object_b',P),('count',C.c_uint8),
                ('kind',C.c_int8),('node_a',C.c_uint8),('node_b',C.c_uint8),
                ('vertices',P),('indices',P)]

class PlayerAction(C.Structure):
    _fields_ = [('action',P),('nodes',C.c_uint8),('mode',C.c_int8),
                ('next',C.c_uint16),('blend',F),('speed',F)]

class Owner:
    def __init__(self, f, motions, build, entries, task, action_view, binary, check):
        self.f, self.m, self.task, self.check = f, motions, task, check
        self.nl = nl = build['player_weld_recipe']['layout_values']['native']
        self.entries = entries
        self.initial = {e.name:C.string_at(e.data,e.host_bytes) for e in entries}
        self.allocate = bind(f,'jp_heap_allocate',P,I)
        self.free_event = bind(f,'event_free',None,P)
        self.update = bind(f,'player_update_welds',None,P,P,P)
        self.event_draw = bind(f,'event_motions_draw',None,P)
        self.stats = bind(f,'jp_heap_statistics',None,P,P)
        self.before = self.heap()
        self.player_work = (P*8).in_dll(f,'player_work')
        self.player_records = (P*8).in_dll(f,'player_records')
        self.old_work, self.old_records = list(self.player_work), list(self.player_records)
        assert not any(self.old_work) and not any(self.old_records)
        raw = Path(binary).read_bytes()
        assert hashlib.sha256(raw).hexdigest() == 'e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
        def at(address):
            if not address:return None
            matches = [e for e in entries if e.address<=address<e.address+e.original_bytes]
            assert len(matches)==1
            e=matches[0]; element,inner=divmod(address-e.address,e.original_element)
            assert inner==0
            return e.data+element*e.host_element
        def original(address,count):
            offset=address-0x8c010000
            assert 0<=offset<=len(raw)-count
            return raw[offset:offset+count]
        assert C.sizeof(Weld)==nl['weld.sizeof']==48
        assert C.sizeof(PlayerAction)==nl['player_action.sizeof']==24
        self.player=C.create_string_buffer(nl['player.sizeof'])
        self.welds=(Weld*33)()
        self.actions=(PlayerAction*131)()
        self.indices=[]
        self.mutable=[]
        for i in range(32):
            base,a,other,count,kind,na,nb,vertices,indices=struct.unpack(
                '<IIIBbBBII',original(0x8c1dc28c+24*i,24))
            assert vertices==0
            if at(base)!=Action.from_address(action_view.data).object:kind=0
            buf=C.create_string_buffer(original(indices,count*4)) if indices else None
            if buf:self.indices.append(buf)
            self.welds[i]=Weld(at(base),at(a),at(other),count,kind,na,nb,None,
                              C.addressof(buf) if buf else None)
            if kind in (1,2,3):
                words=struct.unpack('<'+'H'*(count*2),original(indices,count*4))
                for ptr,side in ((a,0),(other,1)):
                    model=Model.from_address(Node.from_address(at(ptr)).model)
                    for ix in words[side::2]:
                        assert ix<model.point_count
                        for arr in (model.points,model.normals):
                            lo=C.cast(arr,P).value+ix*12
                            self.mutable.append((lo,lo+12))
        self.stand=View()
        check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(
            7,b'sonic_stand_action',C.byref(self.stand))==0,'held genuine stand action for event previous entry')
        row=original(0x8c1db888,16)
        self.actions[0]=PlayerAction(self.stand.data,row[4],struct.unpack_from('<b',row,5)[0],
                                    struct.unpack_from('<H',row,6)[0],
                                    struct.unpack_from('<f',row,8)[0],struct.unpack_from('<f',row,12)[0])
        player=C.addressof(self.player)
        P.from_address(player+nl['player.anim.actions']).value=C.addressof(self.actions)
        P.from_address(player+nl['player.anim.welds']).value=C.addressof(self.welds)
        C.c_int16.from_address(player+nl['player.anim']+nl['player_anim.state']).value=3
        self.player_work[0]=task.work
        self.player_records[0]=player
        self.update(task.work,None,player)
        self.diagnostics=dict(live=sum(bool(w.vertices) for w in self.welds),
                              mode=C.c_int16.from_address(player+nl['player.anim.weld_mode']).value,
                              kinds=[w.kind for w in self.welds],
                              action_root=Action.from_address(action_view.data).object,
                              first_root=self.welds[0].base)
        check(sum(bool(w.vertices) for w in self.welds)==15 and
              C.c_int16.from_address(player+nl['player.anim.weld_mode']).value==1,
              ('original make allocates fifteen ordinary Sonic weld buffers and enters ready mode',self.diagnostics))
        self.current=action_view
        self.samples=[]
        self.reset()
        self.immutable_baseline=self.immutable_hash()

    def heap(self):
        total,largest=I(),I()
        self.stats(C.byref(total),C.byref(largest))
        return total.value,largest.value

    def immutable_hash(self):
        out=hashlib.sha256()
        for e in self.entries:
            raw=bytearray(C.string_at(e.data,e.host_bytes))
            for lo,hi in self.mutable:
                start=max(lo,e.data)-e.data;end=min(hi,e.data+e.host_bytes)-e.data
                if start<end:raw[start:end]=b'\x00'*(end-start)
            out.update(raw)
        return out.hexdigest()

    def reset(self, texlist=None):
        nl=self.nl
        self.free_event(self.task.work)
        self.event=self.allocate(nl['event_work.sizeof'])
        assert self.event
        C.memset(self.event,0,nl['event_work.sizeof'])
        P.from_address(self.task.work+nl['task_work.event']).value=self.event
        def entry(view,blend):
            ptr=self.allocate(nl['event_motion.sizeof']);assert ptr
            C.memset(ptr,0,nl['event_motion.sizeof'])
            C.c_uint8.from_address(ptr+nl['event_motion.blend']).value=blend
            C.memmove(ptr+nl['event_motion.action'],view.data,C.sizeof(Action))
            P.from_address(ptr+nl['event_motion.texlist']).value=texlist
            F.from_address(ptr+nl['event_motion.speed']).value=.5
            return ptr
        self.cur=entry(self.current,8)
        self.prev=entry(self.stand,0)
        P.from_address(self.event+nl['event_work.motions']).value=self.cur
        P.from_address(self.event+nl['event_work.previous']).value=self.prev
        self.step=0

    def draw(self):
        nl=self.nl
        before=self.heap()[0]
        self.event_draw(self.task.work)
        self.step+=1
        expected_frame=float(self.step) if self.step<8 else 0. if self.step==8 else .5
        previous=P.from_address(self.event+nl['event_work.previous']).value
        self.check(F.from_address(self.event+nl['event_work.motion_frame']).value==expected_frame
                   and previous==(self.prev if self.step<8 else None),
                   'real event blend advances and original callback retires previous entry on eighth draw')
        self.check(self.heap()[0]-before==(80 if self.step==8 else 0),
                   'actual heap accounts for previous event entry released by original routine')
        self.check(self.immutable_hash()==self.immutable_baseline,
                   'all canonical bytes except exact original weld vertex/normal destinations remain intact')
        self.samples.append(dict(step=self.step,frame=expected_frame,
                                 previous_live=bool(previous),free_bytes=self.heap()[0]))

    def close(self):
        C.c_int16.from_address(C.addressof(self.player)+self.nl['player.anim.weld_mode']).value=2
        self.update(self.task.work,None,C.addressof(self.player))
        self.check(all(not w.vertices for w in self.welds) and
                   all(C.string_at(e.data,e.host_bytes)==self.initial[e.name] for e in self.entries),
                   'original weld FREE restores all canonical records before Motion shutdown')
        self.free_event(self.task.work)
        self.player_work[:]=self.old_work
        self.player_records[:]=self.old_records
        self.check(self.heap()==self.before,'all event and weld heap allocations retire before task owner destruction')
        self.check(bind(self.m,'jp_motions_release',I,Q)(self.stand.token)==0,
                   'stand action released only after event and weld retirement')
