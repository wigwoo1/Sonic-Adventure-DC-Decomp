/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65fe14(int param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  undefined1 *puVar5;
  
  piVar1 = DAT_8c65ff4c;
  iVar2 = *DAT_8c65ff4c;
  puVar5 = (undefined1 *)(*DAT_8c65ff4c + 0x28 + param_1 * 0x2c);
  iVar3 = *DAT_8c65ff4c + (int)DAT_8c65ff44 + param_1 * 0x78;
  uVar4 = *(undefined4 *)
           (*DAT_8c65ff4c + (int)DAT_8c65ff48 +
            (1 - *(int *)((int)DAT_8c65ff46 + *DAT_8c65ff4c)) * 0x60 + param_1 * 4);
  puVar5[1] = *puVar5;
  puVar5[5] = puVar5[4];
  if (*(int *)(puVar5 + 0x1c) != 0) {
    *(undefined4 *)(puVar5 + 0x20) = *(undefined4 *)(puVar5 + 0x1c);
    *(undefined4 *)(puVar5 + 0x1c) = 0;
  }
  if ((PTR_DAT_8c65ff50[param_1] & 0x1f) == 0) {
    uVar4 = FUN_8c65ffb8(param_2,iVar3,puVar5,uVar4,1);
  }
  else {
    uVar4 = FUN_8c66005c(param_2 * 8 + iVar2 + 4,iVar3,puVar5,uVar4);
  }
  if (puVar5[7] != '\0') {
    *(undefined4 *)(puVar5 + 0x10) =
         *(undefined4 *)
          (param_1 * 4 +
          (1 - *(int *)((int)DAT_8c65ff46 + *piVar1)) * 0x60 + *piVar1 + (int)DAT_8c65ff48);
  }
  if (puVar5[0xd] != '\0') {
    puVar5[0xc] = 0;
    puVar5[0xd] = 0;
  }
  puVar5[0xc] = puVar5[0xc] | puVar5[2];
  puVar5[0xc] = puVar5[0xc] | puVar5[3];
  return uVar4;
}

