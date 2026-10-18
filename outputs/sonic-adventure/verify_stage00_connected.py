"""Execute the original task and main getter together versus their native C connection."""
import ctypes,hashlib,json,pathlib,struct
from verify_startup import State,reference,signed
from verify_stage00_task import Work,Task,Initialize
from verify_level_variant import Fields
ROOT=pathlib.Path(__file__).resolve().parents[2]
def main():
    root=ROOT/'outputs/sonic-adventure';dll_path=ROOT/'work/sonic-adventure/build/stage00_task.dll'
    dll=ctypes.CDLL(str(dll_path));native=dll.sa_stage00_update_with_fields
    native.argtypes=[ctypes.POINTER(Task),ctypes.POINTER(Fields),ctypes.c_void_p,Initialize];native.restype=None
    destructor_address=ctypes.cast(dll.sa_stage00_destructor_0c900d72,ctypes.c_void_p).value
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    overlay=(ROOT/'work/sonic-adventure/decoded/SONICADV/STG00.BIN').read_bytes()
    base=bytearray(16*1024*1024);base[0x10000:0x10000+len(binary)]=binary;base[0x900000:0x900000+len(overlay)]=overlay
    values=[0,1,127,255,256,32767,32768,65535]
    cases=[(state,0x1234,65535,7) for state in range(256)]
    cases.extend((state,first,second,cache) for state in range(4) for first in values for second in values for cache in [0,255,32768])
    count=0;total_steps=0
    for initial,first,second,cache in cases:
        work=Work(initial,(ctypes.c_uint8*5)(11,22,33,44,55),cache);task=Task(ctypes.pointer(work),0x12345678)
        fields=Fields(signed(first,16),signed(second,16));events=[];errors=[]
        @Initialize
        def initialize(context,pointer):
            if context!=0x1234 or ctypes.addressof(pointer.contents)!=ctypes.addressof(task):errors.append('Arguments differ')
            events.append('initialize')
        ram=bytearray(base);ram[0x880100:0x880108]=bytes(work)
        struct.pack_into('<I',ram,0x880020,0x8C880100);struct.pack_into('<I',ram,0x880018,0x12345678)
        struct.pack_into('<HH',ram,0x7493BA,first,second)
        state=State();state.r[:]=[0x12340000+i for i in range(16)];state.r[4]=0x8C880000;state.r[15]=0x8CF00000
        state.pr=0x8C000100;state.pc=0x0C900000;state.sr=0x700000F0;original_events=[]
        def initialize_hook(memory,regs,sr):
            assert regs[4]==0x8C880000;original_events.append('initialize');return sr
        # No hook for 8c04f7e0: its actual retail instructions execute here.
        total_steps+=reference(ram,state,{0x0C900D00:initialize_hook})
        before=bytes(fields);native(ctypes.byref(task),ctypes.byref(fields),0x1234,initialize)
        assert not errors and events==original_events
        assert bytes(work)==ram[0x880100:0x880108] and bytes(fields)==before
        expected=struct.unpack_from('<I',ram,0x880018)[0]
        assert task.destructor==(destructor_address if expected==0x0C900D72 else expected)
        assert state.r[13]==0x1234000D and state.r[14]==0x1234000E and state.r[15]==0x8CF00000 and state.pr==0x8C000100
        count+=1
    report={'routines':['STG00:0x0c900000','main:0x8c04f7e0'],'cases':count,'original_instruction_steps':total_steps,
            'task_fields_and_initializer_calls_match':True,'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'binary_sha256':hashlib.sha256(binary).hexdigest(),'overlay_sha256':hashlib.sha256(overlay).hexdigest(),
            'scope':'Original task/getter instructions versus their connected native implementations. Initialization body, complete engine task ABI and playable gameplay are not verified.'}
    (root/'analysis/stage00_connected_verification.json').write_text(json.dumps(report,indent=2))
    print(count,'connected STG00/getter comparisons passed')
if __name__=='__main__':main()
