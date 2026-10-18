import ghidra.app.script.GhidraScript;
import java.nio.file.*;
import java.io.*;
public class ExportOverlaySelectorReferences extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();if(args.length<2)throw new IllegalArgumentException("Output TSV, then addresses");
  Path out=Path.of(args[0]);Files.createDirectories(out.getParent());
  try(BufferedWriter writer=Files.newBufferedWriter(out)){
   writer.write("target\tfrom\ttype\tinstruction\tfunction_entry\tfunction_name\n");
   for(int i=1;i<args.length;i++){
    var address=toAddr(Long.parseUnsignedLong(args[i].replaceFirst("^0x",""),16));
    var references=currentProgram.getReferenceManager().getReferencesTo(address);
    while(references.hasNext()){
     var ref=references.next();var ins=getInstructionAt(ref.getFromAddress());var fun=getFunctionContaining(ref.getFromAddress());
     writer.write(address+"\t"+ref.getFromAddress()+"\t"+ref.getReferenceType()+"\t"+(ins==null?"":ins)+"\t"+(fun==null?"":fun.getEntryPoint())+"\t"+(fun==null?"":fun.getName())+"\n");
    }
   }
  }
  println("Wrote "+out);
 }
}
