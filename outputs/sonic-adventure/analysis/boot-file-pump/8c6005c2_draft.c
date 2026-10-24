/* Ghidra draft only; not buildable/verified source. */

int callback_8c6005c2(void)

{
  short sVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  ushort uVar7;
  int iStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  uint uStack_30;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  
  iVar5 = (**(code **)(*(int *)(*(int *)PTR_DAT_8c6006b4 + 0xc) + 0x40))();
  puVar2 = PTR_DAT_8c5ffc0c;
  if (*(int *)(*(int *)PTR_DAT_8c6006b4 + 0x1c) == 0) {
    return iVar5;
  }
  *(undefined4 *)(*(int *)PTR_DAT_8c5ffc0c + 0x20) = 1;
  iVar5 = *(int *)(*(int *)puVar2 + 0x1c);
  if (iVar5 == 0) {
    uVar6 = 0;
  }
  else {
    uVar6 = *(undefined4 *)(iVar5 + 0x3c);
  }
  iVar3 = (**(code **)(*(int *)(*(int *)puVar2 + 0xc) + 0x10))(uVar6,&uStack_28);
  if (iVar5 == 0) goto LAB_8c5ffc80;
  uVar7 = (ushort)iVar3;
  if ((iVar3 == 0) || (iVar3 == 1)) {
    sVar1 = *(short *)(iVar5 + 0x48);
    *(undefined4 *)(*(int *)puVar2 + 0x1c) = 0;
    *(ushort *)(iVar5 + 0x50) = uVar7;
    *(undefined4 *)(iVar5 + 0x3c) = 0;
    *(undefined2 *)(iVar5 + 0x48) = 0;
    if (iVar3 != 1) goto LAB_8c5ffc80;
    if (sVar1 == 1) {
      *(undefined4 *)(iVar5 + 0x18) = uStack_28;
      *(int *)(iVar5 + 0x14) = *(int *)(iVar5 + 0x14) + *(int *)(iVar5 + 0x1c);
      *(undefined2 *)(iVar5 + 0x4a) = 2;
      if (*(int *)(iVar5 + 0x24) != 0) {
        (**(code **)(iVar5 + 0x24))(*(undefined4 *)(iVar5 + 0x28));
      }
      goto LAB_8c5ffc80;
    }
    if (sVar1 != 3) {
      if (sVar1 != 4) {
        if (sVar1 == 7) {
          iVar4 = (*(code *)PTR_FUN_8c5ffc10)(*(int *)puVar2 + 0x40,&uStack_40);
          if (iVar4 == 0) {
            **(undefined4 **)(iVar5 + 8) = uStack_40;
            *(undefined4 *)(*(int *)(iVar5 + 8) + 4) = uStack_3c;
            *(undefined4 *)(*(int *)(iVar5 + 8) + 8) = uStack_38;
            *(undefined4 *)(*(int *)(iVar5 + 8) + 0xc) = uStack_34;
            *(uint *)(*(int *)(iVar5 + 8) + 0x10) = uStack_30 & DAT_8c5ffc14;
            goto LAB_8c5ffc80;
          }
          *(undefined2 *)(iVar5 + 0x50) = 5;
          *(undefined2 *)(iVar5 + 0x52) = 0xffed;
          *(undefined4 *)(iVar5 + 0x44) = 0;
        }
        else {
          if (sVar1 != 8) goto LAB_8c5ffc80;
          iVar4 = (**(code **)(*(int *)(*(int *)puVar2 + 0xc) + 0x48))
                            (DAT_8c5ffc20,DAT_8c5ffc1c,*(undefined4 *)(iVar5 + 4),DAT_8c5ffc18);
          if (iVar4 != 0) {
            *(int *)(iVar5 + 0x3c) = iVar4;
            *(undefined2 *)(iVar5 + 0x50) = 4;
            *(undefined2 *)(iVar5 + 0x48) = 6;
            *(int *)(*(int *)puVar2 + 0x1c) = iVar5;
            goto LAB_8c5ffc80;
          }
          *(undefined2 *)(iVar5 + 0x50) = 5;
          *(undefined2 *)(iVar5 + 0x52) = 0xffe4;
          *(undefined4 *)(iVar5 + 0x44) = 0;
          **(undefined4 **)(iVar5 + 4) = 0;
        }
        goto LAB_8c5ffc34;
      }
      *(undefined4 *)(iVar5 + 0x18) = *(undefined4 *)(iVar5 + 0x20);
      *(int *)(iVar5 + 0x14) = *(int *)(iVar5 + 0x14) + *(int *)(iVar5 + 0x1c);
    }
    *(undefined2 *)(iVar5 + 0x4a) = 2;
  }
  else {
    if (iVar3 == 0x41) {
      iVar4 = (**(code **)(*(int *)(*(int *)puVar2 + 0xc) + 0x24))
                        (*(undefined4 *)(iVar5 + 0x3c),&iStack_44);
      if (iVar4 == 0) {
        if (iStack_44 < 1) {
          *(undefined2 *)(iVar5 + 0x4a) = 2;
        }
        else {
          *(undefined2 *)(iVar5 + 0x4a) = 0;
        }
        *(undefined4 *)(iVar5 + 0x18) = *(undefined4 *)(iVar5 + 0x20);
      }
      else if (iVar4 == 1) {
        *(int *)(iVar5 + 0x18) = iStack_44;
      }
      goto LAB_8c5ffc80;
    }
    if (iVar3 == 4) {
      *(undefined4 *)(iVar5 + 0x40) = uStack_20;
      if (*(short *)(iVar5 + 0x48) == 1) {
        *(undefined4 *)(iVar5 + 0x18) = uStack_28;
      }
      goto LAB_8c5ffc80;
    }
    if (iVar3 == 0x43) goto LAB_8c5ffc80;
    if ((iVar3 != 5) && (iVar3 != 0x40)) {
      *(ushort *)(iVar5 + 0x50) = uVar7 & 0xff;
      *(undefined4 *)(iVar5 + 0x3c) = 0;
      *(undefined4 *)(*(int *)puVar2 + 0x1c) = 0;
      goto LAB_8c5ffc80;
    }
    *(ushort *)(iVar5 + 0x50) = uVar7;
    *(short *)(iVar5 + 0x52) = (short)uStack_28;
    *(undefined4 *)(iVar5 + 0x44) = uStack_24;
    *(undefined4 *)(iVar5 + 0x3c) = 0;
    *(undefined4 *)(*(int *)puVar2 + 0x1c) = 0;
    if (*(short *)(iVar5 + 0x52) == -0x1d) {
      if (*(int *)(iVar5 + 0x44) == 0x3a) {
        iVar4 = FUN_8c600688();
        if ((iVar4 == 6) || (iVar4 == 0)) {
          *(undefined2 *)(iVar5 + 0x52) = 0xffe9;
        }
      }
      else if (*(int *)(iVar5 + 0x44) == (int)DAT_8c5ffd14) {
        *(undefined2 *)(iVar5 + 0x52) = 0xffdb;
      }
    }
LAB_8c5ffc34:
    if ((*(short *)(iVar5 + 0x52) == -0x17) || (*(short *)(iVar5 + 0x52) == -0x21)) {
      *(undefined4 *)(*(int *)puVar2 + 0x34) = 1;
    }
    if (*(int *)(*(int *)puVar2 + 0x24) != 0) {
      (**(code **)(*(int *)puVar2 + 0x24))
                (*(undefined4 *)(*(int *)puVar2 + 0x28),(int)*(short *)(iVar5 + 0x52));
    }
    if (*(int *)(iVar5 + 0x34) != 0) {
      (**(code **)(iVar5 + 0x34))(*(undefined4 *)(iVar5 + 0x38),(int)*(short *)(iVar5 + 0x52));
    }
  }
LAB_8c5ffc80:
  *(undefined4 *)(*(int *)puVar2 + 0x20) = 0;
  return iVar3;
}

