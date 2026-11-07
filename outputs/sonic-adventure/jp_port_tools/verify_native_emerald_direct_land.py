"""Render bounded Emerald Coast models directly in the original land callback."""
import ctypes as C
import inspect
from pathlib import Path

import verify_native_emerald_land_dispatch as dispatch
import verify_native_emerald_textured_scene as textured
import verify_native_emerald_vq_scene as vq
from native_packet_test_support import bind


def dispatch_models(context,image,listing,render_model):
    source = inspect.getsource(dispatch.scheduled_land)
    anchor = '        observed.append(i)'
    assert source.count(anchor)==1
    source = source.replace(anchor,anchor+'''
        try:
            render_model(i)
            get(after)
            assert bytes(after)==matrices[i]
        except Exception as error:
            errors.append((i,repr(error)))
''')
    namespace = dict(vars(dispatch),render_model=render_model)
    exec(compile(source,dispatch.__file__,'exec'),namespace)
    lib = context['f']
    saved = (C.c_float*16)()
    bind(lib,'njGetMatrix',None,C.c_void_p)(saved)
    translate = bind(lib,'njTranslate',None,C.c_void_p,*([C.c_float]*3))
    set_matrix = bind(lib,'njSetMatrix',None,C.c_void_p,C.c_void_p)
    translate(None,9,-4-textured.scene.DISPLAY_DROP,-4)
    try:
        return namespace['scheduled_land'](context,image,listing)
    finally:
        set_matrix(None,saved)


def draw_land(context,track):
    source = inspect.getsource(textured.draw_textured_land)

    def swap(old,new):
        nonlocal source
        assert source.count(old)==1,old
        source = source.replace(old,new)

    swap('    source_triangles = 0','    vq_words = {}\n    source_triangles = 0')
    swap('    for entry in (42,91,92,93,126,388,393):',
         '    def render_model(entry):\n        nonlocal source_triangles')
    swap('    assert used(0)>prior_bytes and emitted_slots',
         '    draw_order = dispatch_models(context,image,listing,render_model)\n'
         '    assert used(0)>prior_bytes and emitted_slots')
    swap('                deferred_slots.add(slot)\n                continue',
         '                if slot not in vq_words:\n'
         '                    vq_words[slot] = verify_terminal_mip(context,runtime,listing,slot)')
    swap('positions.append(Vec(tx+cosine*x+sine*z+9,ty+y-4-scene.DISPLAY_DROP,tz-sine*x+cosine*z-4))',
         'positions.append(Vec(x,y,z))')
    swap('            if attr&0x400000:\n                environment_slots.add(slot)\n                continue',
         '            environment = bool(attr&0x400000)\n'
         '            if environment: environment_slots.add(slot)')
    swap('points,_,n,meshes,materials,mesh_count,material_count',
         'points,normals,n,meshes,materials,mesh_count,material_count')
    swap('            native_model = Model(vertices,None,n,',
         "            normal_array = (Vec*n)(*(Vec(*read_at('<3f',normals+12*j)) for j in range(n))) if environment else None\n"
         '            native_model = Model(vertices,normal_array,n,')
    swap('draw(C.byref(native_model),n,0,1,','draw(C.byref(native_model),n,n if environment else 0,1,')
    swap('deferred_vq_mip_slots=sorted(deferred_slots)',
         'vq_texture_words=list(vq_words.values()),visibility_draw_order=draw_order')
    swap('deferred_environment_slots=sorted(environment_slots)',
         'submitted_environment_slots=sorted(environment_slots),direct_callback_draws=len(draw_order)')
    namespace = dict(vars(textured),dispatch_models=dispatch_models,verify_terminal_mip=vq.verify_terminal_mip)
    exec(compile(source,textured.__file__,'exec'),namespace)
    return namespace['draw_textured_land'](context,track)


def scene_caller(raw):
    raw = vq.scene_caller(raw).replace('from verify_native_emerald_vq_scene import draw_land',
                                     'from verify_native_emerald_direct_land import draw_land')
    anchor = "    land['nonempty_vq_stage_batches']=len(vq_batches)"
    assert raw.count(anchor)==1
    raw = raw.replace(anchor,anchor+'''
    check(land['direct_callback_draws']==13,'the original land callback invokes bounded model drawing 13 times')
''')
    return raw.replace('sonic_emerald_vq_scene','sonic_emerald_direct_land').replace(
        'original_Emerald_Coast_VQ_mipmapped_land_and_Sonic_rendered_together',
        'original_Emerald_Coast_land_callback_direct_GPU_submission_verified')


def exercise(context,inputs,track):
    source = inspect.getsource(textured.scene.exercise)
    namespace = dict(vars(textured.scene),__file__=__file__,scene_caller=scene_caller)
    exec(compile(source,textured.scene.__file__,'exec'),namespace)
    namespace['exercise'](context,inputs,track)


if __name__=='__main__':
    scene = textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n",1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor)==1
    runner = runner.replace(anchor,'from verify_native_emerald_direct_land import exercise')
    exec(compile('if True:\n'+runner,scene.__file__,'exec'),dict(vars(scene)))
