"""Checked scalar return-class derivation; deliberately does not close paired-FMOV states."""
import csv,hashlib,json,struct
from fractions import Fraction
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
GROUPS={'8c10d058':'collision-angle','8c10d0f8':'collision-angle','8c10e6a0':'collision-angle','8c10fb94':'collision-angle-numeric','8c10fbb8':'collision-angle-numeric','8c10e5ac':'draw-frame-acos','8c10ef84':'draw-frame-trig','8c10e7b8':'draw-frame-trig'}
class UnclosedDomainError(ValueError):pass

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def value(word):
 exp=(word>>23)&255;frac=word&0x7fffff
 assert exp!=255,'Nonfinite constant is not a finite interval endpoint'
 result=Fraction((1<<23)|frac if exp else frac)*Fraction(2)**(exp-150 if exp else -149)
 return -result if word>>31 else result

def tables():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();rows={};files=[]
 for entry,group in GROUPS.items():
  path=P/'analysis'/group/(entry+'_instructions.tsv');files.append(path);body=list(csv.DictReader(path.open(),delimiter='\t'))
  for r in body:
   a=int(r['address'],16);assert binary[a-0x8c010000:a-0x8c010000+int(r['length'])]==bytes.fromhex(r['bytes']),r
   assert r['address'] not in rows;rows[r['address']]=r['instruction'].lstrip('_')
 return binary,rows,files

