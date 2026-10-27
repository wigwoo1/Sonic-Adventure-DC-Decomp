/* Ghidra draft only; not buildable/verified source. */

int FUN_8c668bee(int param_1,undefined4 param_2,undefined4 param_3,uint param_4)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined1 *puVar7;
  undefined4 *puVar8;
  int local_18;
  
  iVar2 = (int)DAT_8c668c3c;
  *(undefined4 *)(&stack0xffffffec + iVar2) = param_2;
  *(undefined4 *)((int)&local_18 + iVar2) = param_3;
  puVar7 = &stack0xfffffff0 + iVar2;
  (*(code *)PTR_FUN_8c668c48)(puVar7,0,(int)DAT_8c668c3e);
  if (*(int *)(DAT_8c668c32 + 4 + param_1) == *(int *)(DAT_8c668c32 + param_1)) {
    iVar3 = -1;
  }
  else {
    iVar3 = 0;
    for (puVar8 = (undefined4 *)(DAT_8c668c36 + param_1); puVar8[3] != 0;
        puVar8 = (undefined4 *)((int)puVar8 + (int)DAT_8c668d66)) {
      iVar3 = iVar3 + 1;
    }
    iVar4 = (int)DAT_8c668d68;
    iVar6 = (int)DAT_8c668d6c;
    puVar8[2] = 0;
    puVar8[3] = 1;
    iVar5 = (int)DAT_8c668d6a;
    puVar8[5] = *(undefined4 *)(iVar4 + param_1);
    puVar1 = PTR_FUN_8c668d7c;
    *puVar8 = *(undefined4 *)((int)&local_18 + iVar2);
    puVar8[1] = param_4 >> 2;
    *(undefined4 *)(iVar5 + (int)puVar8) = 0;
    puVar8[4] = 0;
    *(undefined4 *)(iVar5 + 4 + (int)puVar8) = 0;
    (*(code *)puVar1)(puVar8 + 6,iVar6 + 4 + param_1);
    (*(code *)PTR_FUN_8c668d80)(puVar7);
    (*(code *)PTR_FUN_8c668d7c)(puVar8 + 6,puVar7,(int)DAT_8c668d6c);
    puVar8[6] = ((param_4 >> 2) - 1) * 4 + *(int *)((int)&local_18 + iVar2);
    puVar8[8] = *(undefined4 *)(&stack0xffffffec + iVar2);
  }
  return iVar3;
}

