#include <stdio.h>

typedef struct
{
  int blood;       // Ѫ����0~100
  int blue;        // ������0~100
  int attack;      // ��������5
  int skillAttack; // ���ܹ�������10
  int skillCost;   // �������ģ�20
} Hero;

typedef struct
{
  int blood;  // Ѫ����0~100
  int attack; // ��������7
} Monster;

void showHero(Hero *hero)
{
  printf("Ӣ��===Ѫ����%d\t��������%d\t������%d\t���ܹ�������%d\t�������ģ�%d\n", hero->blood, hero->attack, hero->blue, hero->skillAttack, hero->skillCost);
}

void showMonster(Monster *monster)
{
  printf("����===Ѫ����%d\t��������%d\t\n", monster->blood, monster->attack);
}
Hero *initHero()
{
  Hero *hero = (Hero *)malloc(sizeof(Hero));
  hero->blood = 100;
  hero->blue = 100;
  hero->attack = 5;
  hero->skillAttack = 10;
  hero->skillCost = 20;
  return hero;
}

Monster *initMonster()
{
  Monster *monster = (Monster *)malloc(sizeof(Monster));
  monster->blood = 100;
  monster->attack = 7;
  return monster;
}
void heroAttack(Hero *hero, Monster *monster)
{
  printf("======Ӣ����ͨ����======\n");
  monster->blood -= hero->attack;
  if (monster->blood <= 0)
  {
    monster->blood = 0;
  }
}

void heroSkill(Hero *hero, Monster *monster)
{
  if (hero->blue < hero->skillCost)
  {
    printf("�������㣬�޷�ʹ�ü���\n");
    return;
  }

  printf("======Ӣ�ۼ��ܹ���======\n");
  monster->blood -= hero->skillAttack;
  hero->blue -= hero->skillCost;
  if (hero->blue < 0)
  {
    hero->blue = 0;
  }
  if (monster->blood <= 0)
  {
    monster->blood = 0;
  }
}

void heroBlueRecover(Hero *hero)
{
  printf("======Ӣ�������ָ�======\n");
  hero->blue += 5;
  if (hero->blue > 100)
  {
    hero->blue = 100;
  }
}

void monsterAttack(Hero *hero, Monster *monster)
{
  printf("======������ͨ����======\n");
  hero->blood -= monster->attack;
  if (hero->blood <= 0)
  {
    hero->blood = 0;
  }
}

void game()
{
  Hero *hero = initHero();
  Monster *monster = initMonster();
  showHero(hero);
  showMonster(monster);

  int turn = 1;
  while (hero->blood > 0 && monster->blood > 0)
  {
    printf("===========================��%d�غϿ�ʼ===========================\n", turn);
    printf("������1~3��1-Ӣ����ͨ������2-Ӣ�ۼ��ܹ�����3-��ʾ˫�����ݣ�4-�˳���Ϸ����");
    int input;
    scanf("%d", &input);
    switch (input)
    {
    case 1:
      heroAttack(hero, monster);
      break;
    case 2:
      heroSkill(hero, monster);
      break;
    case 3:
      showHero(hero);
      showMonster(monster);
      continue;
    case 4:
      printf("�˳���Ϸ\n");
      exit(0);
    default:
      printf("�������\n");
      continue;
      break;
    }
    showMonster(monster);
    if (monster->blood <= 0)
    {
      printf("GAME VICTORY --> ������������Ϸ����\n");
      printf("===========================��%d�غϽ���===========================\n", turn);
      break;
    }

    monsterAttack(hero, monster);
    showHero(hero);

    if (hero->blood <= 0)
    {
      printf("GAME OVER --> Ӣ����������Ϸ����\n");
      printf("===========================��%d�غϽ���===========================\n", turn);
      break;
    }

    heroBlueRecover(hero);
    showHero(hero);
    printf("===========================��%d�غϽ���===========================\n\n\n", turn++);
  }
}

int main()
{
  game();
  return 0;
}
