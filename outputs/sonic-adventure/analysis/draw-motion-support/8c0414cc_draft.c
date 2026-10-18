/* Ghidra draft only; not buildable/verified source. */

void callback_8c0414cc(int param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  uVar3 = *(undefined4 *)(param_1 + 0x18);
  uVar2 = *(undefined4 *)(param_1 + 0x1c);
  puVar1 = (undefined4 *)((uint)*DAT_8c0415c4 * 0xc + DAT_8c0415e0);
  *puVar1 = *(undefined4 *)(param_1 + 0x14);
  puVar1[1] = uVar3;
  puVar1[2] = uVar2;
  return;
}

