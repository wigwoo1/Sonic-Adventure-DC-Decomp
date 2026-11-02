"""Qualify real native task source: heap, failure rollback, callbacks and transforms."""
import argparse
import ctypes as C
import json
import math
import random
from pathlib import Path
from native_task_test_support_v2 import Runtime,Task,Work,Spec,Vector,Angles,Matrix,put_pointer,sha

def basic_heap_and_pool(t):
    t.call('jp_heap_reset');t.verify(t.stats()==(0x90000,0x90000),'whole arena initially free')
    t.verify(not t.call('jp_heap_allocate',0),'zero allocation returns null')
    rng=random.Random(0x1007);blocks=[]
    for i in range(240):
        size=rng.randrange(1,1537);address=t.allocate(size)
        t.verify(address%16==0,'allocation alignment')
        t.verify(all(address+size<=other or other+length<=address for other,length,pattern in blocks),'live blocks disjoint')
        pattern=(i*31+7)&255;C.memset(address,pattern,size);blocks.append((address,size,pattern))
    rng.shuffle(blocks)
    for address,size,pattern in blocks:
        t.verify(C.string_at(address,size)==bytes([pattern])*size,'live payload retained')
        t.call('jp_heap_release',address)
    t.verify(t.stats()==(0x90000,0x90000),'arbitrary frees coalesce complete heap')
    baseline=t.fresh()
    tasks=[t.call('task_create',0,7,None) for _ in range(4096)]
    t.verify(all(tasks) and len(set(tasks))==4096,'all 4096 task slots unique')
    t.verify(not t.call('task_create',0,7,None),'pool exhaustion returns null')
    for address in tasks:t.call('task_destroy',address)
    t.verify(t.stats()==baseline,'all task slots recovered without changing heap pool')
    return dict(heap_live_payloads=240,task_slots_exhausted_and_recovered=4096)

def allocation_failures(t):
    baseline=t.fresh();cases=0
    costs=[(2,128),(4,96),(1,112),(8,48)]
    # These are host record sizes, with the original allocator's 16-byte blocks.
    actual=[(2,t.layout.work_bytes),(4,2*t.layout.force_bytes),(1,t.layout.motion_bytes),(8,16)]
    t.verify(costs==[(bit,(size+t.layout.heap_header_bytes+15)&~15) for bit,size in actual],
             'native allocation costs match compiled record sizes')
    for flags in range(1,16):
        previous=0
        for bit,cost in costs:
            if not flags&bit:continue
            probe=t.call('task_create',0,7,None);t.verify(bool(probe),'free-list probe')
            t.call('task_destroy',probe)
            room=previous+32;filler=t.leave_heap_room(room)
            t.verify(not t.call('task_create',flags,0,None),'auxiliary allocation failure returns null')
            t.verify(t.stats()==(room,room),'failed creation frees all prior auxiliary records')
            recovered=t.call('task_create',0,7,None)
            t.verify(recovered==probe,'failed creation returns the task slot')
            t.call('task_destroy',recovered);t.call('jp_heap_release',filler)
            t.verify(t.stats()==baseline,'failed creation restores heap after filler release')
            previous+=cost;cases+=1
    # Successful all-record allocation with exactly enough real heap space.
    filler=t.leave_heap_room(sum(cost for _,cost in costs))
    task=t.call('task_create',15,0,None);t.verify(bool(task),'exact-fit task creation succeeds')
    t.verify(t.stats()==(0,0),'all auxiliary record allocations consume exact-fit remainder')
    t.call('task_destroy',task);t.call('jp_heap_release',filler)
    t.verify(t.stats()==baseline,'exact-fit records coalesce on destruction')
    # Regression for the observed 16-byte-tail overwrite. The native allocator
    # must consume the entire free block, leaving only the intact end header.
    largest=t.stats()[1]
    filler=t.call('jp_heap_allocate',largest-16-t.layout.heap_header_bytes)
    t.verify(bool(filler),'short-tail regression allocation succeeds')
    t.verify(t.stats()==(0,0),'native allocator absorbs a tail smaller than its header')
    t.verify(not t.call('jp_heap_allocate',1),'end header remains a terminating empty free-list tail')
    t.call('jp_heap_release',filler)
    t.verify(t.stats()==baseline,'short-tail block frees and coalesces without a cycle')
    # Initialization failure before a pool exists.
    t.call('jp_heap_reset');filler=t.leave_heap_room(32);t.call('task_init')
    t.verify(not t.call('task_create',0,0,None),'pool initialization OOM leaves no task slot')
    t.verify(t.stats()==(32,32),'failed pool allocation leaves heap intact')
    t.call('jp_heap_release',filler);t.fresh()
    return dict(auxiliary_failure_cases=cases,exact_fit_case=True,pool_initialization_failure=True)

