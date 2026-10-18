from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_retail_pipeline.py').read_text()
text=text.replace('file.read(288)','file.read(576)').replace('len(payload)==288','len(payload)==576')
a=text.index('    s=State()');b=text.index("(out/'analysis/",a)
body=text[a:b]
body=body.replace('native[0xa10000:0xa10000+len(payload)]','native[0xa10000+request_index*288:0xa10000+request_index*288+len(payload)]')
body=body.replace('(0,history,0xa20008,0xa60028),(1,(0,0),0xa40008,0xa6002c)', '(0,left_history,0xa20008,0xa60028),(1,right_history,0xa40008,0xa6002c)')
body=body.replace("offset=offset,pair_sha256", "offset=offset+request_index*288,request_index=request_index,pair_sha256")
setup='''    full_payload=payload
    for request_index in range(2):
        payload=full_payload[request_index*288:(request_index+1)*288]
        left_history=struct.unpack_from('<2h',ram,0xa60028)
        right_history=struct.unpack_from('<2h',ram,0xa6002c)
        for memory in (ram,native):
            struct.pack_into('<I',memory,0xa6000c,1)
            struct.pack_into('<I',memory,0xa60018,0x8ca10000+request_index*288)
            memory[0xa20000:0xa20300]=bytes([0x55])*768
            memory[0xa40000:0xa40300]=bytes([0x55])*768
'''
text=text[:a]+setup+''.join('    '+line if line.strip() else line for line in body.splitlines(True))+text[b:]
text=text.replace('file_decode_retail_pipeline_verification.json','file_decode_retained_history_verification.json')
text=text.replace('218 full','436 full').replace('first288-byte encoded prefix (eight consecutive pairs)','first576-byte encoded prefix split into two288-byte decode requests retaining the same file handle')
text=text.replace('Zero starting histories;', 'Zero initial histories; second request starts from first request stored signed16-bit histories;')
text=text.replace('original file-dispatch stereo pipeline comparisons passed','retained-history original file pipeline comparisons passed')
(root/'verify_file_decode_retained_history.py').write_text(text)
