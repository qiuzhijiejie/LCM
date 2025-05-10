;SMG12864ZK标准字符点阵型液晶显示模块的演示程序[ASM51编程语言][MCS51三线式串行接口方式]
;应用产品: SMG12864ZK SMG12864G2-ZK SMG12864G3-ZK标准中文字符点阵型液晶显示模块
;  本演示程序适用于SMG12864ZK液晶显示模块与MCS51系列单片机采用MCS51三线式串行接口
;方式的硬件连线方式。
;  本演示程序包括ST7920兼容芯片的MCS51三线式串行接口方式子程序集,SMC12864系列
;标准中文字符点阵型液晶显示模块的接口子程序集,ST7920兼容芯片的液晶显示控制器的
;通用子程序集,以及SMG12864系列标准中文字符点阵型液晶显示的演示子程序.
;  本演示的内容为,在16列ASCII字符(8列中文)X4行的液晶显示屏上显示" SMG12864ZK ",
;"中文字符图形点阵","液晶显示模块","长沙太阳人SUNMAN",所有数字0..9依次全屏显示
;一遍,显示清屏,所有英文字母依次显示,显示自定义字符,图形模式图片显示.
;-------------------------------------------------------------------------------
;创建日期: 2009.10.29
;软件环境: VW For Windows
;硬件环境: AT89C52 MCU,MCS-51系列 MCU,etc.
;创建人: 褚建军(chujianjun@sunman.cn),谭超(tanchao@sunman.cn).
;技术支持: 电话(0731-84167806,84167807),传真(0731-84167807),网址(www.sunman.cn)
;版权信息: 长沙太阳人电子有限公司版权所有,转载请指明出处.
;参考网页: http:/www.sunman.cn/lcm/product/SMG12864ZK.html
;-------------------------------------------------------------------------------
;SMG12864ZK产品引脚说明及演示连线
;PIN1: VSS  [电源地]-------------------VSS
;PIN2: VDD  [电源正极]-----------------VDD(+5V)
;PIN3: NC   [空脚]
;PIN4: CS   [片选信号输入]-------------P3.0
;PIN5: STD  [串行数据输入]-------------P3.1
;PIN6: SCLK [串行移位脉冲输入]---------P3.2
;PIN7: D0   [Data I/O]-----------------P1.0
;PIN8: D1   [Data I/O]-----------------P1.1
;PIN9: D2   [Data I/O]-----------------P1.2
;PIN10:D3   [Data I/O]-----------------P1.3
;PIN11:D4   [Data I/O]-----------------P1.4
;PIN12:D5   [Data I/O]-----------------P1.5
;PIN13:D6   [Data I/O]-----------------P1.6
;PIN15:PSB  [模块内部已选择,悬空]
;PIN16:NC   [空脚]
;PIN17:RST  [复位端,低电平有效]--------VDD
;PIN18:NC   [空脚]
;PIN19:BLA  [背光源正极]---------------接20欧电阻到+5V.
;PIN20:BLK  [背光源负极]---------------VSS
;请参见http://download.sunman.cn/lcm/product/1/SMG12864ZK.pdf
;字符表参见http://download.sunman.cn/lcm/reference/ST7920_GB_CHARACTER_TABLE.pdf
;芯片资料参见http://download.sunman.cn/lcm/reference/ST7920.pdf
;-------------------------------------------------------------------------------
;以下为产品接口引脚在演示程序中的预定义
;用户在编写应用程序时,需按自己的实际硬件连线来重新定义
CSPIN           BIT P3.0                ;CS对应单片机引脚
STDPIN          BIT P3.1                ;STD对应单片机引脚
SCLKPIN         BIT P3.2                ;SCLK对应单片机引脚
;-------------------------------------------------------------------------------
;以下CXPOS,CYPOS变量用于指示当前操作字符的位置的预定义
;用户在编写应用程序时,需按自己的实际软件程序需要来重新定义
CXPOS           EQU 20H                 ;列方向地址指针(用于CHARLCDPOS子程序)
CYPOS           EQU 21H                 ;行方向地址指针(用于CHARLCDPOS子程序)
FCHARBUF        EQU 22H                 ;上一个显示的ASCII字符
;-------------------------------------------------------------------------------
;以下GXPOS,GYPOS变量用于指示绘图区域RAM的位置的预定义
;用户在编写应用程序时,需按自己的实际软件程序需要来重新定义
GXPOS           EQU 23H                 ;列方向地址指针(用于GLCDPOS子程序)
GYPOS           EQU 24H                 ;行方向地址指针(用于GLCDPOS子程序)

;-------------------------------------------------------------------------------
        ORG     0000H                   ;
        JMP     START

        ORG     100H
;-------------------------------------------------------------------------------
USERCHAR1CODE   EQU 0FFFFH              ;自定义字符1代码
USERCHAR2CODE   EQU 0FFFDH              ;自定义字符2代码
USERCHAR3CODE   EQU 0FFFBH              ;自定义字符3代码
USERCHAR4CODE   EQU 0FFF9H              ;自定义字符4代码
;以下自定义字符点阵代码,用户可在实际使用时根据需要自己编写,位置参见芯片资料第13页
CHAR1CGRAMTAB:                          ;自定义字符1点阵
        DB  000H,00fH
        DB  000H,07fH
        DB  001H,0f0H
        DB  003H,0c0H
        DB  007H,00fH
        DB  00eH,03fH
        DB  01cH,0feH
        DB  039H,0fcH
        DB  033H,0fcH
        DB  073H,0feH
        DB  067H,0ffH
        DB  067H,0e7H
        DB  0cfH,0e3H
        DB  0cfH,0e1H
        DB  0cfH,0e4H
        DB  0c0H,006H
