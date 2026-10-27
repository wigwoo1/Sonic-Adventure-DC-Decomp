"""Verification-only additional mandatory TAS purge in canonical preflight."""
import channel_request_reference_cpu as old
code=old.code;marker='if pc in (0x8c66049a,0x8c6604ec):';assert code.count(marker)==1
code=code.replace(marker,'if pc in (0x8c66049a,0x8c6604ec,0x8c65f1d2):')
namespace=dict(old.namespace);exec(compile(code,'input_pump_original_cpu','exec'),namespace)
reference=namespace['reference']
