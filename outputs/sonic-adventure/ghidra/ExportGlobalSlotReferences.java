import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import java.nio.file.*;
import java.io.*;
public class ExportGlobalSlotReferences extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();if(args.length<2)throw new IllegalArgumentException("output directory and hexadecimal slots");
  Path out=Path.of(args[0]);Files.createDirectories(out);Path target=out.resolve("slot_references.tsv");
  if(Files.exists(target))throw new IllegalStateException("Preserve earlier reference evidence");
  try(BufferedWriter w=Files.newBufferedWriter(target)){
   w.write("slot\tfrom\tfunction\ttype\tinstruction\n");
   for(int i=1;i<args.length;i++){
    var slot=toAddr(Long.parseUnsignedLong(args[i].replaceFirst("^0x",""),16));
    var refs=currentProgram.getReferenceManager().getReferencesTo(slot);
    while(refs.hasNext()){
     var r=refs.next();Function f=getFunctionContaining(r.getFromAddress());var instruction=getInstructionAt(r.getFromAddress());
     w.write(slot+"\t"+r.getFromAddress()+"\t"+(f==null?"":f.getEntryPoint())+"\t"+r.getReferenceType()+"\t"+(instruction==null?"":instruction.toString())+"\n");
    }
   }
  }
 }
}
