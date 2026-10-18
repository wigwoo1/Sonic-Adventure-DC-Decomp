"""Validate the recovered native PRS routine against every extracted PRS file."""
import ctypes, hashlib, json, pathlib
ROOT=pathlib.Path(__file__).resolve().parents[2]
def main():
    root=ROOT/'outputs/sonic-adventure'
    records=json.loads((root/'analysis/prs_inventory.json').read_text())
    dll_path=ROOT/'work/sonic-adventure/build/prs_decompress.dll'
    dll=ctypes.CDLL(str(dll_path));function=dll.sa_prs_decompress_8c04c5e0
    function.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint8)]
    function.restype=ctypes.c_uint32;results=[]
    for record in records:
        assert record['status']=='decoded'
        source=(ROOT/'work/sonic-adventure/extracted/files'/record['source']).read_bytes()
        assert hashlib.sha256(source).hexdigest()==record['compressed_sha256']
        encoded=ctypes.create_string_buffer(source)
        size=record['decoded_bytes'];output=ctypes.create_string_buffer(size+64)
        ctypes.memset(output,0xA5,size+64)
        count=function(ctypes.cast(encoded,ctypes.POINTER(ctypes.c_uint8)),
                       ctypes.cast(ctypes.byref(output,32),ctypes.POINTER(ctypes.c_uint8)))
        assert count==size,record['source']+': output length differs'
        actual=output.raw[32:32+size]
        assert hashlib.sha256(actual).hexdigest()==record['decoded_sha256'],record['source']+': decoded content differs'
        assert output.raw[:32]==bytes([0xA5])*32 and output.raw[-32:]==bytes([0xA5])*32,'Output guard changed'
        assert encoded.raw[:len(source)]==source,'Input changed'
        results.append({'resource':record['source'],'decoded_bytes':count,'content_hash_matches':True,'guards_preserved':True})
    report={'routine':'0x8c04c5e0','files_verified':len(results),
            'decoded_bytes_verified':sum(r['decoded_bytes'] for r in results),
            'source_sha256':hashlib.sha256((root/'source/prs_decompress.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'files':results,
            'scope':'Native output compared with prior Python PRS decoding and its hashes; not yet an original-instruction or hardware equivalence test.'}
    (root/'analysis/prs_native_verification.json').write_text(json.dumps(report,indent=2))
    print('Native PRS matches:',len(results),'files;',report['decoded_bytes_verified'],'decoded bytes')
if __name__=='__main__':main()
