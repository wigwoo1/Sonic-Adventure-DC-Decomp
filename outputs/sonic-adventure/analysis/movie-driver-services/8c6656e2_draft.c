/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6656e2(void)

{
  short sVar1;
  undefined *puVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  undefined4 local_1c;
  int local_18;
  
  uVar6 = DAT_8c665744;
  *DAT_8c665748 = 0;
  local_18 = (*(code *)PTR_FUN_8c66574c)(0);
  uVar3 = (*(code *)PTR_FUN_8c66574c)(4);
  *DAT_8c665750 = 5;
  if (local_18 == DAT_8c665754) {
    if (uVar3 == 1) {
      *DAT_8c665750 = 5;
    }
    else if (0x10 < uVar3) {
      *DAT_8c6657c4 = 6;
      *DAT_8c6657cc = DAT_8c6657c8;
      uVar6 = DAT_8c6657d0;
    }
  }
  else {
    *DAT_8c665750 = 5;
  }
  iVar4 = (int)DAT_8c6657c0;
  iVar5 = *DAT_8c6657d4;
  if (iVar5 == 0) {
    *DAT_8c6657d8 = 0;
  }
  else {
    if (iVar5 != 1) {
      if (iVar5 == 2) {
        *DAT_8c665820 = 3;
      }
      else if (((iVar5 == 4) || (iVar5 == 5)) || (iVar5 == 6)) {
        *DAT_8c665820 = 0x7f;
        iVar4 = DAT_8c665824;
      }
      else if (iVar5 == 8) {
        *DAT_8c665820 = 0x1f;
      }
      else if (iVar5 == 9) {
        *DAT_8c665820 = 0xf;
      }
      else {
        *DAT_8c6658cc = 0xf;
      }
      goto LAB_8c665846;
    }
    *DAT_8c6657d8 = 1;
  }
  iVar4 = 0;
LAB_8c665846:
  puVar2 = PTR_FUN_8c6658d0;
  (*(code *)PTR_FUN_8c6658d0)((int)DAT_8c6658c0,uVar6);
  (*(code *)puVar2)((int)DAT_8c6658c2,0x20);
  (*(code *)puVar2)(8,0);
  (*(code *)puVar2)((int)DAT_8c6658c4,3);
  (*(code *)puVar2)(0x48,9);
  if ((*DAT_8c6658d4 & 1) == 0) {
    (*(code *)PTR_thunk_FUN_8c653444_8c6658dc)(0x44,0);
  }
  else {
    (*(code *)PTR_thunk_FUN_8c653444_8c6658dc)(0x44,DAT_8c6658d8);
  }
  (*(code *)puVar2)(0x5c,0);
  (*(code *)puVar2)(0x68,DAT_8c6658e0);
  (*(code *)puVar2)(0x6c,DAT_8c6658e4);
  uVar6 = DAT_8c6659d4;
  sVar1 = DAT_8c6659c0;
  if (*DAT_8c6658e8 == 6) {
    (*(code *)puVar2)((int)DAT_8c6658c6,DAT_8c6658ec);
    uVar6 = *DAT_8c6658f0;
    sVar1 = DAT_8c6658c8;
  }
  (*(code *)puVar2)((int)sVar1,uVar6);
  (*(code *)puVar2)(0x4c,(int)DAT_8c6659c2);
  (*(code *)puVar2)(0x74,0);
  (*(code *)puVar2)(0x78,DAT_8c6659d8);
  (*(code *)puVar2)((int)DAT_8c6659c4,0);
  (*(code *)puVar2)(0x30,iVar4);
  (*(code *)puVar2)((int)DAT_8c6659c6,DAT_8c6659dc);
  (*(code *)puVar2)((int)DAT_8c6659c8,DAT_8c6659dc);
  (*(code *)puVar2)((int)DAT_8c6659ca,0);
  (*(code *)puVar2)((int)DAT_8c6659cc,0xffffffff);
  (*(code *)puVar2)(0x80);
  (*(code *)puVar2)((int)DAT_8c6659ce,DAT_8c6659e0);
  (*(code *)puVar2)((int)DAT_8c6659d0,DAT_8c6659e4);
  (*(code *)puVar2)(0x74,1);
  if (*DAT_8c6659e8 == 6) {
    uVar6 = DAT_8c6659f0;
    if (*DAT_8c6659e8 == 6) {
      uVar6 = DAT_8c6659ec;
    }
    (*(code *)puVar2)(0x7c,uVar6);
    iVar4 = DAT_8c6659f8;
    puVar2 = PTR_FUN_8c6659f4;
    iVar7 = 0;
    local_1c = 0;
    iVar5 = DAT_8c6659fc;
    do {
      (*(code *)puVar2)(iVar5,&local_1c,4);
      iVar7 = iVar7 + 1;
      iVar5 = iVar5 + 4;
    } while (iVar7 < iVar4);
    return 1;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

