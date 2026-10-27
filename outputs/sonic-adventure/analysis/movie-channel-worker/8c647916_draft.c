/* Ghidra draft only; not buildable/verified source. */

void callback_8c647916(void)

{
  int iVar1;
  undefined *puVar2;
  undefined *puVar3;
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
  int iVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  int *piVar18;
  int iVar19;
  undefined4 *puVar20;
  
  puVar2 = PTR_FUN_8c6479c0;
  iVar1 = DAT_8c6479bc;
  iVar19 = (int)DAT_8c6479b8;
  piVar18 = (int *)(DAT_8c6479c4 + 8);
  puVar20 = (undefined4 *)(DAT_8c6479bc + 0x100);
LAB_8c6479a0:
  do {
    puVar14 = PTR_FUN_8c647c3c;
    puVar13 = PTR_FUN_8c647c2c;
    puVar12 = PTR_FUN_8c647c28;
    puVar11 = PTR_FUN_8c647c24;
    puVar10 = PTR_FUN_8c647c20;
    puVar9 = PTR_FUN_8c647c1c;
    puVar8 = PTR_FUN_8c647c18;
    puVar7 = PTR_FUN_8c647c14;
    puVar6 = PTR_FUN_8c647c10;
    puVar5 = PTR_FUN_8c647c0c;
    puVar4 = PTR_FUN_8c647c08;
    puVar3 = PTR_FUN_8c647c00;
    switch(*(undefined4 *)(iVar1 + 0x108)) {
    default:
      (*(code *)PTR_thunk_FUN_8c668e5c_8c647c44)();
      goto LAB_8c6479a0;
    case 1:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)puVar3)(*(undefined4 *)(iVar1 + 0x114));
      break;
    case 3:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)PTR_FUN_8c647c04)
                         (*(undefined4 *)(iVar1 + 0x114),iVar1 + 0x14c,
                          *(undefined4 *)(iVar1 + 0x11c),iVar1 + 0x16c,
                          *(undefined4 *)(iVar1 + 0x124));
      break;
    case 4:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(*(int *)(iVar1 + 0x114) + 0x28);
      iVar15 = (*(code *)puVar4)(*(undefined4 *)(iVar1 + 0x114),*(undefined4 *)(iVar1 + 0x118));
      break;
    case 5:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(*(int *)(iVar1 + 0x114) + 0x28);
      iVar15 = (*(code *)puVar5)(*(undefined4 *)(iVar1 + 0x114),*(undefined4 *)(iVar1 + 0x118),
                                 *(undefined4 *)(iVar1 + 0x11c));
      break;
    case 6:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(*(int *)(iVar1 + 0x114) + 0x28);
      iVar15 = (*(code *)puVar6)(*(undefined4 *)(iVar1 + 0x114),*(undefined4 *)(iVar1 + 0x118),
                                 *(undefined4 *)(iVar1 + 0x11c));
      break;
    case 7:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)puVar7)(*(undefined4 *)(iVar1 + 0x114),iVar1 + 300);
      break;
    case 8:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)puVar8)(*(undefined4 *)(iVar1 + 0x114),iVar1 + 300,iVar1 + 0x13c);
      break;
    case 9:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)puVar10)(*(undefined4 *)(iVar1 + 0x114),*(undefined4 *)(iVar1 + 0x118));
      break;
    case 10:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)puVar9)(*(undefined4 *)(iVar1 + 0x114),iVar1 + 300,
                                 *(undefined4 *)(iVar1 + 0x11c),*(undefined4 *)(iVar1 + 0x120));
      break;
    case 0xb:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)puVar13)(*(undefined4 *)(iVar1 + 0x114),iVar1 + 300,
                                  *(undefined4 *)(iVar1 + 0x11c),*(undefined4 *)(iVar1 + 0x120));
      break;
    case 0xc:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)PTR_FUN_8c647c30)
                         (*(undefined4 *)(iVar1 + 0x114),iVar1 + 300,*(undefined4 *)(iVar1 + 0x11c),
                          *(undefined4 *)(iVar1 + 0x120),iVar1 + 0x16c,
                          *(undefined4 *)(iVar1 + 0x128));
      break;
    case 0xd:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)PTR_FUN_8c647c34)
                         (*(undefined4 *)(iVar1 + 0x114),iVar1 + 300,*(undefined4 *)(iVar1 + 0x11c),
                          *(undefined4 *)(iVar1 + 0x120),iVar1 + 0x16c,
                          *(undefined4 *)(iVar1 + 0x128));
      break;
    case 0xe:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)PTR_FUN_8c647c38)
                         (*(undefined4 *)(iVar1 + 0x114),iVar1 + 300,*(undefined4 *)(iVar1 + 0x11c),
                          *(undefined4 *)(iVar1 + 0x120),*(undefined4 *)(iVar1 + 0x124));
      break;
    case 0xf:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)puVar11)(*(undefined4 *)(iVar1 + 0x114),*(undefined4 *)(iVar1 + 0x118));
      break;
    case 0x10:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)puVar12)(*(undefined4 *)(iVar1 + 0x114),*(undefined4 *)(iVar1 + 0x118));
      break;
    case 0x11:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)puVar14)(*(undefined4 *)(iVar1 + 0x114),*(undefined4 *)(iVar1 + 0x118),
                                  *(undefined4 *)(iVar1 + 0x11c));
      break;
    case 0x12:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)puVar2)(*(undefined4 *)(iVar1 + 0x114));
      break;
    case 0x14:
      *(undefined4 *)(iVar1 + 0x110) = *(undefined4 *)(iVar1 + 0x114);
      iVar15 = (*(code *)PTR_FUN_8c647c40)
                         (*(undefined4 *)(iVar1 + 0x114),iVar1 + 300,*(undefined4 *)(iVar1 + 0x11c),
                          *(undefined4 *)(iVar1 + 0x120),*(undefined4 *)(iVar1 + 0x124));
    }
    if (iVar15 == iVar19) {
      (*(code *)PTR_FUN_8c647c48)(*(undefined4 *)(iVar1 + 0x110));
    }
    *piVar18 = iVar15;
    uVar17 = *(undefined4 *)(iVar1 + 0x17c);
    uVar16 = *(undefined4 *)(iVar1 + 0x108);
    *puVar20 = 0;
    *(undefined4 *)(iVar1 + 0x104) = 0;
    *(undefined4 *)(iVar1 + 0x108) = 0;
    *(undefined4 *)(iVar1 + 0x10c) = 0;
    *(undefined4 *)(iVar1 + 0x110) = 0;
    *(undefined4 *)(iVar1 + 0x114) = 0;
    *(undefined4 *)(iVar1 + 0x118) = 0;
    *(undefined4 *)(iVar1 + 0x11c) = 0;
    *(undefined4 *)(iVar1 + 0x120) = 0;
    *(undefined4 *)(iVar1 + 0x124) = 0;
    *(undefined4 *)(iVar1 + 0x128) = 0;
    if (*(int *)(iVar1 + 0x174) != 0) {
      (**(code **)(iVar1 + 0x174))(2,uVar16,iVar15,uVar17);
    }
  } while( true );
}

