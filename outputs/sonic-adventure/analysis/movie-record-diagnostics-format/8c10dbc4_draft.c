/* Ghidra draft only; not buildable/verified source. */

int FUN_8c10dbc4(int param_1,int param_2,int param_3,byte *param_4)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  undefined1 *puVar6;
  uint uVar7;
  int *piVar8;
  uint *puVar9;
  undefined4 *puVar10;
  int iVar11;
  int iVar12;
  int *piVar13;
  byte *pbVar14;
  byte *pbVar15;
  byte abStack_10020 [65536];
  undefined4 local_20;
  code *local_c;
  
  local_c = FUN_8c600ec6;
  iVar2 = (int)DAT_8c10dca4;
  pbVar15 = (byte *)((int)&local_20 + DAT_8c10dca8 + iVar2);
  piVar13 = (int *)((int)&local_20 + DAT_8c10dca6 + iVar2);
  *piVar13 = 0;
  piVar13[1] = 0;
  piVar13[2] = 0;
  *pbVar15 = 0;
  pbVar15[1] = 0;
  pbVar15[2] = 0;
  pbVar15[3] = 0;
  pbVar15[4] = 0;
  pbVar15[5] = 0;
  pbVar15[6] = 0;
  pbVar15[7] = 0;
  pbVar15[8] = 0;
  pbVar15[9] = 0;
  pbVar15[10] = 0;
  pbVar15[0xb] = 0;
  pbVar15[0x10] = 0;
  pbVar15[0x11] = 0;
  pbVar15[0x12] = 0;
  pbVar15[0x13] = 0;
  pbVar15[0x14] = 0;
  pbVar15[0x15] = 0;
  pbVar15[0x16] = 0;
  pbVar15[0x17] = 0;
  pbVar15[0x18] = 0;
  pbVar15[0x19] = 0;
  pbVar15[0x1a] = 0;
  pbVar15[0x1b] = 0;
  pbVar15[0x1c] = 0;
  pbVar15[0x1d] = 0;
  pbVar15[0x1e] = 0;
  pbVar15[0x1f] = 0;
  pbVar15[0x20] = 0;
  pbVar15[0x21] = 0;
  pbVar15[0x22] = 0;
  pbVar15[0x23] = 0;
  pbVar15[0x24] = 0;
  pbVar15[0x25] = 0;
  pbVar15[0x26] = 0;
  pbVar15[0x27] = 0;
  pbVar15[0x28] = 0;
  pbVar15[0x29] = 0;
  pbVar15[0x2a] = 0;
  pbVar15[0x2b] = 0;
  if (param_1 == 0) {
    if (param_3 == 0) {
      *DAT_8c10dcb4 = (int)DAT_8c10dcaa;
      return -1;
    }
    if ((*(byte *)(param_3 + 0x10) & 7) == 0) {
      *DAT_8c10dcb4 = (int)DAT_8c10dcac;
      return -1;
    }
    if (((*(byte *)(param_3 + 0x10) & 4) == 0) && ((*(byte *)(param_3 + 0x10) & 1) != 0)) {
      *DAT_8c10dcb4 = (int)DAT_8c10dcae;
      return -1;
    }
    piVar13[3] = param_2;
  }
  *piVar13 = param_1;
  piVar13[1] = param_3;
  piVar13[2] = 0;
  *pbVar15 = 0;
  (&stack0xffffffe8)[iVar2] = 0x30;
  iVar3 = FUN_8c10e576(piVar13);
  if (iVar3 != 0) {
    return -1;
  }
  iVar12 = (int)&local_c + iVar2;
  iVar3 = (int)DAT_8c10dcb0;
  *(int *)(&stack0xfffffff0 + iVar2) = iVar12;
  do {
    while( true ) {
      if (((*param_4 == 0) || (iVar5 = FUN_8c10e576(piVar13), iVar5 != 0)) || ((*pbVar15 & 1) != 0))
      {
        FUN_8c10e566(piVar13);
        return piVar13[2];
      }
      if (*param_4 == 0x25) break;
      *(byte **)((int)&local_20 + iVar2) = param_4;
      while ((**(char **)((int)&local_20 + iVar2) != '\0' &&
             (**(char **)((int)&local_20 + iVar2) != '%'))) {
        *(int *)((int)&local_20 + iVar2) = *(int *)((int)&local_20 + iVar2) + 1;
      }
      FUN_8c10e50e(param_4,*(int *)((int)&local_20 + iVar2) - (int)param_4,piVar13);
      param_4 = *(byte **)((int)&local_20 + iVar2);
    }
    *pbVar15 = 0;
    *(int *)(pbVar15 + 0x10) = iVar12;
    *(int *)(pbVar15 + 0x14) = iVar12;
    pbVar15[0x18] = 0;
    pbVar15[0x19] = 0;
    pbVar15[0x1a] = 0;
    pbVar15[0x1b] = 0;
    pbVar15[0x1c] = 0;
    pbVar15[0x1d] = 0;
    pbVar15[0x1e] = 0;
    pbVar15[0x1f] = 0;
    pbVar15[0x20] = 0;
    pbVar15[0x21] = 0;
    pbVar15[0x22] = 0;
    pbVar15[0x23] = 0;
    pbVar15[0x24] = 0;
    pbVar15[0x25] = 0;
    pbVar15[0x26] = 0;
    pbVar15[0x27] = 0;
    pbVar15[0x28] = 0;
    pbVar15[0x29] = 0;
    pbVar15[0x2a] = 0;
    pbVar15[0x2b] = 0;
    while( true ) {
      pbVar14 = param_4 + 1;
      bVar1 = *pbVar14;
      if (((bVar1 != 0x2d) && (bVar1 != 0x2b)) && ((bVar1 != 0x20 && (bVar1 != 0x23)))) break;
      bVar1 = *pbVar14;
      param_4 = pbVar14;
      if (bVar1 == 0x2d) {
        bVar1 = *pbVar15 | 2;
LAB_8c10dcca:
        *pbVar15 = bVar1;
      }
      else {
        if (bVar1 == 0x2b) {
          bVar1 = *pbVar15 | 8;
          goto LAB_8c10dcca;
        }
        if (bVar1 == 0x20) {
          if ((*pbVar15 & 8) == 0) {
            *pbVar15 = *pbVar15 | 0x10;
          }
        }
        else if (bVar1 == 0x23) {
          bVar1 = *pbVar15 | 4;
          goto LAB_8c10dcca;
        }
      }
    }
    pbVar15[1] = 0x20;
    if (*pbVar14 == 0x30) {
      pbVar14 = param_4 + 2;
      pbVar15[1] = 0x30;
    }
    pbVar15[4] = 0;
    pbVar15[5] = 0;
    pbVar15[6] = 0;
    pbVar15[7] = 0;
    if (*pbVar14 == 0x2a) {
      uVar7 = *(int *)((int)&local_20 + DAT_8c10dd8e + iVar2) + 4;
      if ((uVar7 & 3) != 0) {
        uVar7 = *(int *)((int)&local_20 + DAT_8c10dd8e + iVar2) + 8;
      }
      piVar8 = (int *)(uVar7 - 4);
      *(uint *)((int)&local_20 + DAT_8c10dd8e + iVar2) = uVar7;
      if (((uint)piVar8 & 3) != 0) {
        piVar8 = (int *)(uVar7 - 8);
      }
      iVar5 = *piVar8;
      *(int *)(pbVar15 + 4) = iVar5;
      if (iVar5 < 0) {
        *pbVar15 = *pbVar15 | 2;
        *(int *)(pbVar15 + 4) = -*(int *)(pbVar15 + 4);
      }
      pbVar14 = pbVar14 + 1;
      if ((int)DAT_8c10dd90 < *(int *)(pbVar15 + 4)) {
        *pbVar15 = *pbVar15 | 1;
        *DAT_8c10dd94 = iVar3;
      }
    }
    if ((PTR_DAT_8c10dd98[*pbVar14] & 4) != 0) {
      pbVar15[4] = 0;
      pbVar15[5] = 0;
      pbVar15[6] = 0;
      pbVar15[7] = 0;
      while ((PTR_DAT_8c10de58[*pbVar14] & 4) != 0) {
        iVar11 = (char)*pbVar14 + -0x30;
        iVar5 = (*(code *)PTR_FUN_8c10dd9c)();
        pbVar14 = pbVar14 + 1;
        if (iVar5 < *(int *)(pbVar15 + 4)) {
          *pbVar15 = *pbVar15 | 1;
          *DAT_8c10de54 = iVar3;
        }
        else {
          *(int *)(pbVar15 + 4) = *(int *)(pbVar15 + 4) * 10 + iVar11;
        }
      }
    }
    pbVar15[8] = 0xff;
    pbVar15[9] = 0xff;
    pbVar15[10] = 0xff;
    pbVar15[0xb] = 0xff;
    param_4 = pbVar14;
    if (*pbVar14 == 0x2e) {
      param_4 = pbVar14 + 1;
      if (*param_4 == 0x2a) {
        uVar7 = *(int *)((int)&local_20 + DAT_8c10de50 + iVar2) + 4;
        if ((uVar7 & 3) != 0) {
          uVar7 = *(int *)((int)&local_20 + DAT_8c10de50 + iVar2) + 8;
        }
        piVar8 = (int *)(uVar7 - 4);
        *(uint *)((int)&local_20 + DAT_8c10de50 + iVar2) = uVar7;
        if (((uint)piVar8 & 3) != 0) {
          piVar8 = (int *)(uVar7 - 8);
        }
        iVar5 = *piVar8;
        *(int *)(pbVar15 + 8) = iVar5;
        if (iVar5 < 0) {
          pbVar15[8] = 0xff;
          pbVar15[9] = 0xff;
          pbVar15[10] = 0xff;
          pbVar15[0xb] = 0xff;
        }
        param_4 = pbVar14 + 2;
        if ((int)DAT_8c10de52 < *(int *)(pbVar15 + 8)) {
          *pbVar15 = *pbVar15 | 1;
          *DAT_8c10de54 = iVar3;
        }
      }
      if ((PTR_DAT_8c10de58[*param_4] & 4) != 0) {
        pbVar15[8] = 0;
        pbVar15[9] = 0;
        pbVar15[10] = 0;
        pbVar15[0xb] = 0;
        while ((PTR_DAT_8c10df38[*param_4] & 4) != 0) {
          iVar11 = (char)*param_4 + -0x30;
          iVar5 = (*(code *)PTR_FUN_8c10de5c)();
          param_4 = param_4 + 1;
          if (iVar5 < *(int *)(pbVar15 + 8)) {
            *pbVar15 = *pbVar15 | 1;
            *DAT_8c10df34 = iVar3;
          }
          else {
            *(int *)(pbVar15 + 8) = *(int *)(pbVar15 + 8) * 10 + iVar11;
          }
        }
      }
    }
    pbVar15[0xc] = 0x20;
    pbVar15[0xd] = 0;
    pbVar15[0xe] = 0;
    pbVar15[0xf] = 0;
    bVar1 = *param_4;
    if (((bVar1 == 0x68) || (bVar1 == 0x6c)) || (bVar1 == 0x4c)) {
      bVar1 = *param_4;
      param_4 = param_4 + 1;
      *(int *)(pbVar15 + 0xc) = (int)(char)bVar1;
    }
    bVar1 = *param_4;
    if (((((bVar1 == 100) || (bVar1 == 0x69)) || (bVar1 == 0x6f)) ||
        ((bVar1 == 0x75 || (bVar1 == 0x78)))) || (bVar1 == 0x58)) {
      if (*(int *)(pbVar15 + 0xc) == 0x6c) {
        uVar7 = *(int *)((int)&local_20 + DAT_8c10df32 + iVar2) + 4;
        if ((uVar7 & 3) != 0) {
          uVar7 = *(int *)((int)&local_20 + DAT_8c10df32 + iVar2) + 8;
        }
        puVar9 = (uint *)(uVar7 - 4);
        *(uint *)((int)&local_20 + DAT_8c10df32 + iVar2) = uVar7;
        if (((uint)puVar9 & 3) != 0) {
          puVar9 = (uint *)(*(int *)((int)&local_20 + DAT_8c10df32 + iVar2) + -8);
        }
        uVar7 = *puVar9;
      }
      else if (*(int *)(pbVar15 + 0xc) == 0x68) {
        bVar1 = *param_4;
        if ((((bVar1 == 0x75) || (bVar1 == 0x58)) || (bVar1 == 0x78)) || (bVar1 == 0x6f)) {
          uVar7 = *(int *)((int)&local_20 + DAT_8c10dff8 + iVar2) + 4;
          if ((uVar7 & 3) != 0) {
            uVar7 = *(int *)((int)&local_20 + DAT_8c10dff8 + iVar2) + 8;
          }
          puVar9 = (uint *)(uVar7 - 4);
          *(uint *)((int)&local_20 + DAT_8c10dff8 + iVar2) = uVar7;
          if (((uint)puVar9 & 3) != 0) {
            puVar9 = (uint *)(*(int *)((int)&local_20 + DAT_8c10dff8 + iVar2) + -8);
          }
          uVar7 = *puVar9 & 0xffff;
        }
        else {
          uVar7 = *(int *)((int)&local_20 + DAT_8c10dff8 + iVar2) + 4;
          if ((uVar7 & 3) != 0) {
            uVar7 = *(int *)((int)&local_20 + DAT_8c10dff8 + iVar2) + 8;
          }
          puVar10 = (undefined4 *)(uVar7 - 4);
          *(uint *)((int)&local_20 + DAT_8c10dff8 + iVar2) = uVar7;
          if (((uint)puVar10 & 3) != 0) {
            puVar10 = (undefined4 *)(*(int *)((int)&local_20 + DAT_8c10dff8 + iVar2) + -8);
          }
          uVar7 = (uint)(short)*puVar10;
        }
      }
      else {
        bVar1 = *param_4;
        if (((bVar1 == 0x75) || (bVar1 == 0x58)) || ((bVar1 == 0x78 || (bVar1 == 0x6f)))) {
          uVar7 = *(int *)((int)&local_20 + DAT_8c10dff8 + iVar2) + 4;
          if ((uVar7 & 3) != 0) {
            uVar7 = *(int *)((int)&local_20 + DAT_8c10dff8 + iVar2) + 8;
          }
          puVar9 = (uint *)(uVar7 - 4);
          *(uint *)((int)&local_20 + DAT_8c10dff8 + iVar2) = uVar7;
          if (((uint)puVar9 & 3) != 0) {
            puVar9 = (uint *)(*(int *)((int)&local_20 + DAT_8c10dff8 + iVar2) + -8);
          }
        }
        else {
          uVar7 = *(int *)((int)&local_20 + DAT_8c10e100 + iVar2) + 4;
          if ((uVar7 & 3) != 0) {
            uVar7 = *(int *)((int)&local_20 + DAT_8c10e100 + iVar2) + 8;
          }
          puVar9 = (uint *)(uVar7 - 4);
          *(uint *)((int)&local_20 + DAT_8c10e100 + iVar2) = uVar7;
          if (((uint)puVar9 & 3) != 0) {
            puVar9 = (uint *)(*(int *)((int)&local_20 + DAT_8c10e100 + iVar2) + -8);
          }
        }
        uVar7 = *puVar9;
      }
      if (*(int *)(pbVar15 + 8) == -1) {
        pbVar15[8] = 1;
        pbVar15[9] = 0;
        pbVar15[10] = 0;
        pbVar15[0xb] = 0;
      }
      FUN_8c10e304(uVar7,iVar12,(int)(char)*param_4,pbVar15);
      *(int *)(&stack0xffffffe4 + iVar2) = iVar12;
      uVar4 = (*(code *)PTR_FUN_8c10e104)(iVar12);
LAB_8c10e144:
      *(undefined4 *)(&stack0xffffffec + iVar2) = uVar4;
    }
    else {
      if (((bVar1 == 0x66) || (bVar1 == 0x65)) ||
         ((bVar1 == 0x45 || ((bVar1 == 0x67 || (bVar1 == 0x47)))))) {
        (*(code *)PTR_FUN_8c10e108)
                  ((int)&local_20 + DAT_8c10e100 + iVar2,iVar12,(int)(char)*param_4,pbVar15);
        *(int *)(&stack0xffffffe4 + iVar2) = iVar12;
        uVar4 = (*(code *)PTR_FUN_8c10e104)(iVar12);
        goto LAB_8c10e144;
      }
      if (bVar1 == 99) {
        puVar6 = *(undefined1 **)(&stack0xfffffff0 + iVar2);
        uVar7 = *(int *)((int)&local_20 + DAT_8c10e100 + iVar2) + 4;
        if ((uVar7 & 3) != 0) {
          uVar7 = *(int *)((int)&local_20 + DAT_8c10e100 + iVar2) + 8;
        }
        puVar10 = (undefined4 *)(uVar7 - 4);
        *(uint *)((int)&local_20 + DAT_8c10e100 + iVar2) = uVar7;
        if (((uint)puVar10 & 3) != 0) {
          puVar10 = (undefined4 *)(uVar7 - 8);
        }
        *puVar6 = (char)*puVar10;
LAB_8c10e182:
        *(int *)(&stack0xffffffe4 + iVar2) = iVar12;
        *(undefined4 *)(&stack0xffffffec + iVar2) = 1;
        *(int *)(pbVar15 + 0x14) = iVar12;
        pbVar15[0x20] = 0;
        pbVar15[0x21] = 0;
        pbVar15[0x22] = 0;
        pbVar15[0x23] = 0;
        *(int *)(pbVar15 + 0x28) = *(int *)(pbVar15 + 4) + -1;
      }
      else if (bVar1 == 0x73) {
        uVar7 = *(int *)((int)&local_20 + DAT_8c10e100 + iVar2) + 4;
        if ((uVar7 & 3) != 0) {
          uVar7 = *(int *)((int)&local_20 + DAT_8c10e100 + iVar2) + 8;
        }
        puVar10 = (undefined4 *)(uVar7 - 4);
        *(uint *)((int)&local_20 + DAT_8c10e100 + iVar2) = uVar7;
        if (((uint)puVar10 & 3) != 0) {
          puVar10 = (undefined4 *)(uVar7 - 8);
        }
        uVar4 = *puVar10;
        *(undefined4 *)(&stack0xffffffe4 + iVar2) = uVar4;
        uVar4 = (*(code *)PTR_FUN_8c10e104)(uVar4);
        *(undefined4 *)(&stack0xffffffec + iVar2) = uVar4;
        if ((*(int *)(pbVar15 + 8) != -1) &&
           (*(int *)(pbVar15 + 8) < *(int *)(&stack0xffffffec + iVar2))) {
          *(undefined4 *)(&stack0xffffffec + iVar2) = *(undefined4 *)(pbVar15 + 8);
        }
        *(int *)(pbVar15 + 0x14) = iVar12;
        pbVar15[0x20] = 0;
        pbVar15[0x21] = 0;
        pbVar15[0x22] = 0;
        pbVar15[0x23] = 0;
        *(int *)(pbVar15 + 0x28) = *(int *)(pbVar15 + 4) - *(int *)(&stack0xffffffec + iVar2);
      }
      else {
        if (bVar1 == 0x70) {
          uVar7 = *(int *)((int)&local_20 + DAT_8c10e100 + iVar2) + 4;
          if ((uVar7 & 3) != 0) {
            uVar7 = *(int *)((int)&local_20 + DAT_8c10e100 + iVar2) + 8;
          }
          puVar10 = (undefined4 *)(uVar7 - 4);
          *(uint *)((int)&local_20 + DAT_8c10e208 + iVar2) = uVar7;
          if (((uint)puVar10 & 3) != 0) {
            puVar10 = (undefined4 *)(uVar7 - 8);
          }
          uVar4 = *puVar10;
          if (*(int *)(pbVar15 + 8) == -1) {
            pbVar15[8] = 1;
            pbVar15[9] = 0;
            pbVar15[10] = 0;
            pbVar15[0xb] = 0;
          }
          FUN_8c10e304(uVar4,iVar12,0x78,pbVar15);
          *(int *)(&stack0xffffffe4 + iVar2) = iVar12;
          uVar4 = (*(code *)PTR_FUN_8c10e20c)(iVar12);
          goto LAB_8c10e144;
        }
        if (bVar1 == 0x6e) {
          uVar7 = *(int *)((int)&local_20 + DAT_8c10e208 + iVar2) + 4;
          if ((uVar7 & 3) != 0) {
            uVar7 = *(int *)((int)&local_20 + DAT_8c10e208 + iVar2) + 8;
          }
          puVar10 = (undefined4 *)(uVar7 - 4);
          *(uint *)((int)&local_20 + DAT_8c10e208 + iVar2) = uVar7;
          if (((uint)puVar10 & 3) != 0) {
            puVar10 = (undefined4 *)(uVar7 - 8);
          }
          *(int *)*puVar10 = piVar13[2];
        }
        else {
          if (bVar1 == 0x25) {
            **(undefined1 **)(&stack0xfffffff0 + iVar2) = 0x25;
            goto LAB_8c10e182;
          }
          *DAT_8c10e210 = iVar3;
          *pbVar15 = *pbVar15 | 1;
        }
      }
    }
    if ((*param_4 != 0x6e) && ((*pbVar15 & 1) == 0)) {
      if (((*pbVar15 & 2) == 0) && (0 < *(int *)(pbVar15 + 0x28))) {
        while (0 < *(int *)(pbVar15 + 0x28)) {
          FUN_8c10e50e(pbVar15 + 1,1,piVar13);
          *(int *)(pbVar15 + 0x28) = *(int *)(pbVar15 + 0x28) + -1;
        }
      }
      if (((*(int *)(pbVar15 + 0x1c) == 0) && (*(int *)(pbVar15 + 0x20) == 0)) &&
         (*(int *)(pbVar15 + 0x24) == 0)) {
        FUN_8c10e50e(*(undefined4 *)(&stack0xffffffe4 + iVar2),
                     *(undefined4 *)(&stack0xffffffec + iVar2),piVar13);
      }
      else {
        iVar5 = *(int *)(pbVar15 + 0x10) - *(int *)(&stack0xffffffe4 + iVar2);
        *(int *)((int)&local_20 + iVar2) = iVar5;
        if (0 < iVar5) {
          FUN_8c10e50e(*(int *)(&stack0xffffffe4 + iVar2),piVar13);
        }
        while (0 < *(int *)(pbVar15 + 0x1c)) {
          FUN_8c10e50e(&stack0xffffffe8 + iVar2,1,piVar13);
          *(int *)(pbVar15 + 0x1c) = *(int *)(pbVar15 + 0x1c) + -1;
        }
        iVar11 = *(int *)(pbVar15 + 0x14);
        iVar5 = *(int *)(pbVar15 + 0x10);
        *(int *)((int)&local_20 + iVar2) = *(int *)((int)&local_20 + iVar2) + (iVar11 - iVar5);
        if (0 < iVar11 - iVar5) {
          FUN_8c10e50e(*(undefined4 *)(pbVar15 + 0x10),piVar13);
        }
        while (0 < *(int *)(pbVar15 + 0x20)) {
          FUN_8c10e50e(&stack0xffffffe8 + iVar2,1,piVar13);
          *(int *)(pbVar15 + 0x20) = *(int *)(pbVar15 + 0x20) + -1;
        }
        FUN_8c10e50e(*(undefined4 *)(pbVar15 + 0x14),
                     *(int *)(&stack0xffffffec + iVar2) - *(int *)((int)&local_20 + iVar2),piVar13);
        while (0 < *(int *)(pbVar15 + 0x24)) {
          FUN_8c10e50e(&stack0xffffffe8 + iVar2,1,piVar13);
          *(int *)(pbVar15 + 0x24) = *(int *)(pbVar15 + 0x24) + -1;
        }
      }
      if (((*pbVar15 & 2) != 0) && (0 < *(int *)(pbVar15 + 0x28))) {
        while (0 < *(int *)(pbVar15 + 0x28)) {
          FUN_8c10e50e(pbVar15 + 1,1,piVar13);
          *(int *)(pbVar15 + 0x28) = *(int *)(pbVar15 + 0x28) + -1;
        }
      }
    }
    param_4 = param_4 + 1;
  } while( true );
}

