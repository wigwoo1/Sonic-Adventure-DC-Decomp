"""Verification-only original CPU scheduling at the real SDK-poll return."""
import transfer_runtime_reference_cpu as old
code=old.code
marker='if runtime_boundary is not None and pc==0x8c65a6e8 and previous_pc==0x8c666e04:';assert code.count(marker)==1
condition="if runtime_boundary is not None and ((pc==0x8c65a6e8 and previous_pc==0x8c666e04) or (pc==0x8c65100a and previous_pc==0x8c650fd2 and getattr(mmio,'runtime',None) is not None and mmio.runtime.pending&(1<<19))):"
code=code.replace(marker,condition)
namespace=dict(old.namespace);exec(compile(code,'buffer_original_cpu','exec'),namespace)
reference=namespace['reference']
