import ghidra.app.script.GhidraScript;
public class SeedCallbackFunctions extends GhidraScript {
 public void run() throws Exception {
  for(String value:getScriptArgs()) {
   var address=toAddr(Long.parseUnsignedLong(value,16));
   disassemble(address);
   if(getFunctionAt(address)==null)createFunction(address,"callback_"+value);
   println("Callback entry "+address+" function="+getFunctionAt(address));
  }
 }
}