CHAR2CGRAMTAB:                          ;自定义字符2点阵
        DB  0f0H,000H
        DB  0feH,000H
        DB  00fH,080H
        DB  003H,0c0H
        DB  0f0H,0e0H
        DB  0fcH,070H
        DB  07fH,038H
        DB  03fH,09cH
        DB  03fH,0ccH
        DB  07fH,0ceH
        DB  0ffH,0e6H
        DB  0e7H,0e6H
        DB  0c7H,0f3H
        DB  087H,0ffH
        DB  027H,0ffH
        DB  060H,003H
CHAR3CGRAMTAB:                          ;自定义字符3点阵
        DB  0c0H,007H
        DB  0ffH,0e7H
        DB  0ffH,0e7H
        DB  0cfH,0e7H
        DB  067H,0e7H
        DB  067H,0ffH
        DB  073H,0ffH
        DB  033H,0ffH
        DB  039H,0ffH
        DB  01cH,0ffH
        DB  00eH,03fH
        DB  007H,00fH
        DB  003H,0c0H
        DB  001H,0f0H
        DB  000H,07fH
        DB  000H,00fH
CHAR4CGRAMTAB:                          ;自定义字符4点阵
        DB  0e0H,003H
        DB  0e7H,0f3H
        DB  0e7H,0f3H
        DB  0e7H,0f3H
        DB  0e7H,0e6H
        DB  0ffH,0e6H
        DB  0ffH,0ceH
        DB  0ffH,0ccH
        DB  0ffH,09cH
        DB  0ffH,038H
        DB  0fcH,070H
        DB  0f0H,0e0H
        DB  003H,0c0H
        DB  00fH,080H
        DB  0feH,000H
        DB  0f0H,000H
