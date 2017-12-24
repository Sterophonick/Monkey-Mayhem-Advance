#include <agb_lib.h>
#include <string.h>
#include "..\includes\defs.h"
int time, px, py, musici, seed, ex, ey, sx, sy, b, by,bananarot,frames,brotdir,anim,animcnt,score,lvs,rng,bs,speed,edir,launching,bx,desc,ed,bt,st;
char *buf[256];

void playSound(int s) {
	REG_SOUNDCNT1_H = 0x0B0F;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
	REG_SOUNDCNT1_X = 0x0080;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
	REG_SD1SAD = (unsigned long)sound[s].song;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
	REG_SD1DAD = 0x040000A0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
	REG_SD1CNT_H = 0xB640;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
	REG_TM0SD = 65536 - (16777216 / sound[s].frequency);                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
	REG_TMSDCNT = 0x00C0;
}

void tstsndcmp(int s, int time) {
	if (musici >= time) {
		REG_SOUNDCNT_H = 0;                                                      //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
		REG_SOUNDCNT_X = 0;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
		REG_DM1SAD = 0;                               //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
		REG_DM1DAD = 0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
		REG_DM1CNT_H = 0;                                                    //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
		REG_TM0D = 0;                         //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
		REG_TM0CNT = 0;
		playSound(s);
		musici = 0;
	}
	else {
		musici++;
	}
}

int main()
{
	int s;
	musici = 900;
	initsound8(1, 22050, 900, (void*)bgm);
	seed = LoadInt(0);
	time = 130,px=88,py=96,bananarot=32,brotdir=-1;
	SetMode(MODE_3 | BG2_ENABLE | OBJ_ENABLE | OBJ_MAP_1D);
	loadSpritePal((void*)sprsPalette);
	loadSpriteGraphics((void*)sprs, 14400);
	memcpy(videoBuffer, bg1Bitmap, 76800);
	initRotSprite(1, SIZE_64, 0);
	RotateSprite(1, 0, 384, 384);
	MoveSprite(&sprites[1], -43, 75);
	initSprite(2, SIZE_64, 128);
	MoveSprite(&sprites[2], 88, 96);
	initRotSprite(3, SIZE_64, 384);
	RotateSprite(3, 0, 384, 384);
	MoveSprite(&sprites[3], -43, -26);
	ex = -43, ey = -26;
	CopyOAM();
	sprintf((char*)buf, "TIME:%d SCORE:%d ", time, score);
	Print(0, 8, (char*)buf, WHITE, BLACK);
	srand(seed);
	seed = bs / 2;
	bs = (rand() % 2) + 1;
	SaveInt(0, seed);
	speed = (rand() % 5) + 1;
	while (time > 0)
	{
		tstsndcmp(1, sound[1].end);
		bt = (rand() % 2) + 1;
		st = (rand() % 5) + 1;
		sprintf((char*)buf, "TIME:%d SCORE:%d  ", time, score);
		Print(0, 8, (char*)buf, WHITE, BLACK);
		WaitForVblank();
		memcpy(videoBuffer, bg1Bitmap, 3840);
		CopyOAM();
		frames++;
		if (!(frames % 30))
		{
			time--;
		}
		if (keyDown(KEY_LEFT))
		{
			sprites[2].attribute1 = SIZE_64 | HORIZONTAL_FLIP | 96;
			px-=3;
			MoveSprite(&sprites[2], px, 96);
			if (px < -64)
			{
				px = 240;
			}
			animcnt++;
			if (!(animcnt % 5))
			{
				anim++;
				if (anim == 2)
				{
					anim = 0;
				}
			}
			if (anim == 0)
			{
				sprites[2].attribute2 = 512+128;
			}
			else if (anim == 1) {
				sprites[2].attribute2 = 512 + 256;
			}
		}
		if (keyDown(KEY_RIGHT))
		{
			sprites[2].attribute1 = SIZE_64 | 96;
			px+=3;
			if (px > 240)
			{
				px = -64;
			}
			animcnt++;
			if (!(animcnt % 5))
			{
				anim++;
				if (anim == 2)
				{
					anim = 0;
				}
			}
			if (anim == 0)
			{
				sprites[2].attribute2 = 512 + 128;
			}
			else if (anim == 1) {
				sprites[2].attribute2 = 512 + 256;
			}
			MoveSprite(&sprites[2], px, 96);
		}
		if (!(frames % 5))
		{
			RotateSprite(1, bananarot+327, 384, 384);
			if (brotdir == -1)
			{
				bananarot--;
			}
			else {
				bananarot++;
			}
			if (bananarot == 32)
			{
				brotdir = -1;
			}
				if (bananarot == 16)
				{
					brotdir = 1;
				}
		}
		if (px < 48)
		{
			if (bs == 1)
			{
				b = 1;
			}
		}
		if (px > 112)
		{
			if (bs == 2)
			{
				b = 1;
			}
		}
		if (b == 0)
		{
			if (bs == 1)
			{
				MoveSprite(&sprites[1], -43, 75);
			}
			if (bs == 2)
			{
				MoveSprite(&sprites[1], 145, 75);
			}
		}
		if ((b == 1)AND(launching==0))
		{
			MoveSprite(&sprites[1], px - 32, 96 - 32);
			if (keyDown(KEY_A))
			{
				bx = px - 32;
				by = 96 - 32;
				launching = 1;
			}
		}
		if (launching == 1)
		{
			if (!(by < -48))
			{
				by-=4;
				MoveSprite(&sprites[1], bx, by);
				if (by < -4)
				{
					if (bx > ex - 32)
					{
						if (bx < ex + 32)
						{
							desc = 1;
						}
					}
				}
			}
			else {
				launching = 0;
				bs = bt;
				b = 0;

			}
		}
		if (!(desc == 1))
		{
			ey = -26;
			if (ed == 1) {
				// set sprite offscreen, and set it up (size,etc)
				initRotSprite(3, SIZE_64, 384);
				MoveSprite(&sprites[3], ex, ey);
				if (NOT(ex < -46)) {
					ex -= speed;
				}
				else {
					ed = 0;
				}
			}
			if (ed == 0) {
				initRotSprite(3, SIZE_64, 384);
				MoveSprite(&sprites[3], ex, ey);
				if (NOT(ex > 160)) {
					ex += speed;
				}
				else {
					ed = 1;
				}
			}
		}
		else {
			if (!(ey > 160))
			{
				ey+=4;
				MoveSprite(&sprites[3], ex, ey);
			}
			else {
				desc = 0;
				speed = st;
				score += 10;
				ex = -43;
			}
		}
		RotateSprite(3, 0, 384, 384);
	}
	while (1)
	{
		SetMode(MODE_3 | BG2_ENABLE);
		memcpy(videoBuffer, bg2Bitmap, 76800);
		SaveInt(0, score);
		while (KEY_ANY_PRESSED);
		while (!(KEY_ANY_PRESSED));
		asm("swi 0x26");
	}
	return 0;
}