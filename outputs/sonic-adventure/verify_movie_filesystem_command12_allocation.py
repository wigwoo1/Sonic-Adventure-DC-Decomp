"""Independent original FAT-space, head-collision and descending allocation vectors."""
from pathlib import Path
from movie_filesystem_command12_vectors import *

def setup(base,entry,args,endian=0,limit=8,free=(),heads=(),policy=0):
    m,s,f,c=fresh(base,entry,args,4 if endian else 0,0x400000a1);d=base.descriptor;meta=v.load(m,d+0x1c);fat=v.load(m,d+0x24);directory=v.load(m,d+0x28)
    for off in (2,8,10,11,12):put(m,d+off,1,1)
    put(m,base.d['header'],1,1);put(m,base.d['status'],1,1);put(m,base.d['status']+0xc,0,1);put(m,base.d['info'],2);put(m,0x8c89cf90+base.channel*4,0)
    raw(m,v.load(m,d+0x20),b'U'*16+bytes(496));put(m,meta,511);put(m,meta+0x30,limit);put(m,meta+0x38,4);put(m,0x8c66d3a4,policy)
    order='big' if endian else 'little';blob=bytearray((0xfffa).to_bytes(2,order)*512)
    for i in free:blob[i*2:i*2+2]=(0xfffc).to_bytes(2,order)
    raw(m,fat,blob);entries=bytearray(1024)
    for i,(type_,head) in enumerate(heads):entries[i*32]=type_;entries[i*32+2:i*32+4]=(head&65535).to_bytes(2,order)
    raw(m,directory,entries);return m,s,f,c,fat

def check(e,m,s,f,c,label,result,wanted=None):
    wanted=bytearray(m) if wanted is None else wanted;sp=s.r[15];r,a,s,f=e.compare(m,s,f,c,label);raw(wanted,sp-160,v.sample(a,sp-160,160));e.require(a==wanted,'independent exact nonstack RAM/FAT/directory/policy;160byte ABI upper scratch');e.require(s.r[0]==result&0xffffffff,'independent query/allocation result')

def main():
    e=Evidence();ram=v.seed_ram()
    queries=[(0,0,8,(),()),(0,0,8,(0,2,7),()),(0,1,8,(0,2,7),((0x33,2),)),(0,1,8,(0,2,7),((0xcc,7),(0x33,2))),(0,1,8,(0,2,7),((1,2),)),(1,0,8,(),()),(1,0,8,(0,1,2),()),(1,0,8,(0,1,2,3),()),(2,0,8,(1,7),()),(0,0,0,(),()),(0,1,-1,(),())]
    for endian in (0,1):
        base=Session(ram,channel=7 if endian else 0,endian=endian);channel=base.channel
        for mode,policy,limit,free,heads in queries:
            m,s,f,c,fat=setup(base,0x8c648b40,[channel,mode],endian,limit,free,heads,policy)
            if mode==1:
                result=0
                while result<4 and result in free:result+=1
            else:result=sum(i in free and (not policy or all(typ not in (0x33,0xcc) or (head&65535)!=i for typ,head in heads)) for i in range(max(0,limit)))
            check(e,m,s,f,c,dict(kind='free_count',endian=endian,mode=mode,policy=policy,limit=limit,free=free,heads=heads),result)
        for query,heads in ((0,()),(0x10002,((0x33,2),)),(2,((1,2),)),(7,((0xcc,7),)),(0xffff,((0x33,1),(0xcc,0xffff))),(5,((0x33,1),(0xcc,7))),(2,((1,2),(0x33,2),(0xcc,7)))):
            m,s,f,c,fat=setup(base,0x8c64b12a,[channel,query],endian,8,(),heads);result=-1 if any(typ in (0x33,0xcc) and (head&65535)==query&65535 for typ,head in heads) else query&65535
            check(e,m,s,f,c,dict(kind='head_collision',endian=endian,query=query,heads=heads),result)
        for free,heads in (((),()),((0,),()),((7,),()),((5,6,7),((0xcc,7),(0x33,6))),((7,),((0xcc,7),)),((0,),((0x33,0),)),((2,7),((1,7),))):
            m,s,f,c,fat=setup(base,0x8c64b0e4,[channel],endian,8,free,heads);wanted=bytearray(m);chosen=-1
            for candidate in sorted(free,reverse=True):
                if any(typ in (0x33,0xcc) and head==candidate for typ,head in heads):raw(wanted,fat+candidate*2,(0xfffa).to_bytes(2,'big' if endian else 'little'))
                else:chosen=candidate;break
            check(e,m,s,f,c,dict(kind='allocate_block',endian=endian,free=free,heads=heads),chosen,wanted)
    e.report('movie_filesystem_command12_allocation_verification.json',{0x8c648b40,0x8c64b12a,0x8c64b0e4},[Path(__file__),P/'analysis/movie_filesystem_command12_audit.json'],'Declaredcoherent mappedmetadata/FAT/directory/policy inputs. Originalcountmode1leadingprefix/othermodescount/optionalnonzero-policyheadexclusion,SDKendianheadmatchinglow16 anddescendingallocationwithcollidingheadblocksreservedFFFA. IndependentcompleteRAM/return/160byteupperABIscratch plusfullCPU/FPUcomparison;alloriginalhelpersrun. Nonzero66d3a4 isexplicitsoftwareinput;naturalsetter/acquisitionnotclaimed. Notphysicalstorage/gameproof.')
if __name__=='__main__':main()