;-------------------------------------------------------------------------------
;以下为图片库点阵代码
;实际使用时请包含由SUNMAN图形点阵代码生成器.EXE自动生成的IMAGELIB.ASM文件来替换该
;部分
IMG_SUNMAN_128x64TAB:
        DB  128,64
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,003H,0c0H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,00fH,0e0H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,028H,0e0H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,020H,000H,000H,000H,000H,038H,038H,0e0H,000H,000H,000H,000H,000H
        DB  000H,000H,001H,0f0H,000H,000H,000H,000H,0fcH,038H,0e0H,000H,000H,000H,000H,000H
        DB  000H,000H,001H,0f8H,000H,000H,000H,003H,0fcH,01fH,0e0H,000H,000H,000H,000H,000H
        DB  000H,000H,001H,0fcH,000H,000H,000H,001H,0fcH,01bH,0c0H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,0feH,000H,000H,000H,000H,038H,01bH,0c0H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,0feH,000H,000H,000H,000H,030H,01bH,0c0H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,07cH,000H,000H,000H,000H,030H,01eH,0c0H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,0fcH,000H,000H,000H,000H,030H,01cH,0fcH,000H,003H,080H,000H,000H
        DB  000H,000H,000H,0f8H,000H,000H,000H,000H,038H,01fH,0feH,000H,003H,0c0H,000H,000H
        DB  000H,000H,000H,0f8H,000H,000H,000H,001H,098H,07fH,0e0H,000H,007H,0e0H,000H,000H
        DB  000H,000H,000H,0f8H,000H,000H,000H,001H,08cH,0fbH,0c0H,000H,007H,0c0H,000H,000H
        DB  000H,000H,000H,0fcH,000H,000H,000H,001H,0cfH,0e7H,080H,000H,00fH,080H,000H,000H
        DB  000H,000H,001H,0f2H,000H,000H,000H,001H,0cdH,08eH,000H,000H,00fH,000H,000H,000H
        DB  000H,000H,001H,0f1H,0c0H,000H,000H,001H,0fcH,01fH,0feH,000H,01cH,000H,000H,000H
        DB  000H,000H,001H,0e1H,0ffH,000H,000H,001H,0bcH,03fH,007H,000H,038H,000H,000H,000H
        DB  000H,000H,001H,0e3H,0ffH,080H,000H,001H,0b8H,0f9H,087H,000H,07eH,000H,000H,000H
        DB  000H,000H,001H,0e7H,0ffH,0c0H,000H,001H,090H,0f1H,087H,000H,0ffH,0c0H,000H,000H
        DB  000H,000H,003H,0cfH,0ffH,0c0H,000H,001H,080H,0b9H,087H,000H,0e7H,0e0H,000H,000H
        DB  000H,000H,003H,0ffH,0ffH,0c0H,000H,001H,080H,033H,08eH,001H,0c0H,0f8H,000H,000H
        DB  000H,000H,003H,0ffH,0f8H,000H,000H,001H,020H,063H,00eH,003H,080H,03cH,000H,000H
        DB  000H,000H,003H,0ffH,0e0H,000H,000H,001H,020H,0e6H,00cH,003H,000H,01fH,000H,000H
        DB  000H,000H,00fH,0ffH,0c0H,000H,000H,001H,040H,0ceH,01cH,007H,000H,00fH,080H,000H
        DB  000H,000H,01fH,0ffH,000H,000H,000H,001H,0c0H,08cH,018H,00eH,000H,007H,0e0H,000H
        DB  000H,000H,07fH,0feH,000H,000H,000H,001H,0c0H,018H,038H,00eH,000H,003H,0fcH,000H
        DB  000H,007H,0ffH,0f8H,000H,000H,000H,001H,0c0H,038H,070H,03cH,000H,001H,0ffH,000H
        DB  000H,03fH,0ffH,0f0H,000H,000H,000H,000H,080H,007H,0e0H,038H,000H,000H,0ffH,0c0H
        DB  000H,01fH,0ffH,0e0H,000H,000H,000H,000H,000H,003H,0c0H,000H,000H,000H,07fH,0c0H
        DB  000H,00fH,0ffH,0f0H,000H,000H,000H,000H,000H,001H,080H,000H,000H,000H,030H,000H
        DB  000H,007H,0ffH,0fcH,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,003H,0ffH,0feH,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,001H,0ffH,0ffH,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,01fH,01fH,080H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,01fH,00fH,0c0H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,01eH,003H,0f0H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,01eH,001H,0f8H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,03eH,000H,0fcH,000H,000H,000H,0e0H,000H,000H,000H,000H,040H,000H,000H
        DB  000H,000H,03eH,000H,0feH,000H,000H,003H,0f1H,082H,061H,09cH,060H,061H,086H,000H
        DB  000H,000H,03eH,000H,07fH,000H,000H,002H,011H,082H,071H,09cH,060H,0e1H,0c6H,000H
        DB  000H,000H,03eH,000H,03fH,080H,000H,006H,019H,082H,071H,09cH,060H,0e1H,0c6H,000H
        DB  000H,000H,07fH,0e0H,01fH,0c0H,000H,006H,001H,082H,079H,09cH,0e0H,0b1H,0e6H,000H
        DB  000H,000H,0ffH,0f0H,01fH,0f0H,000H,003H,081H,082H,069H,09cH,0e1H,0b1H,0a6H,000H
        DB  000H,001H,0ffH,0f8H,00fH,0fcH,000H,001H,0e1H,082H,069H,096H,0a1H,091H,0a6H,000H
        DB  000H,003H,0fbH,0fcH,007H,0ffH,000H,000H,071H,082H,06dH,096H,0a1H,019H,0b6H,000H
        DB  000H,007H,0f0H,0fcH,007H,0ffH,080H,000H,019H,082H,065H,093H,0a1H,0f9H,096H,000H
        DB  000H,00fH,0e0H,07cH,001H,0ffH,0e0H,006H,019H,082H,067H,093H,0a3H,019H,09eH,000H
        DB  004H,07fH,0c0H,01cH,000H,0ffH,0fcH,006H,019H,086H,063H,093H,0a3H,00dH,08eH,000H
        DB  001H,0ffH,000H,008H,000H,07fH,0feH,006H,019H,086H,063H,093H,022H,00dH,08eH,000H
        DB  000H,010H,000H,000H,000H,01fH,0ffH,003H,0f0H,0fcH,061H,093H,026H,00dH,086H,000H
        DB  000H,000H,000H,000H,000H,007H,0ffH,000H,0e0H,078H,061H,091H,026H,005H,086H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
        DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
;-------------------------------------------------------------------------------
;演示用字符串定义
STR1:   DB     " SMG12864ZK ",0
STR2:   DB     "中文字符图形点阵",0
STR3:   DB     "液晶显示模块",0
STR4:   DB     "长沙太阳人SUNMAN",0
STR5:   DW      USERCHAR1CODE,USERCHAR2CODE,00H
STR6:   DW      USERCHAR3CODE,USERCHAR4CODE,00H
;-------------------------------------------------------------------------------

START:  MOV     SP,#60H
MAIN:
        CALL    EXSAMPLE                ;调用演示程序
        JMP     MAIN
EXSAMPLE:                               ;演示程序
;0.演示前的准备,LCD液晶显示控制器初始化,所有显示全清零
        CALL    LCDRESET                ;液晶显示控制器初始化
        MOV     A,#' '
        MOV     B,A
        CALL    CHARLCDFILL             ;显示清屏

;1.字符串演示: 在(2,0)字符位置显示" SMG12864ZK "
        MOV     CXPOS,#2                ;设置当前显示位置为第3列
        MOV     CYPOS,#0                ;设置当前显示位置为第1行
        MOV     DPTR,#STR1
        CALL    PUTSTR                  ;显示字符串
        CALL    EXDELAY                 ;延时约300mS
        CALL    EXDELAY                 ;延时约300mS

;2.字符串演示: 在(0,1)字符位置显示"中文字符图形点阵"
        MOV     CXPOS,#0                ;设置当前显示位置为第1列
        MOV     CYPOS,#1                ;设置当前显示位置为第2行
        MOV     DPTR,#STR2
        CALL    PUTSTR                  ;显示字符串
        CALL    EXDELAY                 ;延时约300mS
        CALL    EXDELAY                 ;延时约300mS