def callbacks_and_deletion(t):
    baseline=t.fresh();steps=[];draws=[];deletes=[]
    # A failing ctypes callback must never be swallowed into a passing proof.
    def sentinel(address):raise RuntimeError('callback propagation sentinel')
    guarded=t.callback(sentinel)
    from native_task_test_support_v2 import Callback
    C.cast(guarded,Callback)(None)
    t.verify(t.callback_errors==['RuntimeError: callback propagation sentinel'],'callback errors captured')
    t.callback_errors.clear()
    callbacks={}
    def step(label):
        def body(address):
            steps.append(label)
            t.verify(t.lib.task_get_current_root_step()==callbacks[label],'current root step callback')
        callbacks[label]=t.callback(body)
        return callbacks[label]
    roots=[t.call('task_create',15,level,step(label)) for level,label in [(0,'zero'),(2,'two'),(8,'first')]]
    t.verify(all(roots),'roots with all auxiliary records allocated')
    for i,address in enumerate(roots):
        record=Task.from_address(address)
        for field,size in [('work',t.layout.work_bytes),('motion',t.layout.motion_bytes),('force',2*t.layout.force_bytes),('extra',16)]:
            t.verify(C.string_at(getattr(record,field),size)==bytes(size),'auxiliary bytes initialized')
        record.draw=t.callback(lambda a,i=i:draws.append('root'+str(i)))
        record.destructor=t.callback(lambda a,i=i:deletes.append('root'+str(i)))
    child=t.call('task_create_child',2,t.callback(lambda a:steps.append('child')),roots[0])
    t.verify(Task.from_address(child).parent==roots[0] and Task.from_address(roots[0]).child==child,'child links')
    Task.from_address(child).draw=t.callback(lambda a:draws.append('child'))
    Task.from_address(child).destructor=t.callback(lambda a:deletes.append('child'))
    t.call('task_step_all');t.verify(steps==['first','zero','two'],'root scheduling order')
    t.verify(not t.call('task_get_current_root_step'),'root callback cleared after scheduling')
    t.call('task_step_children',roots[0]);t.verify(steps[-1]=='child','explicit child stepping')
    t.call('task_draw_all');t.verify(draws==['root2','root0','child','root1'],'parent then child draw order')
    t.call('task_destroy_all');t.verify(deletes==['root0','child','root1','root2'],'destructor hierarchy order')
    t.verify(t.stats()==baseline,'hierarchy destruction frees all records')
    # Iterators save their successor before a callback destroys the current task.
    for phase in ['step','draw','child_step']:
        log=[];parent=t.call('task_create',2,0,None) if phase=='child_step' else None
        def remove_current(address):log.append(address);t.call('task_destroy',address)
        callback=t.callback(remove_current)
        if parent:
            tasks=[t.call('task_create_child',2,callback,parent) for _ in range(3)]
            expected=list(reversed(tasks));t.call('task_step_children',parent)
            t.verify(not Task.from_address(parent).child,'children detach while stepping')
            t.call('task_destroy',parent)
        else:
            tasks=[t.call('task_create',2,0,callback if phase=='step' else None) for _ in range(3)]
            if phase=='draw':
                for address in tasks:Task.from_address(address).draw=callback
            expected=list(reversed(tasks));t.call('task_step_all' if phase=='step' else 'task_draw_all')
        t.verify(log==expected,'current-task deletion preserves '+phase+' iteration')
        t.verify(t.stats()==baseline,'callback destruction releases auxiliary records')
    destroyed=[];victim=t.call('task_create',2,0,None)
    Task.from_address(victim).destructor=t.callback(lambda a:destroyed.append(a))
    t.call('task_kill',victim);t.verify(not destroyed,'kill defers destruction')
    t.call('task_step_all');t.verify(destroyed==[victim],'next step performs scheduled destruction once')
    t.verify(t.stats()==baseline,'scheduled destruction frees records')
    return dict(self_deleting_iteration_phases=3,callback_failures_propagate=True)

