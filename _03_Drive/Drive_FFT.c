/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_FFT.c
* By  			 : 
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : FFT算法
*********************************************************************************************************
*/
#include "math.h"
#include "Drive_FFT.h"

#define SWAP(a,b)  tempr=(a);(a)=(b);(b)=tempr

void conjugate_complex(u16 n, complex in[], complex out[])
{
    int i = 0;
    for(i = 0; i < n; i++)
    {
        out[i].imag = -in[i].imag;
        out[i].real = in[i].real;
    }
}

void c_abs(complex f[], float out[], u16 n)
{
    int i = 0;
    float t;
    for(i = 0; i < n; i++)
    {
        t = f[i].real * f[i].real + f[i].imag * f[i].imag;
        out[i] = sqrt(t);
    }
}


void c_plus(complex a, complex b, complex *c)
{
    c->real = a.real + b.real;
    c->imag = a.imag + b.imag;
}

void c_sub(complex a, complex b, complex *c)
{
    c->real = a.real - b.real;
    c->imag = a.imag - b.imag;
}

void c_mul(complex a, complex b, complex *c)
{
    c->real = a.real * b.real - a.imag * b.imag;
    c->imag = a.real * b.imag + a.imag * b.real;
}

void c_div(complex a, complex b, complex *c)
{
    c->real = (a.real * b.real + a.imag * b.imag) / (b.real * b.real + b.imag * b.imag);
    c->imag = (a.imag * b.real - a.real * b.imag) / (b.real * b.real + b.imag * b.imag);
}


void Wn_i(u16 n, u16 i, complex *Wn, char flag)
{
    Wn->real = cos(2 * PI * i / n);
    if(flag == 1)
        Wn->imag = -sin(2 * PI * i / n);
    else if(flag == 0)
        Wn->imag = -sin(2 * PI * i / n);
}

/*
*************************************************************************
*	功能	：	傅里叶变换
*	参数	：	1：转换长度
						2：FFT数组
**************************************************************************
*/

void fft(u16 N, complex f[])
{
    complex t, wn; //中间变量
		int i, j, k, m, n, l, r , M;
		
    int la, lb, lc;
    /*----计算分解的级数M=log2(N)----*/
    for(i = N, M = 1; (i = i / 2) != 1; M++)
		{
			
		}	
    /*----按照倒位序重新排列原信号----*/
    for(i = 1, j = N / 2; i <= N - 2; i++)
    {
        if(i < j)
        {
            t = f[j];
            f[j] = f[i];
            f[i] = t;
        }
        k = N / 2;
        while(k <= j)
        {
            j = j - k;
            k = k / 2;
        }
        j = j + k;
    }

    /*----FFT算法----*/
    for(m = 1; m <= M; m++)
    {
        la = pow(2, m); //la=2^m代表第m级每个分组所含节点数
        lb = la / 2; //lb代表第m级每个分组所含碟形单元数
        //同时它也表示每个碟形单元上下节点之间的距离
        /*----碟形运算----*/
        for(l = 1; l <= lb; l++)
        {
            r = (l - 1) * pow(2, M - m);
            for(n = l - 1; n < N - 1; n = n + la) //遍历每个分组，分组总数为N/la
            {
                lc = n + lb; //n,lc分别代表一个碟形单元的上、下节点编号
                Wn_i(N, r, &wn, 1); //wn=Wnr
                c_mul(f[lc], wn, &t); //t = f[lc] * wn复数运算
                c_sub(f[n], t, &(f[lc])); //f[lc] = f[n] - f[lc] * Wnr
                c_plus(f[n], t, &(f[n])); //f[n] = f[n] + f[lc] * Wnr
            }
        }
    }
}

//傅里叶逆变换
void ifft(u16 N, complex f[])
{
    int i = 0;
    conjugate_complex(N, f, f);
    fft(N, f);
    conjugate_complex(N, f, f);
    for(i = 0; i < N; i++)
    {
        f[i].imag = (f[i].imag) / N;
        f[i].real = (f[i].real) / N;
    }
}

/*
*************************************************************************
*	功能	：	把ADC数据转化为FFT数据
*	参数	：	1：ADC数据数组
						2：FFT存储数组
						3：数组长度
**************************************************************************
*/

void FFT_Exchange(u16 *Buff,u16 * retbuf,u16 Lenth)
{
	u16 i = 0; 
	complex ComplexBuff[ADC1_DMA2_LENTH];
	for(i = 0;i < Lenth;i ++)
	{
		ComplexBuff[i].imag = 0;
		ComplexBuff[i].real = Buff[i];  
	}
	
	fft(Lenth,ComplexBuff);
	
	for(i = 0;i < Lenth;i ++)
	{ 
		retbuf[i] = sqrt(ComplexBuff[i].imag*ComplexBuff[i].imag + ComplexBuff[i].real*ComplexBuff[i].real)/500;
	} 
}
/*
*************************************************************************
*	功能	：	从FFT后数据中提取出前七次谐波
*	参数	：	1：存放数组（偶次位存放幅度，奇数位存放频率）
						2：FFT存储数组
						3：数组长度
**************************************************************************
*/

void FFT_Harmonic(float *harm,u16 *retbuf,u16 lenth)
{
		u16 max,i,max_i,j=1;
		max=retbuf[1];
		max_i=1;
	
		for(i=2;i<lenth/2;i++)
		{
			if(max<retbuf[i])
			{
				max=retbuf[i];
				max_i=i;
			}		
		}	
		harm[0]=retbuf[0]/(float)lenth/2.0f;
		harm[1]=0;
		for(i=1;i<lenth/2;i++)
		{
			if(retbuf[i]>4500)
			{
				harm[2*j+1]=i;
				harm[2*j]=retbuf[i];
				j++;
			}	
		}
		
//		for(i=1;i<8;i++)
//		{
//			if(max_i*i<lenth/2)
//			{
//				harm[2*i]=retbuf[max_i*i]/(float)lenth*2.0f/2.0f;
//				if(retbuf[max_i*i]>1241)
//					harm[2*i+1]=max_i*i;
//				else 
//					harm[2*i+1]=0;
//			}
//			else
//			{
//				harm[2*i]=0;
//				if(retbuf[max_i*i]>1241)
//					harm[2*i+1]=max_i*i;
//				else 
//					harm[2*i+1]=0;
//			}
//		}
		
		
}

void FFT_Exchange_Measuring(void)
{
//	u16 i = 0; 
//	complex ComplexBuff[MEASURING_ADC_LENTH];
//	
//	for(i = 0;i < MEASURING_ADC_LENTH;i ++)
//	{
//		ComplexBuff[i].imag = 0.0f;
//		ComplexBuff[i].real = Measuring.FFT_Buff[i];   
//	}
//	
//	fft(MEASURING_ADC_LENTH,ComplexBuff);
//	
//	for(i = 0;i < MEASURING_ADC_LENTH;i ++)
//	{ 
//		Measuring.FFT_Buff[i] = sqrt(ComplexBuff[i].imag*ComplexBuff[i].imag + ComplexBuff[i].real*ComplexBuff[i].real);
//	} 
	 
}










