import ghidra.app.script.GhidraScript;
public class SeedOwnerHelperBranches extends GhidraScript {
 public void run() throws Exception {
  long base=0x8c07950eL;
  for(int state=0;state<15;state++) {
   short displacement=getShort(toAddr(0x8c079510L+state*2L));
   var target=toAddr(base+displacement);
   if(getInstructionAt(target)==null)println("state "+state+" target "+target+" disassembled="+disassemble(target));
  }
 }
}
