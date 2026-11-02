import ghidra.app.script.GhidraScript;
public class SeedEvidenceFunctions extends GhidraScript {
 public void run() throws Exception {
  for(String arg:getScriptArgs()){
   var address=toAddr(Long.parseUnsignedLong(arg.replaceFirst("^0x",""),16));
   if(getInstructionAt(address)==null&&!disassemble(address))throw new IllegalStateException("Cannot disassemble "+address);
   if(getFunctionAt(address)==null){
    if(getFunctionContaining(address)!=null)throw new IllegalStateException("Existing overlapping function at "+address);
    if(createFunction(address,"original_entry_"+arg)==null)throw new IllegalStateException("Cannot create function "+address);
   }
   println("Seeded original pointer target "+address+" in read-only analysis session");
  }
 }
}