;3.字符串演示: 在(2,2)字符位置显示"液晶显示模块"
        MOV     CXPOS,#2                ;设置当前显示位置为第3列
        MOV     CYPOS,#2                ;设置当前显示位置为第3行
        MOV     DPTR,#STR3
        CALL    PUTSTR                  ;显示字符串
        CALL    EXDELAY                 ;延时约300mS
        CALL    EXDELAY                 ;延时约300mS

;4.字符串演示: 在(0,3)字符位置显示"长沙太阳人SUNMAN"
        MOV     CXPOS,#0                ;设置当前显示位置为第1列
        MOV     CYPOS,#3                ;设置当前显示位置为第4行
        MOV     DPTR,#STR4
        CALL    PUTSTR                  ;显示字符串
        CALL    EXDELAY                 ;延时约300mS
        CALL    EXDELAY                 ;延时约300mS

;5.字符填充演示: 整屏显示0..9 空格一遍
        MOV     R7,#'0'                 ;初始数字为0
ESP_PA:                                 ;
        MOV     A,R7                    ;
        MOV     B,A
        CALL    CHARLCDFILL             ;整屏显示为A的值
        CALL    EXDELAY                 ;延时300mS
        INC     R7
        CJNE    R7,#'9'+1,ESP_PA        ;
        MOV     A,#' '                  ;
        MOV     B,A
        CALL    CHARLCDFILL             ;显示清屏
        CALL    EXDELAY                 ;延时300mS

;6.字符演示: 依次显示A..Z 一遍
        MOV     R7,#'A'                 ;依次显示A..Z一遍
ESP_PB:                                 ;
        MOV     A,R7                    ;
        MOV     B,#00H
        CALL    PUTCHAR                 ;当前位置显示为A的值
        CALL    EXDELAY                 ;延时300mS
        INC     R7
        CJNE    R7,#'Z'+1,ESP_PB       ;

;7.自定义字符演示: 装入自定义字符点阵发生器
        MOV     DPTR,#CHAR1CGRAMTAB
        MOV     A,#00H
        CALL    SETCHARCGRAM            ;装入自定义字符字符点阵
        MOV     DPTR,#CHAR2CGRAMTAB
        MOV     A,#01H
        CALL    SETCHARCGRAM            ;装入自定义字符字符点阵
        MOV     DPTR,#CHAR3CGRAMTAB
        MOV     A,#02H
        CALL    SETCHARCGRAM            ;装入自定义字符字符点阵
        MOV     DPTR,#CHAR4CGRAMTAB
        MOV     A,#03H
        CALL    SETCHARCGRAM            ;装入自定义字符字符点阵

;8.自定义字符演示：在(4,0)(5,0)(4,1)(5,1)字符位置显示自定义字符1,2,3,4
        MOV     CXPOS,#4                ;设置当前显示位置为第5列
        MOV     CYPOS,#0                ;设置当前显示位置为第1行
        MOV     A,#USERCHAR1CODE%256
        MOV     B,#USERCHAR1CODE/256
        CALL    PUTCHAR                 ;显示自定义字符1
        CALL    EXDELAY                 ;延时约300mS
        MOV     A,#USERCHAR2CODE%256
        MOV     B,#USERCHAR2CODE/256
        CALL    PUTCHAR                 ;显示自定义字符2
        CALL    EXDELAY                 ;延时约300mS
        MOV     CXPOS,#4                ;设置当前显示位置为第5列
        MOV     CYPOS,#1                ;设置当前显示位置为第2行
        MOV     A,#USERCHAR3CODE%256
        MOV     B,#USERCHAR3CODE/256
        CALL    PUTCHAR                 ;显示自定义字符3
        CALL    EXDELAY                 ;延时约300mS
        MOV     A,#USERCHAR4CODE%256
        MOV     B,#USERCHAR4CODE/256
        CALL    PUTCHAR                 ;显示自定义字符4
        CALL    EXDELAY                 ;延时约300mS
        CALL    EXDELAY                 ;延时约300mS

;9.图形模式演示: 在(0,0)点阵位置显示图片
        MOV     A,#' '
        MOV     B,A
        CALL    CHARLCDFILL             ;字符显示清屏
        CALL    LCDGRAPHON              ;开图形模式
        MOV     DPTR,#IMG_SUNMAN_128x64TAB
        MOV     GXPOS,#0
        MOV     GYPOS,#0
        CALL    PUTIMAGE                ;显示图片
        CALL    EXDELAY                 ;延时约300mS
        CALL    EXDELAY                 ;延时约300mS
        MOV     A,#00H
        CALL    GLCDFILL                ;图形区域清屏
        CALL    LCDGRAPHOFF             ;关图形模式
        RET
EXDELAY:                                ;演示延时子程序
        MOV     R2,#30                  ;延时约300mS
EDY_PA: MOV     R1,#100
EDY_PB: MOV     R0,#49
        DJNZ    R0,$
        DJNZ    R1,EDY_PB
        DJNZ    R2,EDY_PA
        RET
