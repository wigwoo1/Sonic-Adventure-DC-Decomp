"""Checked reset/first-slot and callee-return proofs; immutable disjoint domain."""
import csv,hashlib,json,re,struct
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes();mp=P/'analysis/movie_record_register_translation.json';manifest=json.loads(mp.read_text());files=[Path(__file__),binary,mp]
 bodies={}
 for body in manifest['routines']:
  path=Path(body['listing']);files.append(path);rows={int(r['address'],16):r for r in csv.DictReader(path.open(),delimiter='\t')}
  for a,row in rows.items():assert raw[a-0x8c010000:a-0x8c010000+2].hex()==row['bytes']
  bodies[int(body['address'],16)]={a:r['instruction'].lstrip('_') for a,r in rows.items()}
 def word(a):return struct.unpack_from('<I',raw,a-0x8c010000)[0]
 def need(entry,expected):
  for a,text in expected.items():assert bodies[entry][a]==text,(hex(entry),hex(a),bodies[entry][a],text)
 def writes14(body):
  kinds=('mov','add','sub','and','or','xor','not','neg','ext','sh','rot','dt','sts','stc')
  return {a:t for a,t in body.items() if t.split()[0].startswith(kinds) and t.rsplit(',',1)[-1].strip()=='r14'}
 # R14 stays zero until it is copied to R0. Its restoration is in RTS's slot.
 need(0x8c647e16,{0x8c647e36:'mov #0x0,r14',0x8c647efc:'mov r14,r0',0x8c647f0c:'rts',0x8c647f0e:'mov.l @r15+,r14'})
 assert writes14(bodies[0x8c647e16])=={0x8c647e36:'mov #0x0,r14',0x8c647f0e:'mov.l @r15+,r14'}
 assert [a for a,t in bodies[0x8c647e16].items() if t=='rts']==[0x8c647f0c]
 for helper in (0x8c647de8,0x8c647f10,0x8c647534,0x8c660898):assert not writes14(bodies[helper])
 # The only mutating subordinate (buffer builder) saves R14 at entrySP-4.
 # Its locals use entrySP-52..-36 and every normal return restores entrySP-4.
 need(0x8c647f4c,{0x8c647f4c:'mov.l r14,@-r15',0x8c647f5c:'add #-0x14,r15',0x8c64808e:'add #0x14,r15',0x8c64809e:'rts',0x8c6480a0:'mov.l @r15+,r14'})
 assert [a for a,t in bodies[0x8c647f4c].items() if t=='rts']==[0x8c64809e]
 stack_writes=[t for t in bodies[0x8c647f4c].values() if t.startswith('mov.l ') and ',@' in t and 'r15)' in t]
 assert all(re.search(r',@\(0x(?:4|8|c|10),r15\)$',t) for t in stack_writes)
 # Calls beneath this builder are only the independently verified byte fill.
 listing=next(r['listing'] for r in manifest['routines'] if r['address']=='0x8c647f4c')
 calls=[r['flows'].strip() for r in csv.DictReader(Path(listing).open(),delimiter='\t') if r['instruction'].lstrip('_').startswith(('jsr ','bsr '))]
 assert set(calls)=={'8c10d31c'}
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];fill=next(r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']=='0x8c10d31c');files.extend((P/fill['source'],P/fill['verification']))
 # Existing byte-fill callee contract is supported by its retained complete
 # CPU proof. Its code/data buffers cannot alias this caller's private frame.
 # Reset calls exactly this fill with zero, fixed nonempty pool bounds.
 pool=word(0x8c668c44);length=word(0x8c668c40)
 assert (pool,length,word(0x8c668c48))==(0x8c8d6938,0x134a0,0x8c10d31c) and pool+length<0xffffffff
 need(0x8c668b38,{0x8c668b3c:'mov.l 0x8c668c44,r4',0x8c668b3e:'mov.l 0x8c668c40,r6',0x8c668b40:'jsr @r3',0x8c668b42:'mov #0x0,r5'})
 need(0x8c66082c,{0x8c660830:'jsr @r3',0x8c660836:'jsr @r2',0x8c660838:'mov #0x8,r4',0x8c660840:'mov.l r0,@r3'})
 assert (word(0x8c660878),word(0x8c66087c))==(0x8c668b38,0x8c668b58)
 # The first scan flag must be zero after reset. No helper precedes the test.
 need(0x8c668b58,{0x8c668b74:'mov #0x0,r13',0x8c668b76:'mov.l 0x8c668c44,r14',0x8c668b7a:'mov r13,r5',0x8c668b7c:'mov.l @r14,r2',0x8c668b7e:'tst r2,r2',0x8c668b80:'bt 0x8c668b8c',0x8c668b8c:'mov r5,r0',0x8c668b8e:'cmp/eq #0x8,r0',0x8c668be2:'mov r14,r0'})
 assert not any(t.startswith(('jsr ','bsr ')) for a,t in bodies[0x8c668b58].items() if a<0x8c668b80)
 # Context save is the only non-fill subordinate here; it restores R14 from
 # offset4 before its FPU stores (whose lowest offset is0x14 even at SZ=1).
 need(0x8c110954,{0x8c110956:'mov.l r14,@(0x4,r4)',0x8c110958:'sts PR,r14',0x8c11095c:'mov.l @(0x4,r4),r14'})
 assert writes14(bodies[0x8c110954])=={0x8c110958:'sts PR,r14',0x8c11095c:'mov.l @(0x4,r4),r14'}
 assert not any(t.startswith(('jsr ','bsr ')) for t in bodies[0x8c110954].values())
 # Bind the proven values to the two original caller comparisons.
 need(0x8c6477cc,{0x8c647838:'jsr @r3',0x8c64783c:'tst r0,r0',0x8c64783e:'bf 0x8c647878',0x8c647840:'bra 0x8c6478ec',0x8c647842:'mov #-0x1,r0'})
 need(0x8c602b68,{0x8c602bae:'jsr @r3',0x8c602bb4:'mov r0,r14',0x8c602bbe:'tst r14,r14',0x8c602bc0:'bf 0x8c602bce'})
 assert word(0x8c647874)==0x8c66082c and word(0x8c602c98)==0x8c647e16
 report=dict(infeasible_rows=['0x8c647840','0x8c647842'],infeasible_conditional_outcomes=[['0x8c64783e','0x8c647840'],['0x8c602bc0','0x8c602bce']],pool_first_flag_after_reset=0,first_allocated_slot=hex(pool),setup_success_guest_R0=0,proofs=['Fixed pool reset writes zero to the first slot before an allocator scan whose first flag must therefore be free; successful allocation returns its nonzero immutable base. Host faults stop before the caller branch and do not fabricate guest zero returns.','Setup keeps R14 zero through normal callee-preserving byte-fill/mode/setter/builder calls, copies it to R0, and only then restores the caller R14 in RTS slot. Its only normal guest return is zero.'],qualified_domain='Immutable original code/literals; valid mapped coherent buffers, disjoint private caller frames and data; actual byte-fill and helper contracts as pinned. No self-modification, destructive aliases, physical timing or arbitrary helper substitution.',sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_record_register_returns_proof.json').write_text(json.dumps(report,indent=2)+'\n');print('PROVED2 own rows and2 outcomes infeasible under declared domain')
if __name__=='__main__':main()
