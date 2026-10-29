/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c074c00) */
/* WARNING: Removing unreachable block (ram,0x8c074bee) */
/* WARNING: Removing unreachable block (ram,0x8c074b68) */
/* WARNING: Removing unreachable block (ram,0x8c074bb0) */
/* WARNING: Removing unreachable block (ram,0x8c074bf6) */
/* WARNING: Removing unreachable block (ram,0x8c074c0a) */

undefined4
FUN_8c074afc(int param_1,int param_2,int param_3,int param_4,double *param_5,undefined4 *param_6,
            undefined4 param_7,uint param_8)

{
  undefined4 uVar1;
  int iVar2;
  float fVar3;
  float in_fr3;
  double dVar4;
  double dVar5;
  undefined8 in_xd2;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float local_2c;
  float fStack_28;
  float fStack_24;
  undefined4 local_20;
  
  uVar1 = *(undefined4 *)(param_4 + 0x4c);
  *(undefined4 *)(param_3 + 0x44) = *(undefined4 *)(param_4 + 0x48);
  *(undefined4 *)(param_3 + 0x48) = uVar1;
  *(undefined4 *)(param_3 + 0x4c) = *(undefined4 *)(param_4 + 0x50);
  local_20 = in_PR;
  if ((*(ushort *)(param_1 + 4) & 1) == 0) {
    if (in_FPSCR_SZ == '\0') {
      in_fr3 = *(float *)(param_2 + 4);
    }
    else {
      in_xd2 = *(undefined8 *)(param_2 + 4);
    }
    fStack_24 = -in_fr3;
    if (in_FPSCR_SZ == '\0') {
      local_2c = fStack_24;
      fStack_24 = *(float *)(param_2 + 8);
    }
    else {
      local_2c = (float)in_xd2;
      in_xd2 = *(undefined8 *)(param_2 + 8);
    }
    fStack_24 = -fStack_24;
    if (in_FPSCR_SZ == '\0') {
      fStack_28 = fStack_24;
      fStack_24 = *(float *)(param_2 + 0xc);
    }
    else {
      fStack_28 = (float)in_xd2;
      in_xd2 = *(undefined8 *)(param_2 + 0xc);
    }
    fStack_24 = -fStack_24;
    if (in_FPSCR_SZ != '\0') {
      fStack_24 = (float)in_xd2;
      local_20 = (undefined4)((ulonglong)in_xd2 >> 0x20);
    }
    fVar3 = (float)(*(code *)PTR_FUN_8c074c64)(&local_2c);
    if (fVar3 != 0.0) {
      (*(code *)PTR_FUN_8c074c68)(&local_2c);
      iVar2 = (*(code *)PTR_FUN_8c074c6c)(&local_2c,param_4 + 0x48,0);
      (*(code *)PTR_FUN_8c074c74)(&local_2c,param_4 + 0x48);
      if ((param_8 & DAT_8c074c78) == 0) {
        if (0x3fff < iVar2) {
          return param_7;
        }
        (*(code *)PTR_FUN_8c074de0)(param_1,param_3,param_4,&local_2c);
      }
      else {
        if (0x3fff < iVar2) {
          return param_7;
        }
        param_7 = 1;
        (*(code *)PTR_FUN_8c074c70)(param_1,param_3,param_4,&local_2c);
      }
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar4 = (double)CONCAT44(*(undefined4 *)param_5,*(undefined4 *)(param_1 + 0x20));
    }
    else {
      dVar4 = *param_5;
    }
    if (in_FPSCR_PR == '\0') {
      dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(dVar4,0),SUB84(dVar4,0));
    }
    else {
      dVar4 = dVar4 - dVar4;
    }
    if (in_FPSCR_SZ == '\0') {
      local_2c = (float)((ulonglong)dVar4 >> 0x20);
      dVar4 = (double)CONCAT44(*(undefined4 *)((int)param_5 + 4),*(undefined4 *)(param_1 + 0x24));
    }
    else {
      local_2c = SUB84(dVar4,0);
      dVar4 = *(double *)((int)param_5 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(dVar4,0),SUB84(dVar4,0));
    }
    else {
      dVar4 = dVar4 - dVar4;
    }
    if (in_FPSCR_SZ == '\0') {
      fStack_28 = (float)((ulonglong)dVar4 >> 0x20);
      dVar4 = (double)CONCAT44(*(undefined4 *)(param_5 + 1),*(undefined4 *)(param_1 + 0x28));
    }
    else {
      fStack_28 = SUB84(dVar4,0);
      dVar4 = param_5[1];
    }
    if (in_FPSCR_PR == '\0') {
      dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(dVar4,0),SUB84(dVar4,0));
    }
    else {
      dVar4 = dVar4 - dVar4;
    }
    uVar1 = (undefined4)((ulonglong)dVar4 >> 0x20);
    fStack_24 = (float)uVar1;
    if (in_FPSCR_SZ != '\0') {
      fStack_24 = SUB84(dVar4,0);
      local_20 = uVar1;
    }
    fVar3 = (float)(*(code *)PTR_FUN_8c074c64)(&local_2c);
    if (fVar3 == 0.0) {
      if (in_FPSCR_SZ == '\0') {
        dVar4 = (double)((ulonglong)*(uint *)(param_2 + 4) << 0x20);
      }
      else {
        dVar4 = *(double *)(param_2 + 4);
      }
      if (in_FPSCR_PR == '\0') {
        dVar4 = (double)CONCAT44(-(float)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0));
      }
      else {
        dVar4 = -dVar4;
      }
      local_2c = SUB84(dVar4,0);
      if (in_FPSCR_SZ == '\0') {
        dVar5 = (double)CONCAT44(*(undefined4 *)(param_2 + 8),local_2c);
        local_2c = (float)((ulonglong)dVar4 >> 0x20);
      }
      else {
        dVar5 = *(double *)(param_2 + 8);
      }
      if (in_FPSCR_PR == '\0') {
        dVar5 = (double)CONCAT44(-(float)((ulonglong)dVar5 >> 0x20),SUB84(dVar5,0));
      }
      else {
        dVar5 = -dVar5;
      }
      fStack_28 = SUB84(dVar5,0);
      if (in_FPSCR_SZ == '\0') {
        dVar4 = (double)CONCAT44(*(undefined4 *)(param_2 + 0xc),fStack_28);
        fStack_28 = (float)(int)((ulonglong)dVar5 >> 0x20);
      }
      else {
        dVar4 = *(double *)(param_2 + 0xc);
      }
      if (in_FPSCR_PR == '\0') {
        dVar4 = (double)CONCAT44(-(float)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0));
      }
      else {
        dVar4 = -dVar4;
      }
      uVar1 = (undefined4)((ulonglong)dVar4 >> 0x20);
      fStack_24 = (float)uVar1;
      if (in_FPSCR_SZ != '\0') {
        fStack_24 = SUB84(dVar4,0);
        local_20 = uVar1;
      }
      fVar3 = (float)(*(code *)PTR_FUN_8c074c64)(&local_2c);
      if (fVar3 == 0.0) {
        return param_7;
      }
      (*(code *)PTR_FUN_8c074c68)(&local_2c);
      iVar2 = (*(code *)PTR_FUN_8c074c6c)(&local_2c,param_4 + 0x48,0);
    }
    else {
      (*(code *)PTR_FUN_8c074c68)(&local_2c);
      iVar2 = (*(code *)PTR_FUN_8c074c6c)(&local_2c,param_4 + 0x48,0);
    }
    if (0x3fff < iVar2) {
      return param_7;
    }
    param_7 = 1;
    (*(code *)PTR_FUN_8c074c70)(param_1,param_3,param_4,&local_2c);
  }
  *(undefined4 *)(param_4 + 4) = *(undefined4 *)(param_4 + 0x60);
  *(undefined4 *)(param_4 + 8) = *(undefined4 *)(param_4 + 100);
  *(undefined4 *)(param_4 + 0xc) = *(undefined4 *)(param_4 + 0x68);
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x60),*param_6);
  }
  else {
    dVar4 = *(double *)(param_4 + 0x60);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_1 + 0x20) = (int)((ulonglong)dVar4 >> 0x20);
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_4 + 100),param_6[1]);
  }
  else {
    *(double *)(param_1 + 0x20) = dVar4;
    dVar4 = *(double *)(param_4 + 100);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_1 + 0x24) = (int)((ulonglong)dVar4 >> 0x20);
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x68),param_6[2]);
  }
  else {
    *(double *)(param_1 + 0x24) = dVar4;
    dVar4 = *(double *)(param_4 + 0x68);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_1 + 0x28) = (int)((ulonglong)dVar4 >> 0x20);
  }
  else {
    *(double *)(param_1 + 0x28) = dVar4;
  }
  *(undefined4 *)(param_4 + 0x10) = *(undefined4 *)(param_4 + 0x6c);
  *(undefined4 *)(param_4 + 0x14) = *(undefined4 *)(param_4 + 0x70);
  *(undefined4 *)(param_4 + 0x18) = *(undefined4 *)(param_4 + 0x74);
  return param_7;
}

