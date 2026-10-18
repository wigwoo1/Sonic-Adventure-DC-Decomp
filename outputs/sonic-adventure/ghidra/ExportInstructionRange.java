import ghidra.app.script.GhidraScript;
import java.io.*;
import ghidra.program.model.address.AddressSet;
public class ExportInstructionRange extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();File file=new File(args[0]);file.getParentFile().mkdirs();
  var start=toAddr(Long.parseUnsignedLong(args[1],16));
  var end=toAddr(Long.parseUnsignedLong(args[2],16));
  try(PrintWriter out=new PrintWriter(file)) {
   out.println("address\topcode\tinstruction\treferences");
   var iterator=currentProgram.getListing().getInstructions(new AddressSet(start,end),true);
   while(iterator.hasNext()) {
    var instruction=iterator.next();var address=instruction.getAddress();StringBuilder refs=new StringBuilder();
    for(var reference:getReferencesFrom(address)) {
     var target=reference.getToAddress();refs.append(reference.getReferenceType()).append(':').append(target);
     if(currentProgram.getMemory().contains(target)&&currentProgram.getMemory().contains(target.add(3)))refs.append('=').append(Integer.toUnsignedString(getInt(target),16));
     refs.append(' ');
    }
    out.println(address+"\t"+Integer.toHexString(getShort(address)&65535)+"\t"+instruction+"\t"+refs);
   }
  }
 }
}
