"""Compare recovered C against the original startup opcodes (test harness only)."""
import ctypes, hashlib, json, pathlib, random, struct

ROOT = pathlib.Path(__file__).resolve().parents[2]
BASE = 0x8C000000
MASK = 0xFFFFFFFF
class State(ctypes.Structure):
    _fields_ = [('r', ctypes.c_uint32 * 16), ('sr', ctypes.c_uint32),
                ('pr', ctypes.c_uint32), ('pc', ctypes.c_uint32)]

def signed(value, bits):
    return value - (1 << bits) if value & (1 << (bits - 1)) else value

def reference(ram, state, hooks=None, fpu=None, mmio=None, vbr=0, vector_instruction=None, special=None, stop_pc=None, cache_writeback=None, instruction_budget=25000000, cache_invalidate=None, cache_purge=None, trace=None):
    """Subset executor for recovered routines; optional trace receives pre-instruction state."""
    regs = list(state.r); sr = state.sr; pc = state.pc; delayed = None; steps = 0
    return_pc = state.pr if stop_pc is None else stop_pc; hooks = hooks or {}; macl = 0 if special is None else special.get('macl',0)
    def index(addr):
        offset=(addr & 0x1fffffff)-0x0c000000
        if not 0 <= offset < len(ram): raise AssertionError(f'Unmapped RAM address {addr:08x} at {pc:08x}')
        return offset
    def read16(addr):
        if mmio is not None and addr in mmio:return mmio[addr] & 0xffff
        return struct.unpack_from('<H', ram, index(addr))[0]
    def write16(addr,value):
        if mmio is not None and addr in mmio:mmio[addr]=value & 0xffff
        else:struct.pack_into('<H',ram,index(addr),value & 0xffff)
    def read32(addr):
        if mmio is not None and addr in mmio:return mmio[addr]
        return struct.unpack_from('<I', ram, index(addr))[0]
    def write32(addr,value):
        if mmio is not None and addr in mmio:mmio[addr]=value
        else:struct.pack_into('<I',ram,index(addr),value)
    while pc != return_pc:
        if pc in hooks:
            sr = hooks[pc](ram, regs, sr); pc = state.pr
            continue
        steps += 1
        if steps > instruction_budget: raise AssertionError(f'Instruction budget exhausted at {pc:08x}; registers={[hex(v) for v in regs]}')
        word = struct.unpack_from('<H', ram, index(pc))[0]
        n = (word >> 8) & 15; m = (word >> 4) & 15
        if trace is not None:
            trace(pc, word, tuple(regs), sr, state.pr,
                  None if fpu is None else bytes(fpu))
        next_pc = delayed if delayed is not None else pc + 2; delayed = None
        if word & 0xF0FF in [0xF0ED,0xF07D,0xF06D] or word & 0xF3FF == 0xF1FD or word & 0xF1FF == 0xF0FD:
            assert fpu is not None and vector_instruction is not None,'Vector arithmetic requires an explicit instruction service'
            vector_instruction(word,fpu)
        elif word & 0xF0FF == 0xF05D:
            assert fpu is not None and not fpu.fpscr&0x80000,'FABS requires PR=0'
            fpu.banks[(fpu.fpscr>>21)&1][n]&=0x7fffffff
        elif word & 0xF0FF == 0x00B3:
            assert cache_writeback is not None,'OCBWB requires explicit writeback service'
            cache_writeback(regs[n])
        elif word & 0xF0FF in (0x0093,0x00A3):
            operation=cache_invalidate if word & 0xF0FF==0x0093 else cache_purge
            assert operation is not None,'Cache operation requires explicit service'
            operation(regs[n])
        elif word == 9: pass
        elif word & 0xF0FF == 0x006A:
            assert fpu is not None
            regs[n] = fpu.fpscr
        elif word & 0xF0FF == 0x406A:
            assert fpu is not None
            fpu.fpscr = regs[n]
        elif word & 0xF0FF == 0x0002: regs[n] = sr
        elif word & 0xF0FF == 0x0022: regs[n] = vbr
        elif word & 0xF0FF == 0x0029: regs[n] = sr & 1
        elif word & 0xF0FF == 0x400E: sr = regs[n]
        elif word & 0xFF00 == 0xCB00: regs[0] |= word & 255
        elif word & 0xFF00 == 0xCA00: regs[0] ^= word & 255
        elif word & 0xFF00 == 0xC700: regs[0]=(((pc+4)&~3)+(word&255)*4)&MASK
        elif word & 0xF0FF == 0x0023: delayed=(pc+4+regs[n])&MASK
        elif word == 11: delayed = state.pr
        elif word & 0xF0FF == 0x00C3:write32(regs[n],regs[0]) # MOVCA.L architectural RAM write; cache effects excluded
        elif word == 0xF3FD:
            assert fpu is not None and not (fpu.fpscr & 0x80000),'FSCHG requires PR=0'
            fpu.fpscr ^= 0x100000
        elif word == 0xFBFD:
            assert fpu is not None and not (fpu.fpscr & 0x80000),'FRCHG requires PR=0'
            fpu.fpscr ^= 0x200000
        elif word >> 12 == 13:
            regs[n] = read32(((pc + 4) & ~3) + (word & 255) * 4)
        elif word >> 12 == 9:
            address = pc + 4 + (word & 255) * 2
            regs[n] = signed(struct.unpack_from('<H', ram, index(address))[0], 16) & MASK
        elif word >> 12 == 14: regs[n] = signed(word & 255, 8) & MASK
        elif word >> 12 == 5: regs[n] = read32(regs[m] + (word & 15) * 4)
        elif word >> 12 == 4 and word & 0xF == 0xF: regs[n] = signed(ram[index(regs[m])], 8) & MASK
        elif word >> 12 == 1: write32((regs[n] + (word & 15) * 4)&MASK, regs[m])
        elif word >> 12 == 7: regs[n] = (regs[n] + signed(word & 255, 8)) & MASK
        elif word >> 12 == 10: delayed = pc + 4 + signed(word & 4095, 12) * 2
        elif word >> 12 == 11:
            state.pr = pc + 4; delayed = pc + 4 + signed(word & 4095, 12) * 2
        elif word & 0xF0FF == 0x400B:
            state.pr = pc + 4; delayed = regs[n]
        elif word & 0xF0FF == 0x402B: delayed = regs[n]
        elif word & 0xF0FF == 0x4022:
            regs[n] = (regs[n] - 4) & MASK
            struct.pack_into('<I', ram, index(regs[n]), state.pr)
        elif word & 0xF0FF == 0x4026:
            state.pr = read32(regs[n]); regs[n] = (regs[n] + 4) & MASK
        elif word & 0xF0FF == 0x4012:
            regs[n] = (regs[n] - 4) & MASK
            write32(regs[n], macl)
        elif word & 0xF0FF == 0x4016:
            macl = read32(regs[n]); regs[n] = (regs[n] + 4) & MASK
        elif word & 0xF0FF == 0x4011: sr = (sr & ~1) | int(regs[n] < 0x80000000)
        elif word & 0xF0FF == 0x4015: sr = (sr & ~1) | int(0 < regs[n] < 0x80000000)
        elif word & 0xFF00 == 0x8B00:
            if not sr & 1: next_pc = pc + 4 + signed(word & 255, 8) * 2
        elif word & 0xFF00 == 0x8900:
            if sr & 1: next_pc = pc + 4 + signed(word & 255, 8) * 2
        elif word & 0xFF00 == 0x8F00:
            delayed = pc + 4 + signed(word & 255, 8) * 2 if not sr & 1 else pc + 4
        elif word & 0xFF00 == 0x8D00:
            delayed = pc + 4 + signed(word & 255, 8) * 2 if sr & 1 else pc + 4
        elif word & 0xF00F == 0x6001: regs[n] = signed(read16(regs[m]),16)&MASK
        elif word & 0xF00F == 0x6002: regs[n] = read32(regs[m])
        elif word & 0xF00F == 0x000E: regs[n] = read32(regs[0] + regs[m])
        elif word & 0xF00F == 0x2007:
            sr=(sr&~0x301)|((regs[n]>>31)<<8)|((regs[m]>>31)<<9)|((regs[n]^regs[m])>>31)
        elif word & 0xF00F == 0x300A:
            lhs=regs[n];rhs=regs[m]+(sr&1);regs[n]=(lhs-rhs)&MASK;sr=(sr&~1)|int(lhs<rhs)
        elif word & 0xF00F == 0x300E:
            total=regs[n]+regs[m]+(sr&1);regs[n]=total&MASK;sr=(sr&~1)|(total>>32)
        elif word == 0x0019: sr &= ~0x301
        elif word & 0xF0FF == 0x4025:
            prior=sr&1;low=regs[n]&1;regs[n]=(regs[n]>>1)|(prior<<31);sr=(sr&~1)|low
        elif word & 0xF0FF == 0x4024:
            incoming=sr&1;sr=(sr&~1)|(regs[n]>>31);regs[n]=((regs[n]<<1)|incoming)&MASK
        elif word & 0xF00F == 0x3004:
            old_q=(sr>>8)&1;flag_m=(sr>>9)&1;new_q=regs[n]>>31
            before=((regs[n]<<1)|(sr&1))&MASK
            if old_q == flag_m:
                result=(before-regs[m])&MASK;overflow=int(result>before)
                new_q=(1-overflow if new_q else overflow) if flag_m==0 else (overflow if new_q else 1-overflow)
            else:
                result=(before+regs[m])&MASK;overflow=int(result<before)
                new_q=(1-overflow if new_q else overflow) if flag_m==0 else (overflow if new_q else 1-overflow)
            regs[n]=result;sr=(sr&~0x101)|(new_q<<8)|int(new_q==flag_m)
        elif word & 0xF0FF == 0x4029: regs[n] >>= 16
        elif word & 0xF00F == 0x3007: sr = (sr & ~1) | int(signed(regs[n],32) > signed(regs[m],32))
        elif word & 0xF00F == 0x000D: regs[n] = signed(read16((regs[0]+regs[m])&MASK),16)&MASK
        elif word & 0xF00F == 0x000C: regs[n] = signed(ram[index((regs[0]+regs[m])&MASK)],8)&MASK
        elif word & 0xF00F == 0x0004: ram[index((regs[0]+regs[n])&MASK)] = regs[m]&255
        elif word & 0xF00F == 0x0006: write32((regs[0]+regs[n])&MASK,regs[m])
        elif word & 0xF00F == 0x0005: write16((regs[0]+regs[n])&MASK,regs[m])
        elif word & 0xF0FF == 0xF08D:
            assert fpu is not None and not fpu.fpscr & 0x80000,'FLDI0 requires modeled FPU and PR=0'
            fpu.banks[(fpu.fpscr>>21)&1][n]=0
        elif word & 0xF0FF == 0x405A:
            assert fpu is not None,'FPUL transfer requires modeled FPU'
            fpu.fpul=regs[n]
        elif word & 0xF0FF == 0xF00D:
            assert fpu is not None,'FPUL transfer requires modeled FPU'
            fpu.banks[(fpu.fpscr>>21)&1][n]=fpu.fpul
        elif word & 0xF0FF == 0xF01D:
            assert fpu is not None,'FLDS requires modeled FPU'
            fpu.fpul=fpu.banks[(fpu.fpscr>>21)&1][n]
        elif word & 0xF0FF == 0xF02D:
            assert fpu is not None,'FLOAT requires modeled FPU'
            from sh4_binary32 import execute_float
            execute_float(fpu,n)
        elif word & 0xF0FF == 0x005A:
            assert fpu is not None, 'STS FPUL requires FPU state'
            regs[n] = fpu.fpul
        elif word & 0xF0FF == 0xF03D:
            assert fpu is not None, 'FTRC requires FPU state'
            from sh4_binary32 import execute_truncate_i32
            execute_truncate_i32(fpu,n)
        elif word & 0xF00F == 0xF002:
            assert fpu is not None, 'FMUL requires FPU state'
            from sh4_binary32 import execute_multiply
            execute_multiply(fpu,n,m)
        elif word & 0xF00F == 0xF003:
            assert fpu is not None, 'FDIV requires FPU state'
            from sh4_binary32 import execute_divide
            execute_divide(fpu,n,m)
        elif word & 0xF00F == 0xF00E:
            assert fpu is not None,'FMAC requires modeled FPU'
            from sh4_binary32 import execute_fmac
            execute_fmac(fpu,n,m)
        elif word & 0xF00F in (0xF000,0xF001):
            assert fpu is not None,'FADD/FSUB requires modeled FPU'
            from sh4_binary32 import execute_add_sub
            execute_add_sub(fpu,n,m,bool(word&1))
        elif word & 0xF00F in (0xF004,0xF005):
            assert fpu is not None,'FCMP requires modeled FPU'
            if fpu.fpscr & 0x80000:
                from sh4_binary64 import execute_compare
                condition=execute_compare(fpu,n,m,word&15==5)
                sr=(sr&~1)|int(condition)
            else:
                bank=(fpu.fpscr>>21)&1;a=fpu.banks[bank][n];b=fpu.banks[bank][m]
                fpu.fpscr&=~0x3f000
                nan=any((v&0x7fffffff)>0x7f800000 for v in (a,b))
                signaling=any((v&0x7fffffff)>0x7f800000 and v&0x400000 for v in (a,b))
                for v in (a,b):assert nan or not (0<(v&0x7fffffff)<0x800000 and not fpu.fpscr&0x40000),'Denormal comparison trap unsupported'
                if fpu.fpscr&0x40000:
                    if (a&0x7fffffff)<0x800000:a&=0x80000000
                    if (b&0x7fffffff)<0x800000:b&=0x80000000
                if signaling or (nan and word&15==5):
                    assert not fpu.fpscr&0x800,'Enabled invalid comparison trap unsupported'
                    fpu.fpscr|=0x10040
                both_zero=(a&0x7fffffff)==0 and (b&0x7fffffff)==0
                if nan:condition=False
                elif word&15==4:condition=a==b or both_zero
                elif both_zero:condition=False
                elif (a^b)&0x80000000:condition=not bool(a&0x80000000)
                else:condition=a<b if a&0x80000000 else a>b
                sr=(sr&~1)|int(condition)
        elif word & 0xF0FF in (0xF09D,0xF04D):
            assert fpu is not None and not fpu.fpscr & 0x80000,'Single-precision FPU operation requires PR=0'
            bank=(fpu.fpscr>>21)&1
            if word & 0xF0FF == 0xF09D:fpu.banks[bank][n]=0x3f800000
            else:fpu.banks[bank][n]^=0x80000000
        elif word & 0xF00F in (0xF006,0xF008,0xF009,0xF00A,0xF00B,0xF00C):
            assert fpu is not None,'FMOV requires modeled FPU'
            if fpu.fpscr & 0x100000:
                active=(fpu.fpscr>>21)&1
                form=word&15
                if form==6:
                    dst_bank=active^(n&1);pair=[read32((regs[0]+regs[m])&MASK),read32((regs[0]+regs[m]+4)&MASK)]
                    fpu.banks[dst_bank][n&~1:(n&~1)+2]=pair
                elif form==8:
                    dst_bank=active^(n&1);pair=[read32(regs[m]),read32((regs[m]+4)&MASK)]
                    fpu.banks[dst_bank][n&~1:(n&~1)+2]=pair
                elif word&15==9:
                    dst_bank=active^(n&1);pair=[read32(regs[m]),read32((regs[m]+4)&MASK)]
                    fpu.banks[dst_bank][n&~1:(n&~1)+2]=pair;regs[m]=(regs[m]+8)&MASK
                elif form==10:
                    src_bank=active^(m&1);pair=list(fpu.banks[src_bank][m&~1:(m&~1)+2])
                    write32(regs[n],pair[0]);write32((regs[n]+4)&MASK,pair[1])
                elif word&15==11:
                    src_bank=active^(m&1);pair=list(fpu.banks[src_bank][m&~1:(m&~1)+2]);regs[n]=(regs[n]-8)&MASK
                    write32(regs[n],pair[0]);write32((regs[n]+4)&MASK,pair[1])
                else:
                    src_bank=active^(m&1);dst_bank=active^(n&1)
                    pair=list(fpu.banks[src_bank][m&~1:(m&~1)+2])
                    fpu.banks[dst_bank][n&~1:(n&~1)+2]=pair
            else:
                pass
            if not (fpu.fpscr & 0x100000):
                bank=(fpu.fpscr>>21)&1;form=word&15
                if form==6:fpu.banks[bank][n]=read32((regs[0]+regs[m])&MASK)
                elif form==8:fpu.banks[bank][n]=read32(regs[m])
                elif form==9:
                    fpu.banks[bank][n]=read32(regs[m]);regs[m]=(regs[m]+4)&MASK
                elif form==10:write32(regs[n],fpu.banks[bank][m])
                elif form==11:
                    regs[n]=(regs[n]-4)&MASK;write32(regs[n],fpu.banks[bank][m])
                else:fpu.banks[bank][n]=fpu.banks[bank][m]
        elif word & 0xF00F == 0xF007:
            assert fpu is not None,'FMOV requires modeled FPU'
            active=(fpu.fpscr>>21)&1;destination=(regs[0]+regs[n])&MASK
            if fpu.fpscr & 0x100000:
                bank=active^(m&1);pair=m&~1
                write32(destination,fpu.banks[bank][pair]);write32((destination+4)&MASK,fpu.banks[bank][pair+1])
            else:write32(destination,fpu.banks[active][m])
        elif word & 0xF00F == 0x6003: regs[n] = regs[m]
        elif word & 0xF00F == 0x600B: regs[n] = (-regs[m]) & MASK
        elif word & 0xF00F == 0x6007: regs[n] = (~regs[m]) & MASK
        elif word & 0xF00F == 0x600F: regs[n] = signed(regs[m]&65535,16)&MASK
        elif word & 0xF00F == 0x6000: regs[n] = signed(ram[index(regs[m])], 8) & MASK
        elif word & 0xF00F == 0x6004:
            address=regs[m]; value=ram[index(address)]; regs[m]=(address+1)&MASK; regs[n]=signed(value,8)&MASK
        elif word & 0xF00F == 0x6005:
            address=regs[m]; value=struct.unpack_from('<H',ram,index(address))[0]; regs[m]=(address+2)&MASK; regs[n]=signed(value,16)&MASK
        elif word & 0xF00F == 0x6006:
            address=regs[m]; regs[m]=(address+4)&MASK; regs[n]=read32(address)
        elif word & 0xF00F == 0x6001:
            regs[n] = signed(struct.unpack_from('<H', ram, index(regs[m]))[0], 16) & MASK
        elif word & 0xFF00 == 0x8500:
            regs[0] = signed(struct.unpack_from('<H', ram, index(regs[m] + (word & 15) * 2))[0], 16) & MASK
        elif word & 0xFF00 == 0x8400:
            regs[0] = signed(ram[index(regs[m] + (word & 15))],8)&MASK
        elif word & 0xFF00 == 0x8100:
            struct.pack_into('<H', ram, index(regs[m] + (word & 15) * 2), regs[0] & 65535)
        elif word & 0xFF00 == 0x8000: ram[index(regs[m] + (word & 15))] = regs[0] & 255
        elif word & 0xFF00 == 0x8800: sr = (sr & ~1) | int(regs[0] == (signed(word & 255, 8) & MASK))
        elif word & 0xF00F == 0x6004:
            value = ram[index(regs[m])]; regs[m] = (regs[m] + 1) & MASK
            regs[n] = signed(value, 8) & MASK
        elif word & 0xF00F == 0x6006:
            value = read32(regs[m])
            if n != m: regs[m] = (regs[m] + 4) & MASK
            regs[n] = value
        elif word & 0xF00F in (0x6004,0x6005):
            width=1 if word&15==4 else 2
            value=struct.unpack_from('<b' if width==1 else '<h',ram,index(regs[m]))[0]&MASK
            if n!=m:regs[m]=(regs[m]+width)&MASK
            regs[n]=value
        elif word & 0xF00F in (0x2004,0x2005):
            width=1 if word&15==4 else 2
            value=regs[m];regs[n]=(regs[n]-width)&MASK
            struct.pack_into('<B' if width==1 else '<H',ram,index(regs[n]),value&((1<<(width*8))-1))
        elif word & 0xF00F == 0x2006:
            regs[n] = (regs[n] - 4) & MASK
            struct.pack_into('<I', ram, index(regs[n]), regs[m])
        elif word & 0xF00F == 0x2000: ram[index(regs[n])] = regs[m] & 255
        elif word & 0xF00F == 0x2002: write32(regs[n],regs[m])
        elif word & 0xF00F == 0x2001: struct.pack_into('<H', ram, index(regs[n]), regs[m] & 65535)
        elif word & 0xF00F == 0x3002: sr = (sr & ~1) | int(regs[n] >= regs[m])
        elif word & 0xF00F == 0x3003: sr = (sr & ~1) | int(signed(regs[n],32) >= signed(regs[m],32))
        elif word & 0xF00F == 0x3006: sr = (sr & ~1) | int(regs[n] > regs[m])
        elif word & 0xF00F == 0x200C: sr = (sr & ~1) | int(any(((regs[n] >> shift) & 255) == ((regs[m] >> shift) & 255) for shift in (0,8,16,24)))
        elif word & 0xF00F == 0x3000: sr = (sr & ~1) | int(regs[n] == regs[m])
        elif word & 0xF00F == 0x3008: regs[n] = (regs[n] - regs[m]) & MASK
        elif word & 0xF00F == 0x300C: regs[n] = (regs[n] + regs[m]) & MASK
        elif word & 0xF00F == 0x2008: sr = (sr & ~1) | int((regs[n] & regs[m]) == 0)
        elif word & 0xF00F == 0x200B: regs[n] |= regs[m]
        elif word & 0xF00F == 0x2009: regs[n] &= regs[m]
        elif word & 0xF00F == 0x200A: regs[n] ^= regs[m]
        elif word & 0xF00F == 0x200E: macl = (regs[n] & 0xffff) * (regs[m] & 0xffff) # MULU.W
        elif word & 0xF00F == 0x200F:
            lhs=regs[n]&65535;rhs=regs[m]&65535
            macl=((lhs-65536 if lhs&32768 else lhs)*(rhs-65536 if rhs&32768 else rhs))&MASK # MULS.W
        elif word & 0xF00F == 0x0007: macl = (regs[n] * regs[m]) & MASK
        elif word & 0xF0FF == 0x001A: regs[n] = macl
        elif word & 0xF0FF == 0x401A:
            macl=regs[n]
            if special is not None:special['macl']=macl
        elif word & 0xF0FF == 0x400A:
            assert special is not None,'LDS MACH requires explicit special-register state'
            special['mach']=regs[n]
        elif word & 0xF0FF == 0x401E:
            assert special is not None,'LDC GBR requires explicit special-register state'
            special['gbr']=regs[n]
        elif word & 0xF0FF == 0x402A: state.pr=regs[n]
        elif word & 0xF0FF == 0x002A: regs[n] = state.pr
        elif word & 0xF0FF == 0x000A:
            assert special is not None,'STS MACH requires explicit special-register state'
            regs[n]=special['mach']
        elif word & 0xF0FF == 0x0012:
            assert special is not None,'STC GBR requires explicit special-register state'
            regs[n]=special['gbr']
        elif word & 0xF00F == 0x600C: regs[n] = regs[m] & 255
        elif word & 0xF00F == 0x600D: regs[n] = regs[m] & 65535
        elif word & 0xFF00 == 0xC900: regs[0] &= word & 255
        elif word & 0xFF00 == 0xC800: sr = (sr & ~1) | int((regs[0] & (word & 255)) == 0)
        elif word & 0xFF00 == 0xC200:
            assert special is not None, 'GBR-relative store requires explicit special-register state'
            write32(special['gbr'] + ((word & 255) * 4), regs[0])
        elif word & 0xF00F == 0x600E: regs[n]=((regs[m]&255)-256 if regs[m]&128 else regs[m]&255)&MASK
        elif word & 0xF0FF == 0x401B:
            at=index(regs[n]);value=ram[at];sr=(sr&~1)|int(value==0);ram[at]=value|0x80
        elif word & 0xF0FF == 0x4010:
            regs[n] = (regs[n] - 1) & MASK; sr = (sr & ~1) | int(regs[n] == 0)
        elif word & 0xF0FF == 0x4005:
            low_bit=regs[n]&1;sr=(sr&~1)|low_bit;regs[n]=(regs[n]>>1)|(low_bit<<31)
        elif word & 0xF0FF == 0x4001:
            sr = (sr & ~1) | (regs[n] & 1); regs[n] >>= 1
        elif word & 0xF0FF == 0x4021:
            sr = (sr & ~1) | (regs[n] & 1); regs[n] = (signed(regs[n],32)>>1)&MASK
        elif word & 0xF0FF == 0x4029: regs[n] >>=16
        elif word & 0xF0FF == 0x4019: regs[n] >>=8
        elif word & 0xF0FF == 0x4009: regs[n] >>= 2
        elif word & 0xF0FF == 0x4028: regs[n]=(regs[n]<<16)&MASK
        elif word & 0xF0FF == 0x4018: regs[n]=(regs[n]<<8)&MASK
        elif word & 0xF00F == 0x400D:
            count = regs[m]
            regs[n] = (regs[n] << (count & 31)) & MASK if count < 0x80000000 else (regs[n] >> ((-count) & 31) if count & 31 else 0)
        elif word & 0xF00F == 0x6008: regs[n] = (regs[m] & 0xffff0000) | ((regs[m] & 255) << 8) | ((regs[m] >> 8) & 255)
        elif word & 0xF00F == 0x6009: regs[n] = ((regs[m] << 16) | (regs[m] >> 16)) & MASK
        elif word & 0xF0FF == 0x4008: regs[n] = (regs[n] << 2) & MASK
        elif word & 0xF0FF == 0x4000:
            sr = (sr & ~1) | (regs[n] >> 31); regs[n] = (regs[n] << 1) & MASK
        elif word & 0xF00F == 0x400C:
            shift = regs[m]
            if shift & 0x80000000:
                count = ((~shift & 31) + 1)
                regs[n] = (signed(regs[n],32) >> count) & MASK
            else: regs[n] = (regs[n] << (shift & 31)) & MASK
        elif word & 0xF0FF == 0x4018: regs[n] = (regs[n] << 8) & MASK
        elif word & 0xF0FF == 0x4024:
            carry = regs[n] >> 31; regs[n] = ((regs[n] << 1) | (sr & 1)) & MASK
            sr = (sr & ~1) | carry
        else: raise AssertionError(f'Unsupported original opcode {word:04x} at {pc:08x}')
        pc = next_pc
    state.r[:] = regs; state.sr = sr; state.pc = pc
    if special is not None:special['macl']=macl
    return steps

