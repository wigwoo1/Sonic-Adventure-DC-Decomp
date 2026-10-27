/* Ghidra draft only; not buildable/verified source. */

void callback_8c64790e(void)

{
  undefined4 *puVar1;
  undefined *puVar2;
  int *piVar3;
  undefined *puVar4;
  undefined *puVar5;
  undefined *puVar6;
  undefined *puVar7;
  undefined *puVar8;
  undefined *puVar9;
  undefined *puVar10;
  undefined *puVar11;
  undefined *puVar12;
  undefined *puVar13;
  undefined *puVar14;
  undefined *puVar15;
  int iVar16;
  undefined4 uVar17;
  int iVar18;
  
  piVar3 = DAT_8c6479c4;
  puVar2 = PTR_FUN_8c6479c0;
  puVar1 = DAT_8c6479bc;
  iVar18 = (int)DAT_8c6479b8;
LAB_8c6479a0:
  do {
    puVar15 = PTR_FUN_8c647c3c;
    puVar14 = PTR_FUN_8c647c2c;
    puVar13 = PTR_FUN_8c647c28;
    puVar12 = PTR_FUN_8c647c24;
    puVar11 = PTR_FUN_8c647c20;
    puVar10 = PTR_FUN_8c647c1c;
    puVar9 = PTR_FUN_8c647c18;
    puVar8 = PTR_FUN_8c647c14;
    puVar7 = PTR_FUN_8c647c10;
    puVar6 = PTR_FUN_8c647c0c;
    puVar5 = PTR_FUN_8c647c08;
    puVar4 = PTR_FUN_8c647c00;
    switch(puVar1[2]) {
    default:
      (*(code *)PTR_thunk_FUN_8c668e5c_8c647c44)();
      goto LAB_8c6479a0;
    case 1:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)puVar4)(puVar1[5]);
      break;
    case 3:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)PTR_FUN_8c647c04)
                         (puVar1[5],puVar1 + 0x13,puVar1[7],puVar1 + 0x1b,puVar1[9]);
      break;
    case 4:
      puVar1[4] = *(undefined4 *)(puVar1[5] + 0x28);
      iVar16 = (*(code *)puVar5)(puVar1[5],puVar1[6]);
      break;
    case 5:
      puVar1[4] = *(undefined4 *)(puVar1[5] + 0x28);
      iVar16 = (*(code *)puVar6)(puVar1[5],puVar1[6],puVar1[7]);
      break;
    case 6:
      puVar1[4] = *(undefined4 *)(puVar1[5] + 0x28);
      iVar16 = (*(code *)puVar7)(puVar1[5],puVar1[6],puVar1[7]);
      break;
    case 7:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)puVar8)(puVar1[5],puVar1 + 0xb);
      break;
    case 8:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)puVar9)(puVar1[5],puVar1 + 0xb,puVar1 + 0xf);
      break;
    case 9:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)puVar11)(puVar1[5],puVar1[6]);
      break;
    case 10:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)puVar10)(puVar1[5],puVar1 + 0xb,puVar1[7],puVar1[8]);
      break;
    case 0xb:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)puVar14)(puVar1[5],puVar1 + 0xb,puVar1[7],puVar1[8]);
      break;
    case 0xc:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)PTR_FUN_8c647c30)
                         (puVar1[5],puVar1 + 0xb,puVar1[7],puVar1[8],puVar1 + 0x1b,puVar1[10]);
      break;
    case 0xd:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)PTR_FUN_8c647c34)
                         (puVar1[5],puVar1 + 0xb,puVar1[7],puVar1[8],puVar1 + 0x1b,puVar1[10]);
      break;
    case 0xe:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)PTR_FUN_8c647c38)(puVar1[5],puVar1 + 0xb,puVar1[7],puVar1[8],puVar1[9]);
      break;
    case 0xf:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)puVar12)(puVar1[5],puVar1[6]);
      break;
    case 0x10:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)puVar13)(puVar1[5],puVar1[6]);
      break;
    case 0x11:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)puVar15)(puVar1[5],puVar1[6],puVar1[7]);
      break;
    case 0x12:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)puVar2)(puVar1[5]);
      break;
    case 0x14:
      puVar1[4] = puVar1[5];
      iVar16 = (*(code *)PTR_FUN_8c647c40)(puVar1[5],puVar1 + 0xb,puVar1[7],puVar1[8],puVar1[9]);
    }
    if (iVar16 == iVar18) {
      (*(code *)PTR_FUN_8c647c48)(puVar1[4]);
    }
    *piVar3 = iVar16;
    uVar17 = puVar1[2];
    *puVar1 = 0;
    puVar1[1] = 0;
    puVar1[2] = 0;
    puVar1[3] = 0;
    puVar1[4] = 0;
    puVar1[5] = 0;
    puVar1[6] = 0;
    puVar1[7] = 0;
    puVar1[8] = 0;
    puVar1[9] = 0;
    puVar1[10] = 0;
    if (puVar1[0x1d] != 0) {
      (*(code *)puVar1[0x1d])(0,uVar17,iVar16,puVar1[0x1f]);
    }
  } while( true );
}

