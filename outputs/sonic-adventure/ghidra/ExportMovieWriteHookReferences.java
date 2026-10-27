import ghidra.app.script.GhidraScript;
import java.io.*;
public class ExportMovieWriteHookReferences extends GhidraScript {
 public void run() throws Exception {
  File directory=new File(getScriptArgs()[0]);directory.mkdirs();
  var cell=toAddr(0x8c67c27cL);int refs=0,loads=0;
  try(PrintWriter out=new PrintWriter(new File(directory,"cell_references.tsv"))) {
   out.println("from\tto\ttype\tfunction\tinstruction");
   for(var ref:getReferencesTo(cell)) {
    var from=ref.getFromAddress();var function=getFunctionContaining(from);var ins=getInstructionAt(from);
    out.println(from+"\t"+cell+"\t"+ref.getReferenceType()+"\t"+(function==null?"unassigned":function.getEntryPoint())+"\t"+(ins==null?"data":ins));refs++;
   }
  }
  try(PrintWriter out=new PrintWriter(new File(directory,"nearby_literal_loads.tsv"))) {
   out.println("address\tliteral\tvalue\toffset_to_cell\tfunction\tinstruction\tnearby_instructions");
   var iterator=currentProgram.getListing().getInstructions(true);
   while(iterator.hasNext()) {
    var ins=iterator.next();var address=ins.getAddress();int word=getShort(address)&65535;
    if((word&0xf000)!=0xd000)continue;
    var literal=toAddr(((address.getOffset()+4)&~3L)+(word&255)*4L);
    if(!currentProgram.getMemory().contains(literal.add(3)))continue;
    long value=Integer.toUnsignedLong(getInt(literal)),physical=value&0x1fffffffL;
    if(physical<0x0c67c000L||physical>=0x0c67c400L)continue;
    var function=getFunctionContaining(address);StringBuilder nearby=new StringBuilder();
    var context=currentProgram.getListing().getInstructions(address.subtract(16),true);
    while(context.hasNext()) {var row=context.next();if(row.getAddress().compareTo(address.add(40))>0)break;nearby.append(row.getAddress()).append(':').append(row).append(" | ");}
    out.println(address+"\t"+literal+"\t"+Long.toHexString(value)+"\t"+(0x0c67c27cL-physical)+"\t"+(function==null?"unassigned":function.getEntryPoint())+"\t"+ins+"\t"+nearby);loads++;
   }
  }
  println("Analyzed references="+refs+", nearby PC-relative literal loads="+loads+". Bounded leads only; computed aliases and unanalyzed code are not excluded.");
 }
}
