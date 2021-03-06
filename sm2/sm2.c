#include <stdio.h>
#include <stdlib.h>
#include<string.h>
//#include <memory.h>
#include <time.h>
#include "sm2.h"
#include "mtsm2.h"

#define SM2_PAD_ZERO TRUE
//#define SM2_PAD_ZERO FALSE


#define SM2_DEBUG   0
/*
#define QBITS 256
#define PBITS 3072
#define MAX_ECC_KEY_LEN		256
#define MAX_ECC_KEY_SIZE	(MAX_ECC_KEY_LEN/8)
*/

struct FPECC{
char *p;
char *a;
char *b;
char *n;
char *x;
char *y;
};


void PrintBuf(unsigned char *buf, int	buflen)
{
	int i;
	Debug_printf("\n");
	Debug_printf("len = %d\n", buflen);
	for(i=0; i<buflen; i++) {
  	if (i % 32 != 31)
  	  Debug_printf("%02x", buf[i]);
  	  else
  	  Debug_printf("%02x\n\r", buf[i]);
  }
  Debug_printf("\n\r");
  return;
}

void Printch(unsigned char *buf, int	buflen)
{
	int i;
	for (i = 0; i < buflen; i++) {
		if (i % 32 != 31)
			Debug_printf("%c", buf[i]);
		else
			Debug_printf("%c\n", buf[i]);
	}
	Debug_printf("\n");
	//return 0;
}
#if SM2_DEBUG

void PrintBig(big data)
{
 int len=0;
 unsigned char buf[10240];

 len=big_to_bytes(0,data,(char *)buf,0);
 PrintBuf(buf,len);
}

unsigned char radom[]  = {0x6C,0xB2,0x8D,0x99,0x38,0x5C,0x17,0x5C,0x94,0xF9,0x4E,0x93,0x48,0x17,0x66,0x3F,0xC1,0x76,0xD9,0x25,0xDD,0x72,0xB7,0x27,0x26,0x0D,0xBA,0xAE,0x1F,0xB2,0xF9,0x6F};
unsigned char radom1[] = {0x4B,0x62,0xEE,0xFD,0x6E,0xCF,0xC2,0xB9,0x5B,0x92,0xFD,0x6C,0x3D,0x95,0x75,0x14,0x8A,0xFA,0x17,0x42,0x55,0x46,0xD4,0x90,0x18,0xE5,0x38,0x8D,0x49,0xDD,0x7B,0x4F};
unsigned char randkey[] = {0x83,0xA2,0xC9,0xC8,0xB9,0x6E,0x5A,0xF7,0x0B,0xD4,0x80,0xB4,0x72,0x40,0x9A,0x9A,0x32,0x72,0x57,0xF1,0xEB,0xB7,0x3F,0x5B,0x07,0x33,0x54,0xB2,0x48,0x66,0x85,0x63};
unsigned char randkeyb[]= {0x33,0xFE,0x21,0x94,0x03,0x42,0x16,0x1C,0x55,0x61,0x9C,0x4A,0x0C,0x06,0x02,0x93,0xD5,0x43,0xC8,0x0A,0xF1,0x97,0x48,0xCE,0x17,0x6D,0x83,0x47,0x7D,0xE7,0x1C,0x80};

struct FPECC Ecc256={
"8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3",
"787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498",
"63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A",
"8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7",
"421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D",
"0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2",
};
unsigned char sm2_par_dig[128] = {
0x78,0x79,0x68,0xB4,0xFA,0x32,0xC3,0xFD,0x24,0x17,0x84,0x2E,0x73,0xBB,0xFE,0xFF,
0x2F,0x3C,0x84,0x8B,0x68,0x31,0xD7,0xE0,0xEC,0x65,0x22,0x8B,0x39,0x37,0xE4,0x98,
0x63,0xE4,0xC6,0xD3,0xB2,0x3B,0x0C,0x84,0x9C,0xF8,0x42,0x41,0x48,0x4B,0xFE,0x48,
0xF6,0x1D,0x59,0xA5,0xB1,0x6B,0xA0,0x6E,0x6E,0x12,0xD1,0xDA,0x27,0xC5,0x24,0x9A,
0x42,0x1D,0xEB,0xD6,0x1B,0x62,0xEA,0xB6,0x74,0x64,0x34,0xEB,0xC3,0xCC,0x31,0x5E,
0x32,0x22,0x0B,0x3B,0xAD,0xD5,0x0B,0xDC,0x4C,0x4E,0x6C,0x14,0x7F,0xED,0xD4,0x3D,
0x06,0x80,0x51,0x2B,0xCB,0xB4,0x2C,0x07,0xD4,0x73,0x49,0xD2,0x15,0x3B,0x70,0xC4,
0xE5,0xD7,0xFD,0xFC,0xBF,0xA3,0x6E,0xA1,0xA8,0x58,0x41,0xB9,0xE4,0x6E,0x09,0xA2,
};

