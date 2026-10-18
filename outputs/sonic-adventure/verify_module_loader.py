"""Verify the recovered wrapper against original opcodes with external-call hooks."""
import ctypes, hashlib, json, pathlib
from verify_startup import State, reference, signed
from decompress_prs import decompress
from verify_prs_instructions import Stream
ROOT=pathlib.Path(__file__).resolve().parents[2]
Read=ctypes.CFUNCTYPE(ctypes.c_int32,ctypes.c_void_p,ctypes.c_char_p,ctypes.c_void_p)
Finish=ctypes.CFUNCTYPE(None,ctypes.c_void_p)
class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('read_file',Read),('finish_load',Finish)]
def main():
    root=ROOT/'outputs/sonic-adventure';dll_path=ROOT/'work/sonic-adventure/build/module_loader.dll'
    dll=ctypes.CDLL(str(dll_path));native=dll.sa_load_compressed_module_8c09da44
    native.argtypes=[ctypes.c_char_p,ctypes.c_void_p,ctypes.c_void_p,ctypes.POINTER(Services)];native.restype=ctypes.c_int32
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    samples=[('empty',Stream().finish()),('literal',bytes([0x17])+b'ABC\0\0'),('overlap',bytes([0x41])+b'A'+bytes([255,0,0]))]
    stream=Stream()
    for value in range(256):stream.literal(value)
    stream.long(1,256,True);samples.append(('extended',stream.finish()))
    results=[]
    for name,source in samples:
      for result in [0,1,7,2147483647,-1,-2147483648]:
        decoded,_=decompress(source);events=[];reference_events=[];finish_snapshots=[]
        scratch=ctypes.create_string_buffer(len(source));output=ctypes.create_string_buffer(len(decoded)+64)
        ctypes.memset(output,0xA5,len(output))
        @Read
        def read(context,filename,address):
            assert context==0x1234 and filename==b'module.PRS' and address==ctypes.addressof(scratch)
            events.append('read');ctypes.memmove(address,source,len(source));return result
        @Finish
        def finish(context):
            assert context==0x1234
            events.append('finish')
            finish_snapshots.append(output.raw[32:32+len(decoded)])
        services=Services(0x1234,read,finish)
        actual=native(b'module.PRS',scratch,ctypes.byref(output,32),ctypes.byref(services))
        ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
        ram[0x8F0000:0x8F000B]=b'module.PRS\0'
        ram[0x900000:0x900000+len(decoded)]=bytes([0xA5])*len(decoded)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=0x8C8F0000;state.r[5]=0x0C900000;state.r[15]=0x8CF00000
        state.sr=0x700000F0;state.pr=0x8C000100;state.pc=0x8C09DA44
        def file_hook(memory,regs,sr):
            assert regs[4]==0x8C8F0000 and regs[5]==0x0CD00000
            reference_events.append('read');memory[0xD00000:0xD00000+len(source)]=source
            regs[0]=result&0xffffffff;regs[4]=0xDEAD0004;regs[5]=0xDEAD0005
            return sr^1
        def decoder_hook(memory,regs,sr):
            assert regs[4]==0x0CD00000 and regs[5]==0x0C900000
            reference_events.append('decompress');memory[0x900000:0x900000+len(decoded)]=decoded
            regs[0]=len(decoded);regs[4]=0xBAD00004;regs[5]=0xBAD00005
            return sr^1
        def finish_hook(memory,regs,sr):
            reference_events.append('finish');regs[0]=0xFEEDC0DE;return sr^1
        steps=reference(ram,state,{0x8C09CC80:file_hook,0x8C04C5E0:decoder_hook,0xAC5FC0C2:finish_hook})
        expected_events=['read']+(['decompress'] if result>=0 else [])+['finish']
        assert reference_events==expected_events and events==['read','finish']
        assert actual==signed(state.r[0],32)==result,'File result was not preserved'
        assert state.r[14]==0x1234000E and state.r[15]==0x8CF00000 and state.pr==0x8C000100,'Original saved state differs'
        assert output.raw[32:32+len(decoded)]==ram[0x900000:0x900000+len(decoded)],'Loader output differs'
        assert finish_snapshots==[bytes(ram[0x900000:0x900000+len(decoded)])],'Synchronization preceded the decoded writes'
        assert output.raw[:32]==bytes([0xA5])*32 and output.raw[-32:]==bytes([0xA5])*32,'Guard differs'
        results.append({'payload':name,'file_result':result,'original_wrapper_steps':steps,'return_and_output_match':True,'original_call_order':reference_events})
    report={'routine':'0x8c09da44','cases':results,
            'source_sha256':hashlib.sha256((root/'source/module_loader.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Original wrapper instructions with file/decompression/synchronization hooks; native wrapper uses recovered PRS routine. External file-I/O and synchronization bodies are not verified here.'}
    (root/'analysis/module_loader_verification.json').write_text(json.dumps(report,indent=2))
    print(len(results),'native/original loader cases passed')
if __name__=='__main__':main()
