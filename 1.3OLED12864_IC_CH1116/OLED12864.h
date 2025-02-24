#ifndef __OLED12864_H__
#define __OLED12864_H__




void Delay(unsigned int time);
void Ins_trans(unsigned char command);
void Data_trans(unsigned char command);
void Column_set(unsigned char column);
void Page_set(unsigned char page);
void Screen_FillClear(unsigned char FC);
void Picture_display(unsigned char *ptr_pic);
void Initial();
void display_zifu_16x16(char page,char column,char *dp);
void display_zifu_16x8(char page,char column,char *dp);
void Screen_FillCol(unsigned char FC) ;
void OLED_fullCOL(unsigned char oddData, unsigned char evenData);
void Display_Sequence();
unsigned int ReadKey() ;
void Display_Sequence2();
void PerformScreenFillSequence();


#endif