;-------------------------------------------------------------------------------
;字符操作区域
;12864ZK字符区域定义为:
;   CXPOS(0-15) 对应于第1列到第16列位置,每个字符列位置实际占位8点,总计对应128点
;   CYPOS(0-3)对应于第1行到第4行,每个行位置实际占位16点,总计对应64点
;警告:直接设置CXPOS的位置时,请将CXPOS的值设置为偶数,否则显示的字符值会不正确.
;-------------------------------------------------------------------------------
;以下为CHARLCDFILL,PUTSTR,PUTCHAR,SETCHARCGRAM为ST7920兼容芯片的液晶显示控制器
;的通用子程序
;-------------------------------------------------------------------------------
;子程序名称:CHARLCDFILL(A,B).
;功能:整屏显示A/B表示的字符.
;输入:A,B.
;输出:无.
;影响:R0,R2.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
CHARLCDFILL:                            ;整屏显示A/B代表的字数据子程序
        MOV     CYPOS,#0                ;第1行位置
        MOV     CXPOS,#0                ;第1列位置
CLF_PA:
        CALL    CHARLCDPOS
        XCH     A,B
        CALL    LCDWD
        XCH     A,B
        CALL    LCDWD
        CALL    CHARCURSORNEXT          ;置字符位置为下一个有效位置
        CALL    CHARCURSORNEXT          ;置字符位置为下一个有效位置
        MOV     R2,CXPOS
        CJNE    R2,#00H,CLF_PA
        MOV     R2,CYPOS
        CJNE    R2,#00H,CLF_PA
        RET
;-------------------------------------------------------------------------------
;子程序名称:PUTSTR(*S).
;功能:写字符串点阵,若*S=0 表示字符串结束.
;输入:*S.
;输出:.
;影响:R0,R1,R2,R3,R4,R5,R6,R7,A,B.
;修改日期:2009.7.17
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
PUTSTR:                                 ;显示字符串子程序,字符码为00H时退出
        CALL    GETSTRCHAR
        JZ      PSR_LAX
        CALL    PUTCHAR
        JMP     PUTSTR
PSR_LAX:
        RET
;-------------------------------------------------------------------------------
;子程序名称:GETSTRCHAR(*S).
;功能:读单个字符串字符代码,本子程序仅供PUTSTR使用,若*S<128 表示字符为ASCII码,
;     A=*S后退出;否则表示为中文汉字代码,B=*S后,再读一个字符置入A中.
;输入:*S(DPTR).
;输出:.
;影响:R0,R1,R2,R3,R4,R5,R6,R7,A,B.
;修改日期:2009.7.17
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
GETSTRCHAR:                             ;字符串中取字符码子程序
        MOV     B,#00H
        CLR     A
        MOVC    A,@A+DPTR
        INC     DPTR
        JNB     ACC.7,GSC_LAX
        MOV     B,A
        CLR     A
        MOVC    A,@A+DPTR
        INC     DPTR
GSC_LAX:
        RET
;-------------------------------------------------------------------------------
;子程序名称:PUTCHAR(A,B).
;功能:在(CXPOS,CYPOS)字符位置写字符A/B.
;输入:A,B.
;输出:无.
;影响:R0.
;修改日期:2009.10.29
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
PUTCHAR:                                ;在(CXPOS,CYPOS)字符位置写字符子程序
        PUSH    DPL
        PUSH    DPH
        JNB     B.7,PCR_LAY
;CHINESE:                               ;高位字节大于128表示为汉字
                                        ;奇数位置判断功能
        PUSH    ACC                     ;写汉字时,CXPOS字符位置在奇数位置,则
        MOV     A,CXPOS                 ;自动补添一个空格对齐后再显示汉字
        JNB     ACC.0,PCR_LDX
        MOV     A,#' '
        CALL    LCDWD
        CALL    CHARCURSORNEXT          ;置字符位置为下一个有效位置
PCR_LDX:
        POP     ACC
        CALL    CHARLCDPOS              ;定位第CXPOS个字符位置的DDRAM地址
        XCH     A,B
        CJNE    A,#0FFH,PCR_LFY
                                        ;若高位字节为0FFH则表示为自定义字符
        CPL     A                       ;则转换为ST7920的字符码
        CALL    LCDWD                   ;写高位字符
        CPL     A
        XCH     A,B
        CALL    CHARCURSORNEXT          ;置字符位置为下一个有效位置
        CPL     A
        CALL    LCDWD                   ;写低位字符
        CPL     A
        JMP     PCR_LFX
PCR_LFY:
        CALL    LCDWD                   ;写高位字符
        XCH     A,B
        CALL    CHARCURSORNEXT          ;置字符位置为下一个有效位置
        CALL    LCDWD                   ;写低位字符
PCR_LFX:
        CALL    CHARCURSORNEXT          ;置字符位置为下一个有效位置
        JMP     PCR_LAX
PCR_LAY:
;ENGLISH:                               ;高位字节小于128表示为ASCII字符
        CALL    CHARLCDPOS              ;定位第CXPOS个字符位置的DDRAM地址

        PUSH    B
        MOV     B,CXPOS
        JNB     B.0,PCR_LEY
                                        ;写ASCII字符时,CXPOS字符位置在奇数位置,则
                                        ;重新写高位字符缓冲区内容
        XCH     A,FCHARBUF
        CALL    LCDWD
        XCH     A,FCHARBUF
        CALL    LCDWD                   ;低字节写ASCII字符
        JMP     PCR_LEX
