"""Retail cancellation wrappers, ending at an explicit runtime-vector boundary."""
import ctypes
import hashlib
import json
import random
import struct
from pathlib import Path
from verify_startup import ROOT,State,reference

PROJECT=ROOT/'outputs/sonic-adventure'
TAIL,SERVICE=0x8C6451A4,0x8C10D9D8
SIZE=16*1024*1024
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ROOT/'work/sonic-adventure/build/stream_cleanup_abort_native.dll'
lib=ctypes.CDLL(str(dll))
functions={TAIL:lib.sa_device_cancel_tail_8c6451a4,SERVICE:lib.sa_device_cancel_service_8c10d9d8}
for function in functions.values():
    function.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)]
    function.restype=None


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    rng=random.Random(TAIL);cases=[]
    for kind in ('device_tail','service_tail','both'):
        for case in range(64):
            ram=bytearray(SIZE);ram[0x10000:0x10000+len(binary)]=binary
            target=0x8C700000+rng.randrange(0x8000)*2
            struct.pack_into('<I',ram,0xBC,target)
            if kind=='device_tail':struct.pack_into('<I',ram,0x6451F8,target)
            entry=SERVICE if kind=='service_tail' else TAIL
            initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)]
            initial.r[15]=0x8C7FF000
            initial.sr,initial.pr,initial.pc=rng.getrandbits(32),0x8C74913C,entry
            expected_ram,expected=bytearray(ram),State.from_buffer_copy(initial)
            reference(expected_ram,expected,stop_pc=target)
            actual_ram,actual=bytearray(ram),State.from_buffer_copy(initial)
            view=(ctypes.c_uint8*SIZE).from_buffer(actual_ram)
            functions[entry](view,ctypes.byref(actual))
            if kind=='both':
                assert actual.pc==SERVICE
                functions[SERVICE](view,ctypes.byref(actual))
            assert actual_ram==expected_ram and bytes(actual)==bytes(expected),(kind,case)
            if kind!='device_tail':assert actual.r[6]==0 and actual.r[7]==8
            assert actual.pr==initial.pr and actual.r[4]==initial.r[4]
            cases.append({'kind':kind,'case':case,'target':hex(target),'matched':True})
    source='source/sa_device_cancel_path.c'
    routines=[]
    for address,end,name in [(TAIL,TAIL+6,'sa_device_cancel_tail_8c6451a4'),
                              (SERVICE,SERVICE+12,'sa_device_cancel_service_8c10d9d8')]:
        routines.append({'name':name,'address':hex(address),'end_exclusive':hex(end),
                         'instruction_bytes':end-address,'retail_span_sha256':hashlib.sha256(binary[address-0x8C010000:end-0x8C010000]).hexdigest()})
    report={'module':'1ST_READ.BIN','cases_passed':192,'leaf_cases_passed':128,'chain_cases_passed':64,
            'routines':routines,'source':source,'source_sha256':digest(PROJECT/source),
            'binary_sha256':hashlib.sha256(binary).hexdigest(),'dll_sha256':digest(dll),
            'verifier_sha256':digest(Path(__file__)),'interpreter_sha256':digest(PROJECT/'verify_startup.py'),
            'bindings_sha256':digest(PROJECT/'analysis/request_device_callback_bindings.json'),
            'comparison_level':'Full 16 MiB RAM and GPR/SR/PR/PC matched at the tail target, preserving the original PR and incoming R4.',
            'scope':'Default device cancellation tail and operation-8 runtime-service tail, separately and together; runtime target randomized.',
            'limitations':['The external runtime-vector target is a platform-service boundary, not recovered native game code.',
                           'Static default-table bindings do not establish complete driver startup or alternate runtime backends.',
                           'No hardware or whole-game verification is claimed.'],
            'cases':cases}
    (PROJECT/'analysis/device_cancel_path_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('128 cancellation leaf comparisons and 64 complete cancellation-tail-chain comparisons passed')


if __name__=='__main__':main()
