import ghidra.app.script.GhidraScript;
import java.io.*;
public class ExportFpuModeWriters extends GhidraScript {
 public void run() throws Exception {
  File file=new File(getScriptArgs()[0]);file.getParentFile().mkdirs();int count=0;
  try(PrintWriter out=new PrintWriter(file)) {
   out.println("address\topcode\tinstruction\tfunction\tprior_instructions");
   var iterator=currentProgram.getListing().getInstructions(true);
   while(iterator.hasNext()) {
    var instruction=iterator.next();var address=instruction.getAddress();int word=getShort(address)&0xffff;
    if((word&0xf0ff)!=0x406a&&(word&0xf0ff)!=0x4066&&word!=0xf3fd&&word!=0xfbfd)continue;
    var function=getFunctionContaining(address);StringBuilder prior=new StringBuilder();
    var start=address.subtract(24);var nearby=currentProgram.getListing().getInstructions(start,true);
    while(nearby.hasNext()) {
     var before=nearby.next();if(before.getAddress().compareTo(address)>=0)break;
     prior.append(before.getAddress()).append(':').append(before).append(" | ");
    }
    out.println(address+"\t"+Integer.toHexString(word)+"\t"+instruction+"\t"+(function==null?"unassigned":function.getEntryPoint())+"\t"+prior);count++;
   }
  }
  println("Exported "+count+" analyzed FPSCR writers/toggles; reachability and input values still need validation.");
 }
}
