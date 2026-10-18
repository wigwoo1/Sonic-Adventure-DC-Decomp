"""Compare STG00 0x0c900e00 global writes with original SH-4 execution."""
import ctypes,hashlib,json,pathlib,random
from verify_startup import State,reference

ROOT=pathlib.Path(__file__).resolve().parents[2]
class Record(ctypes.Structure):
    _fields_=[(f'word_{i:02x}',ctypes.c_uint32) for i in (0,4,8,12,16,20)]

def main():
    root=ROOT/'outputs/sonic-adventure'
    overlay=(ROOT/'work/sonic-adventure/decoded/SONICADV/STG00.BIN').read_bytes()
    assert overlay[0xf20:0xf24]==(0x8c799788).to_bytes(4,'little')
    dll_path=ROOT/'work/sonic-adventure/build/stage00_global_initialize.dll'
    native=ctypes.CDLL(str(dll_path)).sa_stage00_global_initialize_0c900e00
    native.argtypes=[ctypes.POINTER(Record)];native.restype=None
    rng=random.Random(0xc900e00);cases=[]
    for case in range(12):
        ram=bytearray(rng.randbytes(16*1024*1024))
        ram[0x900000:0x900000+len(overlay)]=overlay
        initial=bytes(ram[0x799788:0x799788+24])
        expected=bytearray(ram)
        state=State();state.r[:]=[0x13570000+i for i in range(16)]
        state.pc=0x0c900e00;state.pr=0x8c000100;state.sr=0x700000f0
        steps=reference(expected,state)
        native_record=Record.from_buffer(ram,0x799788)
        native(ctypes.byref(native_record))
        if ram!=expected:
            difference=next(i for i,(actual,wanted) in enumerate(zip(ram,expected)) if actual!=wanted)
            raise AssertionError(f'case {case}, steps={steps}: RAM differs at {difference:#x}: native={ram[difference:difference+16].hex()} retail={expected[difference:difference+16].hex()}')
        assert bytes(ram[0x799788:0x799788+24])==bytes.fromhex('00000000f401000000000000640000000000000038ffffff')
        cases.append({'case':case,'initial_record_sha256':hashlib.sha256(initial).hexdigest(),
          'original_instruction_steps':steps,'complete_ram_matches':True})
    report={'module':'STG00.BIN','routine':'0x0c900e00','end_exclusive':'0x0c900e26',
      'cases_passed':len(cases),'cases':cases,
      'decoded_module_sha256':hashlib.sha256(overlay).hexdigest(),
      'retail_span_sha256':hashlib.sha256(overlay[0xe00:0xe26]).hexdigest(),
      'instruction_listing':'analysis/instructions/0c900e00.tsv',
      'instruction_listing_sha256':hashlib.sha256((root/'analysis/instructions/0c900e00.tsv').read_bytes()).hexdigest(),
      'source':'source/stage00_global_initialize.c',
      'source_sha256':hashlib.sha256((root/'source/stage00_global_initialize.c').read_bytes()).hexdigest(),
      'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
      'native_build_succeeded':True,
      'global_address':'0x8c799788','global_ram_offset':'0x799788',
      'scope':'All 16 MiB RAM bytes compared against execution of the original function, including the six global words and preservation of every surrounding byte. The candidate initializer is verified in its current code/data export; semantic names and whole-overlay code/data boundaries remain incomplete.'}
    (root/'analysis/stage00_global_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{len(cases)} full-RAM comparisons passed')
if __name__=='__main__':main()
