"""Verification-only scheduling at the real idle-helper return boundary."""
import interrupt_context_reference_cpu as old
code=old.code
code=code.replace('cache_prefetch=None):','cache_prefetch=None,runtime_boundary=None):')
marker='    while pc != return_pc:';assert code.count(marker)==1
code=code.replace(marker,'''    previous_pc=None
'''+marker+'''
        if runtime_boundary is not None and pc==0x8c65a6e8 and previous_pc==0x8c666e04:
            sr,pc,state.pr=runtime_boundary(ram,regs,sr,pc,state.pr,fpu,special)
        previous_pc=pc''')
namespace=dict(old.namespace);exec(compile(code,'runtime_original_cpu','exec'),namespace)
reference=namespace['reference']