PCR_LEY:                                ;CXPOS字符位置在偶数位置则
        CALL    LCDWD                   ;高字节写ASCII字符后补显空格
        MOV     FCHARBUF,A
        PUSH    ACC
        MOV     A,#' '                  ;如果为偶数位置,则将奇数位置显示为空格
        CALL    LCDWD
        POP     ACC
PCR_LEX:
        POP     B
        CALL    CHARCURSORNEXT          ;置字符位置为下一个有效位置

PCR_LAX:
        POP     DPH
        POP     DPL
        RET
;-------------------------------------------------------------------------------
;子程序名称:SETCHARCGRAM(A,*S).
;功能:将DPTR所指32字节数据设置到自定义字符A(0-3)的CGRAM中.
;输入:A,DPTR.
;输出:无.
;影响:R0,R2,A.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
SETCHARCGRAM:
        PUSH    ACC
        MOV     A,#34H
        CALL    LCDWC                   ;扩充指令集,图形模式关闭
        MOV     A,#02H
        CALL    LCDWC                   ;SR=0 激活CGRAM
        MOV     A,#30H
        CALL    LCDWC                  ;恢复为基本指令集
        POP     ACC
        ANL     A,#03H
        SWAP    A
        ORL     A,#40H
        CALL    LCDWC                   ;设置CGRAM地址
        MOV     R2,#32
SCG_PA:
        CLR     A
        MOVC    A,@A+DPTR
        CALL    LCDWD
        INC     DPTR
        DJNZ    R2,SCG_PA
        RET
;-------------------------------------------------------------------------------
;以下为CHARLCDPOS,CHARCURSORNEXT,LCDRESET为ST7920兼容芯片的液晶显示控制器的
;16ASCII字符X4行的SMG12864系列标准中文字符点阵型液晶显示模块的接口程序.
;-------------------------------------------------------------------------------
;子程序名称:CHARLCDPOS().
;功能:设置(CXPOS,CYPOS)字符位置的DDRAM地址.
;输入:无.
;输出:无.
;影响:无.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
CHARLCDPOS:                             ;设置(CXPOS,CYPOS)字符位置的DDRAM地址
        PUSH    ACC                     ;保护ACC的值
        ANL     CXPOS,#0FH              ;X位置范围(0到15)
        MOV     A,CXPOS
        CLR     C
        RRC     A
        MOV     R0,A
        MOV     A,CYPOS
        CJNE    A,#0,LPS_LA1
        MOV     A,#80H                  ;第一行 DDRAM:80H--87H
        ADD     A,R0
        JMP     LPS_LAX
LPS_LA1:CJNE    A,#1,LPS_LA2
        MOV     A,#90H                  ;第二行 DDRAM:90H--97H
        ADD     A,R0
        JMP     LPS_LAX
LPS_LA2:CJNE    A,#2,LPS_LA3
        MOV     A,#88H                  ;第三行 DDRAM:88H--8FH
        ADD     A,R0
        JMP     LPS_LAX
LPS_LA3:CJNE    A,#3,LPS_LAX
        MOV     A,#98H                  ;第四行 DDRAM:98H--9FH
        ADD     A,R0
        JMP     LPS_LAX
LPS_LAX:                                ;
        CALL    LCDWC
        POP     ACC                     ;恢复ACC的值
        RET
;-------------------------------------------------------------------------------
;子程序名称:CHARCURSORNEXT().
;功能:置字符位置为下一个有效位置.
;输入:无.
;输出:无.
;影响:R0.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
CHARCURSORNEXT:                         ;置字符位置为下一个有效位置子程序
        INC     CXPOS                   ;字符位置加1
        ANL     CXPOS,#0FH              ;字符位置CXPOS的有效范围为(0到15)
        MOV     R0,CXPOS
        CJNE    R0,#0,CSN_LAX           ;CXPOS为0表示要换行
        INC     CYPOS
        ANL     CYPOS,#03H              ;字符位置CYPOS的有效范围为(0到3)
CSN_LAX:
        RET
;-------------------------------------------------------------------------------
;子程序名称:LCDRESET().
;功能:液晶显示控制器初始化.
;输入:无.
;输出:无.
;影响:R0,ACC.
;修改日期:2009.10.29
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
LCDRESET:                              ;液晶显示控制器初始化子程序
        MOV     A,#33H                 ;接口模式设置
        CALL    LCDWC
        CALL    DELAY3MS                ;延时3MS
        MOV     A,#30H                 ;基本指令集
        CALL    LCDWC
        CALL    DELAY3MS                ;延时3MS
        MOV     A,#30H                 ;重复送基本指令集
        CALL    LCDWC
        CALL    DELAY3MS                ;延时3MS
        MOV     A,#01H                 ;清屏控制字
        CALL    LCDWC
        CALL    DELAY3MS                ;延时3MS
        CALL    LCDON                  ;字符显示开
        RET
DELAY3MS:                               ;延时3MS子程序
        MOV     R1,#15
DL3_PA: MOV     R2,#100
        DJNZ    R2,$
        DJNZ    R1,DL3_PA
        RET