#else
//unsigned char radom1[] = {0x4C,0x62,0xEE,0xFD,0x6E,0xCF,0xC2,0xB9,0x5B,0x92,0xFD,0x6C,0x3D,0x95,0x75,0x14,0x8A,0xFA,0x17,0x42,0x55,0x46,0xD4,0x90,0x18,0xE5,0x38,0x8D,0x49,0xDD,0x7B,0x4F};
unsigned char radom1[] = {0x4B,0x62,0xEE,0xFD,0x6E,0xCF,0xC2,0xB9,0x5B,0x92,0xFD,0x6C,0x3D,0x95,0x75,0x14,0x8A,0xFA,0x17,0x42,0x55,0x46,0xD4,0x90,0x18,0xE5,0x38,0x8D,0x49,0xDD,0x7B,0x4F};
	
/*SM2*/
struct FPECC Ecc256={
"FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF",
"FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC",
"28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93",
"FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123",
"32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7",
"BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0",
};

unsigned char sm2_par_dig[128] = {
0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,
0x28,0xE9,0xFA,0x9E,0x9D,0x9F,0x5E,0x34,0x4D,0x5A,0x9E,0x4B,0xCF,0x65,0x09,0xA7,
0xF3,0x97,0x89,0xF5,0x15,0xAB,0x8F,0x92,0xDD,0xBC,0xBD,0x41,0x4D,0x94,0x0E,0x93,
0x32,0xC4,0xAE,0x2C,0x1F,0x19,0x81,0x19,0x5F,0x99,0x04,0x46,0x6A,0x39,0xC9,0x94,
0x8F,0xE3,0x0B,0xBF,0xF2,0x66,0x0B,0xE1,0x71,0x5A,0x45,0x89,0x33,0x4C,0x74,0xC7,
0xBC,0x37,0x36,0xA2,0xF4,0xF6,0x77,0x9C,0x59,0xBD,0xCE,0xE3,0x6B,0x69,0x21,0x53,
0xD0,0xA9,0x87,0x7C,0xC6,0x2A,0x47,0x40,0x02,0xDF,0x32,0xE5,0x21,0x39,0xF0,0xA0,
};

unsigned char enkey[32] = {
0xB1,0x6B,0xA0,0xDA,0x27,0xC5,0x24,0x9A,0xF6,0x1D,0x6E,0x6E,0x12,0xD1,0x59,0xA5,
0xB6,0x74,0x64,0x34,0xEB,0xD6,0x1B,0x62,0xEA,0xEB,0xC3,0xCC,0x31,0x5E,0x42,0x1D,
};
#endif


#define SEED_CONST 0x1BD8C95A

int sm3_e(unsigned char *userid, int userid_len, unsigned char *xa, int xa_len, unsigned char *ya, int ya_len, unsigned char *msg, int msg_len, unsigned char *e)
{
/*
??????????????ID??????????????????????????????HASH??
[????] userid?? ????ID
[????] userid_len?? userid????????
[????] xa?? ??????X????
[????] xa_len: xa????????
[????] ya?? ??????Y????
[????] ya_len: ya????????
[????] msg??????????????
[????] msg_len?? msg????????
[????] e??32????????????????????

????????
		??1??????????
		  0??????
*/
	unsigned char *buf;
	int userid_bitlen;

	if((xa_len > 32) || (ya_len > 32))
		return -1;

	buf = malloc(2+userid_len+128+32+32);
	if(buf == NULL)
		return -1;

	userid_bitlen = userid_len << 3;
	buf[0] = (userid_bitlen >> 8) & 0xFF;
	buf[1] = userid_bitlen & 0xFF;

	memcpy(buf+2, userid, userid_len);
	memcpy(buf+2+userid_len, sm2_par_dig, 128);

	memset(buf+2+userid_len+128, 0, 64);
	memcpy(buf+2+userid_len+128+32-xa_len, xa, 32);
	memcpy(buf+2+userid_len+128+32+32-ya_len, ya, 32);

	sm3(buf, 2+userid_len+128+32+32, e);
	free(buf);

#if SM2_DEBUG
	Debug_printf("sm3_e: ");
	PrintBuf(e, 32);
#endif

	buf = malloc(msg_len+32);
	if(buf == NULL)
		return -1;

	memcpy(buf, e, 32);
	memcpy(buf+32, msg, msg_len);
	sm3(buf, 32+msg_len, e);

	free(buf);

	return 0;

}


