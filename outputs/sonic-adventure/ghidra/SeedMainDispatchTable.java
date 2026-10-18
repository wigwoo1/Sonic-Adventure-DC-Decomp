import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.pcode.JumpTable;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.SourceType;
import java.nio.file.*;
import java.io.*;
import java.util.ArrayList;
public class SeedMainDispatchTable extends GhidraScript {
 public void run() throws Exception {
  Path output=Path.of(getScriptArgs()[0]);Files.createDirectories(output);
  Address branch=toAddr(0x8c0a9792L);Function function=getFunctionAt(toAddr(0x8c0a966cL));
  if(function==null||!getInstructionAt(branch).toString().startsWith("braf"))throw new IllegalStateException("Unexpected dispatcher bytes/listing");
  long before=function.getBody().getNumAddresses();ArrayList<Address> destinations=new ArrayList<>();
  try(BufferedWriter writer=Files.newBufferedWriter(output.resolve("table_targets.tsv"))){
   writer.write("index\tword_address\tsigned_offset\ttarget\texisting_instruction\n");
   for(int i=0;i<83;i++){
    Address word=toAddr(0x8c0a979cL+i*2);short offset=currentProgram.getMemory().getShort(word);
    Address target=toAddr((branch.getOffset()+4+offset)&0xffffffffL);
    if(target.getOffset()<0x8c0a9842L||target.getOffset()>0x8c0ae0acL||(target.getOffset()&1)!=0)throw new IllegalStateException("Invalid target "+target);
    boolean existing=getInstructionAt(target)!=null;
    if(!existing&&!disassemble(target))throw new IllegalStateException("Cannot decode "+target);
    destinations.add(target);getInstructionAt(branch).addOperandReference(0,target,RefType.COMPUTED_JUMP,SourceType.USER_DEFINED);
    writer.write(i+"\t"+word+"\t"+offset+"\t"+target+"\t"+existing+"\n");
   }
  }
  new JumpTable(branch,destinations,true,0).writeOverride(function);
  CreateFunctionCmd.fixupFunctionBody(currentProgram,function,monitor);
  Files.writeString(output.resolve("body_change.tsv"),"before_bytes\tafter_bytes\n"+before+"\t"+function.getBody().getNumAddresses()+"\n");
  println("Temporary table override: "+before+" -> "+function.getBody().getNumAddresses()+" function-body bytes");
 }
}
