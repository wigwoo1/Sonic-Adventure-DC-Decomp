import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;

public class CreateFunctionAt extends GhidraScript {
    @Override
    public void run() throws Exception {
        if (currentProgram == null || getScriptArgs().length < 1) {
            throw new IllegalArgumentException("Pass a function entry address in hexadecimal.");
        }
        long entry = Long.parseUnsignedLong(getScriptArgs()[0].replaceFirst("^0x", ""), 16);
        if (!disassemble(toAddr(entry))) {
            throw new IllegalStateException("Could not disassemble at " + toAddr(entry));
        }
        Function function = currentProgram.getFunctionManager().getFunctionAt(toAddr(entry));
        if (function == null) {
            function = createFunction(toAddr(entry), null);
        }
        if (function == null) {
            throw new IllegalStateException("Could not create function at " + toAddr(entry));
        }
        println("Function established at " + function.getEntryPoint() + " through " + function.getBody().getMaxAddress());
    }
}
