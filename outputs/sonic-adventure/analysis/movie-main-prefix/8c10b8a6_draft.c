/* Ghidra draft only; not buildable/verified source. */

void FUN_8c10b8a6(int param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  (*(code *)PTR_FUN_8c10b95c)();
  (*(code *)PTR_FUN_8c10b960)();
  if (param_1 == 0) {
    (*(code *)PTR_FUN_8c10b970)(*DAT_8c10b96c,*DAT_8c10b968,*DAT_8c10b964);
    uVar1 = 20000;
    uVar2 = 0;
  }
  else {
    (*(code *)PTR_FUN_8c10b970)(*DAT_8c10b974,0,1);
    uVar1 = DAT_8c10b97c;
    uVar2 = DAT_8c10b978;
  }
  (*(code *)PTR_FUN_8c10b980)(0xffffffff,0,uVar1,0,uVar2);
  (*(code *)PTR_FUN_8c10b984)(0,0);
  (*(code *)PTR_FUN_8c10b988)(0,0);
  (*(code *)PTR_FUN_8c10b98c)(0x10);
  return;
}

