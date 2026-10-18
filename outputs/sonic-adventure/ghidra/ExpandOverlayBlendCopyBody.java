import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.AddressSet;
public class ExpandOverlayBlendCopyBody extends GhidraScript {
 public void run() throws Exception {
  var function=getFunctionAt(toAddr(0x8c10cddcL));
  if(function==null)throw new IllegalStateException("Missing copy entry");
  if(Integer.toUnsignedLong(getInt(toAddr(0x8c10cdecL)))!=0x8c10ce38L)throw new IllegalStateException("Unexpected table literal");
  AddressSet body=new AddressSet(function.getBody());
  for(int length=0;length<=64;length+=4){
   long target=Integer.toUnsignedLong(getInt(toAddr(0x8c10ce38L+length)));
   if(target!=0x8c10ce32L-length)throw new IllegalStateException("Unexpected switch entry");
   disassemble(toAddr(target));
  }
  body.addRange(toAddr(0x8c10cdf2L),toAddr(0x8c10ce35L));
  function.setBody(body);
  println("Expanded 17 byte-checked copy switch destinations in read-only project memory.");
 }
}
