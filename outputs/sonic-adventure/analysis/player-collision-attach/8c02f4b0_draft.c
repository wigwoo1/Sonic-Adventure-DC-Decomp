/* Ghidra draft only; not buildable/verified source. */

void FUN_8c02f4b0(int param_1,undefined4 param_2,byte param_3,byte param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  char *pcVar3;
  undefined4 uVar4;
  ushort *puVar5;
  int iVar6;
  char in_FPSCR_SZ;
  
  puVar5 = (ushort *)(*(code *)PTR_FUN_8c02f5b4)(1,0x1c,&DAT_8c78c380);
  uVar4 = DAT_8c02f5c4;
  pcVar3 = DAT_8c02f5c0;
  puVar2 = PTR_s_CANT_GET_COLLI_WORK_____8c02f5bc;
  puVar1 = PTR_FUN_8c02f5b8;
  if (puVar5 == (ushort *)0x0) {
    do {
      do {
      } while (*pcVar3 < '\0');
      (*(code *)puVar1)(uVar4,puVar2);
    } while( true );
  }
  iVar6 = (*(code *)PTR_FUN_8c02f5b4)(param_4,0x34);
  if (iVar6 != 0) {
    (*(code *)PTR_FUN_8c02f5c8)(iVar6,param_2,(uint)param_4 * 0x34);
    *(int *)(puVar5 + 6) = iVar6;
    puVar5[1] = puVar5[1] | 0x20;
    *puVar5 = (ushort)param_3;
    if (((param_3 == 2) || (param_3 == 3)) || (param_3 == 7)) {
      puVar5[1] = puVar5[1] | 0x800;
    }
    puVar5[2] = (ushort)param_4;
    *(int *)(puVar5 + 8) = param_1;
    puVar5[1] = puVar5[1] & (ushort)DAT_8c02f5cc;
    *(ushort **)(*(int *)(param_1 + 0x20) + 0x38) = puVar5;
    if ((param_3 != 0) && (param_3 != 1)) {
      FUN_8c02f400(puVar5);
      return;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar5 + 4) = DAT_8c02f5b0;
    }
    else {
      *(ulonglong *)(puVar5 + 4) = CONCAT44(PTR_FUN_8c02f5b4,DAT_8c02f5b0);
    }
  }
  return;
}

