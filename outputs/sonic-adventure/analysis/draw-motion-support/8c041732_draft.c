/* Ghidra draft only; not buildable/verified source. */

void callback_8c041732(uint *param_1)

{
  ushort *puVar1;
  undefined4 *puVar2;
  
  puVar1 = DAT_8c0418e4;
  puVar2 = (undefined4 *)param_1[1];
  if ((puVar2 != (undefined4 *)0x0) && ((*param_1 & 8) == 0)) {
    *(undefined4 *)(DAT_8c0418f8 + (uint)*DAT_8c0418e4 * 4) = *puVar2;
    *(undefined4 *)(DAT_8c0418fc + (uint)*puVar1 * 4) = puVar2[1];
  }
  return;
}

