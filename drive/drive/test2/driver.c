#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/sched.h>
#include<asm/io.h>

#define P_UCON1    0x50004004
#define P_ULCON1   0x50004000
#define P_UBRDIV1  0x50004028
#define P_UTXH1    0x50004020
#define P_URXH1    0x50004024
#define P_UTRSTAT1 0X50004010
#define P_UFCON1   0x50004008

volatile unsigned int* V_UCON1;
volatile unsigned int* V_ULCON1;
volatile unsigned int* V_UBRDIV1;
volatile unsigned int* V_UTXH1;
volatile unsigned int* V_URXH1;
volatile unsigned int* V_UTRSTAT1;
volatile unsigned int* V_UFCON1;

MODULE_AUTHOR("Asmcos");
MODULE_DESCRIPTION("module example");
MODULE_LICENSE("GPL");

int n = 10;
char *p = "hello";
module_param(n, int, 0);
module_param(p, charp, 0655);

char s3c2440_serial_read(void)
{
        unsigned char a;
        while (!(*V_UTRSTAT1 & 1<<0)) 
                ;
         //读寄存器
        a = *V_URXH1; 
        if(a == '\r')
                printk("\n");
        else
                printk("%c", a);
        return a;
}

void s3c2440_serial_write(char ch)
{
     char a;
     while(!(*V_UTRSTAT1 & 1<<1))
                ;
        //读寄存器
         *V_UTXH1=ch;
         return ;
}

int __init
s3c2440_serial_init (void)
{
        V_UCON1   = ioremap(P_UCON1,   4);    
        V_ULCON1  = ioremap(P_ULCON1,  4);  
        V_UBRDIV1 = ioremap(P_UBRDIV1, 4);  
        V_UTXH1   = ioremap(P_UTXH1,   1);   
        V_URXH1   = ioremap(P_URXH1,   1);  
        V_UTRSTAT1= ioremap(P_UTRSTAT1,4);
        V_UFCON1  = ioremap(P_UFCON1,  4);  
        
        *V_UCON1   =   5;
        *V_ULCON1  =   3;
        *V_UBRDIV1 =  26;
        *V_UFCON1   =  0;

        while (1) {
                char ch = s3c2440_serial_read();
                s3c2440_serial_write(ch);
                if(ch == '\r')
                        s3c2440_serial_write('\n');
        }
        return 0;
}
void __exit
s3c2440_serial_exit (void)
{
        iounmap(V_UCON1   );    
        iounmap(V_ULCON1  );      //  = ioremap(P_ULCON1,  4);  
        iounmap(V_UBRDIV1 );     // = ioremap(P_UBRDIV1, 4);  
        iounmap(V_UTXH1   );     // = ioremap(P_UTXH1,   4);   
        iounmap(V_URXH1   );    //= ioremap(P_URXH1,   4);  
        iounmap(V_UTRSTAT1);    //= ioremap(P_UTRSTAT1,4);
        iounmap(V_UFCON1  );    //= ioremap(P_UFCON11  4);  
        printk("module exit\n");
        return ;
}

module_init(s3c2440_serial_init);
module_exit(s3c2440_serial_exit);
