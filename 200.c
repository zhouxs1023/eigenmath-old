/* Code for computing an MD5 checksum. */

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21 \

#define F(x,y,z) (((x) &(y) ) |((~x) &(z) ) ) 
#define G(x,y,z) (((x) &(z) ) |((y) &(~z) ) ) 
#define H(x,y,z) ((x) ^(y) ^(z) ) 
#define I(x,y,z) ((y) ^((x) |(~z) ) )  \

#define ROTATE_LEFT(x,n) (((x) <<(n) ) |((x) >>(32-(n) ) ) )  \

#define FF(a,b,c,d,x,s,ac) { \
(a) += F((b) ,(c) ,(d) ) +(x) +(unsigned int) (ac) ; \
(a) = ROTATE_LEFT((a) ,(s) ) ; \
(a) += (b) ; \
} \

#define GG(a,b,c,d,x,s,ac) { \
(a) += G((b) ,(c) ,(d) ) +(x) +(unsigned int) (ac) ; \
(a) = ROTATE_LEFT((a) ,(s) ) ; \
(a) += (b) ; \
} \

#define HH(a,b,c,d,x,s,ac) { \
(a) += H((b) ,(c) ,(d) ) +(x) +(unsigned int) (ac) ; \
(a) = ROTATE_LEFT((a) ,(s) ) ; \
(a) += (b) ; \
} \

#define II(a,b,c,d,x,s,ac) { \
(a) += I((b) ,(c) ,(d) ) +(x) +(unsigned int) (ac) ; \
(a) = ROTATE_LEFT((a) ,(s) ) ; \
(a) += (b) ; \
} \

/*1:*/
#line 3 "md5.w"


char*s[14]= {
"",
"d41d8cd98f00b204e9800998ecf8427e",
"a",
"0cc175b9c0f1b6a831c399e269772661",
"abc",
"900150983cd24fb0d6963f7d28e17f72",
"message digest",
"f96b697d7cb7938d525a2f31aaf161d0",
"abcdefghijklmnopqrstuvwxyz",
"c3fcd3d76192e4007dfb496cca67e13b",
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
"d174ab98d277d9f5a5611c2c9f419d9f",
"12345678901234567890123456789012345678901234567890123456789012345678901234567890",
"57edf4a22be3c955ac49da2e2107b67a",
};

main()
{
int i;
for(i= 0;i<14;i+= 2)
chk(s[i],s[i+1]);
}

chk(s,expect)
char*s,*expect;
{
int i;
unsigned char digest[16];
char ans[100];
MD5Calc(digest,s,strlen(s));
for(i= 0;i<16;i++)
sprintf(ans+2*i,"%02x",digest[i]);
if(strcmp(ans,expect))
printf("fail\n");
}

/*:1*//*2:*/
#line 60 "md5.w"


MD5Calc(digest,buf,len)
unsigned char*digest,*buf;
int len;
{
int i,n;
unsigned int state[4];
unsigned char pad[64];
state[0]= 0x67452301;
state[1]= 0xefcdab89;
state[2]= 0x98badcfe;
state[3]= 0x10325476;
n= len/64;
for(i= 0;i<n;i++){
MD5Transform(state,buf);
buf+= 64;
}
memset(pad,0,64);
n= len%64;
if(n==0)
pad[0]= 0x80;
else{
memcpy(pad,buf,n);
pad[n]= 0x80;
if(n> 55){
MD5Transform(state,pad);
memset(pad,0,64);
}
}
len*= 8;
pad[56]= len;
pad[57]= len>>8;
pad[58]= len>>16;
pad[59]= len>>24;
MD5Transform(state,pad);
for(i= 0;i<4;i++){
digest[0]= state[i];
digest[1]= state[i]>>8;
digest[2]= state[i]>>16;
digest[3]= state[i]>>24;
digest+= 4;
}
}

/*:2*//*3:*/
#line 107 "md5.w"


