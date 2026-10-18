// Candidate roots for the selected STG modules; runtime reachability remains unverified.
import ghidra.app.script.GhidraScript;
public class SeedOverlay extends GhidraScript {
 public void run() throws Exception {
  var entry=currentProgram.getMinAddress();
  int first=getShort(entry)&0xffff;
  boolean plausibleRoot=first==0x2fe6||first==0x5548||first==0x4f22||(first>>12)==0xd||(first&0xff00)==0x7f00;
  if(plausibleRoot) {
   addEntryPoint(entry);disassemble(entry);
   if(getFunctionAt(entry)==null)createFunction(entry,"module_root_candidate");
  }else println("Unclassified module header; only pointed-to prologue candidates will be seeded");
  var block=currentProgram.getMemory().getBlock(entry);long length=block.getSize();
  java.util.Set<Long> candidates=new java.util.HashSet<>();
  for(long offset=0;offset+4<=length;offset+=4) {
   long pointer=Integer.toUnsignedLong(getInt(entry.add(offset)));
   if((pointer&1)!=0||pointer<entry.getOffset()||pointer>=entry.getOffset()+length-2)continue;
   var address=toAddr(pointer);int opcode=getShort(address)&0xffff;
   // Only pointed-to PR saves or callee-saved GPR pushes are admitted as candidates.
   if(opcode==0x4f22||((opcode&0xff0f)==0x2f06&&((opcode>>4)&15)>=8&&((opcode>>4)&15)<=14))candidates.add(pointer);
  }
  for(long pointer:candidates) {
   var address=toAddr(pointer);
   if(getFunctionContaining(address)!=null)continue;
   addEntryPoint(address);disassemble(address);
   if(getFunctionAt(address)==null)createFunction(address,"callback_candidate_"+Long.toHexString(pointer));
  }
  println("Pointed-to prologue candidates: "+candidates.size());
 }
}
