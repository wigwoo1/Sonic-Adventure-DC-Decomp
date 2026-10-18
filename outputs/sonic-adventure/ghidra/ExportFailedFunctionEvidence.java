import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import java.nio.file.*;
import java.io.*;
public class ExportFailedFunctionEvidence extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();if(args.length<2)throw new IllegalArgumentException("Output directory, then hexadecimal function addresses");
  Path out=Path.of(args[0]);Files.createDirectories(out);
  DecompInterface decompiler=new DecompInterface();decompiler.openProgram(currentProgram);
  try(BufferedWriter summary=Files.newBufferedWriter(out.resolve("diagnostics.tsv"))){
   summary.write("address\tfunction\tbody_bytes\tminimum\tmaximum\tcompleted\terror\n");
   for(int index=1;index<args.length;index++){
    long value=Long.parseUnsignedLong(args[index].replaceFirst("^0x",""),16);Function function=getFunctionAt(toAddr(value));
    if(function==null)throw new IllegalStateException("Missing indexed function "+args[index]);
    try(BufferedWriter listing=Files.newBufferedWriter(out.resolve(args[index]+"_instructions.tsv"))){
     listing.write("address\tlength\tbytes\tinstruction\tflows\n");
     InstructionIterator instructions=currentProgram.getListing().getInstructions(function.getBody(),true);
     while(instructions.hasNext()){
      Instruction instruction=instructions.next();StringBuilder bytes=new StringBuilder();
      for(byte b:instruction.getBytes())bytes.append(String.format("%02x",b&255));
      StringBuilder flows=new StringBuilder();for(var target:instruction.getFlows())flows.append(target).append(' ');
      listing.write(instruction.getAddress()+"\t"+instruction.getLength()+"\t"+bytes+"\t"+instruction+"\t"+flows+"\n");
     }
    }
    DecompileResults result=decompiler.decompileFunction(function,60,monitor);
    String error=result.getErrorMessage()==null?"":result.getErrorMessage().replace('\t',' ').replace('\n',' ');
    summary.write(function.getEntryPoint()+"\t"+function.getName()+"\t"+function.getBody().getNumAddresses()+"\t"+function.getBody().getMinAddress()+"\t"+function.getBody().getMaxAddress()+"\t"+result.decompileCompleted()+"\t"+error+"\n");summary.flush();
    if(result.decompileCompleted()&&result.getDecompiledFunction()!=null)Files.writeString(out.resolve(args[index]+"_draft.c"),"/* Ghidra draft only; not buildable/verified source. */\n"+result.getDecompiledFunction().getC());
    println(function.getEntryPoint()+" completed="+result.decompileCompleted()+" "+error);
   }
  }finally{decompiler.dispose();}
 }
}
