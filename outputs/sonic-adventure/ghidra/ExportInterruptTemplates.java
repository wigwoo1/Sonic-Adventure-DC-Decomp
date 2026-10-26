import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.AddressSet;
import java.nio.file.*;
import java.io.*;
/* Temporary listing changes only; caller must use a read-only project. These
 * copied vector fragments are not ordinary callable Ghidra functions. */
public class ExportInterruptTemplates extends GhidraScript {
 public void run() throws Exception {
  Path out=Path.of(getScriptArgs()[0]);Files.createDirectories(out);
  long[][] ranges={{0x8c65e7a4L,0x8c65e7d8L},{0x8c65e7d8L,0x8c65e80cL}};
  try(BufferedWriter summary=Files.newBufferedWriter(out.resolve("template_diagnostics.tsv"))){
   summary.write("address\tend_exclusive\tinstruction_rows\tkind\n");
   for(long[] range:ranges){
    var start=toAddr(range[0]);var end=toAddr(range[1]-1);
    clearListing(start,end);disassemble(start);
    int count=0;
    try(BufferedWriter listing=Files.newBufferedWriter(out.resolve(Long.toHexString(range[0])+"_instructions.tsv"))){
     listing.write("address\tlength\tbytes\tinstruction\tflows\n");
     var instructions=currentProgram.getListing().getInstructions(new AddressSet(start,end),true);
     while(instructions.hasNext()){
      var instruction=instructions.next();StringBuilder bytes=new StringBuilder(),flows=new StringBuilder();
      for(byte b:instruction.getBytes())bytes.append(String.format("%02x",b&255));
      for(var target:instruction.getFlows())flows.append(target).append(' ');
      listing.write(instruction.getAddress()+"\t"+instruction.getLength()+"\t"+bytes+"\t"+instruction+"\t"+flows+"\n");count++;
     }
    }
    summary.write(start+"\t"+toAddr(range[1])+"\t"+count+"\tcopied vector fragment, not a callable function\n");
   }
  }
 }
}
