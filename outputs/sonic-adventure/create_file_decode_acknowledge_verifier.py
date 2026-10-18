from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_request_activate.py').read_text().replace('file_decode_request_activate','file_decode_completion_acknowledge').replace('s.pc=0x8c644c56','s.pc=0x8c644c7e')
text=text.replace("if phase==0:struct.pack_into('<2I',wanted,0xc,1,0)", "if phase==3:struct.pack_into('<I',wanted,0xc,0)")
text=text.replace('nine-instruction request activation644c56..644c68','seven-instruction completion acknowledgment644c7e..644c8c').replace('idle becomes phase1 with result0, busy states preserve every byte','completed phase3 becomes idle phase0 while all other states remain unchanged; result, histories and request fields preserved')
text=text.replace('decode request activation comparisons passed','decode completion acknowledgment comparisons passed')
(root/'verify_file_decode_completion_acknowledge.py').write_text(text)
text=(root/'build_file_decode_request_activate.ps1').read_text().replace('file_decode_request_activate','file_decode_completion_acknowledge')
(root/'build_file_decode_completion_acknowledge.ps1').write_text(text)
text=(root/'register_file_decode_request_activate.py').read_text().replace('file_decode_request_activate','file_decode_completion_acknowledge').replace('644c56','644c7e').replace('644c68','644c8c').replace('634c56','634c7e').replace('634c68','634c8c').replace('Decode request activation:','Decode completion acknowledgment:')
(root/'register_file_decode_completion_acknowledge.py').write_text(text)
