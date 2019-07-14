#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<graphics.h>
#include<stdlib.h>
#include<math.h>
#include"mymouse.h"

typedef struct node
{
  int data,pos,l,x,y;
  struct node *next,*left,*right,*up_left,*up_right,*down_left,*down_right;
}node;

typedef struct head
{
  node *start;
  struct head *next;
}head;

typedef struct ball
{
  float x,y;
  int colour;
}ball;

typedef struct mouse
{
  int button,x,y;
}mouse;

typedef struct ultimte_head
{
  head *ultimate_start;
  int avaiable;
}ultimate_head;

long int level,current_state,condition,score;

int highscore,current_ball[3];

void display_node(head *t);

void increase_level()
{
  level=level+1;
  return;
}

int isempty_q(ultimate_head *ut)
{
  if(ut->ultimate_start==NULL)
    return 1;
  return 0;
}

int isfull_q(ultimate_head *ut)
{
  if(ut->avaiable==15)
    return 1;
  return 0;
}

int is_near_full_q(ultimate_head *ut)
{
  if(ut->avaiable==14)
    return 1;
  return 0;
}

int random1()
{
  int i=rand()%4;
  switch(i)
  {
    case 0:return 9;
    case 1:return 10;
    case 2:return 12;
    case 3:return 13;
  }
  return 0;
}

int turn_bright(int colour)
{
    switch(colour)
    {
	case 9:return 1;
	case 10:return 2;
	case 12:return 4;
	case 13:return 5;
    }
    return colour;
}

int turn_light(int colour)
{
    switch(colour)
    {
	case 1: return 9;
	case 2: return 10;
	case 4: return 12;
	case 5:return 13;
    }
    return colour;
}

int current_ball_colour()
{
  int colour=current_ball[0];
  current_ball[0]=current_ball[1];
  current_ball[1]=current_ball[2];
  current_ball[2]=random1();
  return colour;
}

void insertend(head *t,int ele,int position)
{
  node *p,*q;
  p=(node*)malloc(sizeof(node));
  p->data=ele;
  p->next=NULL;
  p->pos=position;
  p->l=level;
  if(t->start==NULL)
    t->start=p;
  else
  {
    q=t->start;
    while(q->next!=NULL)
      q=q->next;
    q->next=p;
  }
  return;
}

void insert(head *t)
{
  int i;
  increase_level();
  if(level%2==1)
    for(i=0;i<15;i++)
      insertend(t,random1(),i+1);
  else
    for(i=0;i<14;i++)
      insertend(t,random1(),i+1);
  return;
}

void connect_left_right(head *t)
{
  node *p,*q,*n;
  n=(node*)malloc(sizeof(node));
  n->data=0;
  n->pos=0;
  n->l=0;
  p=t->start;
  q=t->start;
  p->left=n;
  p->right=p->next;
  p=p->next;
  while(p->next!=NULL)
  {
    p->left=q;
    p->right=p->next;
    p=p->next;
    q=q->next;
  }
  p->right=n;
  p->left=q;
  return;
}

void connect_bottom(ultimate_head *ut)
{
  head *hp;
  node *p,*q ,*n;
  n=(node*)malloc(sizeof(node));
  n->data=0;
  n->pos=0;
  n->l=0;
  if(ut->avaiable!=1)
    return;
  hp=ut->ultimate_start;
  p=hp->start;
  p->left=p->down_left=p->down_right=n;
  p->right=p->next;
  p=p->next;
  q=hp->start;
  while(p->next!=NULL)
  {
    p->left=q;
    p->right=p->next;
    p->down_left=p->down_right=n;
    p=p->next;
    q=q->next;
  }
  p->right=n;
  p->down_left=n;
  p->down_right=n;
  p->left=q;
  return;
}