MD5Transform(state,buf)
unsigned int*state;
unsigned char*buf;
{
int i;
unsigned int a,b,c,d,x[16];
for(i= 0;i<16;i++){
x[i]= (unsigned int)buf[0]
|(unsigned int)buf[1]<<8
|(unsigned int)buf[2]<<16
|(unsigned int)buf[3]<<24;
buf+= 4;
}
a= state[0];
b= state[1];
c= state[2];
d= state[3];
/*4:*/
#line 138 "md5.w"

FF(a,b,c,d,x[0],S11,0xd76aa478);
FF(d,a,b,c,x[1],S12,0xe8c7b756);
FF(c,d,a,b,x[2],S13,0x242070db);
FF(b,c,d,a,x[3],S14,0xc1bdceee);
FF(a,b,c,d,x[4],S11,0xf57c0faf);
FF(d,a,b,c,x[5],S12,0x4787c62a);
FF(c,d,a,b,x[6],S13,0xa8304613);
FF(b,c,d,a,x[7],S14,0xfd469501);
FF(a,b,c,d,x[8],S11,0x698098d8);
FF(d,a,b,c,x[9],S12,0x8b44f7af);
FF(c,d,a,b,x[10],S13,0xffff5bb1);
FF(b,c,d,a,x[11],S14,0x895cd7be);
FF(a,b,c,d,x[12],S11,0x6b901122);
FF(d,a,b,c,x[13],S12,0xfd987193);
FF(c,d,a,b,x[14],S13,0xa679438e);
FF(b,c,d,a,x[15],S14,0x49b40821);

/*:4*/
#line 126 "md5.w"
;
/*5:*/
#line 158 "md5.w"

GG(a,b,c,d,x[1],S21,0xf61e2562);
GG(d,a,b,c,x[6],S22,0xc040b340);
GG(c,d,a,b,x[11],S23,0x265e5a51);
GG(b,c,d,a,x[0],S24,0xe9b6c7aa);
GG(a,b,c,d,x[5],S21,0xd62f105d);
GG(d,a,b,c,x[10],S22,0x2441453);
GG(c,d,a,b,x[15],S23,0xd8a1e681);
GG(b,c,d,a,x[4],S24,0xe7d3fbc8);
GG(a,b,c,d,x[9],S21,0x21e1cde6);
GG(d,a,b,c,x[14],S22,0xc33707d6);
GG(c,d,a,b,x[3],S23,0xf4d50d87);
GG(b,c,d,a,x[8],S24,0x455a14ed);
GG(a,b,c,d,x[13],S21,0xa9e3e905);
GG(d,a,b,c,x[2],S22,0xfcefa3f8);
GG(c,d,a,b,x[7],S23,0x676f02d9);
GG(b,c,d,a,x[12],S24,0x8d2a4c8a);

/*:5*/
#line 127 "md5.w"
;
/*6:*/
#line 178 "md5.w"

HH(a,b,c,d,x[5],S31,0xfffa3942);
HH(d,a,b,c,x[8],S32,0x8771f681);
HH(c,d,a,b,x[11],S33,0x6d9d6122);
HH(b,c,d,a,x[14],S34,0xfde5380c);
HH(a,b,c,d,x[1],S31,0xa4beea44);
HH(d,a,b,c,x[4],S32,0x4bdecfa9);
HH(c,d,a,b,x[7],S33,0xf6bb4b60);
HH(b,c,d,a,x[10],S34,0xbebfbc70);
HH(a,b,c,d,x[13],S31,0x289b7ec6);
HH(d,a,b,c,x[0],S32,0xeaa127fa);
HH(c,d,a,b,x[3],S33,0xd4ef3085);
HH(b,c,d,a,x[6],S34,0x4881d05);
HH(a,b,c,d,x[9],S31,0xd9d4d039);
HH(d,a,b,c,x[12],S32,0xe6db99e5);
HH(c,d,a,b,x[15],S33,0x1fa27cf8);
HH(b,c,d,a,x[2],S34,0xc4ac5665);

/*:6*/
#line 128 "md5.w"
;
/*7:*/
#line 198 "md5.w"

II(a,b,c,d,x[0],S41,0xf4292244);
II(d,a,b,c,x[7],S42,0x432aff97);
II(c,d,a,b,x[14],S43,0xab9423a7);
II(b,c,d,a,x[5],S44,0xfc93a039);
II(a,b,c,d,x[12],S41,0x655b59c3);
II(d,a,b,c,x[3],S42,0x8f0ccc92);
II(c,d,a,b,x[10],S43,0xffeff47d);
II(b,c,d,a,x[1],S44,0x85845dd1);
II(a,b,c,d,x[8],S41,0x6fa87e4f);
II(d,a,b,c,x[15],S42,0xfe2ce6e0);
II(c,d,a,b,x[6],S43,0xa3014314);
II(b,c,d,a,x[13],S44,0x4e0811a1);
II(a,b,c,d,x[4],S41,0xf7537e82);
II(d,a,b,c,x[11],S42,0xbd3af235);
II(c,d,a,b,x[2],S43,0x2ad7d2bb);
II(b,c,d,a,x[9],S44,0xeb86d391);

/*:7*/
#line 129 "md5.w"
;
state[0]+= a;
state[1]+= b;
state[2]+= c;
state[3]+= d;
}

/*:3*/