void sm2_keygen(unsigned char *wx, int *wxlen, unsigned char *wy, int *wylen,unsigned char *privkey, int *privkeylen)
{

	struct FPECC *cfig = &Ecc256;
	epoint *g;
    big a,b,p,n,x,y,key1;
    miracl *mip = mirsys(20,0);
    mip->IOBASE = 16;
	char mem[MR_BIG_RESERVE(7)];
    memset(mem, 0, MR_BIG_RESERVE(7));
    p =mirvar_mem(mem, 0);
	a = mirvar_mem(mem, 1);
    b = mirvar_mem(mem, 2);
    n = mirvar_mem(mem, 3);
    x = mirvar_mem(mem, 4);
    y = mirvar_mem(mem, 5);
    key1 = mirvar_mem(mem, 6);
    cinstr(p,cfig->p);
	cinstr(a,cfig->a);
    cinstr(b,cfig->b);
	cinstr(n,cfig->n);
	cinstr(x,cfig->x);
    cinstr(y,cfig->y);
	ecurve_init(a,b,p,MR_PROJECTIVE);
   // g = epoint_init();
   // epoint_set(x,y,0,g); 
	 char mem1[MR_ECP_RESERVE(2)]; 
	 memset(mem1 ,0, MR_ECP_RESERVE(2));
	 g = epoint_init_mem(mem1,0);
	 epoint_set(x,y,0,g); 
    irand(1);
    bigrand(n,key1);   ////????db
    //bytes_to_big(32, (char *)radom1, key1);
    ecurve_mult(key1,g,g); //????Pb
    epoint_get(g,x,y);
    *wxlen = big_to_bytes(32, x, (char *)wx, TRUE);
   	*wylen = big_to_bytes(32, y, (char *)wy, TRUE);
	*privkeylen = big_to_bytes(32, key1, (char *)privkey, TRUE);
	//mirkill(key1);
	//mirkill(p);
	//mirkill(a);
	//mirkill(b);
	//mirkill(n);
	//mirkill(x);
	//mirkill(y);
	//epoint_free(g);
	mirexit();
}
int kdf(unsigned char *zl, unsigned char *zr, int klen, unsigned char *kbuf)
{
	/*
	return 0: kbuf = 0, ????
		   1: kbuf ??
	*/
	unsigned char buf[70];
	unsigned char digest[32];
	unsigned int ct = 0x00000001; //??????????32????????????????ct=0x00000001
	int i, m, n;
	unsigned char *p;
	memcpy(buf, zl, 32);
	memcpy(buf+32, zr, 32);
	m = klen / 32;
	n = klen % 32;
	p = kbuf;
	for(i = 0; i < m; i++)
	{
		buf[64] = (ct >> 24) & 0xFF;
		buf[65] = (ct >> 16) & 0xFF;
		buf[66] = (ct >> 8) & 0xFF;
		buf[67] = ct & 0xFF;
		sm3(buf, 68, p);
		p += 32;
		ct++;
	}
	/*??i??1???klen/v???????b.1)????Hai=Hv(Z ?? ct)??b.2) ct++*/
	if(n != 0)
	{
		buf[64] = (ct >> 24) & 0xFF;
		buf[65] = (ct >> 16) & 0xFF;
		buf[66] = (ct >> 8) & 0xFF;
		buf[67] = ct & 0xFF;
		sm3(buf, 68, digest);
	}
	/*??klen/v??????????Ha!?klen/v? = Ha?klen/v?????????Ha!?klen/v???Ha?klen/v?????????(klen ?
(v ?? ?klen/v?))????*/
	memcpy(p, digest, n);
	/*??K = Ha1||Ha2||  ||*/
	for(i = 0; i < klen; i++)
	{
		if(kbuf[i] != 0)
			break;
	}

	if(i < klen)
		return 1;
	else
		return 0;

}

