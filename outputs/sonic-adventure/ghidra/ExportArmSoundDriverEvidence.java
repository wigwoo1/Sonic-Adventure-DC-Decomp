// Full ARM instruction bytes and analysis-input hashes, separate from draft C.
import ghidra.app.script.GhidraScript;
import java.io.*;
import java.security.MessageDigest;
public class ExportArmSoundDriverEvidence extends GhidraScript {
 public void run() throws Exception {
  File directory=new File(getScriptArgs()[0],currentProgram.getName());directory.mkdirs();
  long count=0,codeBytes=0;
  try(PrintWriter out=new PrintWriter(new File(directory,"instructions.tsv"))){
   out.println("address\tlength\tbytes\tinstruction\tflows\tfunction_entry");
   var iterator=currentProgram.getListing().getInstructions(true);
   while(iterator.hasNext()&&!monitor.isCancelled()){
    var instruction=iterator.next();byte[] bytes=instruction.getBytes();
    StringBuilder hex=new StringBuilder(),flows=new StringBuilder();
    for(byte value:bytes)hex.append(String.format("%02x",value&255));
    for(var target:instruction.getFlows())flows.append(target).append(' ');
    var function=currentProgram.getFunctionManager().getFunctionContaining(instruction.getAddress());
    out.println(instruction.getAddress()+"\t"+bytes.length+"\t"+hex+"\t"+instruction.toString().replace('\t',' ')+"\t"+flows+"\t"+(function==null?"":function.getEntryPoint()));
    count++;codeBytes+=bytes.length;
   }
  }
  var block=currentProgram.getMemory().getBlock(toAddr(0));
  byte[] image=new byte[(int)block.getSize()];currentProgram.getMemory().getBytes(block.getStart(),image);
  StringBuilder digest=new StringBuilder();
  for(byte value:MessageDigest.getInstance("SHA-256").digest(image))digest.append(String.format("%02x",value&255));
  try(PrintWriter out=new PrintWriter(new File(directory,"program_metadata.tsv"))){
   out.println("key\tvalue");out.println("language\t"+currentProgram.getLanguageID());
   out.println("image_base\t"+currentProgram.getImageBase());out.println("image_size\t"+image.length);
   out.println("image_sha256\t"+digest);out.println("instruction_rows\t"+count);out.println("instruction_bytes\t"+codeBytes);
  }
 }
}
