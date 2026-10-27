/* Ghidra draft only; not buildable/verified source. */

int FUN_8c647f4c(int param_1,uint param_2,int param_3)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined2 *puVar7;
  uint local_34;
  undefined2 local_24;
  
  puVar2 = PTR_DAT_8c647fd4;
  puVar1 = PTR_DAT_8c647fd0;
  puVar7 = DAT_8c647fcc;
  iVar5 = *DAT_8c647fc8;
  if (param_1 == 0xf) {
    *(undefined4 *)(iVar5 + 4) = 0;
    iVar5 = *(int *)puVar1;
    local_24 = (undefined2)*(undefined4 *)puVar2;
  }
  else if (param_1 == 0x21) {
    *(undefined4 *)(iVar5 + 4) = 1;
    local_24 = (undefined2)*(undefined4 *)(puVar2 + 4);
    iVar5 = *(int *)(puVar1 + 4);
  }
  else if (param_1 == 0x41) {
    *(undefined4 *)(iVar5 + 4) = 2;
    iVar5 = *(int *)(puVar1 + 8);
    local_24 = (undefined2)*(undefined4 *)(puVar2 + 8);
  }
  else {
    if (param_1 != 0x7a) {
      return (int)DAT_8c647fb2;
    }
    *(undefined4 *)(iVar5 + 4) = 3;
    iVar5 = *(int *)(puVar1 + 0xc);
    local_24 = (undefined2)*(undefined4 *)(puVar2 + 0xc);
  }
  puVar1 = PTR_FUN_8c6480e0;
  local_34 = 1;
  for (iVar6 = 0; iVar3 = DAT_8c6480e4, iVar6 < 8; iVar6 = iVar6 + 1) {
    if ((param_2 & local_34) != 0) {
      *(undefined1 *)(puVar7 + 1) = 1;
      iVar4 = (int)DAT_8c6480d8;
      *puVar7 = (short)iVar6;
      *(int *)(puVar7 + 0xe) = iVar6 * 0x4c + iVar3;
      *(undefined1 *)(puVar7 + 5) = 0;
      *(undefined1 *)((int)puVar7 + 0xb) = 0;
      *(undefined1 *)(puVar7 + 6) = 0;
      *(int *)(puVar7 + 0x16) = param_3;
      *(int *)(puVar7 + 0x10) = param_3 + 0x20;
      *(int *)(puVar7 + 0x12) = iVar4 + param_3;
      *(int *)(puVar7 + 0x14) = (iVar5 + 1) * 0x200 + param_3 + 0x20;
      *(undefined1 *)(puVar7 + 4) = 0;
      *(undefined1 *)((int)puVar7 + 9) = 0;
      puVar7[7] = 1;
      puVar7[8] = (short)iVar5;
      puVar7[9] = local_24;
      puVar7[10] = 0x20;
      (*(code *)puVar1)(puVar7 + 0x1c,0,0x10);
      (*(code *)puVar1)(puVar7 + 0x24,0,(int)DAT_8c6480da);
      (*(code *)puVar1)(*(undefined4 *)(puVar7 + 0x16),0x47,0x20);
      (*(code *)puVar1)(*(undefined4 *)(puVar7 + 0x10),0x53,0x20);
      (*(code *)puVar1)(*(undefined4 *)(puVar7 + 0x12),0x46,0x20);
      (*(code *)puVar1)(*(undefined4 *)(puVar7 + 0x14),0x44,0x20);
      param_3 = param_3 + param_1 * 0x200 + 0x20;
    }
    local_34 = local_34 << 1;
    puVar7 = (undefined2 *)((int)puVar7 + (int)DAT_8c6480dc);
  }
  return 0;
}

