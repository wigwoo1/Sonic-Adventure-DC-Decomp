// Seed addresses established by direct inspection of the USA Rev A startup.
import ghidra.app.script.GhidraScript;
public class SeedDreamcast extends GhidraScript {
 public void run() throws Exception {
  long[] addresses={0x8c010000L,0x8c0100a0L,0x8c0100c8L,0x8c0100d0L,0x8c0100d8L,0x8c053940L,0x8c5ff8b8L,0x8c5ff936L};
  for(long value:addresses){var address=toAddr(value);addEntryPoint(address);disassemble(address);if(getFunctionAt(address)==null)createFunction(address,"startup_"+Long.toHexString(value));}
 }
}
