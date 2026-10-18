// Queue helper advances LR by four on successful enqueue. Seed those return
// destinations; the original direct-flow analysis did not classify them as code.
import ghidra.app.script.GhidraScript;
public class SeedArmSequencerReturns extends GhidraScript {
 public void run() throws Exception {
  long delta=currentProgram.getName().startsWith("MASARU")?8:0;
  for(long[] span:new long[][]{{0x4290,0x4298},{0x47fc,0x4810}}){
   var first=toAddr(span[0]+delta);var last=toAddr(span[1]+delta+3);
   clearListing(first,last);disassemble(first);
   for(long a=span[0]+delta;a<=span[1]+delta;a+=4)
    if(getInstructionAt(toAddr(a))==null)throw new Exception("Missing queue-return instruction at "+toAddr(a));
  }
 }
}
