"""Capture actual verified renderer backbuffers; no generated/retouched images.
PNG files contain the opaque window's displayed RGB channels. Retail-derived
screenshots stay under private work/, outside GitHub publication selection.
"""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,zlib
from pathlib import Path
import verify_native_boot_entry as boot
import verify_native_exit_logo as frozen
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def png(path,rgba,width,height):
    assert len(rgba)==width*height*4
    rgb=bytearray(width*height*3);rgb[0::3]=rgba[0::4];rgb[1::3]=rgba[1::4];rgb[2::3]=rgba[2::4]
    raw=b''.join(b'\0'+rgb[y*width*3:(y+1)*width*3] for y in range(height))
    def chunk(kind,data):return struct.pack('>I',len(data))+kind+data+struct.pack('>I',zlib.crc32(kind+data)&0xffffffff)
    path.write_bytes(b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>IIBBBBB',width,height,8,2,0,0,0))+chunk(b'IDAT',zlib.compress(raw,9))+chunk(b'IEND',b''))
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','inputs','asset-root','save-proof','pixel-dll','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    ap.add_argument('--capture-worker',action='store_true');a=ap.parse_args()
    for n in ['workspace','build','inputs','asset_root','save_proof','pixel_dll','output']:setattr(a,n,getattr(a,n).resolve())
    r,d,out=a.workspace,a.build,a.output;b=read(d/'build_verification.json')
    if not a.capture_worker:
        assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir();child=out/'run';child.mkdir()
        args=[sys.executable,'-B',str(Path(__file__).resolve())]
        for n in ['workspace','build','inputs','asset_root','save_proof','pixel_dll']:args+=['--'+n.replace('_','-'),str(getattr(a,n))]
        args+=['--output',str(child),'--capture-worker']
        run=subprocess.run(args,capture_output=True,creationflags=subprocess.CREATE_NO_WINDOW)
        (out/'stdout.txt').write_bytes(run.stdout);(out/'stderr.txt').write_bytes(run.stderr)
        assert run.returncode==0 and not run.stderr,(run.returncode,run.stderr.decode()[-6000:])
        result=read(child/'result.json');entry=read(child/'boot_entry.json');capture=read(child/'screenshots.json')
        assert result['checks']==1176 and result['actual_GPU_images']==result['actual_Presents']==190 and entry['checks']==9
        assert capture['readback_frames_observed']==190 and len(capture['screenshots'])==3
        for row in capture['screenshots']:assert sha(row['path'])==row['sha256']
        report=dict(status='actual_native_boot_test_window_screenshots_captured',checks=1185,selected_native_entry='boot_init',actual_GPU_images=190,actual_Presents=190,readback_frames_observed=190,screenshots=capture['screenshots'],native_rendering_functions_replaced=False,capture_uses_actual_window_backbuffers=True,opaque_window_RGB_channels_preserved=True,images_generated_by_AI=False,retail_derived_images_private=True,screenshots_published_to_GitHub=False,build_report_sha256=sha(d/'build_verification.json'),save_proof_sha256=sha(a.save_proof),pixel_helper_DLL_sha256=sha(a.pixel_dll),capture_script_sha256=sha(Path(__file__)),worker_result_sha256=sha(child/'result.json'),boot_entry_report_sha256=sha(child/'boot_entry.json'),capture_manifest_sha256=sha(child/'screenshots.json'),playable=False,overall_completion_percent=10)
        (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True);return
    frames=out/'frames';frames.mkdir();observed=0;shots=[];wanted={16:'fade-in',32:'logo',180:'fade-out'};original_bind=frozen.bind
    def observe(library,name,result,*types):
        native=original_bind(library,name,result,*types)
        if name!='jp_window_readback':return native
        def after_readback(window,output,count,failure):
            nonlocal observed
            rc=native(window,output,count,failure)
            if rc:return rc
            observed+=1
            if observed in wanted:
                width=C.c_uint32();height=C.c_uint32()
                assert original_bind(library,'jp_window_size',C.c_int,C.c_void_p,C.c_void_p,C.c_void_p)(window,C.byref(width),C.byref(height))==0
                assert count>=width.value*height.value*4
                path=frames/('frame-'+str(observed).zfill(3)+'-'+wanted[observed]+'.png')
                rgba=C.string_at(output,width.value*height.value*4);png(path,rgba,width.value,height.value)
                frame_lib=C.CDLL(b['frames_DLL']);level=bind(frame_lib,'jp_title_fade_level',C.c_int)()
                shots.append(dict(frame=observed,phase=wanted[observed],title_fade_level=level,width=width.value,height=height.value,path=str(path),sha256=sha(path),actual_RGBA_sha256=hashlib.sha256(rgba).hexdigest()))
            if observed in wanted or observed==190:(out/'screenshots.json').write_text(json.dumps(dict(readback_frames_observed=observed,screenshots=shots),indent=2)+'\n')
            return rc
        return after_readback
    frozen.bind=observe
    sys.argv=[boot.__file__]
    for n in ['workspace','build','inputs','asset_root','save_proof','pixel_dll','output']:sys.argv+=['--'+n.replace('_','-'),str(getattr(a,n))]
    sys.argv+=['--worker','movie0'];boot.main();raise AssertionError('native child exit returned')
if __name__=='__main__':main()
