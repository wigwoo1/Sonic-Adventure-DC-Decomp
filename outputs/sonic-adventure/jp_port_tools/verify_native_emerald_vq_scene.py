"""Exercise original Emerald Coast VQ mipmapped materials on the native GPU."""
import ctypes as C
import inspect
from pathlib import Path

import verify_native_emerald_textured_scene as textured
from native_packet_test_support import bind


class Level(C.Structure):
    _fields_ = [(name,C.c_uint32) for name in ('width','height','format','mip_count')]
    _fields_ += [(name,C.c_size_t) for name in ('encoded_offset','required_source_bytes','rgba_bytes')]


def verify_terminal_mip(context,runtime,listing,slot):
    gpu = C.CDLL(context['b']['GPU_DLL'])
    owner = textured.Record.from_address(runtime.lookup(C.byref(listing),slot))
    surface = owner.texinfo.texsurface
    tsp,tcw = owner.tspparambuffer,owner.texparambuffer
    assert tcw&0xc0000000==0xc0000000
    level = surface.nWidth.bit_length()-1
    info,rgba = Level(),(C.c_uint8*4)()
    word = tcw&0x1fffff
    data,size = runtime.surface_data(word),runtime.surface_bytes(word)
    decode = bind(gpu,'jp_texel_decode',C.c_int,*([C.c_uint32]*4),
                  C.c_void_p,C.c_size_t,C.c_void_p,C.c_size_t,
                  C.c_void_p,C.c_size_t,C.c_void_p)
    assert decode(tsp,tcw,level,0,data,size,None,0,rgba,4,C.byref(info))==0,slot
    assert (info.width,info.height,info.encoded_offset,info.required_source_bytes)==(1,1,2048,2049)
    code = C.c_uint8.from_address(data+2048).value
    pixel = int.from_bytes(C.string_at(data+code*8+6,2),'little')
    from native_pvm_test_support import color16
    assert bytes(rgba)==color16(pixel,info.format),slot
    return word


def draw_land(context,track):
    source = inspect.getsource(textured.draw_textured_land)
    anchor = '''                deferred_slots.add(slot)
                continue'''
    assert source.count(anchor)==1
    source = source.replace(anchor,'''                if slot not in vq_words:
                    vq_words[slot] = verify_terminal_mip(context,runtime,listing,slot)''')
    anchor = '    source_triangles = 0'
    assert source.count(anchor)==1
    source = source.replace(anchor,'    vq_words = {}\n'+anchor)
    source = source.replace('deferred_vq_mip_slots=sorted(deferred_slots)',
                            'vq_terminal_mip_verified_slots=sorted(vq_words),vq_texture_words=list(vq_words.values())')
    namespace = dict(vars(textured),verify_terminal_mip=verify_terminal_mip)
    exec(compile(source,textured.__file__,'exec'),namespace)
    return namespace['draw_textured_land'](context,track)


def scene_caller(raw):
    raw = textured.textured_caller(raw)
    raw = raw.replace('from verify_native_emerald_textured_scene import draw_textured_land',
                      'from verify_native_emerald_vq_scene import draw_land as draw_textured_land')
    anchor = "    land['nonempty_textured_stage_batches']=len(land_batches)"
    assert raw.count(anchor)==1
    raw = raw.replace(anchor,anchor+'''
    vq_words=set(land['vq_texture_words'])
    vq_batches=[b for b in land_batches if b.header[3]&0x1fffff in vq_words]
    check(bool(vq_batches),'nonempty retained batches use original VQ mipmapped stage surfaces')
    land['nonempty_vq_stage_batches']=len(vq_batches)
''')
    return raw.replace('sonic_emerald_textured_scene','sonic_emerald_vq_scene').replace(
        'original_Emerald_Coast_textured_land_and_Sonic_rendered_together',
        'original_Emerald_Coast_VQ_mipmapped_land_and_Sonic_rendered_together')


def exercise(context,inputs,track):
    source = inspect.getsource(textured.exercise)
    source = source.replace('scene_caller=textured_caller','scene_caller=vq_caller')
    namespace = dict(vars(textured),__file__=__file__,vq_caller=scene_caller)
    exec(compile(source,textured.__file__,'exec'),namespace)
    namespace['exercise'](context,inputs,track)


if __name__=='__main__':
    runner = Path(textured.scene.__file__).read_text().split("if __name__ == '__main__':\n",1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor)==1
    runner = runner.replace(anchor,'from verify_native_emerald_vq_scene import exercise')
    exec(compile('if True:\n'+runner,textured.scene.__file__,'exec'),dict(vars(textured.scene)))
