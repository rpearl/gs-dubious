#include <stdio.h>

FILE * fstream;
unsigned char fileversion;
unsigned int filetable;

unsigned int LOAD_INT(unsigned int pos)
{
     unsigned int num;
     unsigned char byte;
     fseek(fstream, pos&0x1FFFFFF, SEEK_SET);
     fread(&byte,1,1,fstream); num=byte;
     fread(&byte,1,1,fstream); num|=(byte<<8);
     fread(&byte,1,1,fstream); num|=(byte<<16);
     fread(&byte,1,1,fstream); num|=(byte<<24);
     return num;
}

unsigned char LOAD_BYTE(unsigned int pos)
{
     unsigned char num;
     fseek(fstream, pos&0x1FFFFFF, SEEK_SET);
     fread(&num, 1, 1, fstream);
     return num;
}

unsigned short LOAD_SHORT(unsigned int pos)
{
     unsigned short num;
     unsigned char byte;
     fseek(fstream, pos&0x1FFFFFF, SEEK_SET);
     fread(&byte,1,1,fstream); num=byte;
     fread(&byte,1,1,fstream); num|=(byte<<8);
     return num;
}

int loadFile(char* name)
{
     if (fstream!=NULL)
     {
          fclose(fstream);
     }
     fstream=fopen(name, "r+b");
     if (fstream!=NULL)
     {
          if (LOAD_INT(0x080000AB)==0x53474141)
          {
               unsigned char l;
               fileversion=0;
               switch (LOAD_BYTE(0x080000AF))
               {
                    case 69: case 73: filetable=0x08320000; break;
                    case 74: filetable=0x8317000; break;
                    case 68: filetable=0x831FE00; break;
                    case 70: case 83: filetable=0x8321800; break;
               }
          }
          else if (LOAD_INT(0x080000AB)==0x474D4241)
          {
               fileversion=10; filetable=0x088000000;
          }
          else if (LOAD_INT(0x080000AB)==0x4D544241)
          {
               fileversion=11; filetable=0x08C28000;
          }
          else
          {
               fileversion=1; filetable=0x08680000;
          }
          return 1;
     }
     else
          return -1;
}
void Initialize(unsigned int R[15])
{
     int C;
     if (fileversion==0)
     {
          R[12]=LOAD_INT(filetable+0x10);
          R[12]=LOAD_INT(R[12]+0x5CC);
     }
     else if (fileversion==1)
     {
          R[12]=LOAD_INT(0x08680010);
          R[12]=LOAD_INT(R[12]+0x5DC);
     }
     else if (fileversion==10)
     {
          R[12]=LOAD_INT(0x08800010);
          R[12]=LOAD_INT(R[12]+0x694);
     }
     else if (fileversion==11)
     {
          R[12]=LOAD_INT(0x08C28010);
          R[12]=LOAD_INT(R[12]+0x52C);
     }
     R[3]=R[1]>>8;
     R[12]+=R[3]<<3;
     R[2]=LOAD_INT(R[12]); R[4]=LOAD_INT(R[12]+4);
     R[1]=R[1]&0xFF;
     if (R[1]==0)
          goto _080155A0;
_08015588:
     R[3]=LOAD_BYTE(R[4]); R[4]++;
     R[2]+=R[3];
     if (R[3]==0xFF)
          goto _08015588;
     R[1]--;
     if (R[1]!=0)
          goto _08015588;
_080155A0:
     R[3]=1;
     R[4]=R[2]&3;
     if (R[4]==0)
          goto _080155C0;
     R[4]=(R[4]<<3)-R[3];
     R[2]=(R[2]|3)^3;
     R[3]=LOAD_INT(R[2]); R[2]+=4;
     R[3]=(R[3]>>1)|(1<<31);
     R[3]=R[3]>>R[4];
_080155C0:
     return;
}

