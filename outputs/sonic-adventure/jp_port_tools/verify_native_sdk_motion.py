"""Compare real SDK interpolators with whole original SH-4 instruction calls."""
import argparse,ctypes as C,hashlib,json,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
U,I,P,F=C.c_uint32,C.c_int32,C.c_void_p,C.c_float
class Info(C.Structure):
    _fields_=[('mdata',P),('p',P),('nb',P),('frame',F),('frames',U),('factor',U),('matrix',P)]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','numeric-dll','output'):
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure'
    d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    b=read(a.build/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    pin=read(p/'jp_port_tools/jp_view_stack_external_inputs.json')
    assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    sys.path.insert(0,str(p));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    from model_reference import make_reference
    reference,reference_hashes=make_reference(executor)
    f=C.CDLL(b['frames_DLL'])
    info=Info.in_dll(f,'_nj_motion_data_info_');assert C.sizeof(Info)==48 and Info.frame.offset==24
    numeric=bind(C.CDLL(str(a.numeric_dll.resolve())),'sa_sh4_vector_numeric',I,U,P,U)
    def service(op,state):assert numeric(op,C.byref(state),state.fpul)==0
    symbols={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    initial=bytearray(16<<20);raw=a.binary.read_bytes();initial[0x10000:0x10000+len(raw)]=raw
    checks=0;records=[]
    def check(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    families=[('linear_vector','_nj_motion_int_f_func_table_',0,False),
              ('cubic_vector','_nj_motion_int_f_func_table_',1,False),
              ('linear_angles','_nj_motion_int_a_func_table_',0,True),
              ('cubic_angles','_nj_motion_int_a_func_table_',1,True),
              ('linear_angles_short','_nj_motion_int_a_func_table_old_',0,True)]
    frames=(0.,.5,4.,8.,12.25,16.,24.,31.)
    float_sets=[((1.,2.,3.),(-4.,5.,-.5),(9.,-8.,7.),(2.,3.,4.)),
                ((1e20,-1e-20,0.),(-1e20,2e-20,3.),(8e19,-4e-20,-5.),(0.,0.,7.))]
    angle_sets=[((65000,-1000,500),(100,22000,-500),(32767,-32000,65000),(60000,4000,0)),
                ((16777217,-16777217,2147483647),(-16777219,16777219,-2147483647),
                 (33554435,-33554435,123456789),(100000003,-100000003,-123456789))]
    function=C.CFUNCTYPE(None,P,U,U,P)
    for name,table,slot,is_angle in families:
        address=(P*2).in_dll(f,table)[slot];native=function(address)
        for dataset,values in enumerate(angle_sets if is_angle else float_sets):
            if name=='linear_angles_short' and dataset:continue
            keys=b''.join(struct.pack('<I',i*8)+struct.pack('<3i' if is_angle else '<3f',*v) for i,v in enumerate(values))
            storage=C.create_string_buffer(b'\xa5'*16+keys+b'\xa5'*16,len(keys)+32);kp=C.addressof(storage)+16
            for bank in (0,1):
                for frame in frames:
                    index=min(int(frame)//8,3);ram=bytearray(initial)
                    ram[0xd00000:0xd00000+len(keys)]=keys;ram[0xd00400:0xd00420]=b'\xa5'*32
                    struct.pack_into('<fI',ram,(symbols['_nj_motion_data_info_']&0xffffff)+12,frame,32)
                    fp=Fpu();fp.fpscr=0x40001|(bank<<21)
                    s=executor.State();s.pc=symbols[name];s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.sr=0x70000000
                    s.r[4]=0x8cd00000;s.r[5]=index;s.r[6]=4;s.r[7]=0x8cd00410
                    reference(ram,s,fpu=fp,vector_instruction=service,special={'gbr':0,'r_bank':[0]*8},instruction_budget=100000)
                    out=C.create_string_buffer(b'\xa5'*44,44)
                    info.frame=frame;info.frames=32
                    native(kp,index,4,C.addressof(out)+16)
                    expected=bytes(ram[0xd00410:0xd0041c]);actual=bytes(out)[16:28]
                    record=dict(function=name,dataset=dataset,bank=bank,frame=frame,
                                expected=expected.hex(),actual=actual.hex())
                    (d/'last_case.json').write_text(json.dumps(record,indent=2)+'\n')
                    check(s.r[15]==0x8cf00000,'whole original interpolator returns with original stack')
                    check(actual==expected,record)
                    check(bytes(out)[:16]==b'\xa5'*16 and bytes(out)[28:]==b'\xa5'*16,
                          'all native output guards intact')
                    check(bytes(storage)==b'\xa5'*16+keys+b'\xa5'*16,'all key bytes and guards unchanged')
                    records.append(record)
    # Constant material is an existing real SDK provider, not a new surrogate.
    material=(U*4)(0x3e800000,0x80000000,0x3f800000,0x3e19999a)
    bind(f,'njSetConstantMaterial',None,P)(material)
    check(bytes((U*4).in_dll(f,'_nj_constant_material_'))==bytes(material),'actual shared constant material words copied exactly')
    out=dict(status='sampled_original_Ninja_linear_cubic_vector_angle_and_short_angle_interpolators_match',
             checks=checks,records=records,original_complete_calls=len(records),
             both_SH4_banks=True,full_range_signed_angle_conversion_samples=True,
             short_angle_extreme_integer_overflow_cases_tested=False,
             original_game_function_hooks_used=False,shape_node_and_full_Simple_or_aura_rendering_verified=False,
             remaining_Simple_provider='njSimpleDrawModel',
             build_verification_sha256=sha(a.build/'build_verification.json'),
             verifier_sha256=sha(__file__),reference_hashes=reference_hashes,
             playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n')
    print(out['status'],checks,'checks',len(records),'whole original calls',flush=True)
if __name__=='__main__':main()
