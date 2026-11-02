/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63c6ba(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  puVar4 = PTR_DAT_8c63c6e4;
  iVar3 = DAT_8c63c6e0;
  iVar2 = DAT_8c63c6dc;
  iVar1 = DAT_8c63c6d8;
  iVar6 = *(int *)(DAT_8c63c6ce + DAT_8c63c6d8);
  while (iVar6 != 0) {
    iVar6 = iVar6 + -1;
    iVar7 = iVar6 * 4;
    iVar5 = *(int *)(*(int *)(iVar1 + iVar7) * 4 + iVar2);
    *(int *)(iVar7 + iVar3) = iVar5;
    iVar5 = *(int *)(iVar5 + DAT_8c63c7de);
    if (iVar5 == 0) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c8d8;
    }
    else if (iVar5 == 1) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c8dc;
    }
    else if (iVar5 == 2) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c8e0;
    }
    else if (iVar5 == DAT_8c63c7e0) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c8e4;
    }
    else if ((iVar5 == 4) || (iVar5 == DAT_8c63c7e2)) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c8e8;
    }
    else if (iVar5 == DAT_8c63c7e4) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c8ec;
    }
    else if (iVar5 == 8) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c8f0;
    }
    else if (iVar5 == DAT_8c63c7e6) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c8f4;
    }
    else if (iVar5 == DAT_8c63c7e8) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c8f8;
    }
    else if (iVar5 == 0x40) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c8fc;
    }
    else if (iVar5 == 0x10) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c900;
    }
    else if (iVar5 == DAT_8c63c7ea) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c904;
    }
    else if (iVar5 == 0x20) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c908;
    }
    else if (iVar5 == DAT_8c63c7ec) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c90c;
    }
    else if (iVar5 == DAT_8c63c7ee) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c910;
    }
    else if (iVar5 == 3) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c914;
    }
    else if (iVar5 == DAT_8c63c7f0) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c918;
    }
    else if ((iVar5 == 5) || (iVar5 == DAT_8c63c7f2)) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c91c;
    }
    else if (iVar5 == DAT_8c63c7f4) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c920;
    }
    else if (iVar5 == 0x13) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c9d4;
    }
    else if (iVar5 == DAT_8c63c7f6) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c9d8;
    }
    else if (iVar5 == 0x25) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c9dc;
    }
    else if (iVar5 == DAT_8c63c8d4) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c9e0;
    }
    else if (iVar5 == DAT_8c63c8d6) {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c9e4;
    }
    else {
      *(undefined **)(puVar4 + iVar7) = PTR_LAB_8c63c9e8;
    }
  }
  return;
}