unsigned short NextChar(unsigned int R[15])
{
     int C;
     if (fileversion==0)
     {
          R[12]=LOAD_INT(filetable+0x10);
          R[12]=LOAD_INT(R[12]+0x56C);
     }
     else if (fileversion==1)
     {
          R[12]=LOAD_INT(0x08680010);
          R[12]=LOAD_INT(R[12]+0x578);
     }
     else if (fileversion==10)
     {
          R[12]=LOAD_INT(0x08800010);
          R[12]=LOAD_INT(R[12]+0x628);
     }
     else if (fileversion==11)
     {
          R[12]=LOAD_INT(0x08C28010);
          R[12]=LOAD_INT(R[12]+0x4A4);
     }
     R[4]=R[1]>>8;
     R[12]+=R[4]<<3;
     R[4]=LOAD_INT(R[12]); R[5]=LOAD_INT(R[12]+4);
     R[12]=R[1]&0xFF;
     R[12]+=R[12];
     R[5]=LOAD_SHORT(R[5]+R[12]);
     R[4]+=R[5];
     R[5]=R[4];
     R[12]=1;
     R[6]=R[4]&3;
     if (R[6]==0)
          goto _08015480;
     R[6]=(R[6]<<3)-R[12];
     R[4]=(R[4]|3)^3;
     R[12]=LOAD_INT(R[4]); R[4]+=4;
     R[12]=(R[12]>>1)|(1<<31);
     R[12]=R[12]>>R[6];
     R[6]=0;
_08015480:
     C=(R[12]&1)==1; R[12]=R[12]>>1;
     if (R[12]==0)
     {
          R[12]=LOAD_INT(R[4]); R[4]+=4;
          C=(R[12]&1)==1; R[12]=(R[12]>>1)|(1<<31);
     }
     if (C)
          goto _08015538;
     C=(R[3]&1)==1; R[3]=R[3]>>1;
     if (!C)
          goto _08015480;
     if (R[3]==0)
     {
          R[3]=LOAD_INT(R[2]); R[2]+=4;
          C=(R[3]&1)==1; R[3]=(R[3]>>1)|(1<<31);
     }
     if (!C)
          goto _08015480;
     R[1]=0;
_080154A8:
     C=(R[12]&1)==1; R[12]=R[12]>>1;
     if (C)
          goto _08015510;
     C=(R[12]&1)==1; R[12]=R[12]>>1;
     if (C)
          goto _080154D4;
     C=(R[12]&1)==1; R[12]=R[12]>>1;
     if (C)
          goto _080154D0;
     C=(R[12]&1)==1; R[12]=R[12]>>1;
     if (C)
          goto _080154F0;
     R[1]+=4;
     goto _080154A8;
_080154D0:
     R[1]++;
_080154D4:
     if (R[12]!=0)
     {
          R[6]++;
          goto _080154A8;
     }
     R[12]=LOAD_INT(R[4]); R[4]+=4;
     C=(R[12]&1)==1; R[12]=(R[12]>>1)|(1<<31);
     if (!C)
          R[1]+=2;
     if (C)
          R[6]++;
     goto _080154A8;
_080154F0:
     R[1]+=2;
     goto _080154D4;
_08015510:
     if (R[12]==0)
          goto _08015524;
_08015514:
     R[6]++;
     R[1]--;
     if ((R[1]>>31)==0)
          goto _080154A8;
     goto _08015480;
_08015524:
     R[12]=LOAD_INT(R[4]); R[4]+=4;
     C=(R[12]&1)==1; R[12]=(R[12]>>1)|(1<<31);
     if (C)
          goto _08015514;
     R[1]++;
     goto _080154A8;
_08015538:
     C=(R[6]&1)==1; R[1]=R[6]>>1;
     R[6]+=R[1];
     R[6]=R[5]-R[6];
     R[5]=LOAD_BYTE(R[6]-1);
     R[6]=LOAD_BYTE(R[6]-2);
     if (C)
     {
          R[1]=R[5]&0xF;
          R[1]=R[6]|(R[1]<<8);
     }
     if (!C)
     {
          R[1]=R[5]<<4;
          R[1]=R[1]|(R[6]>>4);
     }
     R[0]=R[1];
     return R[0];
}



void decompressString(char str[520], unsigned int ind) {
     if (fstream==NULL) {
          return;
     }
     unsigned short c, n, l;
     unsigned int R[15];
     unsigned int i;
     for (i=0; i<15; i++)
     {
          R[i]=0;
     }
     R[1]=(unsigned int)(ind);
     Initialize(R);
     i=0; c=0;
     do
     {
          l=c;
          c=NextChar(R);
          str[i]=(unsigned char)c;
          i++;
     }
     while (R[0]!=0 && i<519 && ((c!=1 && c!=3) || (l>=17 && l<=20) || l==26 || l==29));
     str[i]=0;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: dubious <filename>");
		return 0;
	}

	loadFile(argv[1]);
	unsigned int num_strings;
	switch (fileversion) {
		case 0:
			num_strings = 10722; break;
		case 1:
			num_strings = 12461; break;
		case 10:
			num_strings = 4944; break;
		case 11:
			num_strings = 6263; break;
	}

	for (unsigned int i = 0; i < num_strings; i++) {
		char s[520];
		decompressString(s, i);
		printf("%s\n", s);
	}
}
