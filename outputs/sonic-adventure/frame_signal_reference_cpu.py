"""Schedule only after the original frame-idle helper's RTS and delay slot."""
import filesystem_write_reference_cpu as old
code=old.code
marker='if runtime_boundary is not None and ((pc==0x8c65a6e8 and previous_pc==0x8c666e04) or (pc==0x8c65100a and previous_pc==0x8c650fd2 and getattr(mmio,\'runtime\',None) is not None and mmio.runtime.pending&(1<<19))):'
assert code.count(marker)==1
code=code.replace(marker,marker[:-2]+" or (pc in (0x8c642060,0x8c6420a0) and previous_pc==0x8c6416ee and getattr(mmio,'runtime',None) is not None)):")
namespace=dict(old.namespace);exec(compile(code,'frame_idle_original_cpu','exec'),namespace)
reference=namespace['reference']
