"""Exercise original Emerald Coast environment maps with their model normals."""
import inspect
from pathlib import Path

import verify_native_emerald_textured_scene as textured
import verify_native_emerald_vq_scene as vq


def draw_land(context,track):
    source = inspect.getsource(textured.draw_textured_land)

    def swap(old,new):
        nonlocal source
        assert source.count(old)==1,old
        source = source.replace(old,new)

    swap('    source_triangles = 0','    vq_words, environment_words = {}, {}\n    source_triangles = 0')
    swap('''                deferred_slots.add(slot)
                continue''','''                if slot not in vq_words:
                    vq_words[slot] = verify_terminal_mip(context,runtime,listing,slot)''')
    swap('''            if attr&0x400000:
                environment_slots.add(slot)
                continue''','''            environment = bool(attr&0x400000)
            if environment:
                environment_slots.add(slot)
                record = Record.from_address(runtime.lookup(C.byref(listing),slot))
                environment_words[slot] = record.texparambuffer&0x1fffff''')
    swap('points,_,n,meshes,materials,mesh_count,material_count',
         'points,normals,n,meshes,materials,mesh_count,material_count')
    swap('            native_model = Model(vertices,None,n,',
         '''            normal_array = None
            if environment:
                assert normals
                vectors = []
                for j in range(n):
                    nx,ny,nz = read_at('<3f',normals+12*j)
                    vectors.append(Vec(cosine*nx+sine*nz,ny,cosine*nz-sine*nx))
                normal_array = (Vec*n)(*vectors)
            native_model = Model(vertices,normal_array,n,''')
    swap('            vertices = (Vec*n)(*positions)', '''            display = positions
            if environment:
                corners = [positions[j] for j in set(order)]
                low = [min(getattr(p,axis) for p in corners) for axis in ('x','y','z')]
                high = [max(getattr(p,axis) for p in corners) for axis in ('x','y','z')]
                center = [(a+b)/2 for a,b in zip(low,high)]
                fit = 5/max(b-a for a,b in zip(low,high))
                side = 5 if slot==57 else -5
                display = [Vec((p.x-center[0])*fit+side,(p.y-center[1])*fit,
                               (p.z-center[2])*fit) for p in positions]
                material.flags |= 0x00800000
            vertices = (Vec*n)(*display)''')
    swap('draw(C.byref(native_model),n,0,1,','draw(C.byref(native_model),n,n if environment else 0,1,')
    swap('deferred_vq_mip_slots=sorted(deferred_slots)',
         'vq_terminal_mip_verified_slots=sorted(vq_words),vq_texture_words=list(vq_words.values())')
    swap('deferred_environment_slots=sorted(environment_slots)',
         'environment_slots=sorted(environment_slots),environment_texture_words=list(environment_words.values()),environment_meshes_repositioned_for_visibility=True')
    namespace = dict(vars(textured),verify_terminal_mip=vq.verify_terminal_mip)
    exec(compile(source,textured.__file__,'exec'),namespace)
    return namespace['draw_textured_land'](context,track)


def scene_caller(raw):
    raw = vq.scene_caller(raw).replace('from verify_native_emerald_vq_scene import draw_land',
                                     'from verify_native_emerald_environment_scene import draw_land')
    anchor = "    land['nonempty_vq_stage_batches']=len(vq_batches)"
    assert raw.count(anchor)==1
    raw = raw.replace(anchor,anchor+'''
    env_words=set(land['environment_texture_words'])
    env_batches=[b for b in land_batches if b.header[3]&0x1fffff in env_words]
    check(len(env_words)==2 and {b.header[3]&0x1fffff for b in env_batches}==env_words,
          'nonempty stage batches use both original environment texture slots')
    env_uvs={(v.u,v.v) for b in env_batches
             for v in decoded.vertices[b.first_vertex:b.first_vertex+b.vertex_count]}
    check(len(env_uvs)>1 and all(-0.001<=u<=1.001 and -0.001<=v<=1.001 for u,v in env_uvs),
          'environment mapping emits varied normal-derived coordinates')
    land['nonempty_environment_stage_batches']=len(env_batches)
    land['distinct_environment_coordinates']=len(env_uvs)
''')
    return raw.replace('sonic_emerald_vq_scene','sonic_emerald_environment_scene').replace(
        'original_Emerald_Coast_VQ_mipmapped_land_and_Sonic_rendered_together',
        'original_Emerald_Coast_environment_mapped_land_and_Sonic_rendered_together')


def exercise(context,inputs,track):
    source = inspect.getsource(textured.scene.exercise)
    namespace = dict(vars(textured.scene),__file__=__file__,scene_caller=scene_caller)
    exec(compile(source,textured.scene.__file__,'exec'),namespace)
    namespace['exercise'](context,inputs,track)


if __name__=='__main__':
    runner = Path(textured.scene.__file__).read_text().split("if __name__ == '__main__':\n",1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor)==1
    runner = runner.replace(anchor,'from verify_native_emerald_environment_scene import exercise')
    exec(compile('if True:\n'+runner,textured.scene.__file__,'exec'),dict(vars(textured.scene)))
