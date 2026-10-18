import ghidra.app.script.GhidraScript;
import java.io.*;
public class ExportGbrStateWriters extends GhidraScript {
 public void run() throws Exception {
  File file=new File(getScriptArgs()[0]);file.getParentFile().mkdirs();int count=0;
  try(PrintWriter out=new PrintWriter(file)) {
   out.println("address\topcode\tinstruction\tfunction\tnearby_instructions");
   var iterator=currentProgram.getListing().getInstructions(true);
   while(iterator.hasNext()) {
    var instruction=iterator.next();var address=instruction.getAddress();int word=getShort(address)&0xffff;
    int kind=word&0xf0ff;
    if(kind!=0x401e&&kind!=0x4017&&kind!=0x0012&&kind!=0x4013)continue;
    var function=getFunctionContaining(address);StringBuilder nearby=new StringBuilder();
    var context=currentProgram.getListing().getInstructions(address.subtract(48),true);
    while(context.hasNext()) {
     var row=context.next();if(row.getAddress().compareTo(address.add(16))>0)break;
     nearby.append(row.getAddress()).append(':').append(row).append(" | ");
    }
    out.println(address+"\t"+Integer.toHexString(word)+"\t"+instruction+"\t"+(function==null?"unassigned":function.getEntryPoint())+"\t"+nearby);count++;
   }
  }
  println("Exported "+count+" analyzed GBR transfers; values/reachability require separate proof.");
 }
}
