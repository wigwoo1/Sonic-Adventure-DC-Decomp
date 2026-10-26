/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c663712(int param_1)

{
  uint *puVar1;
  int iVar2;
  undefined4 uVar3;
  ushort *puVar4;
  ushort *puVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  ushort *puVar9;
  int local_30;
  int local_2c;
  uint local_28;
  uint local_24;
  undefined4 local_1c;
  undefined1 *local_18;
  
  puVar8 = &DAT_8c88f7a8;
  (*(code *)PTR_FUN_8c6637e8)(&local_28,param_1);
  puVar4 = *(ushort **)PTR_DAT_8c6637ec;
  puVar9 = (ushort *)0x0;
  while (puVar4 != (ushort *)0x0) {
    if (((*puVar4 & DAT_8c6637f0) == local_28) &&
       (puVar6 = (undefined1 *)0x0, puVar5 = puVar4, local_18 != (undefined1 *)0x0)) {
      do {
        puVar4 = puVar5;
        puVar7 = puVar6;
        if (*(int *)(puVar5 + (int)puVar6 * 2 + 6) == 0) {
          puVar4 = (ushort *)0x0;
          puVar7 = local_18;
          puVar8 = puVar6;
          puVar9 = puVar5;
        }
        puVar6 = puVar7 + 1;
        puVar5 = puVar4;
      } while (puVar6 < local_18);
    }
    if (puVar9 == (ushort *)0x0) {
      puVar4 = *(ushort **)(puVar4 + 2);
    }
  }
  if (puVar9 == (ushort *)0x0) {
    puVar9 = (ushort *)(*(code *)PTR_FUN_8c6637f4)();
    if (puVar9 != (ushort *)0x0) {
      puVar4 = puVar9 + 6;
      do {
        puVar4[0] = 0;
        puVar4[1] = 0;
        puVar4 = puVar4 + 2;
      } while (puVar4 < puVar9 + 0x26);
      puVar8 = (undefined1 *)0x0;
      (*(code *)PTR_FUN_8c6637f8)(PTR_DAT_8c6637ec,puVar9);
      puVar1 = DAT_8c6637fc;
      *puVar9 = *puVar9 | (ushort)local_28;
      if ((*puVar1 & 4) == 0) {
        iVar2 = (*(code *)PTR_FUN_8c663900)(1,local_1c,puVar9);
      }
      else {
        FUN_8c663c86(&local_30,&local_2c);
        iVar2 = (*(code *)PTR_FUN_8c663900)(local_30 < local_2c,local_1c,puVar9);
      }
      if (iVar2 == 0) goto LAB_8c66382a;
      (*(code *)PTR_FUN_8c663904)(puVar9);
    }
    uVar3 = 3;
  }
  else {
LAB_8c66382a:
    *(uint *)(param_1 + 0x1c) = (int)puVar8 * (local_24 >> 2) * 4 + *(int *)(puVar9 + 4);
    *(int *)(puVar9 + (int)puVar8 * 2 + 6) = param_1;
    *puVar9 = *puVar9 | 2;
    uVar3 = 0;
  }
  return uVar3;
}

