/* Ghidra draft only; not buildable/verified source. */

void FUN_8c03fe60(uint *param_1)

{
  ushort uVar1;
  undefined *puVar2;
  
  puVar2 = PTR_LAB_8c04004c;
  uVar1 = *DAT_8c040044;
  *DAT_8c040050 = *(undefined4 *)(PTR_PTR_8c040048 + (uint)*DAT_8c040040 * 4);
  if ((uVar1 & 1) != 0) {
    puVar2 = PTR_LAB_8c040054;
  }
  *DAT_8c040058 = puVar2;
  if ((uVar1 & 2) == 0) {
    puVar2 = PTR_LAB_8c040064;
    if ((*param_1 & 0x20) != 0) {
      puVar2 = PTR_LAB_8c040068;
    }
  }
  else {
    puVar2 = PTR_LAB_8c04005c;
    if ((*param_1 & 0x20) != 0) {
      puVar2 = PTR_LAB_8c040060;
    }
  }
  *DAT_8c04006c = puVar2;
  puVar2 = PTR_LAB_8c040070;
  if ((uVar1 & 4) != 0) {
    puVar2 = PTR_LAB_8c040074;
  }
  *DAT_8c040078 = puVar2;
  return;
}

