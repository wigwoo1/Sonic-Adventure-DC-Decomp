"""Observe mandatory TAS purges at the two newly recovered write locks.

The original integer/memory semantics are unchanged. Extend the existing
verification-only purge-site list; do not change prior CPU/proof files.
"""
import input_submission_reference_cpu as old
marker='if pc in (0x8c66049a,0x8c6604ec,0x8c65f1d2,0x8c65f02c):'
replacement='if pc in (0x8c66049a,0x8c6604ec,0x8c65f1d2,0x8c65f02c,0x8c66054c,0x8c6605b6):'
assert old.code.count(marker)==1
code=old.code.replace(marker,replacement)
assert code.replace(replacement,marker)==old.code
namespace=dict(old.namespace)
exec(compile(code,'write_request_original_cpu','exec'),namespace)
reference=namespace['reference']
