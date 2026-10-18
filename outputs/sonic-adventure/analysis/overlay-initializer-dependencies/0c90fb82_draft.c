/* Ghidra draft only; not buildable/verified source. */

void FUN_0c90fb82(int param_1)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  undefined4 local_50;
  undefined4 local_4c;
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
  
  uVar2 = DAT_0c90fc84;
  local_48[0] = *(int *)PTR_DAT_0c90fc7c;
  piVar4 = local_48;
  local_48[1] = *(undefined4 *)(PTR_DAT_0c90fc7c + 4);
  local_48[2] = *(undefined4 *)(PTR_DAT_0c90fc7c + 8);
  local_48[3] = *(undefined4 *)(PTR_DAT_0c90fc7c + 0xc);
  local_48[4] = *(undefined4 *)(PTR_DAT_0c90fc7c + 0x10);
  local_34 = *(undefined4 *)(PTR_DAT_0c90fc7c + 0x14);
  local_30 = *(undefined4 *)(PTR_DAT_0c90fc7c + 0x18);
  local_2c = *(undefined4 *)(PTR_DAT_0c90fc7c + 0x1c);
  local_28 = *(undefined4 *)(PTR_DAT_0c90fc7c + 0x20);
  local_24 = *(undefined4 *)(PTR_DAT_0c90fc7c + 0x24);
  local_20 = *(undefined4 *)(PTR_DAT_0c90fc7c + 0x28);
  local_1c = *(undefined4 *)(PTR_DAT_0c90fc7c + 0x2c);
  local_18 = *(undefined4 *)(PTR_DAT_0c90fc7c + 0x30);
  local_14 = *(undefined4 *)(PTR_DAT_0c90fc7c + 0x34);
  local_50 = *(undefined4 *)PTR_DAT_0c90fc80;
  local_4c = *(undefined4 *)(PTR_DAT_0c90fc80 + 4);
  bVar1 = *(byte *)(*(int *)(param_1 + 0xec) + 0x192);
  uVar5 = (uint)(char)bVar1;
  if (3 < bVar1) {
    uVar5 = 0;
  }
  while (local_48[0] != -1) {
    iVar6 = *(int *)(*(int *)(*piVar4 * 4 + param_1 + 0x24c) + 4);
    iVar7 = 0;
    iVar3 = *(int *)(iVar6 + 0x10);
    uVar8 = (uint)*(ushort *)(iVar6 + 0x16);
    if (uVar8 != 0) {
      do {
        iVar7 = iVar7 + 1;
        *(uint *)(iVar3 + 0xc) = (uint)*(ushort *)((int)&local_50 + (uVar5 & 0xff) * 2);
        *(uint *)(iVar3 + 0x10) = *(uint *)(iVar3 + 0x10) | uVar2;
        iVar3 = iVar3 + 0x14;
      } while (iVar7 < (int)uVar8);
    }
    piVar4 = piVar4 + 1;
    local_48[0] = *piVar4;
  }
  return;
}

