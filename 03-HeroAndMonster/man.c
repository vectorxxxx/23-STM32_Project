#include <stdio.h>

typedef struct
{
  int blood;       // 血量，0~100
  int blue;        // 蓝量，0~100
  int attack;      // 攻击力，5
  int skillAttack; // 技能攻击力，10
  int skillCost;   // 技能消耗，20
} Hero;

typedef struct
{
  int blood;  // 血量，0~100
  int attack; // 攻击力，7
} Monster;

void showHero(Hero *hero)
{
  printf("英雄===血量：%d\t攻击力：%d\t蓝量：%d\t技能攻击力：%d\t技能消耗：%d\n", hero->blood, hero->attack, hero->blue, hero->skillAttack, hero->skillCost);
}

void showMonster(Monster *monster)
{
  printf("怪物===血量：%d\t攻击力：%d\t\n", monster->blood, monster->attack);
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
  printf("======英雄普通攻击======\n");
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
    printf("蓝量不足，无法使用技能\n");
    return;
  }

  printf("======英雄技能攻击======\n");
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
  printf("======英雄蓝量恢复======\n");
  hero->blue += 5;
  if (hero->blue > 100)
  {
    hero->blue = 100;
  }
}

void monsterAttack(Hero *hero, Monster *monster)
{
  printf("======怪物普通攻击======\n");
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
    printf("===========================第%d回合开始===========================\n", turn);
    printf("请输入1~3（1-英雄普通攻击，2-英雄技能攻击，3-显示双方数据，4-退出游戏）：");
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
      printf("退出游戏\n");
      exit(0);
    default:
      printf("输入错误\n");
      continue;
      break;
    }
    showMonster(monster);
    if (monster->blood <= 0)
    {
      printf("GAME VICTORY --> 怪物死亡，游戏结束\n");
      printf("===========================第%d回合结束===========================\n", turn);
      break;
    }

    monsterAttack(hero, monster);
    showHero(hero);

    if (hero->blood <= 0)
    {
      printf("GAME OVER --> 英雄死亡，游戏结束\n");
      printf("===========================第%d回合结束===========================\n", turn);
      break;
    }

    heroBlueRecover(hero);
    showHero(hero);
    printf("===========================第%d回合结束===========================\n\n\n", turn++);
  }
}

int main()
{
  game();
  return 0;
}
