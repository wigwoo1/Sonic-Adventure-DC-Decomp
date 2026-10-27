"""Verification-only mandatory SH-4 TAS purge in the coherent RAM domain."""
import buffer_transfer_reference_cpu as old
code=old.code;marker='        elif word & 0xF0FF == 0x401B:';assert code.count(marker)==1
code=code.replace(marker,marker+"\n            if pc in (0x8c66049a,0x8c6604ec):\n                assert cache_purge is not None,'TAS needs explicit coherent purge service'\n                cache_purge(regs[n])")
namespace=dict(old.namespace);exec(compile(code,'request_original_cpu','exec'),namespace)
reference=namespace['reference']