void connect(ultimate_head *ut)
{
  head *hp,*hq;
  node *pt,*qt,*pb,*qb, *n;
  n=(node*)malloc(sizeof(node));
  n->data=0;
  n->l=0;
  n->pos=0;
  hp=ut->ultimate_start;
  hq=ut->ultimate_start;
  while(hp->next!=NULL)
    hp=hp->next;
  while(hq->next->next!=NULL)
    hq=hq->next;
  pt=hp->start;
  qt=hp->start;
  pb=hq->start;
  qb=hq->start;
  if(pt->l%2==1)     //20
  {
    connect_left_right(hp);
    pt->down_left=n;
    pt->down_right=pb;
    pb->up_left=pt;
    pb->up_right=pt->next;
    pt=pt->next;
    pb=pb->next;
    while(pt->next!=NULL)
    {
      pt->down_left=qb;
      pt->down_right=pb;
      pb->up_left=pt;
      pb->up_right=pt->next;
      if(pb->next!=NULL)
	pb=pb->next;           //////////////////////////////////////////////take under consideration
      qb=qb->next;
      qt=qt->next;
      pt=pt->next;
    }
    pt->down_right=n;
    pt->down_left=qb;
    qb->up_right=pt;
  }
  else
  {
    connect_left_right(hp);
    pt->down_left=pb;
    pt->down_right=pb->next;
    pb->up_left=n;
    pb->up_right=pt;
    pt=pt->next;
    pb=pb->next;
    while(pb->next!=NULL)
    {
      pt->down_left=pb;
      pt->down_right=pb->next;
      pb->up_right=pt;
      pb->up_left=qt;
      pt=pt->next;
      qt=qt->next;
      pb=pb->next;
      qb=qb->next;
    }
    pb->up_left=qt;
    pb->up_right=n;
  }
  return;
}

void connect_top(head *t)
{
  node *p,*n;
  n=(node*)malloc(sizeof(node));
  n->data=0;
  n->pos=0;
  n->l=0;
  connect_left_right(t);
  display_node(t);
  p=t->start;
  while(p!=NULL)
  {
    p->up_left=n;
    p->up_right=n;
    p=p->next;///////////////////////////////////////////forgot
  }
  return;
}

void display_node(head *t)
{
    node *p;
    p=t->start;
    while(p!=NULL)
    {
	p=p->next;
    }
    return;
}
void ultimate_insert(ultimate_head *ut)
{
  head *hp,*hq;
  if(isfull_q(ut))
  {
    //printf("error");
    return;
  }
  hp = (head*)malloc(sizeof(head));
  hp->start=NULL;
  hp->next=NULL;
  insert(hp);
  if(isempty_q(ut))
  {
    ut->ultimate_start=hp;
    ut->avaiable=1;
    connect_bottom(ut);
    //printf("connected bottom sucessfully\n");
    display_node(ut->ultimate_start);
  }
  else if(is_near_full_q(ut))
  {
    //printf("now hwere it's started working");
    hq=ut->ultimate_start;
    connect_top(hp);
    //printf("connect top sucessfully");
    while(hq->next!=NULL)
      hq=hq->next;
    hq->next=hp;
    ut->avaiable=15;
    connect(ut);
  }
  else
  {
    //printf("it's running now");
    hq = ut->ultimate_start;
    while(hq->next!=NULL)
    {
      hq=hq->next;
      //printf("it goimg next");
    }
    hq->next=hp;
    display_node(hp);
    ut->avaiable=ut->avaiable+1;
    connect(ut);
  //  printf("connected sucessfully ");
  }
  return;
}

void make_sure_15(ultimate_head *ut)
{
  srand(time(NULL));
  if(ut->avaiable==15)
    return;
  while(ut->avaiable!=15)
  {
    //printf("making");
    ultimate_insert(ut);
    //printf("making done %d",ut->avaiable);
  }
  return;
}

void display(ultimate_head *ut)
{
  head *hp;
  node *p;
  hp=ut->ultimate_start;
  while(hp!=NULL)
  {
    p=hp->start;
    while(p!=NULL)
    {
      printf("%d ",p->data);
      p=p->next;
    }
    printf("\n");
    if(hp->start->l%2==1)
	printf(" ");
    hp=hp->next;
  }
  return;
}

void give_screen(ultimate_head *ut,ball *b);

