"""Actual queue configuration from Ninja word counts and bypass selectors."""
import argparse,ctypes as C,hashlib,json,random,subprocess
from pathlib import Path
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
class Lists(C.Structure):_fields_=[('model',C.c_uint32),('words',C.c_uint32*5),('minimum_bytes',C.c_size_t*5)]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.baseline/'verification.json').read_bytes())
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists();d.mkdir()
    assert sha(b['DLL'])==b['DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    owner=r/'outputs/sonic-adventure/native/jp_native_vertex_lists.c';compile=list(b['commands'][0]['command']);compile[compile.index('-c')+1]=str(owner);compile[-1]=str(d/'vertex_lists.obj')
    link=list(b['commands'][-1]['command']);old=next(n for n in link if Path(n).name=='queue_allocation.obj');kept={n:sha(n) for n in link if n.endswith('.obj') and n!=old};link[link.index(old)]=compile[-1];link[-1]=str(d/'jp_native_frames.dll');commands=[]
    for i,cmd in enumerate([compile,link]):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));assert run.returncode==0 and not log.stat().st_size,log.read_text()
    lib=C.CDLL(link[-1]);configure=bind(lib,'jp_vertex_lists_configure',C.c_int,*([C.c_int32]*5),C.POINTER(Lists));init=bind(lib,'jp_queue_initialize',C.c_int,C.c_size_t);stop=bind(lib,'jp_queue_shutdown',None);clear=bind(lib,'jp_queue_clear_error',None);cap=bind(lib,'jp_queue_capacity_bytes',C.c_size_t,C.c_uint32);data=bind(lib,'jp_queue_data',C.c_void_p,C.c_uint32);reserve=bind(lib,'jp_queue_reserve',C.c_int,C.c_uint32,C.c_uint32)
    checks=0;value=Lists();rng=random.Random(0x4c495354)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    check(configure(-1,0,131072,0,32768,C.byref(value))==7,'uninitialized owner rejected')
    cases=[(-1,0,131072,0,32768),(10,2,30,4,5),(10,2,-1,4,5),(10,2,30,4,-1)]
    cases += [tuple(rng.randrange(10000) for _ in range(5)) for _ in range(100)]
    for args in cases:
        check(init(16<<20)==0,'fresh real queue budget');op,om,tp,tm,pt=args;model=0
        if op<0:op=100;model=1
        elif tp<0 or pt<0:tp=100;pt=0;model=2
        words=[op,om,tp+pt,tm,0];minimum=[((words[i]*4+31)//32)*32 for i in [0,2,4,1,3]]
        check(configure(*args,C.byref(value))==0 and value.model==model and list(value.words)==words,'original effective list selection/merge')
        check(list(value.minimum_bytes)==minimum and [cap(i) for i in range(5)]==minimum,'actual host queue allocation and order')
        for i,n in enumerate(minimum):
            check((data(i) is None) if not n else data(i)>0xffffffff and data(i)%32==0,'native width/alignment')
        check(reserve(0,1024)==0 and cap(0)>=32768,'opaque bypass placeholder remains growable')
    check(init(1024)==0,'tight budget');check(configure(8,0,8,0,0,C.byref(value))==0,'initial configuration')
    saved=bytes(value);before=[(data(i),cap(i)) for i in range(5)]
    for args,error in [((0,0,0,0,0),1),((1,-1,1,0,0),1),((-1,0,-1,0,0),1),((2147483647,0,1,0,1),1),((-1,0,131072,0,32768),4)]:
        check(configure(*args,C.byref(value))==error,'invalid or over-budget configuration rejected');check(bytes(value)==saved and [(data(i),cap(i)) for i in range(5)]==before,'failure preserves caller output and all actual queues');clear()
    check(configure(1,0,1,0,0,None)==1,'null output rejected');clear();stop()
    for n,h in kept.items():assert sha(n)==h,n
    result=dict(status='actual_Ninja_selected_host_vertex_lists_verified',checks=checks,commands=commands,DLL=link[-1],DLL_sha256=sha(link[-1]),kept_objects_sha256=kept,baseline_proof_sha256=sha(a.baseline/'verification.json'),authored_source_sha256={str(p.relative_to(r)):sha(p) for p in [owner,owner.with_suffix('.h'),Path(__file__).resolve()]},native_functions_replaced=False,retail_inputs_used=False,artifact_publishable=False,njInitVertexBuffer_integrated=False,kmSetSystemConfiguration_integrated=False,full_system_boot_verified=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',flush=True)
if __name__=='__main__':main()
