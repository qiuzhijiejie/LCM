/*****************************************/
/* 汉字写入函数(图形方式)CCW2_PR         */
/* 函数参数选择：PARA1                   */
/* 内部调用函数：PR1(),PR11()            */
/* 连接字库：      CCTAB                 */
/* 函数调用入口：  CCW2_PR()             */
/* 函数输入变量：  O_X,O_Y,CODE_1        */
/*****************************************/
CCW2_PR()
    {
     unsigned int i,j,count1;
          i=para1;
          j=i*O_Y+O_X+0x0800;  /* "0x0800"为图形区起始地址可根据客户要求自己定 */
       for(count1=0;count1<16;count1++)
         {
           dat1=j%256;
           dat2=j/256;
           com=0x24;
           PR1();
           dat2=CCTAB[code_1][count1];   /* 写入汉字左侧代码 */
           com=0xc0;
           PR11();
           dat2=CCTAB[code_1][count1+16];  /* 写入汉字右侧代码 */
           PR11();
           j=j+i;
         }
      }