void all_bright(node *s,ball *b)
{
  int j;
  s->data=turn_bright(b->colour);
  setcolor(BLACK);
  for(j=1;j<=10;j++)
  {
    circle(s->x,s->y,j);
    delay(10);
  }
  score++;
  if(s->right->data==b->colour)
    all_bright(s->right,b);

  if(s->left->data==b->colour)
    all_bright(s->left,b);

  if(s->up_right->data==b->colour)
    all_bright(s->up_right,b);

  if(s->down_right->data==b->colour)
    all_bright(s->down_right,b);

  if(s->up_left->data==b->colour)
    all_bright(s->up_left,b);

  if(s->down_left->data==b->colour)
    all_bright(s->down_left,b);

  return;
}


void print_surrounding(ultimate_head *ut,int a,int p,ball *b)
{
    head *hp;
    node *q;
    int i;
    hp=ut->ultimate_start;
    for(i=0;i<a;i++)
	hp=hp->next;
    q=hp->start;
    for(i=0;i<p;i++)
	q=q->next;
    all_bright(q,b);
    give_screen(ut,b);
    delay(100);
    return;
}


void remove_all_bright(ultimate_head *ut)
{
    head *hp;
    node *p;
    hp=ut->ultimate_start;
    while(hp!=NULL)
    {
	p=hp->start;
	while(p!=NULL)
	{
	    if(p->data==1||p->data==2||p->data==4||p->data==5)
		p->data=0;
	    p=p->next;
	}
	hp=hp->next;
    }
    return;
}

int check_for_all_zero(ultimate_head *ut)
{
    head *hp;
    node *p;
    hp=ut->ultimate_start;
    p=hp->start;
    while(p!=NULL)
    {
	if(p->data!=0)
	    return 0;
	p=p->next;
    }
    hp->start=NULL;
    hp=hp->next;
    ut->ultimate_start=ut->ultimate_start->next;
    ut->avaiable--;
    make_sure_15(ut);
    connect_bottom(ut);
    return 1;
}

void loss_connection(ultimate_head *ut)
{
  head *hp;
  node *p;
  hp=ut->ultimate_start;
  while(hp->next!=NULL)
  {
    p=hp->start;
    while(p!=NULL)
    {
      if(p->up_left->data==0&&p->up_right->data==0&&p->data!=0)
      {
	p->data=0;
	hp=ut->ultimate_start;
      }
      p=p->next;
    }
    hp=hp->next;
  }
  return;
}

void mouse_click(mouse *m);

void outer_rectangle()
{
  int maxx=getmaxx();
  int maxy=getmaxy();
  line(10,0,maxx-11,0);        //top
  line(0,10,0,maxy-11);                    //left
  line(maxx-1,10,maxx-1,maxy-11);   //right
  line(10,maxy-1,maxx-11,maxy-1); //bottom line
  arc(10,10,90,180,10);
  arc(maxx-11,10,0,90,10);
  arc(10,maxy-11,180,270,10);
  arc(maxx-11,maxy-11,270,0,10);
}

void show_balls()
{
  int i;
  int maxy=getmaxy();
  for(i=1;i<=3;i++)
  {
    setcolor(current_ball[3-i]);
    circle(500-i*30,maxy-30,10);
    setfillstyle(SOLID_FILL,current_ball[3-i]);
    floodfill(500-i*30,maxy-30,current_ball[3-i]);
  }
  return;
}

void help_menu()
{
  int maxx=getmaxx(),ch;
  settextstyle(3,HORIZ_DIR,7);
  outtextxy(maxx/2,40,"HELP");
  gotoxy(1,10);
  printf("  1:In this Bubble Shooter you are only allow to touch only single ball.\n");
  printf("\n  2:Once ball get touch it will vanished whether it is touch our not.\n");
  printf("\n  3:Balls will move down after every 5 steps.\n");
  printf("\n  4:You can change the colour of ball just by simply pressing enter.\n");
  printf("\n  5:You can exit the game just by simply pressing esc from keyboard.\n");
  printf("\n  6:Don't get panic if balls look like it's touching but it's not touched");
  printf("\n  SO NOW ENJOY\n");
  outer_rectangle();
  fflush(stdin);
  while(!kbhit())
  {
    ch=getch();
    if(ch==27)
    {
      clearviewport();
      return;
    }
  }
}

