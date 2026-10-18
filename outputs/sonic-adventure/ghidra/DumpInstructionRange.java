import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Instruction;

public class DumpInstructionRange extends GhidraScript {
    @Override
    public void run() throws Exception {
        if (currentProgram == null || getScriptArgs().length < 2) {
            throw new IllegalArgumentException("Pass a start address and byte length.");
        }
        long start = Long.parseUnsignedLong(getScriptArgs()[0].replaceFirst("^0x", ""), 16);
        long length = Long.parseLong(getScriptArgs()[1]);
        long end = start + length;
        for (long cursor = start; cursor < end;) {
            Instruction instruction = getInstructionAt(toAddr(cursor));
            if (instruction == null) {
                if (!disassemble(toAddr(cursor))) {
                    throw new IllegalStateException("Could not disassemble at " + toAddr(cursor));
                }
                instruction = getInstructionAt(toAddr(cursor));
            }
            if (instruction == null || instruction.getMaxAddress().getOffset() >= end) {
                throw new IllegalStateException("Instruction crosses requested range at " + toAddr(cursor));
            }
            StringBuilder bytes = new StringBuilder();
            for (byte value : instruction.getBytes()) {
                if (bytes.length() != 0) bytes.append(' ');
                bytes.append(String.format("%02x", value & 0xff));
            }
            println(instruction.getAddress() + "  " + bytes + "  " + instruction);
            cursor = instruction.getMaxAddress().getOffset() + 1;
        }
    }
}
