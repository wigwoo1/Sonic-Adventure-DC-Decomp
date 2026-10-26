"""Verification-only decoder extension; old decoders and native sources unchanged."""
import movie_diagnostics_reference_cpu as old
code=old.code
marker='        elif word == 9: pass';assert code.count(marker)==1
extra=r'''        elif word==0x002b:
            assert special is not None
            sr=special['ssr'];regs.sr=sr;delayed=special['spc'];special['returned']=1
        elif word&0xf0ff in (0x0032,0x0042,0x003a):
            regs[n]=special[{0x0032:'ssr',0x0042:'spc',0x003a:'sgr'}[word&0xf0ff]]
        elif word&0xf0ff in (0x403e,0x404e):
            special['ssr' if word&0xf0ff==0x403e else 'spc']=regs[n]
        elif word&0xf0ff in (0x4003,0x4013,0x4023,0x4033,0x4043,0x4002,0x4012,0x4022,0x4052,0x4062):
            kind=word&0xf0ff
            if kind==0x4003:value=sr
            elif kind==0x4002:value=special['mach']
            elif kind==0x4012:value=macl
            elif kind==0x4022:value=state.pr
            elif kind==0x4052:value=fpu.fpul
            elif kind==0x4062:value=fpu.fpscr
            else:value=special[{0x4013:'gbr',0x4023:'vbr',0x4033:'ssr',0x4043:'spc'}[kind]]
            regs[n]=(regs[n]-4)&MASK;write32(regs[n],value)
        elif word&0xf0ff in (0x4007,0x4017,0x4027,0x4037,0x4047,0x4006,0x4016,0x4026,0x4056,0x4066):
            kind=word&0xf0ff;value=read32(regs[n])
            if kind==0x4007:sr=valid_sr(value);regs.sr=sr
            elif kind==0x4006:special['mach']=value
            elif kind==0x4016:macl=value
            elif kind==0x4026:state.pr=value
            elif kind==0x4056:fpu.fpul=value
            elif kind==0x4066:fpu.fpscr=value&0x3fffff
            else:special[{0x4017:'gbr',0x4027:'vbr',0x4037:'ssr',0x4047:'spc'}[kind]]=value
            regs[n]=(regs[n]+4)&MASK
'''
code=code.replace(marker,extra+marker)
marker='    def read32(addr):';assert code.count(marker)==1
code=code.replace(marker,marker+"\n        if addr in (0xff000020,0xff000024,0xff000028):return special[{0xff000020:'tra',0xff000024:'expevt',0xff000028:'intevt'}[addr]]")
code=code.replace('fpu.fpscr = regs[n]','fpu.fpscr = regs[n]&0x3fffff')
namespace=dict(old.namespace);exec(compile(code,'interrupt_context_original_cpu','exec'),namespace)
reference=namespace['reference']