int sm2_encrypt(unsigned char *msg,int msglen, unsigned char *wx,int wxlen, unsigned char *wy,int wylen, unsigned char *outmsg)
{

	struct FPECC *cfig = &Ecc256;
    big x2, y2, c1, c2, k;
    big a,b,p,n,x,y;
    epoint *g, *w;
	int ret = -1;
	int i;
	unsigned char zl[32], zr[32];
	unsigned char *tmp;
    miracl *mip;
	tmp = malloc(msglen+64);
	if(tmp == NULL){
		Debug_printf("error0\n");
		return -1;
	}
	mip = mirsys(20, 0);
	mip->IOBASE = 16;
	
	char mem[MR_BIG_RESERVE(10)];
    memset(mem, 0, MR_BIG_RESERVE(10));
	
    p= mirvar_mem(mem, 0);
	a=mirvar_mem(mem, 1);
    b=mirvar_mem(mem, 2);
    n=mirvar_mem(mem, 3);
    x=mirvar_mem(mem, 4);
    y=mirvar_mem(mem, 5);
	k=mirvar_mem(mem, 6);
	x2=mirvar_mem(mem, 7);
	y2=mirvar_mem(mem, 8);
	c1=mirvar_mem(mem, 9);
	c2=mirvar_mem(mem, 10);
    cinstr(p,cfig->p);
	cinstr(a,cfig->a);
    cinstr(b,cfig->b);
	cinstr(n,cfig->n);
	cinstr(x,cfig->x);
    cinstr(y,cfig->y);
	ecurve_init(a,b,p,MR_PROJECTIVE);
    //g=epoint_init();
	//w=epoint_init();
	 //char mem[MR_ECP_RESERVE(3)]; 
	 char mem1[MR_ECP_RESERVE(2)]; 
	 memset(mem1 ,0, MR_ECP_RESERVE(2));
	 g = epoint_init_mem(mem1,0);
	 w = epoint_init_mem(mem1,1);
    epoint_set(x,y,0,g);
	bytes_to_big(wxlen,(char *)wx,x);
	bytes_to_big(wylen,(char *)wy,y);
	epoint_set(x,y,0,w);
    //irand(time(NULL));
		irand(0);
sm2_encrypt_again:
#if SM2_DEBUG
	bytes_to_big(32, (char *)radom1, k);
#else
	do
	{
		bigrand(n, k);
	} 
	while (k->len == 0);
#endif
	ecurve_mult(k, g, g);
	epoint_get(g, c1, c2);
	big_to_bytes(32, c1, (char *)outmsg, TRUE);
	big_to_bytes(32, c2, (char *)outmsg+32, TRUE);
	//??????????????C1
	if(point_at_infinity(w)){
		Debug_printf("error1\n");
		goto exit_sm2_encrypt;
	}
	//??????????????S
	ecurve_mult(k, w, w);
	epoint_get(w, x2, y2);
	big_to_bytes(32, x2, (char *)zl, TRUE);
	big_to_bytes(32, y2, (char *)zr, TRUE);
	//??????????????[k]PB
	if (kdf(zl, zr, msglen, outmsg+64) == 0){
		Debug_printf("error2\n");
		goto sm2_encrypt_again;
	}
	//????t = KDF,????t????,????A1
	for(i = 0; i < msglen; i++)
	{
		outmsg[64+i] ^= msg[i];
	}
	//????C2
	memcpy(tmp, zl, 32);
	memcpy(tmp+32, msg, msglen);
	memcpy(tmp+32+msglen, zr, 32);
	sm3(tmp, 64+msglen, &outmsg[64+msglen]);
	//????C3
	ret = msglen+64+32;
	Debug_printf("key:");
	//cotnum(k, stdout);
	//????C,C??outmsg
exit_sm2_encrypt:
	//mirkill(x2);  
	//mirkill(y2);  
	//mirkill(c1);  
	//mirkill(c2);  
	//mirkill(k);
	//mirkill(a);  
	//mirkill(b);
    //mirkill(p);  
	//mirkill(n);  
	//mirkill(x);
	//mirkill(y);
    //epoint_free(g); 
	//epoint_free(w);
	mirexit();
	free(tmp);
	return ret;
}


int sm2_decrypt(unsigned char *msg,int msglen, unsigned char *privkey, int privkeylen, unsigned char *outmsg)
{

	struct FPECC *cfig = &Ecc256;
    big x2, y2, c, k;
    big a,b,p,n,x,y,key1;
    epoint *g;
	unsigned char c3[32];
	unsigned char zl[32], zr[32];
	int i, ret = -1;
	unsigned char *tmp;
    miracl *mip;
	if(msglen < 96)
		return 0;
	msglen -= 96;
	tmp = malloc(msglen+64);
	if(tmp == NULL)
		return 0;
	mip = mirsys(20, 0);
	mip->IOBASE = 16;

	char mem[MR_BIG_RESERVE(10)];
  memset(mem, 0, MR_BIG_RESERVE(10));

	x2=mirvar_mem(mem, 0);
	y2=mirvar_mem(mem, 1);
	c=mirvar_mem(mem, 2);
	k = mirvar_mem(mem, 3);
    p = mirvar_mem(mem, 4);
	a = mirvar_mem(mem, 5);
    b = mirvar_mem(mem, 6);
    n = mirvar_mem(mem, 7);
    x = mirvar_mem(mem, 8);
    y = mirvar_mem(mem, 9);
    key1 = mirvar_mem(mem, 10);
    bytes_to_big(privkeylen,(char *)privkey,key1);
    cinstr(p,cfig->p);
	cinstr(a,cfig->a);
    cinstr(b,cfig->b);
	cinstr(n,cfig->n);
	cinstr(x,cfig->x);
    cinstr(y,cfig->y);
	ecurve_init(a,b,p,MR_PROJECTIVE);
   // g = epoint_init();
	 char mem1[MR_ECP_RESERVE(1)]; 
	 memset(mem1 ,0, MR_ECP_RESERVE(1));
	 g = epoint_init_mem(mem1,0);
	bytes_to_big(32, (char *)msg, x);
	bytes_to_big(32, (char *)msg+32, y);
    if(!epoint_set(x,y,0,g))
		goto exit_sm2_decrypt;  //??????????????????
	if(point_at_infinity(g))
		goto exit_sm2_decrypt;  //????S
	ecurve_mult(key1, g, g);
	epoint_get(g, x2, y2);	
	big_to_bytes(32, x2, (char *)zl, TRUE);
	big_to_bytes(32, y2, (char *)zr, TRUE); //????[db]c1
	if (kdf(zl, zr, msglen, outmsg) == 0)
		goto exit_sm2_decrypt;    //????t
	for(i = 0; i < msglen; i++)
	{
		outmsg[i] ^= msg[i+64];
	}   //????M??outsmg
	memcpy(tmp, zl, 32);
	memcpy(tmp+32, outmsg, msglen);
	memcpy(tmp+32+msglen, zr, 32);
	sm3(tmp, 64+msglen, c3);//????u
	if(memcmp(c3, msg+64+msglen, 32) != 0)
		goto exit_sm2_decrypt;
	ret =  msglen;
exit_sm2_decrypt:
	//mirkill(x2);  
	//mirkill(y2);  
	//mirkill(c);  
	//mirkill(k);
	//mirkill(p);
	//mirkill(a); 
	//mirkill(b); 
	//mirkill(n); 
	//mirkill(x); 
	//mirkill(y); 
	//mirkill(key1);
    //epoint_free(g);
	mirexit();
	free(tmp);
	return ret;
}

