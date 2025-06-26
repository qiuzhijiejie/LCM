 extern void Write_One_Byte(char addr,char thedata);
 extern char Read_One_Byte(char addr);


void DELAY2(unsigned int t);
void I2C_Start(void);
void I2C_Stop(void);
void SEND_0(void); /* SEND ACK */
void SEND_1(void);
bit Check_Acknowledge(void);
void WriteI2CByte(char b)reentrant;
char ReadI2CByte(void)reentrant;