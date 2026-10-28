"""Byte-backed annotated call graph and local R10 facts; retain the last caller gap."""
import collections,csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;W=P.parents[1]/'work/sonic-adventure'
ENTRY=0x8c029b00
binary=W/'extracted/1ST_READ.BIN';raw=binary.read_bytes()
reg=json.loads((P/'analysis/recovered_routines.json').read_text())['routines']
registry={int(r['address'],16):r for r in reg if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'}
listings=collections.defaultdict(list)
for path in (P/'analysis').rglob('*_instructions.tsv'):
 try:entry=int(path.name.split('_')[0],16)
 except ValueError:continue
 if 0x8c010000<=entry<0x8d000000:listings[entry].append(path)
queue=[ENTRY];bodies={};unresolved=[];instructions={};pins={str(binary):hashlib.sha256(raw).hexdigest()}
while queue:
 entry=queue.pop(0)
 if entry in bodies:continue
 r=registry.get(entry,{})
 end=int(r['end_exclusive'],16) if r.get('end_exclusive') else None
 available={};flow=collections.defaultdict(set);used=[]
 for path in listings[entry]:
  rows=list(csv.DictReader(path.open(),delimiter='\t'))
  if not rows or any(bytes.fromhex(row['bytes'])!=raw[int(row['address'],16)-0x8c010000:int(row['address'],16)-0x8c010000+int(row['length'])] for row in rows):continue
  used.append(str(path));pins[str(path)]=hashlib.sha256(path.read_bytes()).hexdigest()
  for row in rows:
   a=int(row['address'],16)
   available[a]=row['instruction'].lstrip('_')
   flow[a].update(int(x,16) for x in re.findall(r'\b[0-9a-fA-F]{8}\b',row.get('flows','')))
 assert available and used,(hex(entry),'missing original byte-checked listing')
 instructions[entry]=available
 calls=[];jumps=[];outside=[]
 for a,t in sorted(available.items()):
  if t.startswith(('jsr ','bsr ')):
   targets=sorted(flow[a])
   if t.startswith('bsr '):targets=[int(t.split()[-1],16)]
   calls.append(dict(address=hex(a),instruction=t,targets=list(map(hex,targets))))
   if targets:queue.extend(targets)
   else:unresolved.append(dict(body=hex(entry),site=hex(a),instruction=t,kind='unresolved_call'))
  elif t.startswith(('jmp ','braf ','bsrf ')):
   jumps.append(dict(address=hex(a),instruction=t,targets=list(map(hex,sorted(flow[a])))))
   unresolved.append(dict(body=hex(entry),site=hex(a),instruction=t,kind='computed_control_obligation'))
  elif t.startswith(('bt ','bf ','bt/s ','bf/s ','bra ')):
   target=int(t.split()[-1],16)
   if target not in available:
    outside.append(dict(address=hex(a),target=hex(target),instruction=t))
    unresolved.append(dict(body=hex(entry),site=hex(a),target=hex(target),kind='outside_direct_branch'))
 refs=[dict(address=hex(a),instruction=t) for a,t in sorted(available.items()) if re.search(r'\br10\b',t)]
 bodies[entry]=dict(address=hex(entry),registered_name=r.get('name'),registered_source=r.get('source'),
  registered_end=hex(end) if end else None,original_rows=len(available),listings=used,
  calls=calls,computed_control=jumps,outside_direct_branches=outside,r10_references=refs)
 if not available:unresolved.append(dict(body=hex(entry),kind='missing_original_body'))

# These are source-level facts about original instructions, not an assumption
# that a calling convention or a finite execution corpus preserves R10.
assert len(bodies)==41 and sum(b['original_rows'] for b in bodies.values())==5981
assert unresolved==[dict(body='0x8c10cddc',site='0x8c10cde8',instruction='jmp @r3',kind='computed_control_obligation')]
root=instructions[ENTRY];copy=instructions[0x8c10cddc]
for call,delay in ((0x8c02a380,0x8c02a382),(0x8c02a498,0x8c02a49a)):
 assert root[call]=='jsr @r3' and root[delay]=='mov #0x58,r0'
assert [copy[a] for a in (0x8c10cdde,0x8c10cde0,0x8c10cde2)]==[
 'mov #0x40,r3','cmp/hs r0,r3','bf 0x8c10ce7c']
long_copy={a:t for a,t in copy.items() if 0x8c10ce7c<=a<=0x8c10ce9a}
assert len(long_copy)==16 and not any(re.search(r'\br10\b',t) or t.startswith(('jsr ','bsr ','jmp ')) for t in long_copy.values())
assert (0x40>=0x58) is False
assert root[0x8c029b56]=='mov #0x1,r10'
assert root[0x8c02a41a]=='cmp/ge r10,r5' and root[0x8c02a41c]=='bt 0x8c02a460'
assert root[0x8c02a488]=='mov r5,r4' and root[0x8c02a48a]=='cmp/pl r4'
assert root[0x8c02a48c]=='bf/s 0x8c02a4a8'
selection={a:t for a,t in root.items() if 0x8c02a460<=a<=0x8c02a488}
assert not any(t.startswith(('jsr ','bsr ','jmp ')) for t in selection.values())
assert all(not re.search(r',r5$',t) or t=='cmp/ge r10,r5' for t in selection.values())
changing=[b for a,b in bodies.items() if a!=ENTRY and b['r10_references']]
assert {b['address'] for b in changing}=={'0x8c63d66c','0x8c640922','0x8c64f840',
 '0x8c64fc68','0x8c64fae8','0x8c650144','0x8c64fe0c'}
for b in changing:
 refs=b['r10_references']
 assert refs[0]['instruction']=='mov.l r10,@-r15'
 assert refs[-1]['instruction']=='mov.l @r15+,r10'
pins[str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
pins[str(P/'analysis/recovered_routines.json')]=hashlib.sha256((P/'analysis/recovered_routines.json').read_bytes()).hexdigest()
report=dict(status='verified_static_inventory_partial_caller_obligation',annotated_body_count=41,
 byte_checked_original_instruction_rows=5981,bodies=list(bodies.values()),
 annotated_direct_calls=sum(len(b['calls']) for b in bodies.values()),
 computed_copy_jump_bypassed_at_both_caller_sites=True,
 copy_count_bytes=88,copy_long_path_original_rows=16,
 copy_R10_preserved_by_absence_of_register_writes=True,
 local_selection_R5_unchanged=True,
 conditional_fact='If R10 is positive at A41a, signed COUNT>=R10 admits only positive R5. '
 'Selection contains no calls or R5 writes; A488 copies R5 to R4 and CMP/PL is true, '
 'so A48c cannot take the skip. This does not prove R10 remains positive across earlier helpers.',
 remaining_R10_changing_helpers=[dict(address=b['address'],source=b['registered_source'],
  first_save=b['r10_references'][0],last_restore=b['r10_references'][-1]) for b in changing],
 original_annotated_control_obligations=unresolved,
 remaining_obligations=['Prove actual R10 save-word preservation and SP restoration for seven rendering/text '
 'helpers, including nested calls, all original branches and global/input/stack aliases.',
 'Confirm annotated indirect-call targets under the admitted original input/control state; '
 'Ghidra call annotations alone are not a proof of all possible dynamic targets.',
 'Recover original callers/renderer initialization contracts or construct actual normal-entry '
 'input reaching A48c->A4a8. No outcome is excluded or declared unreachable here.'],
 unaccounted_caller_outcomes=[['0x8c02a48c','0x8c02a4a8']],
 new_original_routine_credit=0,fresh_original_native_comparisons=0,
 registry_entries=len(reg),registry_unique_module_address_pairs=len({(r.get('module','1ST_READ.BIN'),r['address']) for r in reg}),
 overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
 scope='Static facts checked against byte-matching original TSV instruction listings. '
 'All41 annotated bodies have listings; no missing PER_BODY entry is treated as missing game code. '
 'Calls follow original Ghidra flow annotations; all direct branches stay inside their exported bodies. '
 'The only computed jump is bypassed by fixed58hex copy counts at the two caller sites. '
 'Save/pop instruction pairs do not prove stack-word preservation. This is not full029b00 qualification, '
 'natural gameplay proof or additional routine credit. Old native source/builds/proofs remain unchanged.',sha256=pins)
(P/'analysis/collision_contact_register_paths_audit.json').write_text(json.dumps(report,indent=2)+'\n')
print('REGISTER PATH AUDIT PASS:41annotatedbodies/5981bytecheckedrows;fixed88bytecopyR10fact;7helperpreservationobligationsOPEN')
