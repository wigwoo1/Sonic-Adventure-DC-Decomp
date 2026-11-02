"""Host ABI declarations and callback failure propagation for task tests."""
import ctypes as C
import hashlib
import json
from pathlib import Path

class Task(C.Structure):
    _fields_ = [(n, C.c_void_p) for n in ['next','prev','parent','child','step','draw',
                                        'destructor','placement','work','motion','force','extra']]
class Vector(C.Structure):
    _fields_ = [(n, C.c_float) for n in ['x','y','z']]
class Angles(C.Structure):
    _fields_ = [(n, C.c_int32) for n in ['x','y','z']]
class Vars(C.Union):
    _fields_ = [('b', C.c_int8*12), ('w', C.c_int16*6), ('l', C.c_int32*3),
                ('f', C.c_float*3), ('p', C.c_void_p*3)]
class Work(C.Structure):
    _fields_ = [('mode',C.c_int8),('submode',C.c_int8),('id',C.c_uint8),('tick',C.c_uint8),
                ('flags',C.c_int16),('timer',C.c_int16),('vars',Vars),('ang',Angles),
                ('pos',Vector),('scl',Vector),('collision',C.c_void_p),('event',C.c_void_p)]
class Spec(C.Structure):
    _fields_ = [('step',C.c_void_p),('create',C.c_uint16),('flags',C.c_int16),
                ('ang',Angles),('pos',Vector),('var',C.c_int32)]
class Layout(C.Structure):
    _fields_ = [(n,C.c_uint32) for n in [
        'version','pointer_bytes','scalar_bytes','task_bytes','work_bytes','motion_bytes',
        'force_bytes','event_bytes','collision_bytes','event_motion_bytes','event_path_bytes',
        'task_spec_bytes','heap_header_bytes','work_vars','work_ang','work_pos','work_collision',
        'work_event','event_motions','event_previous','event_paths','event_motion_next',
        'event_path_next','collision_flags','collision_shapes']]
Matrix = C.c_float*16
Callback = C.CFUNCTYPE(None,C.c_void_p)

def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

class Runtime:
    def __init__(self, directory):
        self.directory = Path(directory).resolve()
        self.build = json.loads((self.directory/'build_verification.json').read_text())
        self.dll = Path(self.build['private_DLL'])
        assert sha(self.dll) == self.build['DLL_sha256']
        assert self.build['status'] == 'real_native_task_runtime_linked_behavior_unverified'
        self.lib = C.CDLL(str(self.dll))
        self.checks = 0
        self.callback_errors = []
        self.callbacks = []
        signatures = {
            'jp_task_runtime_get_layout':([C.POINTER(Layout)],None),
            'jp_heap_allocate':([C.c_int],C.c_void_p),
            'jp_heap_allocate_zeroed':([C.c_int,C.c_int],C.c_void_p),
            'jp_heap_release':([C.c_void_p],None),
            'jp_heap_statistics':([C.POINTER(C.c_int),C.POINTER(C.c_int)],None),
            'task_create':([C.c_uint16,C.c_int,C.c_void_p],C.c_void_p),
            'task_create_child':([C.c_uint16,C.c_void_p,C.c_void_p],C.c_void_p),
            'task_create_children':([C.POINTER(Spec),C.c_void_p],C.c_void_p),
            'task_get_current_root_step':([],C.c_void_p),
            'njInitMatrix':([C.c_void_p,C.c_int32,C.c_int32],None),
            'njSetMatrix':([C.c_void_p,C.c_void_p],None),
            'njGetMatrix':([C.c_void_p],None),
        }
        for name in ['task_destroy','task_kill','task_step_children','task_draw_children',
                     'task_destroy_children','collision_free','event_free']:
            signatures[name]=([C.c_void_p],None)
        for name in ['jp_heap_reset','task_init','task_step_all','task_draw_all','task_destroy_all']:
            signatures[name]=([],None)
        for name,(args,result) in signatures.items():
            fn=getattr(self.lib,name);fn.argtypes=args;fn.restype=result
        self.layout=Layout();self.lib.jp_task_runtime_get_layout(C.byref(self.layout))
        for native,expected in [('version',1),('pointer_bytes',C.sizeof(C.c_void_p)),
                ('scalar_bytes',4),('task_bytes',C.sizeof(Task)),('work_bytes',C.sizeof(Work)),
                ('task_spec_bytes',C.sizeof(Spec)),('heap_header_bytes',4*C.sizeof(C.c_void_p))]:
            self.verify(getattr(self.layout,native)==expected,'native ABI '+native)
        for field in ['vars','ang','pos','collision','event']:
            self.verify(getattr(self.layout,'work_'+field)==getattr(Work,field).offset,'work field '+field)
        self.matrix_stack=(Matrix*8)()
        self.lib.njInitMatrix(C.byref(self.matrix_stack),8,0)

    def verify(self,condition,message):
        assert condition,message
        self.checks+=1

    def call(self,name,*args):
        result=getattr(self.lib,name)(*args)
        assert not self.callback_errors, ('callback failure',self.callback_errors)
        return result

    def callback(self,fn):
        @Callback
        def guarded(address):
            try:
                fn(address)
            except BaseException as error:
                self.callback_errors.append(type(error).__name__+': '+str(error))
        self.callbacks.append(guarded)
        return C.cast(guarded,C.c_void_p).value

    def stats(self):
        total,largest=C.c_int(),C.c_int()
        self.lib.jp_heap_statistics(C.byref(total),C.byref(largest))
        return total.value,largest.value

    def fresh(self):
        self.call('jp_heap_reset');self.call('task_init')
        self.verify(self.stats()==(0x90000-(4096*C.sizeof(Task)+self.layout.heap_header_bytes),)*2,
                    'native task pool initialized within heap')
        return self.stats()

    def allocate(self,size):
        address=self.call('jp_heap_allocate_zeroed',1,size)
        self.verify(bool(address),'allocation succeeds')
        self.verify(C.string_at(address,size)==bytes(size),'calloc initializes all record bytes')
        return address

    def leave_heap_room(self,room):
        total,largest=self.stats()
        self.verify(total==largest and room%16==0 and room>=self.layout.heap_header_bytes,'coalesced test heap and valid aligned native header')
        filler=self.call('jp_heap_allocate',largest-room-self.layout.heap_header_bytes)
        self.verify(bool(filler) and self.stats()==(room,room),'controlled actual heap exhaustion')
        return filler

    def current_matrix(self):
        matrix=Matrix();self.lib.njGetMatrix(C.byref(matrix));return bytes(matrix)

def put_pointer(record,offset,value):
    C.c_void_p.from_address(record+offset).value=value
