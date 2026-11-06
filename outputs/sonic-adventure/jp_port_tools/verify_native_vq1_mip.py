"""Qualify the primary-backed VQ1x1 path and unchanged prior decoder paths."""
import argparse,ctypes as C,faulthandler,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
from native_packet_test_support import bind
from verify_native_texels import Level,expanded
I,U,P,S=C.c_int32,C.c_uint32,C.c_void_p,C.c_size_t
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','primary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure';b=read(a.build/'build_verification.json');primary=read(a.primary/'primary.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['GPU_DLL'])==b['GPU_DLL_sha256'] and sha(a.primary/'TexCache.cpp')==primary['source_sha256']
    assert primary['commit']=='7adbf3caef251b153e348df3f469a0f35878339d' and primary['factual_contract']==dict(VQ_1x1_index_offset=2048,codebook_corner=3,corner_name='bottom right')
    fault=(d/'native_fault.txt').open('w');faulthandler.enable(file=fault,all_threads=True)
    lib=C.CDLL(b['GPU_DLL']);describe=bind(lib,'jp_texel_describe',I,U,U,U,U,P);decode=bind(lib,'jp_texel_decode',I,U,U,U,U,P,S,P,S,P,S,P);checks=cases=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    source=(C.c_ubyte*2050)();out=(C.c_ubyte*12)();before=bytes(source)
    for fmt in range(3):
        for log in range(3,11):
            tsp=((log-3)<<3)|(log-3);tcw=0xc0000000|(fmt<<27)
            for code in range(256):
                for corner in range(4):struct.pack_into('<H',source,code*8+corner*2,(code*313+corner*7919+log*43+fmt*67)&65535)
                source[2048]=code;source[2049]=(code+37)&255;metadata=Level()
                check(describe(tsp,tcw,log,0,C.byref(metadata))==0 and (metadata.width,metadata.height,metadata.format,metadata.mip_count,metadata.encoded_offset,metadata.required_source_bytes,metadata.rgba_bytes)==(1,1,fmt,log+1,2048,2049,4),'all original sides/formats/code indices use exact1x1 bounds')
                C.memset(out,0x5a,len(out));result=Level();check(decode(tsp,tcw,log,0,source,2049,None,0,C.addressof(out)+4,4,C.byref(result))==0,'actual bounded VQ1x1 decode')
                want=expanded((code*313+3*7919+log*43+fmt*67)&65535,fmt)
                check(bytes(out[:4])==bytes(out[8:])==b'Z'*4 and bytes(out[4:8])==want and bytes(result)==bytes(metadata),'primary-backed bottom-right corner and first index; guards unchanged');cases+=1
    # Preserve output/metadata on all short/invalid/overlapping requests.
    for source_ptr,size,output_ptr,capacity,result_ptr,want in [(source,2048,C.addressof(out)+4,4,C.byref(Level()),3),(source,2049,C.addressof(out)+4,3,C.byref(Level()),4),(None,2049,C.addressof(out)+4,4,C.byref(Level()),1),(source,2049,None,4,C.byref(Level()),1),(source,2049,C.addressof(out)+4,4,None,1),(source,2049,source,4,C.byref(Level()),1)]:
        C.memset(out,0x5a,len(out));old=bytes(source);check(decode(0,0xc8000000,3,0,source_ptr,size,None,0,output_ptr,capacity,result_ptr)==want and bytes(out)==b'Z'*len(out) and bytes(source)==old,'invalid VQ1x1 bounds/alias rejected before writes')
    metadata=Level();C.memset(C.byref(metadata),0x5a,C.sizeof(metadata));old=bytes(metadata)
    check(decode(0,0xc8000000,3,0,source,2049,None,0,C.byref(metadata),4,C.byref(metadata))==1 and bytes(metadata)==old,'output/metadata overlap rejects atomically')
    # Guard the byte immediately after the EXACT required2049 bytes using a
    # real inaccessible page. Reading the neighbouring2x2 index would fault.
    kernel=C.WinDLL('kernel32',use_last_error=True);alloc=bind(kernel,'VirtualAlloc',P,P,S,U,U);protect=bind(kernel,'VirtualProtect',I,P,S,U,P);free=bind(kernel,'VirtualFree',I,P,S,U)
    block=alloc(None,8192,0x3000,4);check(bool(block),'actual two-page guard allocation');previous=U();check(protect(block+4096,4096,1,C.byref(previous))!=0,'actual source-end no-access page')
    at=block+4096-2049;C.memmove(at,source,2049);check(decode(0,0xc8000000,3,0,at,2049,None,0,C.addressof(out)+4,4,C.byref(Level()))==0,'exact input end succeeds without reading neighbouring index byte');check(free(block,0,0x8000)!=0,'real guard allocation released')
    # Full frozen regression differs only by removing the one former
    # unsupported-case expectation which this milestone deliberately fulfills.
    view=d/'legacy_view';view.mkdir();vb=dict(b,DLL=b['GPU_DLL'],DLL_sha256=b['GPU_DLL_sha256'],exit_code=0,diagnostic_bytes=0);(view/'build_verification.json').write_text(json.dumps(vb,indent=2)+'\n')
    original=p/'jp_port_tools/verify_native_texels.py';raw=original.read_bytes();target=b'(0,0xc8000000,3,0,2),';assert raw.count(target)==1
    caller=d/'private_prior_texel_caller.py';caller.write_bytes(raw.replace(target,b''));report=d/'legacy_regression.json'
    run=subprocess.run([sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(view),'--report',str(report)],capture_output=True,env=dict(os.environ,PYTHONPATH=str(p/'jp_port_tools')),timeout=180,creationflags=subprocess.CREATE_NO_WINDOW)
    (d/'stdout.txt').write_bytes(run.stdout);(d/'stderr.txt').write_bytes(run.stderr);assert not run.returncode,(run.returncode,run.stderr.decode(errors='replace')[-6000:]);regression=read(report)
    faulthandler.disable();fault.close();out=dict(status='primary_backed_VQ_1x1_all_sides_formats_and_indices_bounds_guard_and_prior_texel_regression_verified',checks=checks,new_native_cases=cases,
        selected_prior_regression_checks=regression['checks'],prior_verifier_sha256=sha(original),derived_caller_sha256=sha(caller),prior_regression_sha256=sha(report),
        primary_reference=primary,primary_metadata_sha256=sha(a.primary/'primary.json'),native_corner=3,index_offset=2048,required_source_bytes=2049,actual_source_end_guard_page_verified=True,
        build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),original_source_and_proofs_preserved=True,console_hardware_comparison=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'new checks;',regression['checks'],'prior checks',flush=True)
if __name__=='__main__':main()
