/* Ghidra draft only; not buildable/verified source. */

void FUN_8c051760(undefined4 param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined *puVar4;
  
  puVar3 = DAT_8c05188c;
  puVar2 = DAT_8c051888;
  puVar1 = DAT_8c051884;
  *DAT_8c051880 = param_1;
  *puVar1 = param_1;
  puVar4 = PTR_FUN_8c051890;
  *puVar2 = param_2;
  *puVar3 = 0;
  (*(code *)PTR_FUN_8c051894)(puVar4);
  (*(code *)PTR_FUN_8c051898)();
  return;
}

