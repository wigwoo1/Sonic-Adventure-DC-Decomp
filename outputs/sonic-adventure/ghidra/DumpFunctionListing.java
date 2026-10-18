import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

public class DumpFunctionListing extends GhidraScript {
    @Override
    public void run() throws Exception {
        if (currentProgram == null || getScriptArgs().length < 1) {
            throw new IllegalArgumentException("Pass a function entry address in hexadecimal.");
        }
        long entry = Long.parseUnsignedLong(getScriptArgs()[0].replaceFirst("^0x", ""), 16);
        Function function = currentProgram.getFunctionManager().getFunctionAt(toAddr(entry));
        if (function == null) {
            throw new IllegalArgumentException("No function starts at " + toAddr(entry));
        }
        InstructionIterator instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
        while (instructions.hasNext() && !monitor.isCancelled()) {
            Instruction instruction = instructions.next();
            StringBuilder bytes = new StringBuilder();
            for (byte value : instruction.getBytes()) {
                if (bytes.length() != 0) bytes.append(' ');
                bytes.append(String.format("%02x", value & 0xff));
            }
            println(instruction.getAddress() + "  " + bytes + "  " + instruction);
        }
    }
}