def real_cleanup(t):
    baseline=t.fresh();task=t.call('task_create',2,0,None);work=Task.from_address(task).work
    collision=t.allocate(t.layout.collision_bytes);shapes=t.allocate(2*56)
    C.c_uint16.from_address(collision+t.layout.collision_flags).value=0x20
    put_pointer(collision,t.layout.collision_shapes,shapes);put_pointer(work,t.layout.work_collision,collision)
    event=t.allocate(t.layout.event_bytes);put_pointer(work,t.layout.work_event,event)
    motions=[t.allocate(t.layout.event_motion_bytes) for _ in range(3)]
    for first,second in zip(motions,motions[1:]):put_pointer(first,t.layout.event_motion_next,second)
    put_pointer(event,t.layout.event_motions,motions[0]);put_pointer(event,t.layout.event_previous,t.allocate(t.layout.event_motion_bytes))
    paths=[t.allocate(t.layout.event_path_bytes) for _ in range(2)]
    put_pointer(paths[0],t.layout.event_path_next,paths[1]);put_pointer(event,t.layout.event_paths,paths[0])
    t.call('task_destroy',task)
    t.verify(t.stats()==baseline,'owned shapes and motion/previous/path chains freed without leaks')
    task=t.call('task_create',2,0,None);work=Task.from_address(task).work
    collision=t.allocate(t.layout.collision_bytes);shapes=t.allocate(56)
    put_pointer(collision,t.layout.collision_shapes,shapes);put_pointer(work,t.layout.work_collision,collision)
    C.memset(shapes,0x5a,56);t.call('collision_free',work)
    t.verify(not Work.from_address(work).collision,'collision pointer cleared')
    t.verify(C.string_at(shapes,56)==bytes([0x5a])*56,'non-owned shapes retained')
    t.call('collision_free',work);t.call('event_free',work);t.call('event_free',work)
    t.call('jp_heap_release',shapes);t.call('task_destroy',task)
    t.verify(t.stats()==baseline,'non-owned shapes freed once by their owner')
    return dict(event_motions=3,previous_motions=1,event_paths=2,owned_and_nonowned_shapes=True)

