"""Derive the exact open angle-fold obligation without assuming atan2 bounds."""
import csv,ctypes,hashlib,json,struct
from pathlib import Path
from prove_movement_model_response_predicates import graph
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def number(raw):return struct.unpack('<f',raw.to_bytes(4,'little'))[0]
def main():
    original=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes()
    def word(a):return int.from_bytes(raw[a-0x8c010000:a-0x8c010000+4],'little')
    listing=P/'analysis/movement-orientation-response/8c074ee4_instructions.tsv';rows,t,pred=graph(listing,raw);assert len(rows)==157
    for a,text in {0x8c074fa0:'fmov fr15,fr3',0x8c074fa2:'fmul fr15,fr3',0x8c074fa4:'fmov fr14,fr0',0x8c074fa8:'fmac fr0,fr14,fr3',0x8c074faa:'jsr @r3',0x8c074fac:'fmov fr3,fr4',0x8c074fbc:'jsr @r3',0x8c074fbe:'fmov.s @r15,fr4',0x8c074fc8:'fmul fr3,fr2',0x8c074fcc:'ftrc fr2,FPUL',0x8c074fce:'sts FPUL,r14',0x8c074fd0:'and r3,r14',0x8c074fd2:'cmp/gt r2,r14',0x8c074fd4:'bf 0x8c074fdc',0x8c074fd6:'mov.l 0x8c075198,r0',0x8c074fd8:'sub r14,r0',0x8c074fda:'mov r0,r14'}.items():assert t[a]==text,(hex(a),t[a])
    assert pred[0x8c074fd6]=={0x8c074fd4} and pred[0x8c074fd8]=={0x8c074fd6} and pred[0x8c074fda]=={0x8c074fd8}
    constant=word(0x8c07518c);mask=word(0x8c075190);limit=word(0x8c075194);turn=word(0x8c075198);assert (constant,mask,limit,turn)==(0x4622f986,0xffff,0x8000,0x10000)
    dll=ROOT/'work/sonic-adventure/build/movement_orientation_response.dll';lib=ctypes.CDLL(str(dll));mul=lib.sa_sh4_multiply;trunc=lib.sa_sh4_truncate_i32;U=ctypes.c_uint32;Ptr=ctypes.POINTER(U)
    mul.argtypes=[Ptr,Ptr,Ptr,Ptr];mul.restype=ctypes.c_int;trunc.argtypes=[U,Ptr,Ptr];trunc.restype=ctypes.c_int
    queries=[]
    def query(value,rm):
        a,b,status,out=U(value),U(constant),U(0x40000|rm),U();assert mul(ctypes.byref(a),ctypes.byref(b),ctypes.byref(status),ctypes.byref(out))==0
        integer=U();assert trunc(out,ctypes.byref(status),ctypes.byref(integer))==0
        queries.append(dict(raw_angle=hex(value),rm=rm,multiplied_word=hex(out.value),integer=hex(integer.value),phase=hex(integer.value&mask)))
        return integer.value
    pi_pointer=word(0x8c10e6f4);pi_word=word(pi_pointer);assert pi_word==0x40490fdb
    boundaries=[]
    for rm in (0,1):
        lo,hi=0x40400000,0x40800000;assert query(lo,rm)<=limit and query(hi,rm)>limit
        while lo+1<hi:
            mid=(lo+hi)//2
            if query(mid,rm)>limit:hi=mid
            else:lo=mid
        lower,upper,pi_integer=query(lo,rm),query(hi,rm),query(pi_word,rm)
        assert lower==limit and upper==limit+1 and pi_integer<=limit
        boundaries.append(dict(rm=rm,last_angle_below_fold=hex(lo),first_angle_producing_fold=hex(hi),last_angle_value=number(lo),first_angle_value=number(hi),retail_pi_word=hex(pi_word),retail_pi_integer=hex(pi_integer),positive_pi_margin_in_binary32_words=hi-pi_word))
    direct_path=P/'analysis/movement_orientation_response_direct_verification.json';range_path=P/'analysis/movement_orientation_response_range_verification.json';direct=json.loads(direct_path.read_text());extra=json.loads(range_path.read_text())
    for r in (direct,extra):
        owner=next(x for x in r['coverage_by_routine'] if x['address']=='0x8c074ee4');assert ['0x8c074fd4','0x8c074fd6'] in owner['missing_outcomes']
    historical=P/'analysis/atan2_full_native_verification.json';old=json.loads(historical.read_text());assert old['services_visited']==[]
    files=[Path(__file__),P/'prove_movement_model_response_predicates.py',original,listing,dll,P/'source/sa_movement_orientation_response.c',P/'source/sh4_multiply.c',P/'source/sh4_truncate_i32.c',P/'source/sa_collision_angle.c',P/'source/sa_draw_frame_dependencies.c',P/'source/model_matrix_stack_pop.c',direct_path,range_path,historical]
    report=dict(status='open_original_angle_fold_obligation',owner_still_unregistered='0x8c074ee4',original_rows_byte_checked=157,unqualified_rows=['0x8c074fd6','0x8c074fd8','0x8c074fda'],unqualified_outcomes=[['0x8c074fd4','0x8c074fd6']],is_unreachability_proof=False,original_code_removed=False,domain_excluded_rows=[],domain_excluded_outcomes=[],numeric_boundary_queries=len(queries),numeric_queries_are_original_native_comparisons=False,numeric_boundaries=boundaries,queries=queries,retail_radians_to_angle_word=hex(constant),retail_phase_mask=hex(mask),retail_half_turn=hex(limit),retail_full_turn=hex(turn),historical_atan2_actual_native_comparisons=old['cases_passed'],historical_atan2_external_services_visited=old['services_visited'],remaining_required_evidence=[
      'Prove a bound for the actual original atan2/core/scaling/classification closure reached by074ee4, or find an ordinary input reaching the fold. The instruction sequence forms a horizontal sum of squares, calls existing signed sqrt, then calls actual atan2. Ideal real arithmetic alone is not a proof of the implemented binary32 result.',
      'For completed positive ordinary scalar arithmetic, the native multiplication/truncation boundary is certified locally above retail PI for RM0/1. This reduces the remaining question to a bound on the actual returned angle. The arithmetic queries are not original-instruction comparisons, not a complete atan2 proof and not routine recovery credit.',
      'The104raw-geometry frontier comparisons cover negative vertical orientation, signed zeros, subnormal/normal/extreme finite values and nonfinite/numeric-stop cases with real callees. Not observing the fold is not an infeasibility proof. Prior5260actual-native atan2 comparisons also constitute a finite corpus, not a universal range certificate.',
      'Preserve all157owner rows and the fold branch. Leave074ee4 unregistered until this obligation is resolved; the fully covered078e40/055d20helpers may be recorded separately. Broader PR/SZ/physical math obligations and0730d8qualification remain open.'
      ],new_original_routine_credit=0,dll_sha256=digest(dll),sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_orientation_response_angle_fold_obligation.json').write_text(json.dumps(report,indent=2)+'\n');print('ORIENTATION FOLD OBLIGATION OPEN;native scalar quantization boundaries',boundaries,';no owner credit')
if __name__=='__main__':main()
