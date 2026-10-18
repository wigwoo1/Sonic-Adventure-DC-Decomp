"""Original list cleanup instructions with bank-aware floating transfers."""
import ctypes,hashlib,json,struct,sys
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
Free=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_uint32)
def main(primary=False,combined=False):
    primary=primary or combined
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    dll_path=ROOT/'work/sonic-adventure/build/resource_list_release.dll'
    name='sa_resource_list_release_8c04b48c' if primary else 'sa_resource_list_release_8c04b82e'
    if combined:name='sa_secondary_resource_release_in_arena'
    function=getattr(ctypes.CDLL(str(dll_path)),name)
    function.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.c_uint32,ctypes.POINTER(Fpu),ctypes.c_void_p,Free];function.restype=ctypes.c_int
    base=0x8c880000;owner=64;resource=256;replacement=512
    nodes=[768+i*64 for i in range(6)];events=[];original=[];errors=[];count=0
    secondary_nodes=[1408,1472]
    head_field=64 if primary else 76;next_field=16 if primary else 12
    @Free
    def free(context,node):
        if context!=0x1234:errors.append('context')
        events.append(node)
        struct.pack_into('<I',arena,node+(12 if node in secondary_nodes else next_field),0x12345678)
        if mutate:struct.pack_into('<I',arena,owner+60,replacement)
        if toggle:native_fpu.fpscr^=0x100000
    def hook(memory,regs,sr):
        node=regs[4]-base;original.append(node)
        struct.pack_into('<I',memory,0x880000+node+(12 if node in secondary_nodes else next_field),0x12345678)
        if mutate:struct.pack_into('<I',memory,0x880000+owner+60,base+replacement)
        if toggle:expected_fpu.fpscr^=0x100000
        return sr
    for length,secondary_length in [(a,b) for a in range(7) for b in (range(3) if combined else [0])]:
     for active in range(2):
      for width in range(2):
       for mutate in [False,True]:
        for toggle in [False,True]:
         for extra in ([False,True] if primary else [False]):
          arena=bytearray([0xa5])*2048
          pointer_fields=[owner+60,resource+head_field,replacement+head_field]+[node+next_field for node in nodes]
          struct.pack_into('<I',arena,owner+60,resource)
          struct.pack_into('<I',arena,resource+head_field,nodes[0] if length else 0)
          struct.pack_into('<I',arena,replacement+head_field,0)
          if combined:
              pointer_fields.extend([resource+76,replacement+76]+[node+12 for node in secondary_nodes])
              secondary_head=secondary_nodes[0] if secondary_length else 0
              struct.pack_into('<I',arena,resource+76,secondary_head);struct.pack_into('<I',arena,replacement+76,secondary_head)
              for i,node in enumerate(secondary_nodes):struct.pack_into('<I',arena,node+12,secondary_nodes[i+1] if i+1<secondary_length else 0)
          if primary:
              pointer_fields.append(resource+68)
              struct.pack_into('<I',arena,resource+68,1152 if extra else 0)
          for i,node in enumerate(nodes):struct.pack_into('<I',arena,node+next_field,nodes[i+1] if i+1<length else 0)
          initial=bytearray(arena)
          for field in pointer_fields:
              value=struct.unpack_from('<I',initial,field)[0]
              if value:struct.pack_into('<I',initial,field,base+value)
          ram[0x880000:0x880800]=initial
          native_fpu=Fpu()
          for bank in range(2):native_fpu.banks[bank][:]=[0x10203040+bank*0x100+i for i in range(16)]
          native_fpu.fpscr=0x40001|(active<<21)|(width<<20)
          expected_fpu=Fpu.from_buffer_copy(native_fpu)
          state=State();state.r[:]=[0x12340000+i for i in range(16)]
          state.r[4]=base+owner;state.r[15]=0x8cf00000;state.pc=0x8c04b48c if primary else 0x8c04b82e;state.pr=0x8c000100;state.sr=0x700000f0
          if combined:state.pc=0x8c04b468
          saved=list(state.r[8:]);events.clear();original.clear()
          reference(ram,state,{0x8c098b42:hook},expected_fpu)
          buffer=(ctypes.c_uint8*len(arena)).from_buffer(arena)
          assert function(buffer,owner,ctypes.byref(native_fpu),0x1234,free)==1
          expected=bytearray(ram[0x880000:0x880800])
          for field in pointer_fields:
              value=struct.unpack_from('<I',expected,field)[0]
              if base<value<base+2048:struct.pack_into('<I',expected,field,value-base)
          label=(length,secondary_length,active,width,mutate,toggle,extra)
          assert arena==expected and bytes(native_fpu)==bytes(expected_fpu),(label,'memory/FPU')
          expected_events=nodes[:length]+([1152] if extra else [])
          if combined:
              expected_events.extend(secondary_nodes[:secondary_length])
              expected_events.append(replacement if mutate and (length or extra or secondary_length) else resource)
          assert events==original==expected_events,(label,'retained next/free order')
          assert list(state.r[8:])==saved and state.pr==0x8c000100
          count+=1
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':count,'routine_start':'0x8c04b48c' if primary else '0x8c04b82e','routine_end_exclusive':'0x8c04b4d2' if primary else '0x8c04b868',
            'retail_span_sha256':hashlib.sha256(binary[0x3b48c:0x3b4d2] if primary else binary[0x3b82e:0x3b868]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/resource_list_release.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'2048 arena bytes, both FPU banks, FPSCR, free sequence and saved integer registers versus original instructions with bank-aware transfer semantics and hooked heap free. Covers PR=0 with both FR/SZ modes, mode changes during free, resource replacement and scribbled next pointers. Entry FPSCR, undefined PR=1 behavior, hardware and gameplay remain unverified.'}
    filename='resource_list_primary_verification.json' if primary else 'resource_list_release_verification.json'
    if combined:
        filename='resource_cleanup_connected_verification.json'
        report['routine_start']='0x8c04b468';report['routine_end_exclusive']='0x8c04b48c'
        report['retail_span_sha256']=hashlib.sha256(binary[0x3b468:0x3b48c]).hexdigest()
        report['connected_helpers']=['0x8c04b48c','0x8c04b82e']
        report['additional_original_routines']=0
        report['first_list_lengths']=list(range(7));report['second_list_lengths']=list(range(3))
    (root/'analysis'/filename).write_text(json.dumps(report,indent=2))
    print(f'{count} bank-aware original cleanup comparisons passed')
if __name__=='__main__':main('--primary' in sys.argv,'--combined' in sys.argv)