void give_screen(ultimate_head *ut,ball *b)
{
  int x1,y1,maxx,state,maxy;
  char a[50];
  head *hp;
  node *p;
  maxx=getmaxx();
  maxy=getmaxy();
  hp=ut->ultimate_start;
  y1=current_state*18+57;
  setcolor(b->colour);
  line(130,280,maxx-131,280);     //limit line
  line(130,67,130,maxy-11); //left limit
  line(maxx-131,67,maxx-131,maxy-11);   //right limit
  arc(140,67,90,180,10);         //
  arc(maxx-141,67,0,90,10);        //
  arc(140,maxy-11,180,270,10);       //
  arc(maxx-141,maxy-11,270,0,10);      //for inner rectangle

  outer_rectangle();

  line(10,57,maxx-11,57);
  arc(10,67,90,180,10);
  arc(maxx-11,67,0,90,10);

  //score card
  line(maxx-111,67,maxx-21,67);
  line(maxx-111,67+90,maxx-21,67+90);
  line(maxx-121,77,maxx-121,67+80);
  line(maxx-11,77,maxx-11,67+80);
  arc(maxx-111,77,90,180,10);
  arc(maxx-111,67+80,180,270,10);
  arc(maxx-21,77,0,90,10);
  arc(maxx-21,67+80,270,0,10);

  //score inner card
  line(maxx-101,107,maxx-31,107);
  line(maxx-101,67+80,maxx-31,67+80);
  line(maxx-111,117,maxx-111,67+70);
  line(maxx-21,117,maxx-21,67+70);
  arc(maxx-101,117,90,180,10);
  arc(maxx-101,67+70,180,270,10);
  arc(maxx-31,117,0,90,10);
  arc(maxx-31,67+70,270,0,10);
  sprintf(a,"%ld",score);
  settextjustify(1,1);
  settextstyle(3,HORIZ_DIR,3);
  outtextxy(maxx-66,83,"SCORE");
  outtextxy(maxx-66,122,a);


  //TILTE
  settextstyle(3,HORIZ_DIR,4);
  outtextxy(maxx/2,20,"BUBBLE SHOOTER");

  //highscore
  line(20,67,100,67);
  line(20,157,100,157);
  line(10,77,10,147);
  line(110,77,110,147);
  arc(20,77,90,180,10);
  arc(20,147,180,270,10);
  arc(100,77,0,90,10);
  arc(100,147,270,0,10);

  line(30,107,90,107);
  line(30,67+80,90,67+80);
  line(20,117,20,67+70);
  line(100,117,100,67+70);
  arc(30,117,90,180,10);
  arc(30,67+70,180,270,10);
  arc(90,117,0,90,10);
  arc(90,67+70,270,0,10);

  settextstyle(3,HORIZ_DIR,1);
  outtextxy(62,82,"HIGHSCORE");
  if(score>highscore)
    highscore=score;
  sprintf(a,"%d",highscore);
  settextstyle(3,HORIZ_DIR,3);
  outtextxy(60,123,a);

  state=0;
 // printf("%d",hp->start->l);
  while(hp!=NULL)
  {
    if(state==current_state)
      break;
    p=hp->start;
    if(p->l%2==1)
      x1=maxx/2-165+11;
    else
      x1=maxx/2-165+11+11;
    while(p!=NULL)
    {
      p->x=x1;
      p->y=y1;
      setfillstyle(SOLID_FILL,p->data);
      setcolor(p->data);
      circle(p->x,p->y,10);
      floodfill(p->x,p->y,p->data);
      if(p->data==0)
      {
	setcolor(BLACK);
	circle(p->x,p->y,10);
      }
      p=p->next;
      x1=x1+22;
    }
    y1=y1-19;
    hp=hp->next;
    state++;
  }
  return;
}

