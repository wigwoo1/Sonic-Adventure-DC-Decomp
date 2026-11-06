"""Static AICA RAM window for original SDK read contracts, no routine hooks."""
import hashlib,inspect,linecache,types
from model_reference import make_reference

def make_bus_reference(executor):
    original=inspect.getsource(executor.reference)
    old="""        offset=(addr & 0x1fffffff)-0x0c000000
        if not 0 <= offset < len(ram): raise AssertionError(f'Unmapped RAM address {addr:08x} at {pc:08x}')
        return offset"""
    new="""        physical=addr & 0x1fffffff
        # Separate2MiB sound RAM follows the original16MiB main-RAM backing.
        # This is a static read-contract fixture, not DMA/ARM scheduling.
        if 0x00800000 <= physical < 0x00a00000:
            assert len(ram)==18<<20
            return (16<<20)+physical-0x00800000
        offset=physical-0x0c000000
        if not 0 <= offset < 16<<20: raise AssertionError(f'Unmapped RAM address {addr:08x} at {pc:08x}')
        return offset"""
    assert original.count(old)==1
    source=original.replace(old,new); filename='<pstm-static-sound-RAM-reference>'
    linecache.cache[filename]=(len(source),None,source.splitlines(keepends=True),filename)
    namespace=dict(vars(executor)); exec(compile(source,filename,'exec'),namespace)
    proxy=types.SimpleNamespace(**dict(vars(executor),reference=namespace['reference']))
    reference,hashes=make_reference(proxy)
    hashes.update(unmodified_startup_reference_sha256=hashlib.sha256(original.encode()).hexdigest(),
                  static_sound_RAM_reference_sha256=hashlib.sha256(source.encode()).hexdigest())
    return reference,hashes
