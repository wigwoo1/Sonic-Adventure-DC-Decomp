import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.AddressSet;
import java.nio.file.*;
import java.io.*;
import java.security.MessageDigest;
import java.util.HexFormat;

/** Read existing analysis only; do not disassemble, rename or create functions. */
public class ExportCrossVersionSpanEvidence extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();
  if(args.length!=2)throw new IllegalArgumentException("input TSV and output directory");
  Path input=Path.of(args[0]),out=Path.of(args[1]);Files.createDirectories(out);
  Path entries=out.resolve("entry_observations.tsv"),references=out.resolve("incoming_references.tsv");
  if(Files.exists(entries)||Files.exists(references))throw new IllegalStateException("Preserve previous evidence");
  var rows=Files.readAllLines(input);
  try(BufferedWriter e=Files.newBufferedWriter(entries);BufferedWriter r=Files.newBufferedWriter(references)){
   e.write("Japan_entry\tUSA_entry\tspan_bytes\tspan_sha256\tfunction_at\tcontaining_function\tinstruction_at\tinstruction_bytes_in_span\tcall_instructions\tcomputed_flow_instructions\tdirect_calls_to_entry\tdata_references_to_entry\tall_references_to_entry\n");
   r.write("Japan_entry\tUSA_entry\tfrom\tfrom_function\ttype\tinstruction\n");
   for(int i=1;i<rows.size();i++){
    monitor.checkCancelled();String[] row=rows.get(i).split("\t",-1);
    var address=toAddr(Long.parseUnsignedLong(row[1],16));int size=Integer.parseInt(row[2]);
    byte[] bytes=new byte[size];int got=currentProgram.getMemory().getBytes(address,bytes);
    String digest=HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(bytes));
    if(got!=size||!digest.equals(row[3]))throw new IllegalStateException("Program bytes differ at "+address);
    var at=getFunctionAt(address);var containing=getFunctionContaining(address);var first=getInstructionAt(address);
    int code=0,calls=0,computed=0,direct=0,data=0,total=0;
    var instructions=currentProgram.getListing().getInstructions(new AddressSet(address,address.add(size-1)),true);
    while(instructions.hasNext()){
     var instruction=instructions.next();code+=instruction.getLength();
     if(instruction.getFlowType().isCall())calls++;
     if(instruction.getFlowType().isComputed())computed++;
    }
    var refs=currentProgram.getReferenceManager().getReferencesTo(address);
    while(refs.hasNext()){
     var ref=refs.next();var owner=getFunctionContaining(ref.getFromAddress());var instruction=getInstructionAt(ref.getFromAddress());
     total++;if(ref.getReferenceType().isCall())direct++;if(ref.getReferenceType().isData())data++;
     r.write(row[0]+"\t"+address+"\t"+ref.getFromAddress()+"\t"+(owner==null?"":owner.getEntryPoint())+"\t"+ref.getReferenceType()+"\t"+(instruction==null?"":instruction.toString())+"\n");
    }
    e.write(row[0]+"\t"+address+"\t"+size+"\t"+digest+"\t"+(at==null?"":at.getEntryPoint())+"\t"+(containing==null?"":containing.getEntryPoint())+"\t"+(first==null?"":first.toString())+"\t"+code+"\t"+calls+"\t"+computed+"\t"+direct+"\t"+data+"\t"+total+"\n");
   }
  }
  println("Validated original bytes and exported existing entry/reference observations for "+(rows.size()-1)+" candidate spans; no analysis modified.");
 }
}
