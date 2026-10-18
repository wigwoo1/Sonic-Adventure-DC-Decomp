import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Instruction;
import java.io.BufferedWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;

public class DumpInstructionRangeFile extends GhidraScript {
    @Override
    public void run() throws Exception {
        if (currentProgram == null || getScriptArgs().length < 3) {
            throw new IllegalArgumentException("Pass start, byte length, and output path.");
        }
        long start = Long.parseUnsignedLong(getScriptArgs()[0].replaceFirst("^0x", ""), 16);
        long end = start + Long.parseLong(getScriptArgs()[1]);
        Path output = Path.of(getScriptArgs()[2]);
        Files.createDirectories(output.getParent());
        try (BufferedWriter writer = Files.newBufferedWriter(output, StandardOpenOption.CREATE,
                StandardOpenOption.TRUNCATE_EXISTING)) {
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
                writer.write(instruction.getAddress() + "  " + bytes + "  " + instruction);
                writer.newLine();
                cursor = instruction.getMaxAddress().getOffset() + 1;
            }
        }
        println("Wrote listing: " + output);
    }
}
