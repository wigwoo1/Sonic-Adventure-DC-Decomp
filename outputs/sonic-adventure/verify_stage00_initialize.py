"""Compare initializer work records and direct calls against retail SH-4 execution."""
import ctypes,hashlib,json,pathlib,struct
from verify_startup import State,reference
from verify_stage00_task import Work,Task
from verify_fpu_cleanup_transfer import Fpu

ROOT=pathlib.Path(__file__).resolve().parents[2]
U32=ctypes.c_uint32
Query=ctypes.CFUNCTYPE(U32,ctypes.c_void_p)
Call2=ctypes.CFUNCTYPE(None,ctypes.c_void_p,U32,U32)
Call3=ctypes.CFUNCTYPE(None,ctypes.c_void_p,U32,U32,U32)
Call4=ctypes.CFUNCTYPE(None,ctypes.c_void_p,U32,U32,U32,U32)
Create=ctypes.CFUNCTYPE(ctypes.c_void_p,ctypes.c_void_p,U32,U32,U32)
Lookup=ctypes.CFUNCTYPE(U32,ctypes.c_void_p,U32,U32)
NoArgs=ctypes.CFUNCTYPE(None,ctypes.c_void_p)

class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('query_variant',Query),
      ('call_8c07c65e',Call2),('call_8c07c676',Call4),
      ('call_8c60526a',Call3),('call_8c09852e',Create),
      ('call_8c014242',Lookup),('call_8c014308',Lookup),
      ('call_8c08a07c',NoArgs),('call_8c04c680',NoArgs)]

