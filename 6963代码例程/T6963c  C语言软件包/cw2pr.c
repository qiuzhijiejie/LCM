/************************************/
/* 西文字符写入函数CW2_PR           */
/* 函数参数选择：PARA1              */
/* 内部调用函数：PR1(),PR11()       */
/* 函数调用入口： CW2_PR()          */
/* 函数输入变量：O_X,O_Y,DTAB,ATTR  */
/************************************/
CW2_PR()
    {
       unsigned char b,i,j,k;
       unsigned int a;
        i=para1;
        a=i*O_Y+O_X;  /* 计算文本显示RAM 地址 */
        i=a%256;
        j=a/256;
        for(a=0;a<16;a++)
           {
            for(k=O_X;k<30;)
              {
               for(b=0;b<11;b++)
                  {
                    dat1=k;
                    dat2=a;
                    com=0x21;  /* 光标位置设置 */
                    PR1();
                    dat1=O_X+j;
                    dat2=O_Y;
                    com=0x24;    /* 设置地址指针 */
                    PR1();
                    dat2=DTAB[b];  /* 写入字符代码 */
                    com=0xc4;
                    PR11();
                    dat2=0x08;  /* 计算相应的属性区RAM地址 */
                    com=0x24;   /* 设置地址指针 */
                    PR1();
                    dat2=attr;  /* 写入属性参数 */
                    com=0xc4;
                    PR11();
                    j++;
                    k++;
                   if(k>=30)break ;
                   }
                }
           }
       }

