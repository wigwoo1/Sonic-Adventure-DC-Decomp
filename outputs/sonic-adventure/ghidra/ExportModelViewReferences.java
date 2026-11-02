import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import java.nio.file.*;
import java.io.*;
public class ExportModelViewReferences extends GhidraScript {
 public void run() throws Exception {
  Path out=Path.of(getScriptArgs()[0]);Files.createDirectories(out);
  Path target=out.resolve("references.tsv");if(Files.exists(target))throw new IllegalStateException("Preserve earlier evidence");
  long[] values={0x8c8ea3e0L,0x8c617c44L,0x8c620f3cL,0x8c63aa96L,0x8c63ac16L,0x8c63acb2L,0x8c63ad32L,0x8c63c6c4L,0x8c63c990L,0x8c63cb9eL,0x8c63ccd0L,0x8c63cddaL};
  try(BufferedWriter w=Files.newBufferedWriter(target)){
   w.write("kind\tquery\tfunction\tfrom\tfrom_function\ttype\n");
   for(long value:values){
    var address=toAddr(value);Function f=getFunctionContaining(address);
    w.write("containing\t"+address+"\t"+(f==null?"":f.getEntryPoint())+"\t\t\t\n");
    var destination=f==null?address:f.getEntryPoint();
    ReferenceIterator refs=currentProgram.getReferenceManager().getReferencesTo(destination);
    while(refs.hasNext()){
     Reference r=refs.next();Function caller=getFunctionContaining(r.getFromAddress());
     w.write("incoming\t"+destination+"\t"+(f==null?"":f.getEntryPoint())+"\t"+r.getFromAddress()+"\t"+(caller==null?"":caller.getEntryPoint())+"\t"+r.getReferenceType()+"\n");
    }
   }
  }
 }
}
