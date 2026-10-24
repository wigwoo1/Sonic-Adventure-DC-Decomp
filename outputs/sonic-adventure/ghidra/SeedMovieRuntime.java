// Observed boot load/entry, not the generic overlay base.
import ghidra.app.script.GhidraScript;
public class SeedMovieRuntime extends GhidraScript {
 public void run() throws Exception {
  var entry=toAddr(0x8ceb0000L);
  if(!currentProgram.getMinAddress().equals(entry) || (getShort(entry)&0xffff)!=0xa00e)
   throw new IllegalStateException("Unexpected MOVIE runtime image/base");
  addEntryPoint(entry);disassemble(entry);
  if(getFunctionAt(entry)==null)createFunction(entry,"movie_runtime_entry_8ceb0000");
 }
}
