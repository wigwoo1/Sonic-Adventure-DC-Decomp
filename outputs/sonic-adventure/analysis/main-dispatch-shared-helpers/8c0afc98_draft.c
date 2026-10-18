/* Ghidra draft only; not buildable/verified source. */

undefined4
FUN_8c0afc98(undefined4 param_1,undefined1 *param_2,undefined4 param_3,undefined8 *param_4)

{
  undefined *puVar1;
  int iVar2;
  uint uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  char in_FPSCR_SZ;
  
  puVar1 = PTR_FUN_8c0afd70;
  iVar2 = (*(code *)PTR_FUN_8c0afd74)(param_2,param_4);
  if (iVar2 == 2) {
    if ((*(ushort *)(param_2 + 4) & 0x800) != 0) {
      (*(code *)PTR_FUN_8c0afd78)(param_2,param_3,param_4);
    }
    (*(code *)PTR_FUN_8c0afd7c)(param_2);
    *param_2 = 0x28;
    *(undefined2 *)((int)param_4 + 0x124) = 0x49;
    *(ushort *)(param_2 + 4) = *(ushort *)(param_2 + 4) & 0xfffd;
    *(undefined2 *)(param_4 + 1) = 0;
  }
  else {
    iVar2 = (*(code *)PTR_FUN_8c0afd74)(param_2,param_4);
    if (iVar2 == 0) {
      return 0;
    }
    if ((*(ushort *)(param_2 + 4) & 0x800) != 0) {
      uVar3 = (*(code *)PTR_FUN_8c0afd80)(param_2);
      if ((uVar3 & 0x200) != 0) {
        *param_2 = 0x1d;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)param_4 + 0x3c) = *(undefined4 *)((int)param_4 + 0xb4);
        }
        else {
          *(undefined8 *)((int)param_4 + 0x3c) = *(undefined8 *)((int)param_4 + 0xb4);
        }
        *(undefined2 *)((int)param_4 + 0x124) = 0x37;
        *(ushort *)(param_2 + 4) = *(ushort *)(param_2 + 4) & 0xfffd;
        *(undefined2 *)(param_4 + 1) = 0;
        goto LAB_8c0afe04;
      }
      (*(code *)PTR_FUN_8c0afee0)(param_2,param_3,param_4);
    }
    *param_2 = 8;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)param_4 + 0x3c) = *(undefined4 *)((int)param_4 + 0xb4);
    }
    else {
      *(undefined8 *)((int)param_4 + 0x3c) = *(undefined8 *)((int)param_4 + 0xb4);
    }
    if ((*(int *)((int)param_4 + 0x6c) != 0) &&
       (iVar2 = *(int *)(*(int *)((int)param_4 + 0x6c) + 0x28), iVar2 != 0)) {
      if (in_FPSCR_SZ == '\0') {
        uVar5 = CONCAT44(*(undefined4 *)(iVar2 + 0x18),param_1);
        uVar4 = CONCAT44(*(undefined4 *)(iVar2 + 0x10),*(undefined4 *)(iVar2 + 0x14));
      }
      else {
        uVar5 = *(undefined8 *)(iVar2 + 0x18);
        uVar4 = *(undefined8 *)(iVar2 + 0x10);
      }
      (*(code *)PTR_FUN_8c0afee4)(uVar4,uVar5,(int)(char)param_2[8]);
    }
    *(undefined2 *)((int)param_4 + 0x124) = 0xe;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)param_4 = DAT_8c0afee8;
    }
    else {
      *param_4 = CONCAT44(PTR_DAT_8c0afeec,DAT_8c0afee8);
    }
    *(ushort *)(param_2 + 4) = *(ushort *)(param_2 + 4) & 0xfffd;
    *(ushort *)(param_2 + 4) = *(ushort *)(param_2 + 4) | 0x500;
    *(undefined2 *)(param_4 + 1) = 0;
  }
LAB_8c0afe04:
  (*(code *)puVar1)(0x11,0,0);
  return 1;
}