def main():
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    dll_path = ROOT / 'work/sonic-adventure/build/startup_sections.dll'
    dll = ctypes.CDLL(str(dll_path))
    function = dll.sa_initialize_sections_8c0100d8
    function.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(State)]
    function.restype = None
    records = []
    for case in ['retail_descriptors', 'empty', 'reversed', 'overlap', 'descriptor_alias'] + [f'random_{i}' for i in range(12)]:
        ram = bytearray(b'\xa5') * (16 * 1024 * 1024)
        ram[0x10000:0x10000 + len(binary)] = binary
        if case != 'retail_descriptors':
            rng = random.Random(case)
            lengths = [rng.randrange(1, 192) for _ in range(4)]
            if case == 'empty': lengths = [0] * 4
            if case == 'reversed': lengths = [-1] * 4
            starts = [BASE + 0x900000 + i * 0x1000 for i in range(4)]
            sources = [BASE + 0xA00000, BASE + 0xA01000]
            if case == 'overlap': sources = [starts[1] - 1, starts[3] - 1]
            if case == 'descriptor_alias':
                starts = [0x8C1111B8, 0x8C1111A8, 0x8C66C2F8, 0x8C66C2E4]
                lengths = [4] * 4
            descriptors = [(0x8C1111AC, starts[0]), (0x8C1111B8, starts[0] + lengths[0]),
                           (0x8C11119C, starts[1]), (0x8C111194, sources[0]), (0x8C1111A8, starts[1] + lengths[1]),
                           (0x8C66C2EC, starts[2]), (0x8C66C2F8, starts[2] + lengths[2]),
                           (0x8C66C2D8, starts[3]), (0x8C66C2D0, sources[1]), (0x8C66C2E4, starts[3] + lengths[3])]
            for addr, value in descriptors: struct.pack_into('<I', ram, addr - BASE, value)
            for source in sources:
                ram[source - BASE:source - BASE + 256] = bytes(256) if case == 'descriptor_alias' else rng.randbytes(256)
        expected = bytearray(ram)
        actual_state = State(); actual_state.r[:] = [0x12340000 + i for i in range(16)]
        actual_state.sr = 0x700000F0; actual_state.pr = 0x8C0100BA; actual_state.pc = 0x8C0100D8
        expected_state = State.from_buffer_copy(actual_state)
        steps = reference(expected, expected_state)
        buffer = (ctypes.c_uint8 * len(ram)).from_buffer(ram)
        function(buffer, ctypes.byref(actual_state))
        assert ram == expected, f'{case}: RAM differs'
        assert bytes(actual_state) == bytes(expected_state), f'{case}: CPU state differs'
        records.append({'case': case, 'original_instructions_executed': steps, 'ram_and_cpu_match': True})
    report = {'binary_sha256': hashlib.sha256(binary).hexdigest(),
              'source_sha256': hashlib.sha256((ROOT / 'outputs/sonic-adventure/source/startup_sections.c').read_bytes()).hexdigest(),
              'dll_sha256': hashlib.sha256(dll_path.read_bytes()).hexdigest(),
              'routine_start': '0x8c0100d8', 'routine_end_exclusive': '0x8c010132',
              'cases': records, 'scope': 'One startup routine; subset instruction interpreter, not a hardware or full-game verification.'}
    (ROOT / 'outputs/sonic-adventure/analysis/startup_verification.json').write_text(json.dumps(report, indent=2))
    print(f'{len(records)} cases passed: complete 16 MiB RAM and register-state comparisons')

if __name__ == '__main__': main()


