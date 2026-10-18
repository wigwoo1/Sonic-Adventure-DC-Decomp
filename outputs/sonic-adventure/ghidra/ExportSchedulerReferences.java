import ghidra.app.script.GhidraScript;
import java.io.*;
import java.util.*;
public class ExportSchedulerReferences extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();File directory=new File(args[0]);directory.mkdirs();
  Set<String> owners=new TreeSet<>();
  try(PrintWriter output=new PrintWriter(new File(directory,"scheduler_references.tsv"))) {
   output.println("target\tsource\ttype\tcontaining_function\tinstruction");
   for(int i=1;i<args.length;i++) {
    var target=toAddr(Long.parseUnsignedLong(args[i],16));
    var references=currentProgram.getReferenceManager().getReferencesTo(target);
    while(references.hasNext()) {
     var reference=references.next();var from=reference.getFromAddress();
     var function=getFunctionContaining(from);var instruction=getInstructionAt(from);
     if(function!=null)owners.add(function.getEntryPoint().toString());
     output.println(target+"\t"+from+"\t"+reference.getReferenceType()+"\t"+(function==null?"":function.getEntryPoint())+"\t"+(instruction==null?"":instruction));
    }
   }
  }
  try(PrintWriter output=new PrintWriter(new File(directory,"scheduler_caller_entries.txt"))) {
   for(String owner:owners)output.println(owner);
  }
 }
}
