/* Ghidra draft only; not buildable/verified source. */

void FUN_8c040c02(uint *param_1,int param_2)

{
  ushort uVar1;
  undefined *puVar2;
  int iVar3;
  
  puVar2 = PTR_FUN_8c040d1c;
  iVar3 = param_2 * 4;
  *(undefined4 *)(DAT_8c040d18 + iVar3) =
       *(undefined4 *)(PTR_PTR_8c040d20 + (uint)*(ushort *)(DAT_8c040d10 + param_2 * 2) * 4);
  if ((*param_1 & 0x20) != 0) {
    puVar2 = PTR_FUN_8c040d24;
  }
  *(undefined **)PTR_DAT_8c040d28 = puVar2;
  uVar1 = *(ushort *)(DAT_8c040d14 + param_2 * 2);
  puVar2 = PTR_LAB_8c040d2c;
  if ((uVar1 & 1) != 0) {
    puVar2 = PTR_LAB_8c040d30;
  }
  *(undefined **)(DAT_8c040d34 + iVar3) = puVar2;
  puVar2 = PTR_LAB_8c040d38;
  if ((uVar1 & 2) != 0) {
    puVar2 = PTR_LAB_8c040d3c;
  }
  *(undefined **)(DAT_8c040d40 + iVar3) = puVar2;
  puVar2 = PTR_LAB_8c040d44;
  if ((uVar1 & 4) != 0) {
    puVar2 = PTR_LAB_8c040d48;
  }
  *(undefined **)(DAT_8c040d4c + iVar3) = puVar2;
  return;
}

