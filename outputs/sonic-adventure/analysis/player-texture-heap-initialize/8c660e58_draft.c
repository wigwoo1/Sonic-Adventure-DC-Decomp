/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c660e58(undefined2 *param_1,undefined4 param_2)

{
  undefined2 uVar1;
  undefined2 *puVar2;
  undefined4 *puVar3;
  
  puVar2 = DAT_8c660f38;
  uVar1 = DAT_8c660f36;
  *(undefined4 *)(DAT_8c660f38 + 2) = 0x24;
  puVar3 = _DAT_8c660f3c;
  *puVar2 = uVar1;
  puVar2[1] = 1;
  *(undefined2 **)(puVar2 + 6) = param_1;
  *(undefined2 **)(puVar2 + 4) = param_1;
  *(undefined4 *)(param_1 + 2) = param_2;
  *param_1 = uVar1;
  param_1[1] = 0;
  *(undefined2 **)(param_1 + 6) = puVar2;
  *(undefined2 **)(param_1 + 4) = puVar2;
  *puVar3 = *(undefined4 *)(puVar2 + 4);
  return;
}

