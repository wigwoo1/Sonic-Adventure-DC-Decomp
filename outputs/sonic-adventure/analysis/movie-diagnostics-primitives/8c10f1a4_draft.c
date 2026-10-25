/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c10f1a4(uint *param_1,char *param_2,int param_3,byte *param_4)

{
  float fVar1;
  undefined *puVar2;
  byte bVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  float *pfVar7;
  uint uVar8;
  int iVar9;
  char *pcVar10;
  char *pcVar11;
  float in_fr3;
  bool bVar12;
  byte in_FPSCR_CAUSE;
  byte in_FPSCR_DN;
  byte in_FPSCR_PR;
  byte in_FPSCR_SZ;
  int local_60;
  float *local_5c;
  int local_58;
  char *local_54;
  char *local_50;
  char acStack_4c [4];
  char local_48 [32];
  undefined1 auStack_28 [12];
  
  uVar8 = *param_1 + 4;
  if ((uVar8 & 3) != 0) {
    uVar8 = *param_1 + 8;
  }
  *param_1 = uVar8;
  local_5c = (float *)(uVar8 - 4);
  pfVar7 = local_5c;
  if (((uint)local_5c & 3) != 0) {
    pfVar7 = (float *)(uVar8 - 8);
  }
  if (in_FPSCR_SZ == 0) {
    in_fr3 = *pfVar7;
  }
  uVar8 = (uint)in_FPSCR_CAUSE << 0xc | (uint)in_FPSCR_DN << 0x12 | (uint)in_FPSCR_PR << 0x13 |
          (uint)in_FPSCR_SZ << 0x14 | DAT_8c10f384;
  if (*(int *)(param_4 + 8) == -1) {
    param_4[8] = 6;
    param_4[9] = 0;
    param_4[10] = 0;
    param_4[0xb] = 0;
  }
  local_48[0] = '0';
  fVar1 = _DAT_8c10f38c;
  if ((char)((uVar8 & 0x100000) >> 0x14) == '\0') {
    fVar1 = DAT_8c10f388;
  }
  if ((char)((uVar8 & 0x80000 | DAT_8c10f384 & 0x80000) >> 0x13) == '\0') {
    bVar12 = (float)((ulonglong)(double)in_fr3 >> 0x20) == fVar1;
  }
  else {
    bVar12 = (double)in_fr3 == (double)CONCAT44(fVar1,DAT_8c10f388);
  }
  local_58 = param_3;
  local_54 = local_48;
  if (bVar12) {
    acStack_4c[0] = '\x01';
    local_48[1] = 0;
  }
  else {
    iVar5 = (*(code *)PTR_FUN_8c10f390)(0x11,auStack_28,&local_60,acStack_4c);
    if (iVar5 != 0) {
      *param_2 = '\0';
      if (*(int *)(param_4 + 4) < *(int *)(param_4 + 8)) {
        *(undefined4 *)(param_4 + 0x28) = *(undefined4 *)(param_4 + 8);
      }
      else {
        *(undefined4 *)(param_4 + 0x28) = *(undefined4 *)(param_4 + 4);
      }
      if (iVar5 == 1) {
        bVar3 = 0x2b;
      }
      else if (iVar5 == -1) {
        bVar3 = 0x2d;
      }
      else {
        bVar3 = 0x2a;
      }
      param_4[1] = bVar3;
      return;
    }
    (*(code *)PTR_FUN_8c10f394)(auStack_28,local_48 + 1,0x11);
  }
  for (iVar5 = 1; local_48[iVar5] == '0'; iVar5 = iVar5 + 1) {
  }
  if (1 < iVar5) {
    iVar6 = (*(code *)PTR_FUN_8c10f398)(local_48 + iVar5);
    iVar9 = 1;
    local_60 = local_60 + iVar5 + -1;
    if (0 < iVar6) {
      do {
        local_48[iVar9] = local_48[iVar5];
        iVar9 = iVar9 + 1;
        iVar5 = iVar5 + 1;
      } while (iVar9 <= iVar6);
    }
    local_48[iVar9] = '\0';
  }
  local_50 = local_48 + 1;
  iVar5 = (*(code *)PTR_FUN_8c10f398)();
  iVar6 = (int)(iVar5 + (uint)(iVar5 < 0)) >> 1;
  pcVar10 = local_50 + iVar5;
  iVar9 = 0;
  pcVar11 = local_50;
  if (0 < iVar6) {
    do {
      pcVar10 = pcVar10 + -1;
      cVar4 = *pcVar11;
      iVar9 = iVar9 + 1;
      *pcVar11 = *pcVar10;
      *pcVar10 = cVar4;
      pcVar11 = pcVar11 + 1;
    } while (iVar9 < iVar6);
  }
  if (iVar5 == 0) {
    iVar5 = 1;
    *local_50 = '0';
    local_60 = 0;
  }
  if ((local_58 == 0x67) || (local_58 == 0x47)) {
    *param_4 = *param_4 | 0x20;
    iVar6 = local_60 + iVar5 + -1;
    if ((iVar6 < -4) || (*(int *)(param_4 + 8) < iVar6)) {
      local_58 = local_58 + -2;
    }
    else {
      local_58 = 0x66;
    }
  }
  if (acStack_4c[0] == '\x01') {
    if ((*param_4 & 0x18) == 8) {
      cVar4 = '+';
    }
    else {
      pcVar11 = param_2;
      if ((*param_4 & 0x18) != 0x10) goto LAB_8c10f3a2;
      cVar4 = ' ';
    }
    *param_2 = cVar4;
    pcVar11 = param_2 + 1;
  }
  else {
    *param_2 = '-';
    pcVar11 = param_2 + 1;
  }
LAB_8c10f3a2:
  local_5c = (float *)(param_2 + 1);
  if (local_58 == 0x66) {
    if (local_60 < 0) {
      local_60 = local_60 + iVar5;
      local_58 = local_60 + *(int *)(param_4 + 8);
      if ((local_58 < iVar5) && (-1 < local_58)) {
        FUN_8c10f8a2(local_48,local_58 + 1);
      }
      if (local_60 < 1) {
        bVar12 = *local_54 != '0';
        if (bVar12) {
          iVar5 = iVar5 + 1;
          local_60 = local_60 + 1;
        }
        uVar8 = (uint)!bVar12;
        if (local_60 < 1) {
          *pcVar11 = '0';
        }
        else {
          uVar8 = 1;
          *pcVar11 = *local_54;
        }
        pcVar11[1] = '.';
        iVar6 = 2;
        pcVar11 = pcVar11 + 2;
        if (local_60 < 0) {
          *(char **)(param_4 + 0x14) = pcVar11;
          if (-local_60 < *(int *)(param_4 + 8)) {
            *(int *)(param_4 + 0x20) = -local_60;
            iVar6 = -local_60;
          }
          else {
            *(undefined4 *)(param_4 + 0x20) = *(undefined4 *)(param_4 + 8);
            iVar6 = *(int *)(param_4 + 8);
          }
          iVar6 = iVar6 + 2;
          *(int *)(param_4 + 8) = *(int *)(param_4 + 8) + local_60;
        }
        while (((int)(uVar8 - !bVar12) < iVar5 && (0 < *(int *)(param_4 + 8)))) {
          iVar6 = iVar6 + 1;
          *pcVar11 = local_48[uVar8];
          uVar8 = uVar8 + 1;
          pcVar11 = pcVar11 + 1;
          *(int *)(param_4 + 8) = *(int *)(param_4 + 8) + -1;
        }
        bVar3 = *param_4;
        if (((bVar3 & 0x20) == 0) || (((bVar3 & 0x20) != 0 && ((bVar3 & 4) != 0)))) {
          if (0 < *(int *)(param_4 + 8)) {
            *(char **)(param_4 + 0x18) = pcVar11;
            *(undefined4 *)(param_4 + 0x24) = *(undefined4 *)(param_4 + 8);
            iVar6 = iVar6 + *(int *)(param_4 + 8);
          }
        }
        else if (pcVar11[-1] == '0') {
          for (; pcVar11[-1] == '0'; pcVar11 = pcVar11 + -1) {
            iVar6 = iVar6 + -1;
          }
        }
        if ((((*param_4 & 4) == 0) && (pcVar11[-1] == '.')) &&
           (((*(int *)(param_4 + 0x20) == 0 && (*(int *)(param_4 + 0x24) == 0)) ||
            ((*param_4 & 0x20) != 0)))) {
          iVar9 = *(int *)(param_4 + 0x20);
          iVar5 = *(int *)(param_4 + 0x24);
          pcVar11 = pcVar11 + -1;
          param_4[0x24] = 0;
          param_4[0x25] = 0;
          param_4[0x26] = 0;
          param_4[0x27] = 0;
          param_4[0x20] = 0;
          param_4[0x21] = 0;
          param_4[0x22] = 0;
          param_4[0x23] = 0;
          *(char **)(param_4 + 0x14) = param_2;
          param_4[0x18] = 0;
          param_4[0x19] = 0;
          param_4[0x1a] = 0;
          param_4[0x1b] = 0;
          iVar6 = ((iVar6 - iVar9) - iVar5) + -1;
        }
        cVar4 = *param_2;
        if ((((cVar4 == '+') || (cVar4 == '-')) || (cVar4 == ' ')) &&
           ((iVar6 < *(int *)(param_4 + 4) && (param_4[1] == 0x30)))) {
          *(float **)(param_4 + 0x10) = local_5c;
          *(int *)(param_4 + 0x1c) = *(int *)(param_4 + 4) - iVar6;
          param_4[0x28] = 0;
          param_4[0x29] = 0;
          param_4[0x2a] = 0;
          param_4[0x2b] = 0;
        }
        else if (iVar6 < *(int *)(param_4 + 4)) {
          *(int *)(param_4 + 0x28) = *(int *)(param_4 + 4) - iVar6;
        }
        else {
          param_4[0x28] = 0;
          param_4[0x29] = 0;
          param_4[0x2a] = 0;
          param_4[0x2b] = 0;
        }
        if (*(char **)(param_4 + 0x14) == param_2) {
          *(undefined4 *)(param_4 + 0x14) = *(undefined4 *)(param_4 + 0x10);
        }
      }
      else {
        bVar12 = *local_54 != '0';
        iVar6 = 0;
        if (bVar12) {
          iVar5 = iVar5 + 1;
          local_60 = local_60 + 1;
        }
        uVar8 = (uint)!bVar12;
        for (; 0 < local_60; local_60 = local_60 + -1) {
          iVar6 = iVar6 + 1;
          *pcVar11 = local_48[uVar8];
          uVar8 = uVar8 + 1;
          pcVar11 = pcVar11 + 1;
        }
        *pcVar11 = '.';
        pcVar10 = pcVar11;
        while( true ) {
          pcVar11 = pcVar10 + 1;
          iVar6 = iVar6 + 1;
          if ((iVar5 <= (int)(uVar8 - !bVar12)) || (*(int *)(param_4 + 8) < 1)) break;
          pcVar10 = local_48 + uVar8;
          uVar8 = uVar8 + 1;
          *pcVar11 = *pcVar10;
          *(int *)(param_4 + 8) = *(int *)(param_4 + 8) + -1;
          pcVar10 = pcVar11;
        }
        bVar3 = *param_4;
        if (((bVar3 & 0x20) == 0) || (((bVar3 & 0x20) != 0 && ((bVar3 & 4) != 0)))) {
          *(char **)(param_4 + 0x14) = pcVar11;
          iVar6 = iVar6 + *(int *)(param_4 + 8);
          *(undefined4 *)(param_4 + 0x20) = *(undefined4 *)(param_4 + 8);
        }
        else if (*pcVar10 == '0') {
          for (; pcVar11[-1] == '0'; pcVar11 = pcVar11 + -1) {
            iVar6 = iVar6 + -1;
          }
        }
        if ((((*param_4 & 4) == 0) && (pcVar11[-1] == '.')) &&
           ((*(int *)(param_4 + 0x20) == 0 || ((*param_4 & 0x20) != 0)))) {
          iVar5 = *(int *)(param_4 + 0x20);
          param_4[0x20] = 0;
          param_4[0x21] = 0;
          param_4[0x22] = 0;
          param_4[0x23] = 0;
          pcVar11 = pcVar11 + -1;
          *(char **)(param_4 + 0x14) = param_2;
          iVar6 = (iVar6 - iVar5) + -1;
        }
        cVar4 = *param_2;
        if ((((cVar4 == '+') || (cVar4 == '-')) || (cVar4 == ' ')) &&
           ((iVar6 < *(int *)(param_4 + 4) && (param_4[1] == 0x30)))) {
          *(float **)(param_4 + 0x10) = local_5c;
          *(int *)(param_4 + 0x1c) = *(int *)(param_4 + 4) - iVar6;
          param_4[0x28] = 0;
          param_4[0x29] = 0;
          param_4[0x2a] = 0;
          param_4[0x2b] = 0;
        }
        else if (iVar6 < *(int *)(param_4 + 4)) {
          *(int *)(param_4 + 0x28) = *(int *)(param_4 + 4) - iVar6;
        }
        else {
          param_4[0x28] = 0;
          param_4[0x29] = 0;
          param_4[0x2a] = 0;
          param_4[0x2b] = 0;
        }
        if (*(char **)(param_4 + 0x14) == param_2) {
          *(undefined4 *)(param_4 + 0x14) = *(undefined4 *)(param_4 + 0x10);
        }
      }
    }
    else {
      iVar6 = 0;
      if (0 < iVar5) {
        do {
          cVar4 = *local_50;
          local_50 = local_50 + 1;
          iVar6 = iVar6 + 1;
          *pcVar11 = cVar4;
          pcVar11 = pcVar11 + 1;
        } while (iVar6 < iVar5);
      }
      *(char **)(param_4 + 0x14) = pcVar11;
      *(int *)(param_4 + 0x20) = local_60;
      iVar6 = 0;
      if ((((*param_4 & 0x20) == 0) && (0 < *(int *)(param_4 + 8))) || ((*param_4 & 4) != 0)) {
        *pcVar11 = '.';
        iVar6 = 1;
        pcVar11 = pcVar11 + 1;
      }
      bVar3 = *param_4;
      if (((bVar3 & 0x20) == 0) || (((bVar3 & 0x20) != 0 && ((bVar3 & 4) != 0)))) {
        *(char **)(param_4 + 0x18) = pcVar11;
        *(undefined4 *)(param_4 + 0x24) = *(undefined4 *)(param_4 + 8);
        iVar6 = iVar6 + *(int *)(param_4 + 8);
      }
      cVar4 = *param_2;
      iVar5 = iVar6 + local_60 + iVar5;
      if ((((cVar4 == '+') || (cVar4 == '-')) || (cVar4 == ' ')) &&
         ((iVar5 < *(int *)(param_4 + 4) && (param_4[1] == 0x30)))) {
        *(float **)(param_4 + 0x10) = local_5c;
        *(int *)(param_4 + 0x1c) = *(int *)(param_4 + 4) - iVar5;
        param_4[0x28] = 0;
        param_4[0x29] = 0;
        param_4[0x2a] = 0;
        param_4[0x2b] = 0;
      }
      else if (iVar5 < *(int *)(param_4 + 4)) {
        *(int *)(param_4 + 0x28) = *(int *)(param_4 + 4) - iVar5;
      }
      else {
        param_4[0x28] = 0;
        param_4[0x29] = 0;
        param_4[0x2a] = 0;
        param_4[0x2b] = 0;
      }
    }
  }
  else {
    if (*(int *)(param_4 + 8) + 1 < iVar5) {
      FUN_8c10f8a2(local_48,*(int *)(param_4 + 8) + 2);
    }
    if (*local_54 != '0') {
      iVar5 = iVar5 + 1;
      local_60 = local_60 + -1;
    }
    uVar8 = (uint)(*local_54 == '0');
    local_60 = local_60 + (iVar5 - uVar8);
    iVar6 = 1;
    *pcVar11 = local_48[uVar8];
    pcVar11[1] = '.';
    pcVar11 = pcVar11 + 1;
    while( true ) {
      pcVar10 = pcVar11 + 1;
      uVar8 = uVar8 + 1;
      iVar6 = iVar6 + 1;
      if ((iVar5 < (int)uVar8) || (*(int *)(param_4 + 8) < 1)) break;
      *pcVar10 = local_48[uVar8];
      *(int *)(param_4 + 8) = *(int *)(param_4 + 8) + -1;
      pcVar11 = pcVar10;
    }
    bVar3 = *param_4;
    if (((bVar3 & 0x20) == 0) || (((bVar3 & 0x20) != 0 && ((bVar3 & 4) != 0)))) {
      *(char **)(param_4 + 0x14) = pcVar10;
      *(undefined4 *)(param_4 + 0x20) = *(undefined4 *)(param_4 + 8);
      iVar6 = iVar6 + *(int *)(param_4 + 8);
    }
    else if (*pcVar11 == '0') {
      for (; pcVar10[-1] == '0'; pcVar10 = pcVar10 + -1) {
        iVar6 = iVar6 + -1;
      }
    }
    if ((((*param_4 & 4) == 0) && (pcVar10[-1] == '.')) &&
       ((*(int *)(param_4 + 0x20) == 0 || ((*param_4 & 0x20) != 0)))) {
      iVar5 = *(int *)(param_4 + 0x20);
      *(char **)(param_4 + 0x14) = param_2;
      pcVar10 = pcVar10 + -1;
      param_4[0x20] = 0;
      param_4[0x21] = 0;
      param_4[0x22] = 0;
      param_4[0x23] = 0;
      iVar6 = (iVar6 - iVar5) + -1;
    }
    if (local_58 == 0x65) {
      cVar4 = 'e';
    }
    else {
      cVar4 = 'E';
    }
    *pcVar10 = cVar4;
    if (local_60 < 0) {
      pcVar10[1] = '-';
      local_60 = -local_60;
    }
    else {
      pcVar10[1] = '+';
    }
    if (local_60 < 100) {
      pcVar10[2] = '0';
      pcVar10 = pcVar10 + 3;
      iVar6 = iVar6 + 4;
      *pcVar10 = '0';
    }
    else {
      pcVar10 = pcVar10 + 4;
      iVar6 = iVar6 + 5;
    }
    pcVar11 = pcVar10 + 1;
    while (0 < local_60) {
      cVar4 = (*(code *)PTR_FUN_8c10f904)();
      puVar2 = PTR_FUN_8c10f908;
      *pcVar10 = cVar4 + '0';
      pcVar10 = pcVar10 + -1;
      local_60 = (*(code *)puVar2)();
    }
    cVar4 = *param_2;
    if ((((cVar4 == '+') || (cVar4 == '-')) || (cVar4 == ' ')) &&
       ((iVar6 < *(int *)(param_4 + 4) && (param_4[1] == 0x30)))) {
      *(float **)(param_4 + 0x10) = local_5c;
      *(int *)(param_4 + 0x1c) = *(int *)(param_4 + 4) - iVar6;
      param_4[0x28] = 0;
      param_4[0x29] = 0;
      param_4[0x2a] = 0;
      param_4[0x2b] = 0;
    }
    else if (iVar6 < *(int *)(param_4 + 4)) {
      *(int *)(param_4 + 0x28) = *(int *)(param_4 + 4) - iVar6;
    }
    else {
      param_4[0x28] = 0;
      param_4[0x29] = 0;
      param_4[0x2a] = 0;
      param_4[0x2b] = 0;
    }
    if (*(char **)(param_4 + 0x14) == param_2) {
      *(undefined4 *)(param_4 + 0x14) = *(undefined4 *)(param_4 + 0x10);
    }
  }
  *pcVar11 = '\0';
  return;
}