void play_button()
{
  int maxx=getmaxx();
  int maxy=getmaxy();
 // rectangle(maxx/2-40,maxy-maxy/4-80,maxx/2+40,maxy-maxy/4);
  setcolor(YELLOW);
  line(maxx/2-30,maxy-maxy/4-80,maxx/2+30,maxy-maxy/4-80);
  line(maxx/2-30,maxy-maxy/4,maxx/2+30,maxy-maxy/4);
  line(maxx/2-40,maxy-maxy/4-70,maxx/2-40,maxy-maxy/4-10);
  line(maxx/2+40,maxy-maxy/4-70,maxx/2+40,maxy-maxy/4-10);
  arc(maxx/2-30,maxy-maxy/4-70,90,180,10);
  arc(maxx/2-30,maxy-maxy/4-10,180,270,10);
  arc(maxx/2+30,maxy-maxy/4-70,0,90,10);
  arc(maxx/2+30,maxy-maxy/4-10,270,0,10);
  setfillstyle(SOLID_FILL,YELLOW);
  floodfill(maxx/2,maxy-maxy/4-40,YELLOW);

  setcolor(BLUE);
  line(maxx/2-15,maxy-maxy/4-60,maxx/2-15,maxy-maxy/4-20);
  line(maxx/2-15,maxy-maxy/4-60,maxx/2+20,maxy-maxy/4-40);
  line(maxx/2-15,maxy-maxy/4-20,maxx/2+20,maxy-maxy/4-40);
  setfillstyle(SOLID_FILL,BLUE);
  floodfill(maxx/2,maxy-maxy/4-40,BLUE);
  return;
}

void exit_button()
{
  int maxy=getmaxy();
  setcolor(WHITE);
  circle(20,maxy-21,10);
  settextstyle(3,HORIZ_DIR,1);
  outtextxy(20,maxy-24,"E");
  return;
}

void help_button()
{
  int maxy=getmaxy();
  setcolor(WHITE);
  circle(20,maxy-51,10);
  settextstyle(3,HORIZ_DIR,1);
  outtextxy(20,maxy-54,"H");
  return;
}

void credits_button()
{
  int maxx=getmaxx();
  int maxy=getmaxy();
  setcolor(WHITE);
  circle(maxx-20,maxy-21,10);
  settextstyle(3,HORIZ_DIR,1);
  outtextxy(maxx-21,maxy-24,"C");

  return;
}

void credit_window(mouse *m)
{
  int maxx=getmaxx();
  int maxy=getmaxy();
  outer_rectangle();
  settextstyle(3,HORIZ_DIR,5);
  setcolor(WHITE);
  outtextxy(maxx/2,100,"CREDITS");
  settextstyle(3,HORIZ_DIR,3);
  outtextxy(maxx/2,maxy/2-50,"Aaditya Rane");
  outtextxy(maxx/2,maxy/2,"16102A0066" );
  outtextxy(maxx/2,maxy/2+50,"Navanit Srisangkar");
  outtextxy(maxx/2,maxy/2+100,"16102A0065");
  outtextxy(maxx/2,maxy/2+150,"Ram Agrawal");
  outtextxy(maxx/2,maxy/2+200,"16102A0053");
  exit_button();
  while(!kbhit())
  {
    mouse_click(m);
    if(m->x>=10&&m->y>=maxy-30&&m->x<=30&&m->y<=maxy-10)
    {
      clearviewport();
      return;
    }
  }
  getch();
  return;
}

void partial_opening_window()
{
  int maxx=getmaxx();
  int maxy=getmaxy();
  clearviewport();
  setcolor(15);
  outer_rectangle();
  settextstyle(3,HORIZ_DIR,5);
  outtextxy(maxx/2,maxy/6,"{BUBBLE SHOOTER}");
  setcolor(13);
  line(140,55,480,55);
  line(140,120,480,120);

  play_button();
  exit_button();
  credits_button();
  help_button();
  return;
}


