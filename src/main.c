#include <libheart.h>
#include "..\includes\defs.h"
#include "..\includes\soundbank.h"
s16 tmr, px, py, musici, seed, ex, ey, sx, sy, b, by,bananarot,frames,brotdir,anim,animcnt,score,lvs,rng,bs,speed,edir,launching,bx,desc,ed,bt,st,ded;
char *buf[256];
mm_sfxhand bgm2 = 0;

void tstsndcmp(int time) {
	mm_sound_effect bgm = {
		{ SFX_BGM },			// id
		(int)(1.0f * (1 << 10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};
	if (musici >= time) {
		bgm2 = mmEffectEx(&bgm);
		musici = 0;
	}
	else {
		musici++;
	}
}

int main()
{
	hrt_EnableSoftReset();
	hrt_Init(1);
	int s;
	musici = 900;
	mmInitDefault((mm_addr)soundbank_bin, 8);
	REG_SOUNDCNT_H = 0x330E;
	mm_sound_effect enedeath = {
		{ SFX_ENEDEATH },			// id
		(int)(1.0f * (1 << 10)),	// rate
		0,		// handle
		255,	// volume
		0,	// panning
	};
	mm_sfxhand enedeth = 0;
	seed = hrt_LoadInt(0);
	tmr = 130,px=88,py=96,bananarot=32,brotdir=-1;
	hrt_SetDSPMode(3, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0);
	hrt_LoadOBJPal((void*)sprsPalette, 255);
	hrt_LoadOBJGFX((void*)sprs, 14400);
	memcpy(VRAM, bg1Bitmap, 76800);
	hrt_CreateOBJ(1, 0, 0, 3, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0);
	hrt_AffineOBJ(1, 0, 384, 384);
	hrt_SetOBJXY(&sprites[1], -43, 75);
	hrt_CreateOBJ(2, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 128);
	hrt_SetOBJXY(&sprites[2], 88, 96);
	hrt_CreateOBJ(3, 0, 0, 3, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 384);
	hrt_AffineOBJ(3, 0, 384, 384);
	hrt_SetOBJXY(&sprites[3], -43, -26);
	ex = -43, ey = -26;
	hrt_CopyOAM();
	sprintf((char*)buf, "TIME:%d SCORE:%d ", tmr, score);
	hrt_PrintOnBitmap(0, 0, (char*)buf);
	srand(seed);
	seed = bs / 2;
	bs = (rand() % 2) + 1;
	hrt_SaveInt(0, seed);
	speed = (rand() % 5) + 1;
	while (tmr > 0)
	{
		tstsndcmp(900);
		bt = (hrt_CreateRNG() % 2) + 1;
		st = (hrt_CreateRNG() % 5) + 1;
		hrt_VblankIntrWait();
		hrt_CopyOAM();
		frames++;
		if (!(frames % 30))
		{
			tmr--;
			hrt_DMA_Copy(3, (u16*)bg1Bitmap, VRAM, 1920, 0x80000000); //DMA Copy
			sprintf((char*)buf, "TIME:%d SCORE:%d  ", tmr, score);
			hrt_PrintOnBitmap(0, 0, (char*)buf);
		}
		if (keyDown(KEY_LEFT))
		{
			sprites[2].attribute1 = 3*16384 | 1*4096 | 96;
			px-=3;
			hrt_SetOBJXY(&sprites[2], px, 96);
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
			sprites[2].attribute1 = 3*16384 | 96;
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
			hrt_SetOBJXY(&sprites[2], px, 96);
		}
		if (!(frames % 5))
		{
			hrt_AffineOBJ(1, bananarot+327, 384, 384);
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
				hrt_SetOBJXY(&sprites[1], -43, 75);
			}
			if (bs == 2)
			{
				hrt_SetOBJXY(&sprites[1], 145, 75);
			}
		}
		if ((b == 1)AND(launching==0))
		{
			hrt_SetOBJXY(&sprites[1], px - 32, 96 - 32);
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
				hrt_SetOBJXY(&sprites[1], bx, by);
				if (by < -4)
				{
					if (bx > ex - 32)
					{
						if (bx < ex + 32)
						{
							desc = 1;
							if (ded == 0)
							{
								enedeth = mmEffectEx(&enedeath);
							}
							ded = 1;
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
				hrt_CreateOBJ(3, 0, 0, 3, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 384);
				hrt_SetOBJXY(&sprites[3], ex, ey);
				if (NOT(ex < -46)) {
					ex -= speed;
				}
				else {
					ed = 0;
				}
			}
			if (ed == 0) {
				hrt_CreateOBJ(3, 0, 0, 3, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 384);
				hrt_SetOBJXY(&sprites[3], ex, ey);
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
				hrt_SetOBJXY(&sprites[3], ex, ey);
			}
			else {
				desc = 0;
				speed = st;
				score += 10;
				ex = -43;
				ded = 0;
			}
		}
		hrt_AffineOBJ(3, 0, 384, 384);
	}
	while (1)
	{
		hrt_SetDSPMode(3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);
		memcpy(VRAM, bg2Bitmap, 76800);
		hrt_SaveInt(0, score);
		while (KEY_ANY_PRESSED)
		{
			hrt_VblankIntrWait();
		}
		while (!(KEY_ANY_PRESSED))
		{
			hrt_VblankIntrWait();
		}
		asm("swi 0x26");
	}
	return 0;
}