;-------------------------------------------------------------------------------
;以下PUTSIZEIMAGE,GLCDFILL,GLCDPOS为图形对象的基本子程序,PUTIMAGE为图形对象的
;扩充子程序
;-------------------------------------------------------------------------------
;子程序名称:PUTSIZEIMAGE(GXPOS,GYPOS,XSIZE,YSIZE,*S).
;功能:在(GXPOS,GYPOS)位置绘制XSIZE列及YISZE行点阵的图形*S(XSIZE仅取8的倍数
;     GXPOS仅取16的倍数).
;输入:位置GXPOS,GYPOS;XSIZE=R6,YSIZE=R7,S=DPTR.
;输出:GXPOS=GXPOS+XSIZE;GYPOS=GYPOS+YSIZE.
;影响:R0,R4,R5.
;修改日期:2009.10.29
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
PUTSIZEIMAGE:                           ;显示图形子程序
                                        ;图形的大小,XSIZE=R6,YSIZE=R7
                                        ;图形的大小循环变量LX=R4,LY=R5
                                        ;图形数据1变量A1=R1,图形数据2变量A2=R2
                                        ;第一列数据的BIT位移变量K=R3
        MOV     R5,#00H                 ;LY=0
PIM_PA:
        PUSH    GXPOS                   ;保存XPOS位置,准备进行一行数据的处理
        MOV     R4,#00H                 ;LX=0
PIM_PB:                                 ;写一行图形数据
        MOV     A,GXPOS
        JB      ACC.3,PIM_LAX
        CALL    GLCDPOS                 ;绘图地址指针定位
PIM_LAX:
        CLR     A                       ;A2=*S
        MOVC    A,@A+DPTR
        INC     DPTR                    ;S++
        CALL    LCDWD
        MOV     A,R4                    ;LX+=8
        ADD     A,#08H
        MOV     R4,A
        MOV     A,GXPOS                 ;GXPOS+=8
        ADD     A,#08H
        MOV     GXPOS,A

        MOV     A,R6                    ;XSIZE与LX比较
        XRL     A,R4
        JZ      PIM_PBX                 ;XSIZE=LX
        JMP     PIM_PB                  ;XSIZE<>LX
PIM_PBX:

        POP     GXPOS                   ;恢复列首位置

        INC     GYPOS                   ;位置转下一行
        INC     R5                      ;LY++

        MOV     A,R7                    ;YSIZE与LY比较
        XRL     A,R5
        JZ      PIM_PAX                 ;YSIZE=LY
        JMP     PIM_PA                  ;YSIZE<>LY
PIM_PAX:
        RET
;-------------------------------------------------------------------------------
;子程序名称:PUTIMAGE(GXPOS,GYPOS,*S).
;功能:在(GXPOS,GYPOS)位置绘制XSIZE列及YISZE行点阵的图形*(S+2).
;输入:位置GXPOS,GYPOS;XSIZE=*S,YSIZE=*(S+1),S=DPTR+2.
;输出:GXPOS=GXPOS+XSIZE;GYPOS=GYPOS+YSIZE.
;影响:R0,R4,R5,R6,R7.
;修改日期:2009.7.17
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
PUTIMAGE:                               ;显示图象子程序,在(XPOS,YPOS)位置显示一幅
                                        ;图象,图象地址指针为DPTR,
                                        ;第2个字节为Y方向点阵数,随后的数据为图象
                                        ;点阵数据.
        CLR     A                       ;图象数据的第1个字节为X方向点阵数
        MOVC    A,@A+DPTR               ;XSIZE=*S
        MOV     R6,A                    ;
        INC     DPTR                    ;S++
        CLR     A                       ;图象数据的第2个字节为Y方向点阵数
        MOVC    A,@A+DPTR               ;YSIZE=*S
        MOV     R7,A                    ;
        INC     DPTR                    ;S++
        CALL    PUTSIZEIMAGE            ;绘制图形
        RET

;-------------------------------------------------------------------------------
;子程序名称:GLCDFILL(A).
;功能:整屏显示A表示的字节数据.
;输入:A.
;输出:无.
;影响:R0,R2,A,B.
;修改日期:2009.10.29
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
GLCDFILL:                               ;整屏显示A代表的字数据子程序
        MOV     GYPOS,#0                ;第1行位置
        MOV     GXPOS,#0                ;第1列位置
GLF_PB:
        CALL    GLCDPOS
        MOV     R2,#16
GLF_PA: CALL    LCDWD
        DJNZ    R2,GLF_PA
        INC     GYPOS
        MOV     R2,GYPOS
        CJNE    R2,#64,GLF_PB

        MOV     GYPOS,#0
        MOV     GXPOS,#0
        RET
;-------------------------------------------------------------------------------
;子程序名称:GLCDPOS(GXPOS,GYPOS)
;功能:设置绘图区域内部RAM指针.
;输入:GXPOS(列变量0-127,对于串行接口,GXPOS只能为16的整数倍).
;     GYPOS(行变量0-63)
;输出:无.
;影响:R0,ACC.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
GLCDPOS:                               ;写入绘图区域内部RAM指针子程序
        PUSH    ACC                    ;R6:Y指针,R7:X指针
        MOV     A,#36H
        CALL    LCDWC                  ;扩充指令集
        MOV     A,GYPOS
        ANL     A,#1FH                 ;绘图区域Y地址范围0-31
        ORL     A,#80H
        CALL    LCDWC                  ;先送Y地址
        MOV     A,GYPOS
        ANL     A,#0E0H
        JZ      GLP_LAY
        MOV     A,GXPOS                ;X地址=GXPOS/16
        SWAP    A
        ANL     A,#07H
        ORL     A,#08H                 ;若GYPOS>=32,则X地址=GXPOS/16+08H
        JMP     GLP_LAX
