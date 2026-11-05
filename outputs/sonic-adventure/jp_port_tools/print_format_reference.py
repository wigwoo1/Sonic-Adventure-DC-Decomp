"""Print formatter reference: existing scalar/PREF CPU plus qualified FCNVSD.
Original frozen executors stay unchanged. No game/runtime function hooks.
"""
import hashlib,inspect,linecache,types
from primitive_reference import make_reference as primitive,exceptional_binary

def class_binary(fpu,n,m,operation):
    if fpu.fpscr&0x80000:
        import sh4_binary64 as d
        if operation in ('add','subtract'):return d.execute_add_sub(fpu,n,m,operation=='subtract')
        return (d.execute_multiply if operation=='multiply' else d.execute_divide)(fpu,n,m)
    return exceptional_binary(fpu,n,m,operation)

def make_reference(executor):
    import sh4_widen_reference as widen
    source=inspect.getsource(executor.reference);original=source
    anchor='        elif word & 0xF0FF == 0xF02D:';assert source.count(anchor)==1
    source=source.replace(anchor,"        elif word & 0xF0FF == 0xF0AD:\n            assert widen.widen(fpu,fpu.fpul,n)==0,'FCNVSD exception outside qualified print scope'\n"+anchor)
    old="                from sh4_binary64 import execute_compare\n                condition=execute_compare(fpu,n,m,word&15==5)\n                sr=(sr&~1)|int(condition)";assert source.count(old)==1
    source=source.replace(old,"                rc,sr=widen.compare(fpu,sr,n,m,word&15==5)\n                assert rc==0,'double comparison exception outside qualified print scope'")
    filename='<print-format-widen-reference>';linecache.cache[filename]=(len(source),None,source.splitlines(keepends=True),filename)
    namespace=dict(vars(executor),widen=widen);exec(compile(source,filename,'exec'),namespace)
    ref,hashes=primitive(types.SimpleNamespace(**namespace))
    # The primitive exceptional dispatcher inspects scalar words. In PR=1,
    # either half of a valid double can resemble a single NaN; dispatch such
    # a call to the existing double arithmetic model instead.
    globals_=dict(ref.__globals__,exceptional_binary=class_binary);ref=types.FunctionType(ref.__code__,globals_,ref.__name__,ref.__defaults__,ref.__closure__)
    hashes.update(frozen_original_reference_sha256=hashlib.sha256(original.encode()).hexdigest(),widen_reference_sha256=hashlib.sha256(inspect.getsource(widen).encode()).hexdigest(),double_dispatch_sha256=hashlib.sha256(inspect.getsource(class_binary).encode()).hexdigest())
    return ref,hashes
