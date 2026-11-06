/* Native recovery of src/system/math.src:_fill_bytes. The original requires
 * a positive count and writes backward using value's low byte. Invalid empty,
 * negative or null requests return without the original wraparound overwrite.
 * A positive request still requires a valid caller-owned destination extent. */
void fill_bytes(void *memory,int value,int count)
{
    if(!memory||count<=0)return;
    unsigned char *end=(unsigned char *)memory+count;
    do {*--end=(unsigned char)value;}while(--count);
}
