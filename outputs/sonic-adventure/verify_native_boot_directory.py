"""Root/SONICADV/missing boot selectors through original mount and real data."""
import ctypes,hashlib,json,shlex
from pathlib import Path
from verify_startup import ROOT
from native_disc_directory import discover_root_files
PROJECT=ROOT/'outputs/sonic-adventure';U=ctypes.c_uint32;BP=ctypes.POINTER(ctypes.c_uint8)
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint64,BP,U)
dll=ROOT/'work/sonic-adventure/build/native_disc_queue.dll';lib=ctypes.CDLL(str(dll))
lib.sa_disc_queue_size.restype=U
lib.sa_disc_queue_init.argtypes=[ctypes.c_void_p,Read,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,U,U]
guest_read=lib.sa_disc_read_guest_sectors;guest_read.argtypes=[ctypes.c_void_p,BP,U,U,U]
manifest=json.loads((PROJECT/'disc_inventory.json').read_text())
track=next(t for t in reversed([shlex.split(s) for s in manifest['gdi'].splitlines()[1:] if s.strip()]) if t[2]=='4')
path=Path(manifest['source'])/track[4];base,stride,offset=int(track[1]),int(track[3]),int(track[5]);sectors=(path.stat().st_size-offset)//stride
cases=[];errors=[]
with path.open('rb') as stream:
    @Read
    def read(user,pos,out,size):
        try:
            stream.seek(pos);data=stream.read(size)
            if len(data)!=size:return -8
            ctypes.memmove(out,data,size);return 0
        except BaseException as exc:errors.append(repr(exc));return -8
    for selection,queries in ((None,('1st_read.bin','ADVLOGOBK.GIF','__MISSING__')),
        ('SONICADV',('action_0.pvr','ACTION_1.PVR','__MISSING__')),
        ('sonicadv',('ACTION_0.PVR','action_1.pvr','__MISSING__')),
        ('__MISSING__',('ACTION_0.PVR',))):
        found,evidence=discover_root_files(lib,read,offset,sectors,base,stride,queries,selection)
        assert not errors,errors
        if selection=='__MISSING__':
            # Mount translates directory-select's -8 into its own -4 result
            # at 8c5fffe6, and never sets the mounted flag on that path.
            assert evidence['mount_result']==0xfffffffc and evidence['mounted_flag']==0 and found==[None]
        else:
            assert evidence['mount_result']==0 and found[-1] is None
            q=ctypes.create_string_buffer(lib.sa_disc_queue_size())
            assert lib.sa_disc_queue_init(q,read,None,offset,sectors,base,stride)==0
            ram=bytearray(16*1024*1024);view=(ctypes.c_uint8*len(ram)).from_buffer(ram)
            hashes=[]
            for file in found[:-1]:
                assert file is not None
                name=('SONICADV/' if selection else '')+file['path']
                expected=next(f for f in manifest['files'] if f['path']==name)
                assert file['lba']==expected['lba'] and file['size']==expected['size']
                count=(file['size']+2047)//2048
                assert guest_read(q,view,file['lba'],count,0x8c100000)==0
                digest=hashlib.sha256(ram[0x100000:0x100000+file['size']]).hexdigest()
                assert digest==expected['sha256'];hashes.append({'path':name,'sha256':digest})
            evidence['file_hashes']=hashes
        cases.append(evidence)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
source_evidence=json.loads((PROJECT/'analysis/native_file_request_lifecycle_verification.json').read_text())
for source,value in source_evidence['sources'].items():assert sha(PROJECT/source)==value,source
report={'cases_passed':len(cases),'cases':cases,'sources':source_evidence['sources'],
 'lifecycle_evidence_sha256':sha(PROJECT/'analysis/native_file_request_lifecycle_verification.json'),
 'dll_sha256':sha(dll),'cleanup_dll_sha256':sha(ROOT/'work/sonic-adventure/build/stream_cleanup_disc_native.dll'),
 'verifier_sha256':sha(Path(__file__)),'directory_helper_sha256':sha(PROJECT/'native_disc_directory.py'),
 'interpreter_sha256':sha(PROJECT/'verify_startup.py'),'inventory_sha256':sha(PROJECT/'disc_inventory.json'),
 'scope':'Original full context/mount/boot-selector/directory-selection chain vs retail RAM/CPU/FPU/MMIO, all directory records independently checked and resolved files read from actual image.',
 'limitations':'Prepared platform buffers/interrupt bootstrap; coherent host-cache services; host disc policies. No complete game startup/rendering or arbitrary nested path traversal.'}
(PROJECT/'analysis/native_boot_directory_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(cases),'boot-directory scenarios passed; root/SONICADV/case-folded selection and missing-directory failure')
