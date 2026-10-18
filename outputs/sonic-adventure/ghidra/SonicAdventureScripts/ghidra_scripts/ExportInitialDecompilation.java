// Export an index and initial pseudocode; this output is not verified source.
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import java.io.*;
public class ExportInitialDecompilation extends GhidraScript {
 public void run() throws Exception {
  File directory=getScriptArgs().length>2?new File(getScriptArgs()[0],currentProgram.getName()):new File(getScriptArgs()[0]);directory.mkdirs();
  int maximum=getScriptArgs().length>1?Integer.parseInt(getScriptArgs()[1]):150;
  DecompInterface decompiler=new DecompInterface();decompiler.openProgram(currentProgram);
  try(PrintWriter index=new PrintWriter(new File(directory,"function_index.tsv"));PrintWriter code=new PrintWriter(new File(directory,"initial_pseudocode.c"))){
   index.println("address\tname\tbody_bytes\tdecompilation_status");
   code.println("/* Ghidra-generated draft. Not buildable or behavior-verified. */");
   var functions=currentProgram.getFunctionManager().getFunctions(true);int exported=0;
   while(functions.hasNext()&&!monitor.isCancelled()){
    var function=functions.next();String status="indexed_only";
    if(maximum==0||exported<maximum){var result=decompiler.decompileFunction(function,15,monitor);if(result.decompileCompleted()&&result.getDecompiledFunction()!=null){code.println("/* Address: "+function.getEntryPoint()+" */");code.println(result.getDecompiledFunction().getC());status="draft_pseudocode";}else{status="failed";}exported++;if(exported%100==0){println("Drafts attempted: "+exported);code.flush();index.flush();}}
    index.println(function.getEntryPoint()+"\t"+function.getName()+"\t"+function.getBody().getNumAddresses()+"\t"+status);
   }
  }finally{decompiler.dispose();}
 }
}
