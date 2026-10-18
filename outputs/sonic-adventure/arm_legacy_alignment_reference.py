"""Adapt only Unicorn's unaligned single-word transfers to ARM DDI 0100I A2.8.

Reference: https://e2e.ti.com/cfs-file/__key/communityserver-discussions-components-files/1023/ARM-Architecture.pdf
All aligned instructions still execute in Unicorn. Correction counts are exposed.
This does not supply ARM7 timing, exception entry or sound-chip semantics.
"""
def install(machine,unicorn,arm,events,mmio,mmio_size):
    registers=[getattr(arm,'UC_ARM_REG_R'+str(i)) for i in range(13)]+[arm.UC_ARM_REG_SP,arm.UC_ARM_REG_LR,arm.UC_ARM_REG_PC]
    corrections=[]
    def hook(uc,pc,size,user):
        word=int.from_bytes(uc.mem_read(pc,4),'little')
        if word&0x0c400000!=0x04000000:return # single word LDR/STR only
        cpsr=uc.reg_read(arm.UC_ARM_REG_CPSR);cond=word>>28
        n,z,c,v=[bool(cpsr&(1<<bit)) for bit in (31,30,29,28)]
        conditions=(z,not z,c,not c,n,not n,v,not v,c and not z,not c or z,n==v,n!=v,not z and n==v,z or n!=v,True,False)
        if not conditions[cond]:return
        rn=(word>>16)&15;rd=(word>>12)&15
        base=pc+8 if rn==15 else uc.reg_read(registers[rn])
        if word&(1<<25):
            assert not word&16,'Unsupported register-controlled address shift'
            rm=word&15;value=pc+8 if rm==15 else uc.reg_read(registers[rm])
            kind=(word>>5)&3;amount=(word>>7)&31
            if kind==0:offset=(value<<amount)&0xffffffff
            elif kind==1:offset=value>>amount if amount else 0
            elif kind==2:
                signed=value if value<0x80000000 else value-0x100000000
                offset=(signed>>(amount or 32))&0xffffffff
            elif amount:offset=((value>>amount)|(value<<(32-amount)))&0xffffffff
            else:offset=((int(c)<<31)|(value>>1))&0xffffffff
        else:offset=word&4095
        updated=(base+offset if word&(1<<23) else base-offset)&0xffffffff
        address=updated if word&(1<<24) else base
        if not address&3:return
        assert rd!=15 and rn!=15,'Unaligned PC transfer is outside this adapter contract'
        writeback=not word&(1<<24) or bool(word&(1<<21))
        assert not (writeback and rn==rd),'Unpredictable base/destination overlap'
        aligned=address&~3;loading=bool(word&(1<<20))
        if loading:
            value=int.from_bytes(uc.mem_read(aligned,4),'little')
            bus_value=value
            # Select the addressed byte as the low byte, preserving the entire word.
            octets=value.to_bytes(4,'little');index=address%4
            value=int.from_bytes(octets[index:]+octets[:index],'little')
            uc.reg_write(registers[rd],value)
        else:
            value=uc.reg_read(registers[rd]);bus_value=value
            uc.mem_write(aligned,value.to_bytes(4,'little'))
        if mmio<=aligned<aligned+4<=mmio+mmio_size:events.append(('read' if loading else 'write',aligned,4,bus_value))
        if writeback:uc.reg_write(registers[rn],updated)
        corrections.append((pc,address,aligned,'read' if loading else 'write'))
        uc.reg_write(arm.UC_ARM_REG_PC,pc+4)
    machine.hook_add(unicorn.UC_HOOK_CODE,hook)
    return corrections
