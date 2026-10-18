/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c040bbc) */

undefined4 FUN_8c040b90(int param_1,int param_2)

{
  ushort uVar1;
  float *pfVar2;
  undefined2 uVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  int iVar6;
  undefined8 *puVar7;
  float fVar8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar5 = (undefined4 *)((char)((char)param_2 * '\x1c') + DAT_8c040d08);
  iVar6 = param_2 * 4;
  uVar3 = 0;
  puVar4 = *(undefined4 **)(param_1 + iVar6);
  uVar1 = 0;
  if (puVar4 != (undefined4 *)0x0) {
    pfVar2 = (float *)(param_1 + 8 + iVar6);
    if (in_FPSCR_SZ == '\0') {
      fVar8 = *pfVar2;
    }
    else {
      fVar8 = (float)((ulonglong)*(undefined8 *)pfVar2 >> 0x20);
    }
    if (in_FPSCR_PR != '\0' || (float)(int)puVar4[1] <= fVar8) {
      return 1;
    }
    puVar7 = (undefined8 *)(iVar6 + param_1 + 8);
    if (in_FPSCR_SZ == '\0') {
      puVar5[3] = *(undefined4 *)puVar7;
    }
    else {
      *(undefined8 *)(puVar5 + 3) = *puVar7;
    }
    *puVar5 = *puVar4;
    puVar5[4] = puVar4[1];
    uVar1 = *(ushort *)((int)puVar4 + 10) & (ushort)DAT_8c040d0c;
    uVar3 = *(undefined2 *)(puVar4 + 2);
  }
  iVar6 = DAT_8c040d10;
  puVar5[5] = 0;
  puVar5[6] = 0;
  *(ushort *)(iVar6 + param_2 * 2) = uVar1;
  *(undefined2 *)(DAT_8c040d14 + param_2 * 2) = uVar3;
  return 0;
}

