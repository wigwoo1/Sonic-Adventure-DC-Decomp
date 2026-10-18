"""Manual-derived bank/width/word-order vectors, not hardware verification."""
import ctypes,hashlib,json,pathlib,struct
ROOT=pathlib.Path(__file__).resolve().parents[2]
class Fpu(ctypes.Structure):
    _fields_=[('banks',(ctypes.c_uint32*16)*2),('fpscr',ctypes.c_uint32),('fpul',ctypes.c_uint32)]
def main():
    dll_path=ROOT/'work/sonic-adventure/build/fpu_cleanup_transfer.dll'
    function=ctypes.CDLL(str(dll_path)).sa_fpu_cleanup_transfer
    function.argtypes=[ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint8)];function.restype=ctypes.c_int
    count=0
    for active in range(2):
     for width in range(2):
      for precision in range(2):
       for pattern in [0,1,0x80000000,0xffffffff,0x3f800000,0x7fc12345,0x11223344]:
        state=Fpu()
        for bank in range(2):state.banks[bank][:]=[(pattern^(0x10203040*bank)^i)&0xffffffff for i in range(16)]
        state.fpscr=0x40001|(active<<21)|(width<<20)|(precision<<19)
        expected=Fpu.from_buffer_copy(state);destination=(ctypes.c_uint8*16)(*[0xa5]*16)
        expected_bytes=bytearray(destination)
        if not precision:
            expected.banks[active][3]=0
            if width:expected_bytes[4:12]=struct.pack('<2I',expected.banks[active^1][2],expected.banks[active^1][3])
            else:expected_bytes[4:8]=bytes(4)
        output=ctypes.cast(ctypes.byref(destination,4),ctypes.POINTER(ctypes.c_uint8))
        assert function(ctypes.byref(state),output)==int(not precision)
        assert bytes(state)==bytes(expected) and bytes(destination)==expected_bytes,(active,width,precision,pattern)
        count+=1
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':count,'original_game_routines_added':0,
            'source_sha256':hashlib.sha256((root/'source/fpu_cleanup_transfer.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'primary_source':'https://www.renesas.com/en/document/mas/sh-4-software-manual',
            'manual_revision':'6.00','manual_sections':['2.5','6','9.32','9.37','9.38'],
            'scope':'Manual-derived support-code vectors covering both physical banks, transfer widths, distinct word patterns and PR=1 rejection. No original routine or hardware comparison, exception emulation, or proof of game entry FPSCR.'}
    (root/'analysis/fpu_cleanup_transfer_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{count} bank-aware transfer vectors passed; PR=1 rejected without changes')
if __name__=='__main__':main()
