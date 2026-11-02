"""Recover the original source-record registration/initialization dependency."""
import csv,hashlib,json,struct
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'recover_draw_matrix_registers.py'
exec(compile(parent.read_text().split('functions=[];routines=[]')[0],str(parent),'exec'),globals())
chosen=chosen.replace('main-dispatch-model-traversal/','model-view-producer/')
chosen=chosen.replace("('fipr ','ftrv ')","('fipr ','ftrv ','fsrra ','fsca ')")
mark="        assert not text.startswith('f'),text";assert chosen.count(mark)==1
chosen=chosen.replace(mark,"        if text.startswith('fsqrt '):return f'{{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[{n}],&f->fpscr,&fr[{n}]);if(status)return status;}}'\n"+mark)
header+='\nint sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);\n'
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Original source-record initialization and scalar/vector dependencies; names inferred.')
pc,table=0x8c63aa10,0x8c63aa14
assert binary[pc-0x8c010000:pc-0x8c010000+4]==bytes.fromhex('23000900')
targets=[pc+4+struct.unpack_from('<h',binary,table-0x8c010000+2*i)[0] for i in range(17)]
BRAF_TARGETS={pc:set(targets)}
cfg='        if w==0xB or w>>12 in (0xA,0xB) or w&0xF0FF in (0x400B,0x402B):';assert chosen.count(cfg)==1
chosen=chosen.replace(cfg,'''        if w==0x0023:
            assert a in BRAF_TARGETS and a+2 in rows
            seen.add(a+2);todo.extend(BRAF_TARGETS[a]);continue
'''+cfg)
mark='        elif w>>12==0xA:';assert chosen.count(mark)==1
chosen=chosen.replace(mark,'''        elif w==0x0023:
            code=f'target=0x{a+4:x}u+r[0];'+simple(a+2)+'switch(target){'+''.join(f'case 0x{t:x}u:goto L{t:x};' for t in sorted(BRAF_TARGETS[a]))+f'default:*stop=0x{a:x}u;return -3;}}'
            slots.add(a+2);terminal=True
'''+mark)
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
functions=[];routines=[]
for start,count in ((0x8c63aa04,36),(0x8c63aa74,16),(0x8c63aa94,171)):
 listing=P/f'analysis/model-view-producer/{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'))
 end=max(int(r['address'],16)+2 for r in rows)
 function,record=recover(start,end,f'sa_model_view_producer_{start:08x}',count)
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sx;(void)m;(void)sr;(void)wr;(void)fload;(void)fstore;(void)stop;')
 functions.append(function);routines.append(record)
target=P/'source/sa_model_view_producer.c';assert not target.exists();target.write_text(header+'\n'.join(functions))
prior=P/'source/sa_renderer_frame_clock_connected.c';text=prior.read_text()
decls='\n'.join('DECL(sa_model_view_producer_'+r['address'][2:]+');' for r in routines)
marker='DECL(sa_overlay_object_draw_candidate_0c9058a0);';assert text.count(marker)==1;text=text.replace(marker,decls+'\n'+marker)
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert text.count(marker)==1
routes='\n'.join(f" if(target=={r['address']}u)return {r['name']}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);" for r in routines)
text=text.replace(marker,marker+'\n'+routes)
connected=P/'source/sa_model_view_producer_connected.c';assert not connected.exists();connected.write_text(text)
prior_build=P/'build_renderer_model_geometry_guarded.ps1';build=P/'build_model_view_producer.ps1';assert not build.exists()
recipe=prior_build.read_text().replace("'sa_renderer_frame_clock_connected.c'","'sa_model_view_producer_connected.c','sa_model_view_producer.c'").replace('renderer_model_geometry_guarded.dll','model_view_producer.dll')
build.write_text(recipe)
files=[Path(__file__),parent,P/'recover_overlay_model_blend.py',P/'recover_main_dispatch_particle_callback.py',P/'recover_draw_render_headers.py',TEMPLATE,TRANSLATOR,BINARY,target,prior,connected,prior_build,build]+[Path(r['listing']) for r in routines]
report=dict(status='byte_checked_static_source_record_producer_behavior_proof_separate',routines=routines,original_instruction_rows=223,computed_switch=dict(pc=hex(pc),table=hex(table),entries=list(map(hex,targets))),prior_sources_preserved=True,new_original_routine_credit=0,sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
out=P/'analysis/model_view_producer_translation.json';assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
