/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c0afc7c) */

undefined4 FUN_8c0afb10(char *param_1,int param_2)

{
  undefined *puVar1;
  code *pcVar2;
  float in_fr3;
  float fVar4;
  double dVar3;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar1 = PTR_FUN_8c0afbd4;
  if ((param_1[2] != '\x02') &&
     (((*(ushort *)(param_1 + 4) & 4) != 0 ||
      ((param_2 != 0 && ((*(uint *)(param_2 + 0x5c) & DAT_8c0afbd0) != 0)))))) {
    param_1[6] = 'x';
    param_1[7] = '\0';
    (*(code *)puVar1)(param_1[8],7,0x3b,6);
    puVar1 = PTR_FUN_8c0afbd8;
    if (('-' < *param_1) && (*param_1 < '3')) {
      (*(code *)PTR_FUN_8c0afbd8)(0x458,0,0);
      *param_1 = '2';
      *(undefined2 *)(param_2 + 0x124) = 0x2e;
      (*(code *)PTR_FUN_8c0afbdc)(param_1);
      return 1;
    }
    if ((*param_1 < '>') || ('D' < *param_1)) {
      if (((int)*(short *)(param_2 + 4) & DAT_8c0afd60) == 0) {
        if (*param_1 != '-') {
          if (*param_1 == ',') {
            (*(code *)PTR_FUN_8c0afd68)();
          }
          (*(code *)puVar1)(0x463,0,0);
          *param_1 = '\x10';
          *(undefined2 *)(param_2 + 0x124) = 0x17;
          pcVar2 = DAT_8c0afd6c;
          *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfffb;
          (*pcVar2)(0,8);
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)(param_2 + 0x3c) = *(undefined4 *)(param_2 + 0xb4);
            in_fr3 = *(float *)(param_2 + 0xc4);
          }
          else {
            *(undefined8 *)(param_2 + 0x3c) = *(undefined8 *)(param_2 + 0xb4);
            in_xd2 = *(undefined8 *)(param_2 + 0xc4);
          }
          if (in_FPSCR_SZ == '\0') {
            *(float *)(param_2 + 0x38) = -in_fr3;
          }
          else {
            *(undefined8 *)(param_2 + 0x38) = in_xd2;
          }
          *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
          return 1;
        }
      }
      else {
        if (in_FPSCR_SZ == '\0') {
          fVar4 = *(float *)(param_2 + 0x38);
        }
        else {
          fVar4 = (float)((ulonglong)*(undefined8 *)(param_2 + 0x38) >> 0x20);
        }
        if (in_FPSCR_PR == '\0') {
          dVar3 = (double)CONCAT44(fVar4 * DAT_8c0afd64,DAT_8c0afd64);
        }
        else {
          dVar3 = (double)CONCAT44(fVar4,DAT_8c0afd64) * (double)CONCAT44(fVar4,DAT_8c0afd64);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_2 + 0x38) = (int)((ulonglong)dVar3 >> 0x20);
        }
        else {
          *(double *)(param_2 + 0x38) = dVar3;
        }
      }
    }
    else {
      (*(code *)PTR_FUN_8c0afbd8)(0x463,0,0);
      *param_1 = 'D';
      *(undefined2 *)(param_2 + 0x124) = 0x6e;
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfffb;
    }
  }
  return 0;
}

