/* Copyright (c) 2013-2015 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef VIDEO_SOFTWARE_H
#define VIDEO_SOFTWARE_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba/core/core.h>
#include <mgba/gba/interface.h>
#include <mgba/internal/gba/io.h>
#include <mgba/internal/gba/video.h>

struct GBAVideoSoftwareSprite {
	struct GBAObj obj;
	int y;
	int endY;
};

struct GBAVideoSoftwareBackground {
	unsigned index;
	int enabled;
	unsigned priority;
	uint32_t charBase;
	int mosaic;
	int multipalette;
	uint32_t screenBase;
	int overflow;
	int size;
	int target1;
	int target2;
	uint16_t x;
	uint16_t y;
	int32_t refx;
	int32_t refy;
	int16_t dx;
	int16_t dmx;
	int16_t dy;
	int16_t dmy;
	int32_t sx;
	int32_t sy;
	int yCache;
	uint16_t mapCache[64];
	int32_t offsetX;
	int32_t offsetY;
};

enum {
#ifdef COLOR_16_BIT
#ifdef COLOR_5_6_5
	GBA_COLOR_WHITE = 0xFFDF,
#else
	GBA_COLOR_WHITE = 0x7FFF,
#endif
#else
	GBA_COLOR_WHITE = 0x00F8F8F8,
#endif
	OFFSET_PRIORITY = 30,
	OFFSET_INDEX = 28,
};

#define FLAG_PRIORITY       0xC0000000
#define FLAG_INDEX          0x30000000
#define FLAG_IS_BACKGROUND  0x08000000
#define FLAG_UNWRITTEN      0xFC000000
#define FLAG_REBLEND        0x04000000
#define FLAG_TARGET_1       0x02000000
#define FLAG_TARGET_2       0x01000000
#define FLAG_OBJWIN         0x01000000
#define FLAG_ORDER_MASK     0xF8000000

#define IS_WRITABLE(PIXEL) ((PIXEL) & 0xFE000000)

struct WindowRegion {
	uint8_t end;
	uint8_t start;
};

struct WindowControl {
	GBAWindowControl packed;
	int8_t priority;
};

#define MAX_WINDOW 5

struct Window {
	uint8_t endX;
	struct WindowControl control;
};

struct GBAVideoSoftwareRenderer {
	struct GBAVideoRenderer d;

	color_t* outputBuffer;
	int outputBufferStride;

	uint32_t* temporaryBuffer;

	GBARegisterDISPCNT dispcnt;

	uint32_t row[GBA_VIDEO_HORIZONTAL_PIXELS];
	uint32_t spriteLayer[GBA_VIDEO_HORIZONTAL_PIXELS];
	int32_t spriteCyclesRemaining;

	// BLDCNT
	unsigned target1Obj;
	unsigned target1Bd;
	unsigned target2Obj;
	unsigned target2Bd;
	bool blendDirty;
	enum GBAVideoBlendEffect blendEffect;
	color_t normalPalette[512];
	color_t variantPalette[512];

	uint16_t blda;
	uint16_t bldb;
	uint16_t bldy;

	GBAMosaicControl mosaic;

	struct WindowN {
		struct WindowRegion h;
		struct WindowRegion v;
		struct WindowControl control;
	} winN[2];

	struct WindowControl winout;
	struct WindowControl objwin;

	struct WindowControl currentWindow;

	int nWindows;
	struct Window windows[MAX_WINDOW];

	struct GBAVideoSoftwareBackground bg[4];

	int oamDirty;
	int oamMax;
	struct GBAVideoSoftwareSprite sprites[128];
	int16_t objOffsetX;
	int16_t objOffsetY;

	uint32_t scanlineDirty[5];
	uint16_t nextIo[REG_SOUND1CNT_LO];
	struct ScanlineCache {
		uint16_t io[REG_SOUND1CNT_LO];
		int32_t scale[2][2];
	} cache[GBA_VIDEO_VERTICAL_PIXELS];
	int nextY;

	int start;
	int end;
};

void GBAVideoSoftwareRendererCreate(struct GBAVideoSoftwareRenderer* renderer);

CXX_GUARD_START

#endif
