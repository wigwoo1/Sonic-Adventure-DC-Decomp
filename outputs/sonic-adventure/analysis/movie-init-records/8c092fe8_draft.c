/* Ghidra draft only; not buildable/verified source. */

void FUN_8c092fe8(void)

{
  code *pcVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  bool bVar7;
  int local_401c;
  undefined1 auStack_4018 [16388];
  
  iVar3 = DAT_8c0930fc;
  puVar2 = PTR_FUN_8c0930f8;
  iVar6 = 0;
  iVar4 = (*(code *)PTR_FUN_8c0930f8)(auStack_4018);
  pcVar1 = (code *)PTR_FUN_8c093100;
  while ((PTR_FUN_8c093100 = pcVar1, iVar4 != 0 &&
         (bVar7 = iVar6 <= iVar3, iVar6 = iVar6 + 1, bVar7))) {
    iVar4 = (*(code *)puVar2)(auStack_4018);
    pcVar1 = (code *)PTR_FUN_8c093100;
  }
  iVar6 = 0;
  iVar4 = (*pcVar1)(&local_401c);
  do {
    if (iVar4 == 0) {
LAB_8c09303c:
      uVar5 = DAT_8c093108;
      if (local_401c == 0) {
        uVar5 = DAT_8c093104;
      }
      (*(code *)PTR_FUN_8c09310c)(uVar5);
      (*(code *)PTR_FUN_8c093110)();
      return;
    }
    bVar7 = iVar3 < iVar6;
    iVar6 = iVar6 + 1;
    if (bVar7) {
      local_401c = 0;
      goto LAB_8c09303c;
    }
    iVar4 = (*pcVar1)(&local_401c);
  } while( true );
}

