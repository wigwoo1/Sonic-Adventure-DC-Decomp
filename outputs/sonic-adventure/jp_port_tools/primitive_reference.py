"""Explicit PREF observation using the frozen scalar/queue instruction model.

No game dependency hooks, cache timing, GPU execution or arbitration model.
The two instruction extensions already passed 1,024 queue executor checks.
"""
import hashlib,inspect

def exceptional_binary(fpu,n,m,operation):
    """SH-4 manual Rev.6 sections 9.26/9.30/9.39/9.45, single, traps disabled.
    Class results only; normal arithmetic remains the historical integer core.
    No physical-hardware measurements are implied by this extension.
    """
    assert fpu.fpscr&0x80f80==0 and fpu.fpscr&0x40003==0x40001
    bank=(fpu.fpscr>>21)&1;a=fpu.banks[bank][n];b=fpu.banks[bank][m]
    aa=a&0x7fffffff;bb=b&0x7fffffff
    if aa<0x800000:aa=0
    if bb<0x800000:bb=0
    assert max(aa,bb)>=0x7f800000
    sign=(a^b)&0x80000000;invalid=False
    if aa>0x7f800000 or bb>0x7f800000:
        result=0x7fbfffff;invalid=any((v&0x7fffffff)>0x7f800000 and v&0x400000 for v in (a,b))
    elif operation in ('add','subtract'):
        right=b^(0x80000000 if operation=='subtract' else 0)
        invalid=aa==bb==0x7f800000 and bool((a^right)&0x80000000)
        result=0x7fbfffff if invalid else a if aa==0x7f800000 else right
    elif operation=='divide':
        invalid=aa==bb==0x7f800000
        result=0x7fbfffff if invalid else sign|(0x7f800000 if aa==0x7f800000 else 0)
    else:
        invalid=not aa or not bb
        result=0x7fbfffff if invalid else sign|0x7f800000
    fpu.banks[bank][n]=result
    fpu.fpscr=(fpu.fpscr&~0x3f000)|(0x10040 if invalid else 0)

def make_reference(executor):
    original=inspect.getsource(executor.reference)
    signature='cache_purge=None, trace=None):'
    anchor='        elif word & 0xF0FF == 0x00B3:'
    store="            struct.pack_into('<I', ram, index(regs[n]), regs[m])"
    assert original.count(signature)==original.count(anchor)==original.count(store)==1
    extended=original.replace(signature,'cache_purge=None, trace=None, cache_prefetch=None):')
    extended=extended.replace(anchor,
        "        elif word & 0xF0FF == 0x0083:\n"
        "            assert cache_prefetch is not None, 'PREF requires explicit service'\n"
        "            cache_prefetch(regs[n])\n"+anchor)
    extended=extended.replace(store,'            write32(regs[n],regs[m])')
    for call,args,operation in [('execute_multiply','',"'multiply'"),('execute_divide','',"'divide'"),('execute_add_sub',',bool(word&1)',"'subtract' if word&1 else 'add'")]:
        statement=f'            {call}(fpu,n,m{args})'
        assert extended.count(statement)==1
        extended=extended.replace(statement,
            '            if any((fpu.banks[(fpu.fpscr>>21)&1][i]&0x7fffffff)>=0x7f800000 for i in (n,m)):\n'
            f'                exceptional_binary(fpu,n,m,{operation})\n'
            '            else:\n    '+statement)
    namespace=dict(vars(executor),exceptional_binary=exceptional_binary)
    exec(compile(extended,'<primitive-pref-reference>','exec'),namespace)
    return namespace['reference'],dict(original_reference_sha256=hashlib.sha256(original.encode()).hexdigest(),
                                      extended_reference_sha256=hashlib.sha256(extended.encode()).hexdigest(),
                                      exceptional_class_service_sha256=hashlib.sha256(inspect.getsource(exceptional_binary).encode()).hexdigest())
