"""Actual renderer initialization, recursive wrapper and nonempty mesh submission."""
import hashlib,itertools,json
from fractions import Fraction
from pathlib import Path
import movement_model_response_fixtures as q
import verify_draw_material_dispatch as material
P=q.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ram=q.v.seed_ram();records=[];checks=0
    def require(ok,label):
        nonlocal checks
        assert ok,(mode,entry,flags,rm,label);checks+=1
    configurations=[(mode,entry,flags,0) for mode,entry,flags in itertools.product((0,4),(q.WALK,q.WRAP),(0,0x100000,0x200000,0x300000))]
    configurations += [(mode,entry,0,1) for mode,entry in itertools.product((0,4),(q.WALK,q.WRAP))]
    for mode,entry,flags,rm in configurations:
        m,s,f,cfg=q.prepare(ram,dict(entry=entry,type=7,flags=16,mesh_gate=0,mode=mode))
        ftype=type(f);inputs=dict(entry=material.ENTRY,flags=flags,mode=mode,depths=(2,2,2),seed=1)
        m,unused,mesh_f=material.prepare(m,inputs);f=ftype.from_buffer_copy(mesh_f);f.fpscr=(mode<<19)|0x40000|rm
        at=material.MODEL&0xffffff;out=q.MESHES&0xffffff;m[out:out+0x28]=m[at:at+0x28]
        vertices,normals=0x8cb70000,0x8cb80000
        q.put(m,q.MESHES,vertices);q.put(m,q.MESHES+4,normals);q.put(m,q.MESHES+8,4)
        for j in range(8):q.vector(m,vertices+j*12,((j%2)*.5,(j//2%2)*.5,4));q.vector(m,normals+j*12,(0,0,1))
        q.vector(m,q.MESHES+0x18,(0,0,2));q.put(m,q.MESHES+0x24,q.bits(1));q.put(m,0x8c754ec8,0)
        clip=q.get(m,0x8c037210)
        for j,x in enumerate((-1,-1,1,1)):q.put(m,clip+j*4,q.bits(x))
        for phase,target in enumerate((0x8c036f60,entry)):
            cfg['entry']=target;s.pc=target;s.pr=0x8cfed000;s.r[4]=q.NODES;sp=s.r[15];before=bytes(m)
            matrix=[Fraction(q.number(x)) for x in f.banks[((f.fpscr>>21)&1)^1]]
            r,path,m,s,f=q.fast.compare(m,s,f,cfg);cfg['gbr']=r['final_gbr']
            require(r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],'actual initializer/accepted mesh full-state and ordered boundary comparison')
            require(s.pc==s.pr==0x8cfed000 and s.r[15]==sp,'normal entry/return without supplied frames')
            if phase==0:require(r['final_gbr']==q.get(m,0x8c036fb4),'actual original initializer installs renderer context')
            else:
                targets=[b['target'] for b in r['boundaries']]
                require(all(hex(a) in targets for a in (0x8c037098,0x8c03718c,0x8c037294,0x8c037350,0x8c0376d0,0x8c036ffc)),'accepted original model cull/project/shade/material/primitive/flag chain executes')
                primitive=material.fixtures.UV if flags&0x200000 else material.fixtures.SOLID
                require(primitive in path,'actual original nonempty solid/textured primitive child executes')
                cache=q.get(before,0x8c88f64c);cx,cy,scale,aspect=[Fraction(q.number(q.get(before,0x8c8ffe00+j*4))) for j in range(4)]
                for i in range(4):
                    point=[Fraction(q.number(q.get(before,vertices+i*12+j*4))) for j in range(3)]+[Fraction(1)]
                    transformed=[sum(matrix[k*4+j]*point[k] for k in range(4)) for j in range(3)]
                    x,y,z=transformed;expected=[q.bits(float(x*scale/z+cx)),q.bits(float(y*scale*aspect/z+cy)),q.bits(float(1/z))]
                    require([q.get(m,cache+i*16+j*4) for j in range(3)]==expected,'independent exact dyadic raw vertex projection into original cache')
                # The original wrapper installs its renderer-state record's
                # packet cursor at890114; the bare traversal uses the live one.
                state_slot=q.get(before,0x8c605ec0)
                packet=q.get(before,q.get(before,state_slot)) if entry==q.WRAP else q.get(before,0x8c890114)
                output=packet&0xffffff
                require(m[output:output+0x100]!=before[output:output+0x100],'actual material and nonempty primitive emit packet bytes')
                require(q.get(m,0x8c88f69c)==q.get(before,0x8c88f69c) and q.get(m,0x8c88f5f8)==q.get(before,0x8c88f5f8),'matrix stack stays balanced across real mesh callbacks')
            records.append(dict(mode=mode,entry=hex(target),flags=hex(flags),rm=rm,phase=phase,status=0,original_rows=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
    files=[Path(__file__),P/'movement_model_response_fixtures.py',P/'verify_draw_material_dispatch.py',P/'verify_draw_primitive_stream.py',P/'verify_draw_geometry_stream.py',P/'verify_draw_gbr_geometry.py',q.v.MP,P/'analysis/movement_model_response_connection.json',q.v.effects.DLL]
    report=dict(original_native_comparisons=len(records),independent_checks=checks,actual_nonempty_mesh_sequences=len(configurations),cases=records,
      supplied_game_callee_outputs=0,supplied_execution_frames=0,new_original_routine_credit=0,dll_sha256=digest(q.v.effects.DLL),sha256={str(p):digest(p) for p in files},
      scope='Actual036f60 renderer initializer followed by036bc0/036f12 normal-entry traversal and accepted037098 mesh callback. Four raw vertices,original projection/lighting/material dispatch and solid/textured/alpha primitive children;bothFPbanks,RM0plusselectedRM1. Initial mesh/material/clip/runtime inputs reuse existing authored fixture,not intermediate results/frames. Independent exactdyadic fourvertex projection and packet-write/stack/context assertions. FullsoftwareCPU/FPU/RAM/device/queue/orderedstate match;not GPU pixels/fullgame or complete oldcallbackqualification.')
    (P/'analysis/movement_model_response_pipeline_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('MODEL RESPONSE PIPELINE PASS',len(records),checks,flush=True)

if __name__=='__main__':main()