int opening_window(mouse *m)
{
  int maxx=getmaxx(),j,op;
  int maxy=getmaxy();
  setcolor(RED);
  settextjustify(1,1);
  settextstyle(3,HORIZ_DIR,10);
  for(j=0;j<10;j++)
  {
    outer_rectangle();
    setcolor(j+1);
    settextstyle(1,HORIZ_DIR,j);
    outtextxy(maxx/2,maxy/2,"WELCOME");
    delay(100);
    clearviewport();
  }
  outtextxy(maxx/2,maxy/2,"WELCOME");
  for(j=2;j<=maxx/2;j++)
  {
    setcolor(BLACK);
    line(j,40,j,maxy-40);
    line(maxx-j,40,maxx-j,maxy-40);
    delay(5);
  }
  setcolor(15);
  outer_rectangle();
  settextstyle(3,HORIZ_DIR,5);
  outtextxy(maxx/2,maxy/6,"{BUBBLE SHOOTER}");
  setcolor(13);
  line(140,55,480,55);
  line(140,120,480,120);

  play_button();
  exit_button();
  credits_button();
  help_button();

  while(!kbhit())
  {
    mouse_click(m);
    if(m->x>=maxx/2-40&&m->y>=maxy-maxy/4-80&&m->x<=maxx/2+40&&m->y<=maxy-maxy/4)
    {
      clearviewport();
      return 1;
    }
    if(m->x>=10&&m->y>=maxy-30&&m->x<=30&&m->y<=maxy-10)
    {
      clearviewport();
      return 2;
    }
    if(m->x>=10&&m->y>=maxy-60&&m->x<=30&&m->y<=maxy-40)
    {
      clearviewport();
      help_menu();
      partial_opening_window();
      mouse_click(m);
    }
    if(m->x>=maxx-31&&m->x<=maxx-11&&m->y>=maxy-31&&m->y<=maxy-11)
    {
      clearviewport();
      credit_window(m);
      clearviewport();
      op=opening_window(m);
      return op;
    }
  }
  getch();
  clearviewport();
  return 0;
}

void mouse_click(mouse *m)
{
    while(!kbhit())
    {
	getmousepos(&m->button,&m->x,&m->y);
 //	gotoxy(5,3);
   //    (m->button&1)==1?printf("down"):printf("UP");
	if(m->button&1==1)
	  return;

     //	gotoxy(20,3);
 //	(m->button&2)==2?printf("DOWN"):printf("UP");

    //	gotoxy(65,2);
   //	printf("X=%03d y=%03d",m->x,m->y);

    }
  return;
}

int find_touch(ultimate_head *ut,ball *b)
{
  int q,a,j;
  head *hp;
  node *p;
  float d;
  hp=ut->ultimate_start;
  a=0;
  while(hp!=NULL)
  {
    p=hp->start;
    q=0;
    while(p!=NULL)
    {
      if(p->data!=0&&b->y<=280&&(p->up_left->data==0||p->up_right->data==0||p->left->data==0||p->right->data==0||p->down_left->data==0||p->down_right->data==0))
      {
	d=sqrt(pow(b->x-p->x,2)+pow(b->y-p->y,2));
	if(d<=20)
	{
	  if(b->colour==turn_light(p->data)||b->colour==turn_bright(p->data))
	  {
	    print_surrounding(ut,a,q,b);
	    remove_all_bright(ut);
	    loss_connection(ut);
	    loss_connection(ut);
	    setcolor(b->colour);
	    return 1;
	  }
	  else
	  {
	    delay(500);
	    return 1;
	  }
	}
      }
      q++;
      p=p->next;
    }
    a++;
    hp=hp->next;
  }
  return 0;
}
void touch(ultimate_head *ut,mouse *m,ball *b)
{
  float x_move,y_move;
  int maxx,maxy,ch,area,reaction=0;
  char *buff;
  maxx=getmaxx();
  maxy=getmaxy();
  b->x=(float)maxx/2;
  b->y=(float)maxy-40;
  setcolor(b->colour);
  circle(b->x,b->y,10);
  area=imagesize((int)b->x-10,(int)b->y-10,(int)b->x+10,(int)b->y+10);
  buff=(char*)malloc(area);
  setfillstyle(SOLID_FILL,b->colour);
  floodfill(b->x,b->y,b->colour);
  getimage((int)b->x-10,(int)b->y-10,(int)b->x+10,(int)b->y+10,buff);
  x_move=(float)m->x-b->x;
  y_move=(float)b->y-m->y;
  x_move=x_move/y_move;
  y_move=-1;
  //getch();
  while(1)
  {
    if(kbhit())
    {
      ch=getch();
      if(ch=='\r')
      {
	x_move=-1*x_move;
	y_move=-1*y_move;
      }
      else
      {
	if(ch==27)
	  break;
      }
    }
    putimage((int)b->x-10,(int)b->y-10,buff,XOR_PUT);
    b->x=b->x+(x_move)*1;
    b->y=b->y+(y_move)*1;
    putimage((int)b->x-10,(int)b->y-10,buff,XOR_PUT);
    reaction=find_touch(ut,b);
    if(reaction==1)
      break;
    delay(2);
    if(b->x>maxx-131-13||b->x<130+13)
      x_move=-1*x_move;
    if(b->y>maxy-13||b->y<57+13)
      y_move=-1*y_move;
 }
 putimage((int)b->x-10,(int)b->y-10,buff,XOR_PUT);
 return;
}

