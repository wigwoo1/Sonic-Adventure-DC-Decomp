import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.AddressSet;
import java.util.*;

/* Read-only project expansion from the two original bounded switch tables.
 * Raw SH-4 control flow includes normal entries into branch delay slots. */
public class ExpandDrawRenderStateBody extends GhidraScript {
 private int signed(int value,int bits){return (value<<(32-bits))>>(32-bits);}
 public void run() throws Exception {
  long start=0x8c654e60L;
  var function=getFunctionAt(toAddr(start));
  if(function==null)throw new IllegalStateException("Missing render-state entry");
  long end=function.getBody().getMaxAddress().getUnsignedOffset()+1;
  Map<Long,List<Long>> switches=new HashMap<>();
  for(long[] spec:new long[][]{{0x8c654fc0L,0x8c654fccL},{0x8c6550a2L,0x8c6550a8L}}){
   if((getShort(toAddr(spec[0]))&0xffff)!=0x0023)throw new IllegalStateException("Unexpected BRAF");
   List<Long> targets=new ArrayList<>();
   for(int i=0;i<12;i++)targets.add(spec[0]+4+getShort(toAddr(spec[1]+i*2)));
   switches.put(spec[0],targets);
  }
  AddressSet body=new AddressSet();Set<Long> walked=new HashSet<>();
  ArrayDeque<Long> todo=new ArrayDeque<>();todo.add(start);
  while(!todo.isEmpty()){
   monitor.checkCancelled();long a=todo.removeFirst();if(!walked.add(a))continue;
   if(a<start||a>=end||(a&1)!=0)throw new IllegalStateException("CFG escaped original bounds: "+Long.toHexString(a));
   var address=toAddr(a);var ins=getInstructionAt(address);
   if(ins==null){clearListing(address,toAddr(a+1));disassemble(address);ins=getInstructionAt(address);}
   if(ins==null||ins.getLength()!=2)throw new IllegalStateException("Cannot disassemble "+address);
   int w=getShort(address)&0xffff;body.addRange(address,toAddr(a+1));
   boolean conditional=(w>>>8==0x89||w>>>8==0x8b||w>>>8==0x8d||w>>>8==0x8f);
   boolean delayed=w==0x000b||w==0x0023||w>>>12==0xa||w>>>12==0xb||(w&0xf0ff)==0x400b||(w&0xf0ff)==0x402b||(conditional&&(w>>>8==0x8d||w>>>8==0x8f));
   if(delayed){
    if(a+3>=end)throw new IllegalStateException("Delay slot escaped bounds");
    if(getInstructionAt(toAddr(a+2))==null)disassemble(address);
    if(getInstructionAt(toAddr(a+2))==null)throw new IllegalStateException("Missing delay slot");
    body.addRange(toAddr(a+2),toAddr(a+3));
   }
   if(w==0x000b||(w&0xf0ff)==0x402b)continue;
   if(w==0x0023){
    var targets=switches.get(a);if(targets==null)throw new IllegalStateException("Unknown computed branch");
    todo.addAll(targets);continue;
   }
   if(w>>>12==0xa){todo.add(a+4+2*signed(w&0xfff,12));continue;}
   if(w>>>12==0xb||(w&0xf0ff)==0x400b){todo.add(a+4);continue;}
   if(conditional){todo.add(a+4+2*signed(w&255,8));todo.add(a+(delayed?4:2));continue;}
   todo.add(a+2);
  }
  if(!function.getBody().subtract(body).isEmpty())throw new IllegalStateException("Expansion dropped old body bytes");
  long old=function.getBody().getNumAddresses();function.setBody(body);
  println("Render-state raw CFG expanded from "+old+" to "+body.getNumAddresses()+" bytes, including both twelve-selector tables. Database changes are discarded by -readOnly.");
 }
}
