/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c66497c(short param_1,ushort *param_2)

{
  ushort *puVar1;
  undefined *puVar2;
  ushort *puVar3;
  undefined *puVar4;
  ushort *puVar5;
  undefined4 uVar6;
  
  puVar2 = PTR_DAT_8c664a84;
  uVar6 = 0;
  *param_2 = *param_2 & (ushort)DAT_8c664a88;
  puVar4 = puVar2 + param_1 * 0x14;
  FUN_8c664b52(puVar4,puVar4 + 4,param_2,0);
  *(int *)(puVar4 + 0x10) = *(int *)(puVar4 + 0x10) + *(int *)(param_2 + 8);
  puVar5 = *(ushort **)(puVar4 + 8);
  puVar3 = (ushort *)0x0;
  while (puVar1 = puVar5, puVar1 != (ushort *)0x0) {
    if ((*(int *)(puVar1 + 6) == *(int *)(param_2 + 8) + *(int *)(param_2 + 6)) ||
       (*(int *)(param_2 + 6) == *(int *)(puVar1 + 8) + *(int *)(puVar1 + 6))) {
      puVar5 = (ushort *)0x0;
      puVar3 = puVar1;
    }
    else {
      puVar5 = *(ushort **)(puVar1 + 4);
    }
  }
  puVar4 = puVar2 + param_1 * 0x14;
  puVar5 = param_2;
  if (puVar3 != (ushort *)0x0) {
    do {
      param_2 = puVar3;
      if (*(int *)(param_2 + 6) == *(int *)(puVar5 + 6) + *(int *)(puVar5 + 8)) {
        *(undefined4 *)(param_2 + 6) = *(undefined4 *)(puVar5 + 6);
        *(int *)(param_2 + 8) = *(int *)(param_2 + 8) + *(int *)(puVar5 + 8);
      }
      else {
        *(int *)(param_2 + 8) = *(int *)(param_2 + 8) + *(int *)(puVar5 + 8);
      }
      FUN_8c664aec(puVar5);
      FUN_8c664b52(puVar4 + 8,puVar4 + 0xc,param_2,uVar6);
      puVar5 = *(ushort **)(puVar4 + 8);
      puVar3 = (ushort *)0x0;
      while (puVar1 = puVar5, puVar1 != (ushort *)0x0) {
        if ((*(int *)(puVar1 + 6) == *(int *)(param_2 + 8) + *(int *)(param_2 + 6)) ||
           (*(int *)(param_2 + 6) == *(int *)(puVar1 + 8) + *(int *)(puVar1 + 6))) {
          puVar5 = (ushort *)0x0;
          puVar3 = puVar1;
        }
        else {
          puVar5 = *(ushort **)(puVar1 + 4);
        }
      }
      puVar5 = param_2;
    } while (puVar3 != (ushort *)0x0);
    puVar4 = puVar2 + param_1 * 0x14;
  }
  FUN_8c664b12(puVar4 + 8,puVar4 + 0xc,param_2);
  return 0;
}