int check_for_5_steps()
{
  score++;
  if(condition==5)
  {
    current_state++;
    condition=0;
    return 1;
  }
  else
   return 0;
}

int ground_touch(ultimate_head *ut)
{
  head *hp;
  node *p;
  hp=ut->ultimate_start;
  p=hp->start;
  while(p!=NULL)
  {
    if(p->y>=270)
      return 1;
    p=p->next;
  }
  return 0;
}

int main()
{
  int a,p,ch,gd=DETECT,gm,maxx,maxy,op_ch,i;
  FILE *fptr;
  mouse m;
  ball b;
  ultimate_head u;
  u.ultimate_start=NULL;
  u.avaiable=0;
  level=0;
  condition=0;
  current_state=7;
  score=0;
  fptr=fopen("C:\\project.txt","r");
  fscanf(fptr,"%d",&highscore);
  printf("%d",highscore);
  fclose(fptr);
  for(i=0;i<3;i++)
    current_ball[i]=random1();
  initgraph(&gd,&gm,"C:\\TurboC3\\BGI");
  initmouse();
  showmouseptr();
  maxx=getmaxx();
  maxy=getmaxy();
  make_sure_15(&u);
  srand(time(NULL));
  op_ch=opening_window(&m);
  if(op_ch==2)
    return 0;
  hidemouseptr();
  clearviewport();
  showmouseptr();
  restrictmouseptr(0,0,maxx-1,380);
  while(1)
  {
      if(kbhit())
      {
	ch=getch();
	if(ch==27)
	   break;
      }
      fflush(stdin);
      b.colour=current_ball_colour();
      setcolor(b.colour);
      circle(maxx/2,maxy-40,10);
      setfillstyle(SOLID_FILL,b.colour);
      floodfill(maxx/2,maxy-40,b.colour);
      give_screen(&u,&b);
      show_balls();
      mouse_click(&m);
      hidemouseptr();
      if(m.button&1==1)
      {
	setfillstyle(SOLID_FILL,BLACK);
	floodfill(maxx/2,maxy-40,b.colour);
	setcolor(BLACK);
	circle(maxx/2,maxy-40,10);
	setcolor(WHITE);
	touch(&u,&m,&b);
	condition++;
	if(check_for_5_steps())
	  clearviewport();
	setfillstyle(SOLID_FILL,BLACK);
	bar(maxx-101,117,maxx-31,67+70);
	give_screen(&u,&b);
	while(1)
	{
	  if(check_for_all_zero(&u)==0)
	    break;
	  else
	  {
	    if(current_state>=2)
	      current_state--;
	    clearviewport();
	  }
	}
      }
   //   setposi(300,300);
      showmouseptr();
      if(ground_touch(&u))
	break;
  }
  if(score>=highscore)
  {
    fptr=fopen("C:\\project.txt","w");
    fprintf(fptr,"%d",score);
    fclose(fptr);
  }
  return 0;
}