def derive(binary,rows,domain='PR0_SZ0'):
 if domain!='PR0_SZ0':raise UnclosedDomainError('Paired-FMOV return/frame/indirect-target closure is not proved; no whole-wrapper qualification')
 def word(a):return int.from_bytes(binary[a-0x8c010000:a-0x8c010000+4],'little')
 def indirect(a):return word(word(a))
 def expect(a,text):assert rows[f'{a:08x}']==text,(hex(a),text,rows.get(f'{a:08x}'))
 # Bind each mathematical case to original operands, control transfers and returns.
 expected={0x8c10e5b4:'fmov fr4,fr5',0x8c10e5c2:'fldi0 fr3',0x8c10e5c4:'fcmp/eq fr3,fr5',0x8c10e5d8:'cmp/eq r1,r2',0x8c10e5e2:'fcmp/eq fr5,fr5',0x8c10e5ee:'fmov.s @r5,fr0',0x8c10e610:'fcmp/gt fr5,fr3',0x8c10e618:'fcmp/eq fr5,fr3',0x8c10e638:'fcmp/gt fr3,fr2',0x8c10e63e:'fmul fr4,fr1',0x8c10e644:'fsub fr1,fr0',0x8c10e648:'fsqrt fr0',0x8c10e64a:'fadd fr0,fr1',0x8c10e64e:'fdiv fr1,fr15',0x8c10e652:'fmul fr4,fr1',0x8c10e656:'fsub fr1,fr0',0x8c10e65a:'fsqrt fr0',0x8c10e65c:'fsub fr0,fr1',0x8c10e660:'fdiv fr4,fr15',0x8c10e664:'jsr @r3',0x8c10e66c:'jsr @r2',0x8c10e66e:'mov #0x1,r4',0x8c10e672:'fmov fr5,fr0',0x8c10ef96:'fmov fr4,fr5',0x8c10efa4:'fldi0 fr3',0x8c10efa6:'fcmp/eq fr3,fr5',0x8c10efc4:'fcmp/eq fr5,fr5',0x8c10eff4:'shad r2,r1',0x8c10eff6:'add #-0x7f,r1',0x8c10eff8:'cmp/ge r0,r1',0x8c10f020:'fcmp/gt fr3,fr5',0x8c10f02e:'fcmp/eq fr3,fr5',0x8c10f03a:'fmov fr4,fr0',0x8c10f042:'fadd fr3,fr5',0x8c10f046:'fadd fr3,fr2',0x8c10f04a:'fdiv fr5,fr15',0x8c10f056:'jsr @r2',0x8c10f09c:'jsr @r3',0x8c10f09e:'fmov fr15,fr4',0x8c10f0a0:'fadd fr0,fr13',0x8c10f0a2:'fdiv fr13,fr12',0x8c10f0ba:'jsr @r3',0x8c10f0bc:'fmov fr15,fr4',0x8c10f0be:'fadd fr0,fr13',0x8c10f0c0:'fdiv fr13,fr12',0x8c10f0d6:'fmac fr0,fr15,fr13',0x8c10f0de:'fdiv fr13,fr4',0x8c10f0e2:'fadd fr15,fr13',0x8c10f0f0:'fdiv fr13,fr4',0x8c10f0f2:'jsr @r14',0x8c10f0f8:'fadd fr0,fr5',0x8c10f100:'fmov fr5,fr0',0x8c10e6b6:'fdiv fr3,fr4',0x8c10e6c2:'fmov fr0,fr4',0x8c10e6dc:'fadd fr3,fr4',0x8c10e6e4:'fmov fr4,fr0',0x8c10fb96:'fmul fr4,fr6',0x8c10fb9a:'mov #0x5,r4',0x8c10fba0:'add #-0x1,r4',0x8c10fba6:'fmac fr0,fr5,fr3',0x8c10fba8:'bt/s 0x8c10fb9e',0x8c10fbb2:'fmul fr6,fr0',0x8c10e806:'add r4,r7',0x8c10e810:'bf 0x8c10e83e',0x8c10e844:'shad r3,r7',0x8c10e848:'or r7,r5',0x8c10e84a:'or r2,r5'}
 for a,text in expected.items():expect(a,text)
 # All precision/bank/size changes must be absent in this closed original family.
 assert not any('FPSCR' in t or t.startswith(('fschg','frchg')) for t in rows.values())
 for a in (0x8c10e67c,0x8c10f114):assert word(a)==0x7fffffff
 for a in (0x8c10e680,0x8c10f118,0x8c10e858):assert word(a)==0x7f800000
 for a in (0x8c10e688,0x8c10f11c,0x8c10e6ec,0x8c10e85c):assert word(a)==0x80000000
 assert indirect(0x8c10e684)==indirect(0x8c10f120)==0x7f800001
 assert indirect(0x8c10e68c)==indirect(0x8c10f128)==0x3f800000
 assert indirect(0x8c10e694)==0x3f000000 and indirect(0x8c10f138)==0x3e000000
 assert indirect(0x8c10f134)==0xbf800000 and indirect(0x8c10f148)==0xbe800000 and indirect(0x8c10f154)==0xbf000000 and indirect(0x8c10f15c)==0xbf400000 and indirect(0x8c10f160)==0x3f400000
 assert indirect(0x8c10f130)==indirect(0x8c10e870)==0
 assert indirect(0x8c10e874)==0x80000000
 assert word(0x8c10e698)==word(0x8c10e6e8)==0x8c10ef84 and word(0x8c10e69c)==word(0x8c10f14c)==0x8c10e7b8
 assert word(0x8c10f13c)==0x8c10fbb8 and word(0x8c10f140)==0x8c10fb94
 # No division needs an ideal asin/atan accuracy theorem. Monotone RM0/RM1
 # rounding and DN flush cannot turn a value in[0,1] into a value outside it.
 # abs(x)<2^32: atan's positive transform is either abs(x) in[0,1), or
 # rounded(abs(x)-1)/rounded(abs(x)+1) in[0,1]. All other atan entry arms
 # return a finite original constant/zero/subnormal or the original qNaN.
 offset_literals=[0x8c10f12c,0x8c10f144,0x8c10f150,0x8c10f158]
 offsets=[abs(value(indirect(a))) for a in offset_literals];assert max(offsets)<1
 half_pi=value(indirect(0x8c10f124));pi_pos=value(indirect(0x8c10e6f4));pi_neg=value(indirect(0x8c10e6f0));assert 0<half_pi<2 and 0<pi_pos<4 and -4<pi_neg<0
 # pow2 scaling is integer exponent editing in the original79row helper.
 # For nonnegative [0,1] inputs,n=-2/-1: positive finite result <=1 or+0.
 # For |x|<=2^36,n=+1: edited exponent <=164, safely below255; underflow
 # returns signed zero. The infinities at its table are only overflow arms.
 scale_exponents=[e+n for e in range(1,128) for n in (-2,-1)];assert max(scale_exponents)<255
 assert 163+1<255 and int.from_bytes(binary[0x10e856-0x10000:0x10e858-0x10000],'little')==255
 # All approximation cases have denominator >=1. This lower bound survives
 # monotone rounding of1+nonnegative and fused1+0.75*x. Numerator <=1 before
 # rounding; generously bound each rounded numerator by2 and quotient by4.
 arg=Fraction(4);square=2*arg*arg
 coefficient_address=word(0x8c10fbb4);coefficients=[word(coefficient_address+4*i) for i in range(6)]
 assert all(abs(value(c))<=1 for c in coefficients)
 accumulator=max(abs(value(c)) for c in coefficients);bounds=[]
 for i in range(5):accumulator=2*(square*accumulator+1);bounds.append(accumulator)
 polynomial=2*arg*accumulator;offset=Fraction(4);atan_bound=2*(polynomial+offset)
 assert polynomial<2**34 and atan_bound<2**36
 # asin general path: rounded x*x<=1,1-x*x>=0,sqrt in[0,1]. Small half
 # divides |x|<.5 by1+sqrt>=1; other half divides |1-sqrt|<=1 by|x|>=.5.
 # Thus it calls the bounded atan with finite magnitude<=4,then scales by2.
 asin_bound=2**37;quadrant_bound=2*(Fraction(2**36)+max(abs(pi_pos),abs(pi_neg)))
 assert quadrant_bound<2**38
 maximum=max([square,polynomial,atan_bound,Fraction(asin_bound),quadrant_bound]+bounds)
 assert maximum<2**126,'All listed rounded finite intermediates stay far from overflow'
 # Bind the previously missing wrapper arms to the exact infinity predicate.
 for entry,mask_ptr,mantissa_ptr,branch,missing in ((0x8c10d058,0x8c10d09c,0x8c10d0a0,0x8c10d078,[0x8c10d082,0x8c10d084,0x8c10d086]),(0x8c10d0f8,0x8c10d13c,0x8c10d140,0x8c10d118,[0x8c10d122,0x8c10d124,0x8c10d126])):
  assert word(mask_ptr)==0x7f800000 and word(mantissa_ptr)==0x7fffff
  expect(branch,'bt '+hex(missing[0]));assert all(f'{a:08x}' in rows for a in missing)
 return dict(domain=domain,checked_operand_and_control_bindings=len(expected),result_classes=['finite','NaN'],infinity_result_possible=False,polynomial_argument_abs_bound=str(arg),polynomial_squared_abs_bound=str(square),horner_abs_bounds=list(map(str,bounds)),polynomial_abs_bound=str(polynomial),atan_abs_bound=str(atan_bound),asin_abs_bound=str(asin_bound),quadrant_abs_bound=str(quadrant_bound),maximum_checked_bound=str(maximum),coefficient_words=list(map(hex,coefficients)),rounding_assumptions=['Monotone binary32 RM0 nearest-even andRM1 towardzero;finite exact bounds below2^126 stay finite;outward2x bounds used for ordinary arithmetic','DN flushing can decrease magnitude;underflow/trap/unsupported states follow existing software policy','Only completed ordinary PR0/SZ0 calls;NaNs may propagate as NaNs,not infinity'],paired_fmov_domain_closed=False,whole_wrappers_qualified=False)