int sm2_key_get_y(unsigned char *wx,int wxlen, unsigned char *wy,int wylen,int cb)
{
	/*
	??????????SM2????
	[????] wx??   ??????X????
	[????] wxlen: wx????????????????32????
	[????] wy??   ??????Y????
	[????] wylen: wy????????????????32????
	????????
	 -1??????????
	  0??????????
	*/

	struct FPECC *cfig = &Ecc256;
    big e, r,s,v;
    big a,b,p,n,x,y;
    epoint *g, *w;
//	int len;
	int ret = -1;
    miracl *mip=mirsys(20, 0);

	mip->IOBASE=16;
	char mem[MR_BIG_RESERVE(10)];
    memset(mem, 0, MR_BIG_RESERVE(10));
    //p=mirvar(0);
	p = mirvar_mem(mem, 0);
	a=mirvar_mem(mem, 1);
    b=mirvar_mem(mem, 2);
    n=mirvar_mem(mem, 3);
    x=mirvar_mem(mem, 4);
    y=mirvar_mem(mem, 5);

	e=mirvar_mem(mem, 6);
	r=mirvar_mem(mem, 7);
	s=mirvar_mem(mem, 8);
	v=mirvar_mem(mem, 9);

    cinstr(p,cfig->p);
	cinstr(a,cfig->a);
    cinstr(b,cfig->b);
	cinstr(n,cfig->n);
	cinstr(x,cfig->x);
    cinstr(y,cfig->y);

	ecurve_init(a,b,p,MR_PROJECTIVE);
	char mem1[MR_ECP_RESERVE(2)]; 
	memset(mem1 ,0, MR_ECP_RESERVE(2));
	g = epoint_init_mem(mem1,0);
	w = epoint_init_mem(mem1,1);
    //g=epoint_init();
	//w=epoint_init();
	
    epoint_set(x,y,0,g);


	bytes_to_big(wxlen,(char *)wx,x);
	//bytes_to_big(wylen,(char *)wy,y);
	if(!epoint_set(x,x,cb,w))
	{
		ret=-1;
		goto exit_sm2_verify;
	}
	epoint_get(w,x,y);
	big_to_bytes(32, y, (char *)wy, TRUE);
	ret=1;

exit_sm2_verify:

	//mirkill(r);
	//mirkill(s);
	//mirkill(v);
	//mirkill(e);
	//mirkill(a);
	//mirkill(b);
    //mirkill(p);
	//mirkill(n);
	//mirkill(x);
	//mirkill(y);
    //epoint_free(g);
	//epoint_free(w);
	mirexit();

	return ret;

}

