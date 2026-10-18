"""Recover original effect profiles, dispatcher, emitters and three main services."""
import csv,hashlib,json,struct
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT_TEMPLATE=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT_TEMPLATE.read_text().split('functions=[];routines=[]')[0],str(PARENT_TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Object-effect profiles, dispatcher and rendering services; names inferred.')
BASE_CHOSEN=chosen
marker="        assert not text.startswith('f'),text";assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,"        if text.startswith('fsca '):return f'status=sa_sh4_vector_numeric(0x{w:04x}u,f,*fpul);if(status)return status;'\n"+marker)
# The already registered four-instruction phase wrapper needs the real shared
# FSCA backend in this connection, rather than the older standalone polynomial.
# These fixed opcode annotations are checked against its canonical span hash.
phase_binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(phase_binary[0x63a9b8-0x10000:0x63a9c0-0x10000]).hexdigest()=='7649d8856eaea978ff83abe507bb62810926e7a57a800edc835e1b6214b49f8c'
phase_rows=((0x8c63a9b8,'4d64','extu.w r4,r4'),(0x8c63a9ba,'5a44','lds r4,FPUL'),(0x8c63a9bc,'0b00','rts'),(0x8c63a9be,'fdf0','_fsca FPUL,dr0'))
phase_listing=PROJECT/'analysis/draw-object-effects-main/8c63a9b8_instructions.tsv'
phase_listing.write_text('address\tlength\tbytes\tinstruction\tflows\n'+''.join(f'{a:08x}\t2\t{raw}\t{text}\t\n' for a,raw,text in phase_rows))
switch_specs={0xc909da2:(0xc909da8,10),0x8c60563c:(0x8c605640,12)}
BRAF_TARGETS={};switches=[]
for pc,(table,count) in switch_specs.items():
 path=ROOT/('work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN' if pc<0x10000000 else 'work/sonic-adventure/extracted/1ST_READ.BIN');base=0xc900000 if pc<0x10000000 else 0x8c010000;data=path.read_bytes()
 targets=[pc+4+struct.unpack_from('<h',data,table-base+i*2)[0] for i in range(count)];BRAF_TARGETS[pc]=set(targets)
 switches.append(dict(pc=hex(pc),table=hex(table),entries=[hex(a) for a in targets],guard='Original bounded selector/table load and BRAF target retained; out-of-proposal targets stop -3.'))
cfg='        if w==0xB or w>>12 in (0xA,0xB) or w&0xF0FF in (0x400B,0x402B):'
assert BASE_CHOSEN.count(cfg)==1
BASE_CHOSEN=BASE_CHOSEN.replace(cfg,'''        if w==0x0023:
            assert a in BRAF_TARGETS and a+2 in rows
            seen.add(a+2);todo.extend(BRAF_TARGETS[a]);continue
'''+cfg)
marker='        elif w>>12==0xA:';assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,'''        elif w==0x0023:
            code=f'target=0x{a+4:x}u+r[0];'+simple(a+2)+'switch(target){'+''.join(f'case 0x{t:x}u:goto L{t:x};' for t in sorted(BRAF_TARGETS[a]))+f'default:*stop=0x{a:x}u;return -3;}}'
            slots.add(a+2);terminal=True
'''+marker)
SPECS=((0xc909d80,'dispatch'),(0xc908f3e,'profile_zero'),(0xc909054,'profile_one_two_three'),(0xc9092da,'profile_four'),(0xc909560,'profile_five'),(0xc9097e4,'profile_six'),(0xc909aa0,'profile_seven'),(0xc908b00,'emit_regular'),(0xc908d80,'emit_special'),(0x8c605628,'material_mode'),(0x8c608d14,'texture_select'),(0x8c63d414,'polygon_array'),(0x8c63a9b8,'phase'))
functions=[];routines=[]
for start,label in SPECS:
 overlay=start<0x10000000;folder='draw-object-effects' if overlay else 'draw-object-effects-main';listing=PROJECT/f'analysis/{folder}/{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 BINARY=ROOT/('work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN' if overlay else 'work/sonic-adventure/extracted/1ST_READ.BIN');binary=BINARY.read_bytes()
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/')
 if overlay:chosen=chosen.replace('0x8C010000','0x0C900000')
 exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_object_effect_{label}_{start:08x}',len(rows))
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;')
 record.update(module='AL_MAIN.BIN' if overlay else '1ST_READ.BIN',load_address='0x0c900000' if overlay else '0x8c010000',leaf_profile=label.startswith('profile_'),existing_registered_entry=label=='phase',registration_requires_behavior_proof=True)
 for switch in switches:
  if start<=int(switch['pc'],16)<end:record['computed_switch']=switch
 functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_object_effects.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT_TEMPLATE,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,source,PROJECT/'source/angle_trig_wrappers.c',PROJECT/'analysis/angle_trig_wrappers_verification.json',PROJECT/'ghidra/export_draw_object_effects.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java',ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN',ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,computed_switches=switches,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='2421 original rows in twelve proposed bodies plus a four-row existing phase-wrapper adapter, statically translated with reached RAM/FPU/return guards. Original signed-offset BRAF tables independently decoded. Shared real FSCA core; old canonical phase source/proof preserved with zero duplicate credit. Behavior and per-entry registration remain separate; compilation alone grants no recovery credit.')
(PROJECT/'analysis/draw_object_effects_translation.json').write_text(json.dumps(report,indent=2)+'\n')
