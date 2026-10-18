// The eight original vector words are direct ARM B instructions at load address zero.
import ghidra.app.script.GhidraScript;
public class SeedArmSoundDriver extends GhidraScript {
 public void run() throws Exception {
  for(long offset=0;offset<32;offset+=4){
   var address=toAddr(offset);int word=currentProgram.getMemory().getInt(address);
   if((word>>>24)!=0xea)throw new Exception("Unexpected vector at "+address);
   long displacement=((long)(word<<8)>>8)*4;
   var target=toAddr(offset+8+displacement);
   addEntryPoint(address);disassemble(address);
   if(getFunctionAt(address)==null)createFunction(address,"vector_"+Long.toHexString(offset));
   addEntryPoint(target);disassemble(target);
   if(getFunctionAt(target)==null)createFunction(target,"vector_target_"+target);
  }
 }
}
