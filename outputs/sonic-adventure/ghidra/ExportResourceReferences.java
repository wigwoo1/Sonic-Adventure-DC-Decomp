// Trace resource-string and pointer-table references without assuming module addresses.
import ghidra.app.script.GhidraScript;
import java.io.*;
import java.nio.file.*;
public class ExportResourceReferences extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();
  try(PrintWriter output=new PrintWriter(args[1])) {
   output.println("resource_address\tresource\tdepth\treference_from\treference_type\tfunction\tinstruction");
   for(String line:Files.readAllLines(Path.of(args[0]))) {
    String[] parts=line.split("\t",2);var target=toAddr(Long.parseUnsignedLong(parts[0],16));
    for(var direct:getReferencesTo(target)) {
     var from=direct.getFromAddress();var function=getFunctionContaining(from);var instruction=getInstructionAt(from);
     output.println(parts[0]+"\t"+parts[1]+"\t1\t"+from+"\t"+direct.getReferenceType()+"\t"+(function==null?"":function.getEntryPoint())+"\t"+(instruction==null?"":instruction));
     if(function==null)for(var indirect:getReferencesTo(from)) {
      var source=indirect.getFromAddress();var owner=getFunctionContaining(source);var op=getInstructionAt(source);
      output.println(parts[0]+"\t"+parts[1]+"\t2\t"+source+"\t"+indirect.getReferenceType()+"\t"+(owner==null?"":owner.getEntryPoint())+"\t"+(op==null?"":op));
     }
    }
   }
  }
 }
}
