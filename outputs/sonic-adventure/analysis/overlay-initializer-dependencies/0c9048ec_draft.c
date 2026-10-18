/* Ghidra draft only; not buildable/verified source. */

void callback_candidate_c9048ec(int param_1)

{
  char cVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  undefined1 *puVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  char *pcVar9;
  uint uVar10;
  int iVar11;
  int local_68;
  short *local_5c;
  undefined4 local_54;
  undefined4 local_50;
  short local_3c [10];
  undefined4 local_28;
  undefined4 local_24;
  
  puVar3 = PTR_DAT_0c904a38;
  puVar2 = PTR_DAT_0c904a34;
  local_28 = *(undefined4 *)PTR_DAT_0c904a28;
  local_24 = *(undefined4 *)(PTR_DAT_0c904a28 + 4);
  local_3c[0] = *(short *)PTR_DAT_0c904a2c;
  local_3c[1] = *(undefined2 *)(PTR_DAT_0c904a2c + 2);
  local_3c[2] = *(undefined2 *)(PTR_DAT_0c904a2c + 4);
  local_3c[3] = *(undefined2 *)(PTR_DAT_0c904a2c + 6);
  local_3c[4] = *(undefined2 *)(PTR_DAT_0c904a2c + 8);
  local_3c[5] = *(undefined2 *)(PTR_DAT_0c904a2c + 10);
  local_3c[6] = *(undefined2 *)(PTR_DAT_0c904a2c + 0xc);
  local_3c[7] = *(undefined2 *)(PTR_DAT_0c904a2c + 0xe);
  local_3c[8] = *(undefined2 *)(PTR_DAT_0c904a2c + 0x10);
  local_54 = *(undefined4 *)PTR_DAT_0c904a30;
  local_50 = *(undefined4 *)(PTR_DAT_0c904a30 + 4);
  pcVar9 = *(char **)(param_1 + 0xec);
  cVar1 = *pcVar9;
  if (cVar1 != '\x06') {
    for (uVar6 = 0; uVar6 < 6; uVar6 = uVar6 + 2) {
      if (pcVar9[*(short *)((int)&local_54 + uVar6) + 0x4c] != -1) {
        puVar4 = puVar3;
        if (cVar1 == '\0') {
          puVar4 = puVar2;
        }
        iVar8 = *(int *)(puVar4 + *(short *)((int)&local_54 + uVar6) * 8 +
                                  (short)pcVar9[*(short *)((int)&local_54 + uVar6) + 0x4c] * 0x60 +
                                  4);
        if (iVar8 != 0) {
          *(undefined4 *)(*(int *)(*(short *)((int)&local_28 + uVar6) * 4 + param_1 + 0x24c) + 4) =
               *(undefined4 *)(iVar8 + 4);
          puVar5 = *(undefined1 **)(*(int *)(iVar8 + 4) + 0x10);
          uVar10 = (uint)*(ushort *)(*(int *)(iVar8 + 4) + 0x16);
          iVar8 = 0;
          if (uVar10 != 0) {
            do {
              iVar8 = iVar8 + 1;
              puVar5[2] = 0xff;
              puVar5[1] = 0xff;
              *puVar5 = 0xff;
              puVar5 = puVar5 + 0x14;
            } while (iVar8 < (int)uVar10);
          }
        }
      }
    }
    local_68 = 0x18;
    iVar8 = 0x18;
    local_5c = (short *)((int)&local_50 + 2);
    for (iVar11 = 0; iVar11 < 9; iVar11 = iVar11 + 1) {
      if (pcVar9[*local_5c + 0x4c] != -1) {
        iVar7 = (short)pcVar9[*local_5c + 0x4c] * 0x60;
        if (cVar1 == '\0') {
          puVar4 = puVar2 + local_68 + iVar7;
        }
        else {
          puVar4 = puVar3 + iVar8 + iVar7;
        }
        iVar7 = *(int *)(puVar4 + 4);
        if (iVar7 != 0) {
          *(undefined4 *)(*(int *)(local_3c[iVar11] * 4 + param_1 + 0x24c) + 0x2c) =
               *(undefined4 *)(iVar7 + 0x2c);
          iVar7 = *(int *)(*(int *)(iVar7 + 0x2c) + 4);
          puVar5 = *(undefined1 **)(iVar7 + 0x10);
          uVar6 = (uint)*(ushort *)(iVar7 + 0x16);
          iVar7 = 0;
          if (uVar6 != 0) {
            do {
              iVar7 = iVar7 + 1;
              puVar5[2] = 0xff;
              puVar5[1] = 0xff;
              *puVar5 = 0xff;
              puVar5 = puVar5 + 0x14;
            } while (iVar7 < (int)uVar6);
          }
        }
      }
      iVar8 = iVar8 + 8;
      local_68 = local_68 + 8;
      local_5c = local_5c + 1;
    }
  }
  return;
}