int sm2_verify_mt(unsigned char *hash,int hashlen,unsigned char  *cr,int rlen,unsigned char *cs,int slen, unsigned char *wx,int wxlen, unsigned char *wy,int wylen)
{
/*
??????????SM2????
[????] hash??    sm3_e()??????
[????] hashlen?? hash??????????????32
[????] cr??  ??????????????????
[????] rlen??cr????????
[????] cs??  ????????????????????
[????] slen??cs????????
[????] wx??   ??????X????
[????] wxlen: wx????????????????32????
[????] wy??   ??????Y????
[????] wylen: wy????????????????32????

????????
	   -1??????????
		0??????????
*/
	Debug_printf("sm2_verify_mt enter:\n");
	struct FPECC *cfig = &Ecc256;
    big e, r,s,v;
    big a,b,p,n,x,y;
    epoint *g, *w;
	int ret = -1;
    miracl *mip=mirsys(20, 0);

	mip->IOBASE=16;
	char mem[MR_BIG_RESERVE(10)];
    memset(mem, 0, MR_BIG_RESERVE(10));
    //p=mirvar(0);
	p = mirvar_mem(mem, 0);
	a=mirvar_mem(mem, 1);
    b=mirvar_mem(mem, 2);
    n=mirvar_mem(mem, 3);
    x=mirvar_mem(mem, 4);
    y=mirvar_mem(mem, 5);

	e=mirvar_mem(mem, 6);
	r=mirvar_mem(mem, 7);
	s=mirvar_mem(mem, 8);
	v=mirvar_mem(mem, 9);

    cinstr(p,cfig->p);
	cinstr(a,cfig->a);
    cinstr(b,cfig->b);
	cinstr(n,cfig->n);
	cinstr(x,cfig->x);
    cinstr(y,cfig->y);

	ecurve_init(a,b,p,MR_PROJECTIVE);
	
    char mem1[MR_ECP_RESERVE(2)]; 
	memset(mem1 ,0, MR_ECP_RESERVE(2));
	g = epoint_init_mem(mem1,0);
	w = epoint_init_mem(mem1,1);
    //g=epoint_init();
	//w=epoint_init();
    epoint_set(x,y,0,g);
	bytes_to_big(wxlen,(char *)wx,x);
	bytes_to_big(wylen,(char *)wy,y);
	if(!epoint_set(x,y,0,w))
		goto exit_sm2_verify;

    bytes_to_big(hashlen,(char *)hash,e);
	bytes_to_big(rlen,(char *)cr,r);
    bytes_to_big(slen,(char *)cs,s);

	if ((compare(r, n) >= 0)  || (r->len == 0))
		goto exit_sm2_verify;

	if ((compare(s, n) >= 0) || (s->len == 0))
		goto exit_sm2_verify;


	add(s, r, a);
	divide(a, n, n);
	if (a->len == 0)
		goto exit_sm2_verify;

#if SM2_DEBUG
	PrintBig(a);
#endif

    ecurve_mult2(s, g, a, w, g);
    epoint_get(g, v, v);
#if SM2_DEBUG
	PrintBig(v);
#endif


	add(v, e, v);
	divide(v, n, n);
#if SM2_DEBUG
	PrintBig(v);
#endif

	if (compare(v, r) == 0)
		ret = 0;

exit_sm2_verify:

	//mirkill(r);
	//mirkill(s);
	//mirkill(v);
	//mirkill(e);
	//mirkill(a);
	//mirkill(b);
    //mirkill(p);
	//mirkill(n);
	//mirkill(x);
	//mirkill(y);
    //epoint_free(g);
	//epoint_free(w);
	mirexit();

	return ret;

}
/*
int main()
{
	Debug_printf("sm2 test....\n");
	
	
	unsigned char dB[] = { 0x16,0x49,0xAB,0x77,0xA0,0x06,0x37,0xBD,0x5E,0x2E,0xFE,0x28,0x3F,0xBF,0x35,0x35,0x34,0xAA,0x7F,0x7C,0xB8,0x94,0x63,0xF2,0x08,0xDD,0xBC,0x29,0x20,0xBB,0x0D,0xA0 };
	unsigned char xB[] = { 0x43,0x5B,0x39,0xCC,0xA8,0xF3,0xB5,0x08,0xC1,0x48,0x8A,0xFC,0x67,0xBE,0x49,0x1A,0x0F,0x7B,0xA0,0x7E,0x58,0x1A,0x0E,0x48,0x49,0xA5,0xCF,0x70,0x62,0x8A,0x7E,0x0A };
	unsigned char yB[] = { 0x75,0xDD,0xBA,0x78,0xF1,0x5F,0xEE,0xCB,0x4C,0x78,0x95,0xE2,0xC1,0xCD,0xF5,0xFE,0x01,0xDE,0xBB,0x2C,0xDB,0xAD,0xF4,0x53,0x99,0xCC,0xF7,0x7B,0xBA,0x07,0x6A,0x42 };
	unsigned char tx[256];
	unsigned char etx[256];
	unsigned char mtx[256];
	FILE *fp=0;
	int wxlen, wylen, privkeylen,len;
	//fopen(&fp, "5.txt", "r");
	//len=fread_s(tx, 256,sizeof(unsigned char), 256, fp);
	fp = fopen("5.txt","r");
	len=fread(tx,1,256,fp);
	tx[len] = 0;
	sm2_keygen(xB, &wxlen, yB, &wylen, dB, &privkeylen);
	Debug_printf("dB: ");
	PrintBuf(dB, 32);
	Debug_printf("xB: ");
	PrintBuf(xB, 32);
	Debug_printf("yB: ");
	PrintBuf(yB, 32);
	sm2_encrypt(tx,len,xB,32,yB,32,etx);
	Debug_printf("\n``````````````````this is encrypt```````````````````\n");
	PrintBuf(etx, 64 +len + 32);
	Debug_printf("\n``````````````````this is decrypt```````````````````\n");
	sm2_decrypt(etx,64+len+32,dB,32,mtx);
	if(sm2_decrypt(etx,64+len+32,dB,32,mtx) < 0)
		Debug_printf("sm2_decrypt error!\n");
	else
	{
		PrintBuf(mtx, len);
		Printch(mtx, len);
	}

	Debug_printf("\n``````````````````this is end```````````````````````\n");
	return 0;
}
*/


