import ghidra.app.script.GhidraScript;
import java.io.*;
public class ExportRoutineInstructions extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();File directory=new File(args[0]);directory.mkdirs();
  for(int i=1;i<args.length;i++) {
   var entry=toAddr(Long.parseUnsignedLong(args[i],16));var function=getFunctionAt(entry);
   if(function==null)throw new IllegalArgumentException("No function at "+entry);
   try(PrintWriter output=new PrintWriter(new File(directory,args[i]+".tsv"))) {
    output.println("address\topcode\tinstruction\treferences");
    var instructions=currentProgram.getListing().getInstructions(function.getBody(),true);
    while(instructions.hasNext()) {
     var instruction=instructions.next();var address=instruction.getAddress();StringBuilder refs=new StringBuilder();
     for(var reference:getReferencesFrom(address)) {
      var target=reference.getToAddress();refs.append(reference.getReferenceType()).append(':').append(target);
      if(currentProgram.getMemory().contains(target)&&currentProgram.getMemory().contains(target.add(3)))refs.append("=").append(Integer.toUnsignedString(getInt(target),16));
      refs.append(' ');
     }
     output.println(address+"\t"+Integer.toHexString(getShort(address)&0xffff)+"\t"+instruction+"\t"+refs);
    }
   }
  }
 }
}
