#@runtime PyGhidra
#@category SonicAdventure.Overlay
"""Seed the three entry points stored in ADVERTISE.BIN's leading pointer table."""

base = currentProgram.minAddress.offset
length = currentProgram.memory.getBlock(currentProgram.minAddress).size
seeded = []
for offset in range(0, min(0x100, length - 3), 4):
    pointer = currentProgram.memory.getInt(currentProgram.minAddress.add(offset)) & 0xFFFFFFFF
    if pointer < base or pointer >= base + length - 2 or pointer & 1:
        continue
    address = toAddr(pointer)
    opcode = getShort(address) & 0xFFFF
    if opcode != 0x4F22 and (opcode & 0xFF0F) != 0x2F06:
        continue
    if getFunctionContaining(address) is not None:
        continue
    addEntryPoint(address)
    disassemble(address)
    if getFunctionAt(address) is None:
        createFunction(address, 'advertise_entry_%08x' % pointer)
    seeded.append(pointer)

if not seeded:
    raise RuntimeError('No valid prologue targets were found in the ADVERTISE header table')
print('Seeded ADVERTISE entry points: ' + ', '.join('%08x' % value for value in seeded))
