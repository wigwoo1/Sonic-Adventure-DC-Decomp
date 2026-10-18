# Dump decoded Ghidra instructions and outgoing references for one address range.
from java.io import File, PrintWriter
from java.lang import Long
from ghidra.program.model.address import AddressSet

args = getScriptArgs()
output_file = File(args[0])
output_file.getParentFile().mkdirs()
start = toAddr(Long.parseUnsignedLong(args[1], 16))
end = toAddr(Long.parseUnsignedLong(args[2], 16))
listing = currentProgram.getListing()
memory = currentProgram.getMemory()
with PrintWriter(output_file) as output:
    output.println("address\topcode\tinstruction\treferences")
    instructions = listing.getInstructions(AddressSet(start, end), True)
    while instructions.hasNext():
        instruction = instructions.next()
        address = instruction.getAddress()
        opcode = getShort(address) & 0xffff
        refs = []
        for reference in getReferencesFrom(address):
            target = reference.getToAddress()
            item = str(reference.getReferenceType()) + ":" + str(target)
            if memory.contains(target) and memory.contains(target.add(3)):
                item += "=" + Long.toString(getInt(target) & 0xffffffff, 16)
            refs.append(item)
        output.println("%s\t%04x\t%s\t%s" %
                       (str(address), opcode, str(instruction), " ".join(refs)))
