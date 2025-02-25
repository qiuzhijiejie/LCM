#include <reg51.h>
#include <stdio.h>
#include <math.h>
#include <absacc.h>
#include <string.h>
/* 设置地址 */
#define  d_add XBYTE[0x8000]
#define  c_add XBYTE[0x8100]
/* MD3,MD2设置参数 */
#define  para1 0x28;
/* 设置判忙位 */
 sbit Acc_0=ACC^0;
 sbit Acc_1=ACC^1;
 sbit Acc_3=ACC^3;
 sbit Acc_6=ACC^6;
/* X,Y坐标 */
unsigned int chart_add_l,chart_add_h;
unsigned char dat1,dat2,com;
/************************/
/*  演示主程序(屏拷贝)  */
/************************/
void main()
     {
       unsigned char i;
      CLEAR();         /* 调清显示函数 */
      dat1=0x28;     /* 设置文本区域宽度=40字节 */
      dat2=0x00;
      com=0x41;
      PR1();
      com=0x43;      /* 设置图形区域宽度=40字节 */
      PR1();
      dat2=0x00;     /* 设置文本区地址,左上角 */
      dat1=0x00;
      com=0x24;
      PR1();
      dat2=0x21;     /* 写入"A" */
      com=0xc4;
      PR11();
      dat1=0x1d;     /* 设置文本区地址,右上角  */
      dat2=0x00;
      com=0x24;
      PR1();
      dat2=0x22;     /* 写入"B" */
      com=0xc4;
      PR11();
      dat1=0x58;       /* 设置文本区地址,左下角 */
      dat2=0x02;
      com=0x24;
      PR1();
      dat2=0x23;       /* 写入"C" */
      com=0xc4;
      PR11();
      dat1=0x75;       /* 设置文本区地址,右下角 */
      dat2=0x02;
      com=0x24;
      PR1();
      dat2=0x24;       /* 写入"D" */
      com=0xc4;
      PR11();
      chart_add_l=0x00;      /* 设置图形区地址 */
      chart_add_h=0x08;      /* 第一行首地址 */
      SCOPY();       /* 调用屏拷贝函数 */
       com=0x98;
       PR12();
     for(i=0;i<100;)  /* 观察显示效果 */
        {}
     }

/**************************/
/*       清显示函数       */
/**************************/
CLEAR()
    {
    unsigned char i,j;
        dat1=0x00;
        dat2=0x00;
        com=0x24;
        PR1();
        com=0xb0;
        PR12();
    for(i=0;i<205;i++)
       {
         for(j=0;j<=40;j++)
            {
                ST3();
                d_add=0x00;
            }
        }
       com=0xb2;
       PR12();
    }
/**********************/
/*    屏拷贝函数    */
/**********************/
SCOPY()
  {
    unsigned char i;
   for(i=0;i<16;i++) /* 屏拷贝16次 */
        {
          dat1=chart_add_l;
          dat2=chart_add_h;
          com=0x24;
          PR1();
          com=0xe8;    /* 写入拷贝指令 */
          PR12();      
          st6();      /* 判屏拷贝状态位正确与否,如不正确将在"st6()"处死循环 */ 
          chart_add_l=chart_add_l;+para1;  /* 修正图形区地址 */
         }
   }
/************************/
/*       写数据函数     */
/************************/
PR1()
    {
      ST01();
      d_add=dat1;
      PR11();
      PR12();
    }

PR11()
    {
      ST01();
      d_add=dat2;
      PR12();
    }
PR2()
    {
      ST01();
      dat2=d_add;
    }
/*************************/
/*       写指令函数      */
/*************************/
PR12()
   {
     ST01();
     c_add=com;
   }
/*************************/
/*      判"S1，S0"函数     */
/*************************/
ST01()
    {
      do
        {
        ACC=c_add;
        }
      while(Acc_0==0,Acc_1==0);
    }
/*************************/
/*        判"S3"函数       */
/*************************/
ST3()
   {
     do
     {
       ACC=c_add;
     }
     while(Acc_3==0);
   }
/************************/
/*        判"S6"函数      */
/************************/
ST6()
  {
    do
      {
       ACC=c_add;
      }
    while(Acc_6==1);
  }
