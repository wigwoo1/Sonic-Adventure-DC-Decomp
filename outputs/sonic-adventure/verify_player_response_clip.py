"""Complete line-clip software comparisons and independent exact finite oracles."""
import hashlib,itertools,json,struct
from pathlib import Path
import player_response_clip_fixtures as q
P=q.P;ENTRY=0x8c64f5e8
IN,OUT,POS,COL,UV,DP,DC,DU=tuple(0x8cb00000+0x1000*j for j in range(8))
NEAR=0x8c88f610
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    target=P/'analysis/player_response_clip_verification.json';assert not target.exists()
    ram=q.v.seed_ram();records=[];seen=set();edges=set();checks=oracles=0
    def require(ok,message):
        nonlocal checks
        assert ok,message;checks+=1
    for zs,flags,textured,mode,rm in itertools.product(((0,2),(2,0),(2,3),(0,.5),(1,1),(1,2),(2,1),(1,0),(0,1)),(0,0x80000000),(False,True),(0,4),(0,1)):
        if flags and not textured and min(zs)<1<max(zs):continue
        c=dict(mode=mode,rm=rm)
        m,s,f,cfg=q.prepare(ram,c);cfg['entry']=s.pc=ENTRY
        for a in (IN,OUT,POS,COL,UV,DP,DC,DU):m[a&0xffffff:(a&0xffffff)+0x100]=bytes([0xcd])*0x100
        for base,values in ((IN,(POS,COL,UV)),(OUT,(DP,DC,DU if textured else 0))):
            for j,value in enumerate(values):q.put(m,base+4*j,value)
        q.put(m,NEAR,q.bits(1))
        points=((0,2,zs[0]),(4,6,zs[1]));colours=((20,40,60,80),(100,120,140,160));uv=((-300,200),(100,-200))
        for j in range(2):
            q.vector(m,POS+12*j,points[j])
            for k,value in enumerate(colours[j]):q.put(m,COL+4*j+k,value,1)
            for k,value in enumerate(uv[j]):q.put(m,UV+4*j+2*k,value,2)
        s.r[4:7]=(IN,OUT,flags)
        before=type(s).from_buffer_copy(s)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        require(result['status']==0,(zs,flags,textured,mode,rm,result))
        require(result['full_state_match'] and result['ordered_boundaries_match'],'whole state and ordered boundaries')
        require(s.pc==s.pr==before.pr and s.r[15]==before.r[15],'normal return and restored stack')
        require(list(s.r[8:15])==list(before.r[8:15]),'callee saved registers')
        crossing=(zs[0]-1)*(zs[1]-1)<0
        expected_p=bytearray([0xcd]*24);expected_c=bytearray([0xcd]*8);expected_u=bytearray([0xcd]*8)
        slot=0
        if crossing:
            # Both crossing pairs use an exactly representable half-way intersection.
            require(set(zs)=={0,2},'independent oracle domain')
            expected_p[:12]=struct.pack('<fff',2,4,1)
            expected_c[:4]=bytes((60,80,100,120))
            if flags:expected_u[:4]=struct.pack('<hh',-100,0)
            slot=1
        if zs[0]>=1:
            expected_p[slot*12:slot*12+12]=struct.pack('<fff',*points[0]);expected_c[slot*4:slot*4+4]=bytes(colours[0])
            if textured:expected_u[slot*4:slot*4+4]=struct.pack('<hh',*uv[0])
        if zs[1]>=1:
            expected_p[12:24]=struct.pack('<fff',*points[1]);expected_c[4:8]=bytes(colours[1])
            if textured:expected_u[4:8]=struct.pack('<hh',*uv[1])
        for base,expected in ((DP,expected_p),(DC,expected_c),(DU,expected_u)):
            require(bytes(m[base&0xffffff:(base&0xffffff)+len(expected)])==bytes(expected),'independent exact output and untouched bytes');oracles+=1
        require(s.r[0]==(2 if max(zs)>=1 else 0),'independent inclusive near-plane return value');oracles+=1
        seen.update(path)
        for i,a in enumerate(path):
            t=q.v.ROWS.get(a,'');skip=2 if t.startswith(('bt/s ','bf/s ')) else 1
            if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
        records.append(dict(z=zs,flags=flags,output_uv=textured,mode=mode,rm=rm,status=0,steps=len(path),return_value=s.r[0]))
        if len(records)%32==0:print('LINE CLIP',len(records),'comparisons',flush=True)
    t=json.loads((P/'analysis/player_response_clip_translation.json').read_text());body=t['routines'][0]
    wanted=set(body['reachable_addresses']);outcomes={tuple(x) for x in body['conditional_edges']}
    missing=wanted-{hex(a) for a in seen};missed=outcomes-edges
    require(not missing and not missed,('complete body coverage',sorted(missing),sorted(missed)))
    files=[Path(__file__),P/'player_response_clip_fixtures.py',P/'player_response_clip_reference.py',P/'write_watch_player_response_clip.py',P/'analysis/player_response_clip_translation.json',P/'analysis/player_response_clip_connection.json',q.v.effects.DLL]
    report=dict(status='complete_original_line_clip_matches_in_declared_software_domain',original_native_comparisons=len(records),checks=checks,independent_exact_oracles=oracles,cases=records,
        coverage=dict(address=hex(ENTRY),static_rows=len(wanted),executed_rows=len(wanted),static_outcomes=len(outcomes),executed_outcomes=len(outcomes),missing_rows=[],missing_outcomes=[]),
        new_original_routine_credit=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,overall_completion_percent=10,dll_sha256=sha(q.v.effects.DLL),
        scope='Complete original296-row entry;16conditional outcomes. Finite near-plane crossing in both directions,visible/hidden/exact-plane endpoints,texture flag/output presence and four retained mode/rounding combinations. Exact geometry/colour/signed-UV results and unchanged slots checked independently. Endpoint equality retains the original fixed output-slot behavior. No physical hardware,all nonfinite/alias domains,or whole renderer qualification is claimed.',
        sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n');print('LINE CLIP COMPLETE',len(records),checks,oracles,flush=True)
if __name__=='__main__':main()
