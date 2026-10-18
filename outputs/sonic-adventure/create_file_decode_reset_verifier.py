from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_request_activate.py').read_text().replace('file_decode_request_activate','file_decode_handle_reset').replace('s.pc=0x8c644c56','s.pc=0x8c644c68')
text=text.replace('cases=0', "fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/byte_fill.dll')).sa_byte_fill;fill.argtypes=fn.argtypes\ncases=0")
text=text.replace("def call(*args):raise AssertionError('Unexpected service')", "def call(ctx,address,mem,fp,srp,r,prp):\n        assert address==0x8c10d31c\n        return fill(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)")
text=text.replace("if phase==0:struct.pack_into('<2I',wanted,0xc,1,0)", "struct.pack_into('<I',wanted,0xc,0);wanted[0x28:0x30]=bytes(8)")
text=text.replace('nine-instruction request activation644c56..644c68','11-instruction handle reset644c68..644c7e connected to native byte fill10d31c').replace('idle becomes phase1 with result0, busy states preserve every byte','every input phase becomes0, both predictor histories cleared, all other handle bytes preserved').replace('No modeled calls.','No modeled calls; original fill body executed by reference.')
text=text.replace('decode request activation comparisons passed','decode handle reset comparisons passed')
(root/'verify_file_decode_handle_reset.py').write_text(text)
text=(root/'build_file_decode_request_activate.ps1').read_text().replace('file_decode_request_activate','file_decode_handle_reset')
(root/'build_file_decode_handle_reset.ps1').write_text(text)
text=(root/'register_file_decode_request_activate.py').read_text().replace('file_decode_request_activate','file_decode_handle_reset').replace('644c68','644c7e').replace('644c56','644c68').replace('634c68','634c7e').replace('634c56','634c68').replace('Decode request activation:','Decode handle reset:')
(root/'register_file_decode_handle_reset.py').write_text(text)