def main():
 binary,rows,files=tables();proof=derive(binary,rows)
 pins=files+[Path(__file__),P/'source/sa_collision_angle.c',P/'source/sa_draw_frame_dependencies.c',P/'source/sh4_compare_bits.h',P/'analysis/collision_angle_qualification.json',ROOT/'work/sonic-adventure/build/collision_angle.dll']
 report=dict(status='checked_scalar_return_class_derivation_only',derivation=proof,original_instruction_rows_byte_checked=len(rows),native_source_changed=False,new_routine_credit=0,wrappers_still_unregistered=['0x8c10d058','0x8c10d0f8'],scope='Machine-checked numeric bounds and original operand/control bindings support the written scalar derivation;not a general SMT proof,hardware proof,or arbitrary paired-FMOV-state reachability proof. No ideal transcendental accuracy assumed. Existing original/native tests remain separate historical evidence.',preconditions=['Original code,literals,coefficient tables unchanged during call','Mapped nonwrapping stack disjoint from original code/tables;no asynchronous memory mutation','SR.FD clear;this certificate explicitly PR0/SZ0 with supported numerical completion policy'],remaining_work='Close paired-FMOV call/return and dynamic-target effects or prove the actual caller ABI before full wrapper qualification. Keep six missing wrapper instructions/two outcomes unqualified.',overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(p):digest(p) for p in pins},original_binary_sha256=hashlib.sha256(binary).hexdigest())
 (P/'analysis/collision_wrapper_scalar_return_class.json').write_text(json.dumps(report,indent=2)+'\n');print('SCALAR RETURN CLASS DERIVATION PASS;paired-FMOV OPEN;NO routine credit',proof['maximum_checked_bound'])
if __name__=='__main__':main()
