/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c645b98) */

void callback_8c645b8e(short param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  
  if ((param_1 >> 4 != 0x32) && (param_1 >> 4 == 0x36)) {
    uVar6 = (*DAT_8c645c48 & *DAT_8c645c44) >> 0xc;
    if (uVar6 == 0) {
      uVar7 = *DAT_8c645c50;
      uVar6 = *DAT_8c645c4c;
      iVar4 = (int)DAT_8c645c3e;
      uVar3 = 0;
      while( true ) {
        if ((int)uVar3 < 0) {
          iVar5 = 1 >> (~uVar3 & 0x1f) + 1;
        }
        else {
          iVar5 = 1 << (uVar3 & 0x1f);
        }
        if (iVar5 == 0x10) break;
        if ((int)uVar3 < 0) {
          uVar2 = 1 >> (~uVar3 & 0x1f) + 1;
        }
        else {
          uVar2 = 1 << (uVar3 & 0x1f);
        }
        if ((uVar2 & uVar7 & uVar6) != 0) {
          FUN_8c645afa(uVar3 * 0x20 + iVar4);
        }
        uVar3 = uVar3 + 1;
      }
    }
    else {
      iVar5 = (int)DAT_8c645c3c;
      iVar4 = (int)DAT_8c645c3a;
      uVar7 = 0;
      while( true ) {
        if ((int)uVar7 < 0) {
          iVar1 = 1 >> (~uVar7 & 0x1f) + 1;
        }
        else {
          iVar1 = 1 << (uVar7 & 0x1f);
        }
        if (iVar1 == iVar5) break;
        if ((int)uVar7 < 0) {
          uVar3 = 1 >> (~uVar7 & 0x1f) + 1;
        }
        else {
          uVar3 = 1 << (uVar7 & 0x1f);
        }
        if ((uVar3 & uVar6) != 0) {
          FUN_8c645afa(uVar7 * 0x20 + iVar4);
        }
        uVar7 = uVar7 + 1;
      }
    }
  }
  return;
}

