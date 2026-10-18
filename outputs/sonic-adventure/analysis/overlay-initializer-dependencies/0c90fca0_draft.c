/* Ghidra draft only; not buildable/verified source. */

void FUN_0c90fca0(int param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int *piVar6;
  int local_48 [5];
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  
  uVar1 = DAT_0c90fec4;
  piVar6 = local_48;
  local_48[0] = *(int *)PTR_DAT_0c90fec0;
  local_48[1] = *(undefined4 *)(PTR_DAT_0c90fec0 + 4);
  local_48[2] = *(undefined4 *)(PTR_DAT_0c90fec0 + 8);
  local_48[3] = *(undefined4 *)(PTR_DAT_0c90fec0 + 0xc);
  local_48[4] = *(undefined4 *)(PTR_DAT_0c90fec0 + 0x10);
  local_34 = *(undefined4 *)(PTR_DAT_0c90fec0 + 0x14);
  local_30 = *(undefined4 *)(PTR_DAT_0c90fec0 + 0x18);
  local_2c = *(undefined4 *)(PTR_DAT_0c90fec0 + 0x1c);
  local_28 = *(undefined4 *)(PTR_DAT_0c90fec0 + 0x20);
  local_24 = *(undefined4 *)(PTR_DAT_0c90fec0 + 0x24);
  local_20 = *(undefined4 *)(PTR_DAT_0c90fec0 + 0x28);
  local_1c = *(undefined4 *)(PTR_DAT_0c90fec0 + 0x2c);
  local_18 = *(undefined4 *)(PTR_DAT_0c90fec0 + 0x30);
  local_14 = *(undefined4 *)(PTR_DAT_0c90fec0 + 0x34);
  while (local_48[0] != -1) {
    iVar3 = *(int *)(*(int *)(*piVar6 * 4 + param_1 + 0x24c) + 4);
    iVar4 = 0;
    iVar2 = *(int *)(iVar3 + 0x10);
    uVar5 = (uint)*(ushort *)(iVar3 + 0x16);
    if (uVar5 != 0) {
      do {
        iVar4 = iVar4 + 1;
        *(undefined4 *)(iVar2 + 0xc) = 0x9e;
        *(uint *)(iVar2 + 0x10) = *(uint *)(iVar2 + 0x10) | uVar1;
        iVar2 = iVar2 + 0x14;
      } while (iVar4 < (int)uVar5);
    }
    piVar6 = piVar6 + 1;
    local_48[0] = *piVar6;
  }
  return;
}

