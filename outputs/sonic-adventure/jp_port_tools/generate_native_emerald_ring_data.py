"""Adapt locally generated JP model and animation declarations for native draw."""
import re

from build_native_simple_material import digest


def generate(gen):
    model = gen/'1st_read/d_8c1b5a18.c'
    frames = gen/'src/object/ring/ring_texanims.inc'
    assert digest(model) == '2b32672d8c3a40c7f97e00f5f55c904abe42ac2ca362ba6437f9070620e9c215'
    assert digest(frames) == '4b7aca9b6e5eea93ff5f1e111935e8935585d39b95680fefe0fff76d470707fe'
    text = model.read_text(encoding='utf-8')

    def declaration(name):
        return re.search(r'^\w+ '+name+r'(?:\[\])? = \{.*?^\};', text, re.M|re.S).group()

    def entries(name):
        return declaration(name).split('{', 1)[1].rsplit('}', 1)[0]

    strips = [int(value) for value in entries('dat_8c1b8440').split(',') if value.strip()]
    uv = re.findall(r'UV\(\d+, \d+\)', entries('dat_8c1b8514'))
    indices, corners = [], []
    cursor = corner = 0
    for _ in range(3):
        header = strips[cursor] & 0xffff
        count = header & 0x7fff
        cursor += 1
        assert 3 <= count <= len(strips)-cursor and count <= len(uv)-corner
        for j in range(count-2):
            order = (j, j+2, j+1) if (header//32768+j)&1 else (j, j+1, j+2)
            indices.extend(strips[cursor+k] for k in order)
            corners.extend(uv[corner+k] for k in order)
        cursor += count
        corner += count
    assert cursor == len(strips) and corner == len(uv) and max(indices) < 48

    material = declaration('dat_8c1b842c')
    assert material.count(', 50,') == 1
    material = material.replace(', 50,', ', 0,')
    data = '\n'.join('static '+part for part in (
        declaration('dat_8c1b86c4'), declaration('dat_8c1b8904'), material))
    return data + f"""
static Sint16 stage_ring_indices[] = {{{', '.join(map(str, indices))}}};
static NJS_COLOR stage_ring_uv[] = {{{', '.join(corners)}}};
static NJS_MESHSET stage_ring_mesh = {{
    NJD_MESHSET_3, {len(indices)//3}, stage_ring_indices, 0, 0, 0, stage_ring_uv
}};
static {declaration('ring_model').replace('ring_model', 'stage_ring_model').replace('dat_8c1b86ac', '&stage_ring_mesh')}
static const struct simple_mesh_span stage_ring_span = {{{len(indices)}, {len(corners)}}};
static NJS_TEXANIM stage_ring_frames[8] = {{
{frames.read_text(encoding='utf-8')}}};
static const NJS_SPRITE stage_ring_sprite = {{{{0}}, 1.0F, 1.0F, 0, 0, stage_ring_frames}};
"""
