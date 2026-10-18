/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c03fe28) */

undefined4 FUN_8c03fe1a(float param_1,undefined4 param_2,undefined4 *param_3)

{
  undefined4 *puVar1;
  ushort *puVar2;
  undefined2 *puVar3;
  ushort uVar4;
  undefined2 uVar5;
  undefined8 in_dr2;
  bool bVar6;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar1 = DAT_8c040038;
  uVar5 = 0;
  uVar4 = 0;
  if (param_3 != (undefined4 *)0x0) {
    if (in_FPSCR_PR == '\0') {
      bVar6 = param_1 < (float)(int)param_3[1];
    }
    else {
      bVar6 = (double)CONCAT44(param_1,param_2) <
              (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),(float)(int)param_3[1]);
    }
    if (!bVar6) {
      return 1;
    }
    *DAT_8c040038 = *param_3;
    puVar1[4] = param_3[1];
    uVar5 = *(undefined2 *)(param_3 + 2);
    uVar4 = *(ushort *)((int)param_3 + 10) & (ushort)DAT_8c04003c;
  }
  puVar3 = DAT_8c040044;
  puVar2 = DAT_8c040040;
  if (in_FPSCR_SZ == '\0') {
    puVar1[3] = param_1;
  }
  else {
    *(ulonglong *)(puVar1 + 3) = CONCAT44(param_1,param_2);
  }
  puVar1[5] = 0;
  puVar1[6] = 0;
  *puVar2 = uVar4;
  *puVar3 = uVar5;
  return 0;
}

