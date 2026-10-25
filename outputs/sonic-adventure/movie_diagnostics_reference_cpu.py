"""Extend the frozen verification decoder; production remains static native C."""
import sh4_widen_reference as model
import verify_movie_clear as base
code=base.cpu_code
marker='        elif word & 0xF0FF == 0xF02D:';assert code.count(marker)==1
code=code.replace(marker,"        elif word & 0xF0FF == 0xF0AD:\n            assert model.widen(fpu,fpu.fpul,n)==0,'Unprobed FCNVSD exception'\n"+marker)
marker="                from sh4_binary64 import execute_compare\n                condition=execute_compare(fpu,n,m,word&15==5)\n                sr=(sr&~1)|int(condition)"
assert code.count(marker)==1
code=code.replace(marker,"                rc,sr=model.compare(fpu,sr,n,m,word&15==5)\n                assert rc==0,'Unprobed double comparison exception'")
namespace=dict(base.cpu_ns,model=model);exec(compile(code,'diagnostic_reference_cpu','exec'),namespace)
reference=namespace['reference']