int sm2_test()
{
	Debug_printf("sm2 new test11....\n");
	
	
	unsigned char dB[] = { 0x16,0x49,0xAB,0x77,0xA0,0x06,0x37,0xBD,0x5E,0x2E,0xFE,0x28,0x3F,0xBF,0x35,0x35,0x34,0xAA,0x7F,0x7C,0xB8,0x94,0x63,0xF2,0x08,0xDD,0xBC,0x29,0x20,0xBB,0x0D,0xA0 };
	unsigned char xB[] = { 0x43,0x5B,0x39,0xCC,0xA8,0xF3,0xB5,0x08,0xC1,0x48,0x8A,0xFC,0x67,0xBE,0x49,0x1A,0x0F,0x7B,0xA0,0x7E,0x58,0x1A,0x0E,0x48,0x49,0xA5,0xCF,0x70,0x62,0x8A,0x7E,0x0A };
	unsigned char yB[] = { 0x75,0xDD,0xBA,0x78,0xF1,0x5F,0xEE,0xCB,0x4C,0x78,0x95,0xE2,0xC1,0xCD,0xF5,0xFE,0x01,0xDE,0xBB,0x2C,0xDB,0xAD,0xF4,0x53,0x99,0xCC,0xF7,0x7B,0xBA,0x07,0x6A,0x42 };
	unsigned char tx[5] = {1,2,3,4,5}; 
	//unsigned char tx[256] ;//[25] = {0x16,0x49,0xAB,0x77,0xA0,0x06,0x37,0xBD,0x5E,0x2E,0x16,0x49,0xAB,0x77,0xA0,0x06,0x37,0xBD,0x5E,0x2E};
	unsigned char etx[256];
	unsigned char mtx[256];
	//FILE *fp=0;
	int wxlen, wylen, privkeylen,len;
	//fopen(&fp, "5.txt", "r");
	//len=fread_s(tx, 256,sizeof(unsigned char), 256, fp);
	//fp = fopen("5.txt","r");
	//len=fread(tx,1,256,fp);
	len =5; 
	//memset(tx,0x31,len);
	
	//tx[len] = 0;   //????2022-04-22
		
		
	PrintBuf(tx, len);
	Debug_printf("dB: ");
	PrintBuf(dB, 32);
	Debug_printf("xB: ");
	PrintBuf(xB, 32);
	Debug_printf("yB: ");
	PrintBuf(yB, 32);	
	Debug_printf("radoml: ");
	PrintBuf(radom1, 32);	
	
	Debug_printf("Before sm2_keygen----------------------------\n\r");	
	sm2_keygen(xB, &wxlen, yB, &wylen, dB, &privkeylen);
	Debug_printf("dB: ");
	PrintBuf(dB, 32);
	Debug_printf("xB: ");
	PrintBuf(xB, 32);
	Debug_printf("yB: ");
	PrintBuf(yB, 32);
	sm2_encrypt(tx,len,xB,32,yB,32,etx);
	Debug_printf("\n``````````````````this is encrypt```````````````````\n");
	PrintBuf(etx, 64 +len + 32);
	Debug_printf("\n``````````````````this is decrypt```````````````````\n");
	sm2_decrypt(etx,64+len+32,dB,32,mtx);
	if(sm2_decrypt(etx,64+len+32,dB,32,mtx) < 0)
		Debug_printf("sm2_decrypt error!\n");
	else
	{
		PrintBuf(mtx, len);
		Printch(mtx, len);
	}
	
	Debug_printf("\n``````````````````this is end```````````````````````\n");
	
	unsigned char ca_publickey[] ={0x4a,0xb4,0x60,0x23,0xee,0x81,0x43,0xd0,0x89,0x0f,0x5b,0xe5,0x88,0x22,0x5e,0x17,0xcd,0x7e,0x19,0x88,0x64,0x74,0xa2,0x4a,0xa4,0xc3,0x3f,0x35,0x9c,0xb7,0xdb,0xe4,0x83,0xb4,0x72,0x95,0x48,0x4e,0x8d,0x3b,0xef,0x03,0x45,0x09,0x8b,0xd5,0x61,0x96,0xd4,0x75,0x0d,0xd4,0x40,0xf0,0x4d,0xbe,0x81,0xa0,0x9d,0x06,0x5c,0xa1,0x59,0xae};
	unsigned char ca_sign[]={0x5f,0xda,0x05,0x59,0x52,0xfe,0xdc,0xcf,0xd0,0x6d,0x46,0xad,0xe4,0xf2,0xbd,0x85,0x59,0x8c,0x12,0xba,0x7c,0xda,0x6c,0xbb,0xa9,0x5d,0x17,0xb9,0x4f,0xa1,0x72,0xdf,0x1f,0x54,0xef,0x50,0x04,0xe9,0x76,0xb4,0x34,0xb4,0x7f,0xf1,0x08,0xcc,0x91,0x22,0x0d,0xcb,0x9f,0x75,0x5c,0xbb,0xf5,0x87,0xa1,0x35,0x18,0xdf,0xc0,0x71,0xa9,0x6e};
	
	unsigned char ca_source[]={0x12,0x00,0x00,0x00,0x00,0x12,0x99,0x00,0x00,0x00,0x04,0x04,0x00,0x33,0x03,0xa0,0x80,0x98,0x56,0xe4,0xfd,0xd3,0x6d,0xba,0x5d,0x38,0x06,0x14,0xe0,0x69,0x84,0x48,0xfa,0x40,0x4d,0xc4,0x9e,0x2d,0xf3,0x70,0xb9,0x65,0x74,0xff,0xaf,0x39,0x0d};
	int ret = mt_sm2_verify(ca_publickey,sizeof(ca_publickey),ca_sign,sizeof(ca_sign),ca_source,sizeof(ca_source));
	if(ret == 1){
		 	Debug_printf("\n``````````````````this is success```````````````````````\n");
	}else
	{ 
		  Debug_printf("\n``````````````````this is failed```````````````````````,ret=%d\n",ret);
  }
	
	unsigned char ca_publickey1[] ={0x46,0xd2,0x68,0x06,0xfa,0xda,0x1d,0xc0,0x0a,0xb8,0xca,0x28,0x48,0x85,0x37,0x05,0x26,0x11,0x59,0xca,0xf5,0x5c,0xe8,0xc5,0x12,0x5b,0xc1,0x71,0x77,0x05,0x39,0x2a,0xca,0xd4,0xd4,0x65,0x84,0x09,0x97,0xdc,0xde,0xc2,0x58,0x02,0x5a,0xa2,0x71,0xfe,0xec,0xf4,0xda,0x60,0xf8,0x6a,0xc2,0x3d,0x96,0x20,0xe9,0x13,0x82,0x9f,0x0e,0x4c};
	unsigned char ca_sign1[]={0xbe,0x00,0x1b,0x1b,0xf6,0x0e,0x9e,0x02,0x1e,0xba,0x5e,0x7d,0xf5,0xa5,0x2f,0x43,0xe1,0x4f,0xce,0x66,0xd9,0xb2,0x55,0xa4,0x95,0x48,0xef,0xfb,0x3c,0x2e,0x18,0xca,0x42,0xd7,0xf2,0x0b,0xb4,0xb8,0x87,0xfb,0xb0,0x44,0x67,0x3b,0x50,0x3a,0x58,0xdc,0x1f,0x85,0x99,0x04,0x9d,0x51,0xe0,0xfc,0x99,0x5e,0x40,0x55,0xbe,0x58,0x15,0x6a};
	
	unsigned char ca_source1[]={0x12,0x03,0x60,0x42,0x70,0x01,0x31,0x14,0x45,0x01,0x04,0x04,0x00,0x21,0x02,0xe1,0xcb,0x8e,0xae,0x77,0xa1,0xb5,0x88,0xca,0xb9,0x1e,0x02,0x20,0xfd,0xa2,0x0b,0x30,0x95,0x9f,0xc9,0x30,0xc9,0x67,0xd1,0xba,0x10,0x61,0x41,0xf4,0x29,0xf2,0xb6};
	ret = mt_sm2_verify(ca_publickey1,sizeof(ca_publickey1),ca_sign1,sizeof(ca_sign1),ca_source1,sizeof(ca_source1));
	if(ret == 1){
		 	Debug_printf("\n``````````````````this is success```````````````````````\n");
	}else
	{ 
		  Debug_printf("\n``````````````````this is failed```````````````````````,ret=%d\n",ret);
  }
	
	
	return 0;
}

