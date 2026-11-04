import ctypes as C
from native_packet_test_support import bind
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
class Limits(C.Structure):_fields_=[('file_bytes',S),('decoded_bytes',S),('buffer_bytes',S),('resident_names',U)]
class State(C.Structure):
    _fields_=[(n,Q) for n in ['files_read','decodes','pvm_loads','transfers','bytes_read','bytes_decoded']]+[(n,S) for n in ['owned_bytes','decoded_bytes']]+[(n,U) for n in ['status','os_error','prs_error','resident_refs','pending_refs']]
class Assets:
    def __init__(self,lib):
        self.init=bind(lib,'jp_assets_initialize',I,C.c_wchar_p,P);self.shutdown=bind(lib,'jp_assets_shutdown',I);self.status=bind(lib,'jp_assets_status',I);self.clear=bind(lib,'jp_assets_clear_error',None)
        self.read=bind(lib,'jp_assets_read_prs',I,C.c_char_p,P,P);self.release=bind(lib,'jp_assets_release_data',I);self.load=bind(lib,'jp_assets_load_prs',I,C.c_char_p,P,U,S)
        self.commit=bind(lib,'jp_assets_commit_game',I,P,P);self.game=bind(lib,'jp_assets_load_game_file',I,C.c_char_p,P);self.recover=bind(lib,'jp_assets_recover_pending',I);self.evict=bind(lib,'jp_assets_release_residents',I)
        self.move=bind(lib,'jp_texture_move_names',I,P,P,U);self.validate=bind(lib,'jp_texture_validate_move',I,P,P,U)
        self.original=bind(lib,'texture_load_pvm',None,C.c_char_p,P);self.original_file=bind(lib,'texture_load_prs_file',I,C.c_char_p,P)
        self.info=bind(lib,'jp_assets_snapshot',I,P);assert bind(lib,'jp_assets_info_size',S)()==C.sizeof(State)
    def snapshot(self):
        state=State();assert self.info(C.byref(state))==0;return state
def literal_prs(data):
    """Independent literal-only stream constructor, including the end marker."""
    output=bytearray();bit=8;control=0
    def add(value):
        nonlocal bit,control
        if bit==8:control=len(output);output.append(0);bit=0
        output[control]|=value<<bit;bit+=1
    for value in data:add(1);output.append(value)
    add(0);add(1);output+=b'\0\0';return bytes(output)
