import ghidra.app.script.GhidraScript;
public class SeedLinearSource extends GhidraScript {
 public void run() throws Exception {
  for(long value:new long[]{0x8c65e0dcL,0x8c65e0eaL}) {
   var target=toAddr(value);
   if(getInstructionAt(target)==null)println("Linear source entry "+target+" disassembled="+disassemble(target));
  }
 }
}