GLP_LAY:
        MOV     A,GXPOS
        SWAP    A
        ANL     A,#07H
GLP_LAX:
        ORL     A,#80H
        CALL    LCDWC                  ;再送X地址
        MOV     A,#30H
        CALL    LCDWC                  ;恢复为基本指令集
        POP     ACC
        RET
;-------------------------------------------------------------------------------
;子程序名称:LCDON()
;功能:开启LCD显示
;输入:无.
;输出:无.
;影响:R0,ACC.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
LCDON:                                 ;显示开启子程序
        MOV     A,#30H                 ;设置为基本指令集
        CALL    LCDWC
        MOV     A,#0CH                 ;
        CALL    LCDWC
        RET
;-------------------------------------------------------------------------------
;子程序名称:LCDOFF()
;功能:关闭LCD显示
;输入:无.
;输出:无.
;影响:R0,ACC.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
LCDOFF:                                ;显示关闭子程序
        MOV     A,#30H                 ;设置为基本指令集
        CALL    LCDWC
        MOV     A,#08H                 ;
        CALL    LCDWC
        RET
;-------------------------------------------------------------------------------
;子程序名称:LCDGRAPHON()
;功能:开启绘图区域显示
;输入:无.
;输出:无.
;影响:R0,ACC.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
LCDGRAPHON:                            ;绘图区域显示开启子程序
        MOV     A,#36H
        CALL    LCDWC
        MOV     A,#30H
        CALL    LCDWC                  ;恢复为基本指令集
        RET
;-------------------------------------------------------------------------------
;子程序名称:LCDGRAPHOFF()
;功能:关闭绘图区域显示
;输入:无.
;输出:无.
;影响:R0,ACC.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
LCDGRAPHOFF:                           ;绘图区域显示关闭子程序
        MOV     A,#34H
        CALL    LCDWC
        MOV     A,#30H
        CALL    LCDWC                  ;恢复为基本指令集
        RET
;-------------------------------------------------------------------------------
;以下LCDWC,LCDWD为ST7920兼容芯片的MCS51三线式串行接口方式的基本子程序,TRANSBYTE,
;TRANSBIT为内部子程序.
;-------------------------------------------------------------------------------
;子程序名称:LCDWC(A).
;功能:送控制字到液晶显示控制器.
;输入:A.
;输出:无.
;影响:R0.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
LCDWC:                                  ;送控制字子程序
        PUSH    ACC
	PUSH	B
	MOV	B,A
	SETB    CSPIN                  ;片选使能
	MOV	A,#0F8H                ;SYNCODE=0F8H,RW=0,RS=0,D0=0
	CALL	TRANSBYTE
	MOV	A,B
	ANL	A,#0F0H
	CALL	TRANSBYTE              ;D7D6D5D4 0000
	MOV	A,B
	SWAP	A
	ANL	A,#0F0H
	CALL	TRANSBYTE              ;D3D2D1D0 0000
	CLR	CSPIN                  ;片选禁止
	POP	B
        POP     ACC
        RET
;-------------------------------------------------------------------------------
;子程序名称:LCDWD(A).
;功能:送数据到液晶显示控制器.
;输入:A.
;输出:无.
;影响:R0.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
LCDWD:                                  ;送数据子程序
        PUSH    ACC
	PUSH	B
	MOV	B,A
	SETB    CSPIN                   ;片选使能
	MOV	A,#0FAH                 ;SYNCODE=0FAH,RW=0,RS=1,D0=0
	CALL	TRANSBYTE
	MOV	A,B
	ANL	A,#0F0H
	CALL	TRANSBYTE               ;D7D6D5D4 0000
	MOV	A,B
	SWAP	A
	ANL	A,#0F0H
	CALL	TRANSBYTE               ;D3D2D1D0 0000
	CLR	CSPIN                   ;片选禁止
	POP	B
        POP     ACC
        RET
;-------------------------------------------------------------------------------
;子程序名称:TRANSBYTE().
;功能:送1字节数据到液晶显示控制器.
;输入:ACC.
;输出:无.
;影响:R0.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
TRANSBYTE:                              ;送1字节数据到液晶显示控制器子程序
        MOV     R0,#8
TSB_PA: RLC     A
        CALL    TRANSBIT
        DJNZ    R0,TSB_PA
        RLC     A
        RET
;-------------------------------------------------------------------------------
;子程序名称:TRANSBIT().
;功能:送1位数据到液晶显示控制器.
;输入:C.
;输出:无.
;影响:无.
;修改日期:2009.10.28
;修改人:chujianjun@sunman.cn,tanchao@sunman.cn
;-------------------------------------------------------------------------------
TRANSBIT:                               ;送1位数据到液晶显示控制器子程序
        MOV     STDPIN,C                ;先送数据到数据口线DI
        NOP
        SETB    SCLKPIN                 ;再使时钟口线发一个负脉冲
        NOP
        CLR     SCLKPIN
        NOP
        SETB    SCLKPIN
        RET

