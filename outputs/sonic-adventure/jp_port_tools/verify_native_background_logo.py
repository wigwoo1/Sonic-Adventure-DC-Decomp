"""Frozen original-logo suite with the actual njSetBackColor startup call."""
import hashlib,json,sys
from pathlib import Path
import verify_native_logo_assets as original
def main():
    source=Path(original.__file__);raw=source.read_text();pin='d036ea53bac3d24f55fe3f7576057f9b1c8a3528bc54539e7d34ed55862904a4'
    assert hashlib.sha256(source.read_bytes()).hexdigest()==pin
    before='    limits=Limits(4<<20,16<<20,32<<20,64);'
    setup='''    from native_video_test_support import Context
    background=Context();background.face[:]=[1,1,1,1];background.words[:]=[0x80000002,0x80800000,0x20800000,0,0]
    check(bind(lib,'kmSetBackGroundRenderState',I,P)(C.byref(background))==0,'real startup background context')
    bind(lib,'njSetBackColor',None,U,U,U)(0,0,0)
'''
    changes={before:setup+before,'report.batches==6 and report.triangles==12':'report.batches==7 and report.triangles==14',"'all original pieces and twelve triangles use actual texture uploads'":"'actual background plus all six original logo pieces use retained texture uploads'",'unchanged_original_logo_composition_file_PRS_GPU_original_frame_wait_and_Present_verified':'original_logo_draw_with_original_back_color_background_GPU_frame_wait_and_Present_verified','original_logo_draw_body_unchanged=True,':'original_logo_draw_body_unchanged=True,original_njSetBackColor_executed=True,original_verifier_sha256='+repr(pin)+','}
    for a,b in changes.items():assert raw.count(a)==1,a;raw=raw.replace(a,b)
    # Only authored verifier code is transformed. Original game bodies and
    # metadata remain in the separately built private probe, unchanged.
    namespace={'__file__':__file__,'__name__':'qualified_background_logo_suite'};exec(compile(raw,str(source),'exec'),namespace);namespace['main']()
if __name__=='__main__':main()
