"""Use the original authored-member gate for the corrected arming boundary."""
import hashlib,inspect,json,sys
from pathlib import Path
import package_native_vblank_chain as frozen
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    argv=sys.argv[1:];verification=Path(argv[argv.index('--verification')+1]);v=json.loads((verification/'verification.json').read_bytes());assert v['retired_Ninja_slot_reuse_fixed'] and v['retired_slot_reuse_checks']==7
    source=inspect.getsource(frozen.main)
    for old,new in [('v[\'checks\']==79096','v[\'checks\']==79103'),('verify_native_vblank_chain.py','verify_native_vblank_rearm.py'),('native/jp_native_vblank_chain.c','native/jp_native_vblank_rearm.c'),('jp_native_vblank_wait_support.lib','jp_native_vblank_rearm_support.lib')]:
        assert source.count(old)==1,(old,source.count(old));source=source.replace(old,new)
    frozen.__file__=__file__;exec(compile(source,__file__,'exec'),frozen.__dict__);frozen.main()
    out=Path(argv[argv.index('--output')+1])/'build_verification.json';result=json.loads(out.read_bytes());result.update(retired_Ninja_slot_reuse_fixed=True,package_tool_sha256=sha(__file__));out.write_text(json.dumps(result,indent=2)+'\n')
if __name__=='__main__':main()