def main():
    root=ROOT/'outputs/sonic-adventure'
    overlay=(ROOT/'work/sonic-adventure/decoded/SONICADV/STG00.BIN').read_bytes()
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    source=root/'source/stage00_initialize.c'
    dll_path=ROOT/'work/sonic-adventure/build/stage00_initialize.dll'
    native=ctypes.CDLL(str(dll_path)).sa_stage00_initialize_0c900d00
    native.argtypes=[ctypes.POINTER(Task),ctypes.POINTER(Services)];native.restype=None
    cases=[];total_steps=0
    for query_result in [0,1,0x76,0x123456ff,0x80000000,0xffffffff]:
        task_work=Work(0x81,(ctypes.c_uint8*5)(9,8,7,6,5),0x4321)
        task=Task(ctypes.pointer(task_work),0x12345678)
        allocated_work=Work(0x55,(ctypes.c_uint8*5)(1,3,5,7,9),0x9876)
        allocated_task=Task(ctypes.pointer(allocated_work),0xabcdef01)
        events=[];ctx=ctypes.c_void_p(0x1234)
        @Query
        def query(_):events.append(('query_variant',));return query_result
        @Call2
        def call65e(_,r4,r5):events.append(('8c07c65e',r4,r5))
        @Call4
        def call676(_,r4,fr4,fr5,fr6):events.append(('8c07c676',r4,fr4,fr5,fr6))
        @Call3
        def call605(_,r4,r5,r6):events.append(('8c60526a',r4,r5,r6))
        @Create
        def create(_,r4,r5,r6):events.append(('8c09852e',r4,r5,r6));return ctypes.addressof(allocated_task)
        @Lookup
        def call242(_,r4,r5):events.append(('8c014242',r4,r5));return 1
        @Lookup
        def call308(_,r4,r5):events.append(('8c014308',r4,r5));return 1
        @NoArgs
        def call07c(_,):events.append(('8c08a07c',))
        @NoArgs
        def call04c(_,):events.append(('8c04c680',))
        services=Services(ctx,query,call65e,call676,call605,create,call242,call308,call07c,call04c)

        ram=bytearray(16*1024*1024)
        ram[0x10000:0x10000+len(binary)]=binary
        ram[0x900000:0x900000+len(overlay)]=overlay
        struct.pack_into('<I',ram,0x880000+0x20,0x8c880100)
        ram[0x880100:0x880108]=bytes(task_work)
        struct.pack_into('<I',ram,0x880200+0x20,0x8c880300)
        ram[0x880300:0x880308]=bytes(allocated_work)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=0x8c880000;state.r[15]=0x8cf00000
        stop_pc=0x8c000100;state.pr=stop_pc;state.pc=0x0c900d00;state.sr=0x700000f0
        fpu=Fpu();fpu.fpscr=0
        for bank in range(2):fpu.banks[bank][:]=[(0x10203040*(bank+1)+i)&0xffffffff for i in range(16)]
        initial_fr15=fpu.banks[0][15]
        original_events=[]
        def observe(label,values=()):
            def invoke(memory,regs,sr):
                arguments=values(regs) if callable(values) else values
                original_events.append((label,*arguments))
                return sr
            return invoke
        def query_hook(memory,regs,sr):
            original_events.append(('query_variant',));regs[0]=query_result;return sr
        def configure_hook(memory,regs,sr):
            bank=(fpu.fpscr>>21)&1
            original_events.append(('8c07c676',regs[4],fpu.banks[bank][4],fpu.banks[bank][5],fpu.banks[bank][6]))
            return sr
        def create_hook(memory,regs,sr):
            original_events.append(('8c09852e',regs[4],regs[5],regs[6]));regs[0]=0x8c880200;return sr
        hooks={0x8c04f7e0:query_hook,
          0x8c07c65e:observe('8c07c65e',lambda r:(r[4],r[5])),
          0x8c07c676:configure_hook,
          0x8c60526a:observe('8c60526a',lambda r:(r[4],r[5],r[6])),
          0x8c09852e:create_hook,
          0x8c014242:observe('8c014242',lambda r:(r[4],r[5])),
          0x8c014308:observe('8c014308',lambda r:(r[4],r[5])),
          0x8c08a07c:observe('8c08a07c'),
          0x8c04c680:observe('8c04c680')}
        steps=reference(ram,state,hooks,fpu=fpu,stop_pc=stop_pc);total_steps+=steps
        expected_work=Work.from_buffer_copy(ram[0x880100:0x880108])
        expected_allocated=Work.from_buffer_copy(ram[0x880300:0x880308])
        native(ctypes.byref(task),ctypes.byref(services))
        assert events==original_events,(events,original_events)
        assert bytes(task_work)==bytes(expected_work)
        assert bytes(allocated_work)==bytes(expected_allocated)
        assert [fpu.banks[0][i] for i in (4,5,6)]==[0,0,0]
        assert fpu.banks[0][15]==initial_fr15
        expected=[('query_variant',),('8c07c65e',0,0x18),
          ('8c07c676',0,0,0,0),('8c60526a',0,0,0),
          ('8c09852e',2,0,0x0c900d76),('8c014242',0x20,2),
          ('8c014308',0x20,2),('8c08a07c',),('8c04c680',)]
        assert events==expected,events
        cases.append({'query_result':query_result,'direct_calls_and_arguments_match':True,
          'caller_and_created_work_records_match':True,'executed_original_instructions':steps})
    report={'module':'STG00.BIN','routine':'0x0c900d00','end_exclusive':'0x0c900d72',
      'cases_passed':len(cases),'original_instructions_executed':total_steps,'cases':cases,
      'decoded_module_sha256':hashlib.sha256(overlay).hexdigest(),
      'retail_span_sha256':hashlib.sha256(overlay[0xd00:0xd72]).hexdigest(),
      'source':'source/stage00_initialize.c',
      'instruction_listing':'analysis/instructions/0c900d00.tsv',
      'instruction_listing_sha256':hashlib.sha256((root/'analysis/instructions/0c900d00.tsv').read_bytes()).hexdigest(),
      'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
      'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
      'scope':'Six getter return patterns compare direct call order and observed arguments, caller and newly created work records, and FPU zero arguments against original SH-4 execution. External routines are call-boundary hooks. Their service behavior, full volatile register effects, and playable stage behavior remain outside scope.',
      'limitations':['External service implementations remain unresolved.','The host C interface does not emulate all SH-4 register/FPU side effects.','The final main routine 0x8c04c680 is a call-boundary hook.','Stage runtime and whole-game playability are unverified.']}
    (root/'analysis/stage00_initialize_recovery.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f"{len(cases)} cases passed, {total_steps} original instructions executed")
if __name__=='__main__':main()
