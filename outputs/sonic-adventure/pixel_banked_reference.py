"""Independent whole-word interleave; the existing game storage stays unchanged."""
import array,sys
def texture_view(banks):
 assert len(banks)==0x800000 and sys.byteorder=='little'
 words=array.array('I');assert words.itemsize==4;words.frombytes(banks)
 view=array.array('I',[0])*len(words);half=len(words)//2
 view[0::2]=words[:half];view[1::2]=words[half:]
 return view.tobytes()