def child_creation_transforms(t):
    baseline=t.fresh();cases=0
    for angles in [(0,0,0),(0,0,0x4000),(0x4000,0,0),(0,0x4000,0),(0x1800,-0x1000,0x2400)]:
        parent=t.call('task_create',2,0,None);work=Work.from_address(Task.from_address(parent).work)
        work.pos=Vector(10,-2,5);work.ang=Angles(*angles)
        existing=t.call('task_create_child',0,t.callback(lambda a:None),parent)
        callback=t.callback(lambda a:None);specs=(Spec*4)()
        positions=[(2,0,0),(0,3,0),(0,0,-4)]
        for i,pos in enumerate(positions):
            specs[i]=Spec(callback,2,-123,Angles(i*100,-i*200,i*300),Vector(*pos),-2147483648+i)
        # Sentinel has no callback. Save a non-unit current matrix across the call.
        original=Matrix(*[float(i+1)/8 for i in range(16)])
        t.lib.njSetMatrix(None,C.byref(original));before=t.current_matrix()
        first=t.call('task_create_children',specs,parent)
        t.verify(first==Task.from_address(parent).child,'new children prepended as one ordered chain')
        address=first;previous=None
        for i,pos in enumerate(positions):
            child=Task.from_address(address);cw=Work.from_address(child.work)
            t.verify(child.parent==parent and child.prev==previous,'bulk child parent/previous links')
            # Independent host trigonometry: the original Z-X-Y matrix composition
            # applies Y, then X, then Z to a row vector.
            x,y,z=pos
            ax,ay,az=[(value&0xffff)*math.tau/65536 for value in angles]
            x,z=x*math.cos(ay)+z*math.sin(ay),-x*math.sin(ay)+z*math.cos(ay)
            y,z=y*math.cos(ax)-z*math.sin(ax),y*math.sin(ax)+z*math.cos(ax)
            x,y=x*math.cos(az)-y*math.sin(az),x*math.sin(az)+y*math.cos(az)
            expected=(x+10,y-2,z+5)
            t.verify(max(abs(a-b) for a,b in zip((cw.pos.x,cw.pos.y,cw.pos.z),expected))<0.00001,'bulk child transformed world position')
            t.verify(tuple((cw.ang.x,cw.ang.y,cw.ang.z))==tuple(a+b for a,b in zip(angles,(i*100,-i*200,i*300))),'bulk child relative angles')
            t.verify(cw.flags==-123 and cw.vars.l[0]==-2147483648+i,'bulk child scalar fields remain 32-bit')
            previous,address=address,child.next
        t.verify(address==existing and Task.from_address(existing).prev==previous,'old child list retained after new chain')
        t.verify(t.current_matrix()==before and C.c_int32.in_dll(t.lib,'_stack_size_').value==1,'bulk creation restores native matrix state')
        t.call('task_destroy',parent);t.verify(t.stats()==baseline,'bulk children destroyed without leaks');cases+=1
    # Partial creation continues after actual heap failures, keeping valid children.
    parent=t.call('task_create',2,0,None);partial_baseline=t.stats();filler=t.leave_heap_room(32)
    specs=(Spec*5)();callback=t.callback(lambda a:None)
    for i,flags in enumerate([2,0,2,0]):specs[i].step=callback;specs[i].create=flags
    first=t.call('task_create_children',specs,parent);t.verify(bool(first),'partial creation returns its successful chain')
    second=Task.from_address(first).next
    t.verify(bool(second) and not Task.from_address(second).next,'only allocation-free children survive partial creation')
    t.verify(not Task.from_address(first).work and not Task.from_address(second).work,'failed work allocations leave no partial work')
    t.verify(t.stats()==(32,32),'partial bulk creation leaks no failed allocations')
    t.call('jp_heap_release',filler);t.verify(t.stats()==partial_baseline,'partial creation restores free heap')
    t.call('task_destroy',parent);t.verify(t.stats()==baseline,'partial child chain cleans up')
    return dict(independent_transform_cases=cases,partial_allocation_case=True)

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--workspace',type=Path,required=True)
    ap.add_argument('--build',type=Path,required=True);a=ap.parse_args()
    t=Runtime(a.build);before=t.checks;sections={}
    for name,fn in [('heap_pool',basic_heap_and_pool),('allocation_failures',allocation_failures),
                    ('callbacks',callbacks_and_deletion),('cleanup',real_cleanup),('children',child_creation_transforms)]:
        start=t.checks;details=fn(t);sections[name]=dict(checks=t.checks-start,**details)
        print(name,t.checks-start,'checks passed',flush=True)
    p=a.workspace.resolve()/'outputs/sonic-adventure'
    for name,h in t.build['authored_source_sha256'].items():assert sha(a.workspace.resolve()/name)==h
    report=dict(status='native_task_runtime_host_contracts_verified',checks_passed=t.checks,
                ABI_checks=before,sections=sections,layout={n:getattr(t.layout,n) for n,_ in t.layout._fields_},
                DLL_sha256=sha(t.dll),build_report_sha256=sha(t.directory/'build_verification.json'),
                verifier_sha256=sha(Path(__file__)),support_sha256=sha(p/'jp_port_tools/native_task_test_support_v2.py'),
                scope='Complete task source with documented native-header split fix, exact four lifecycle bodies and authored native math; native host allocation, scheduling, ownership, failure rollback and geometric API contracts. No dummy lifecycle functions.',
                limitations=['Host API contracts, not SH-4 CPU/register equality or complete-game behavior.',
                             'Valid host-owned records and supported callback mutation patterns; deleting an arbitrary cached successor, malformed/cyclic lists and hostile size arithmetic are not qualified.',
                             'Windows x64 only; disc records still require explicit decoding/relocation. Runtime union aliases and all other game modules need separate review.',
                             'Game-containing DLL and generated external source remain private; no new gameplay or English localization claim.'],
                game_containing_artifact_publishable=False,overall_completion_percent=10,goal_paused=False,playable=False)
    target=t.directory/'runtime_verification.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
    print(t.checks,'native task runtime checks passed')

if __name__=='__main__':main()
