#include "ScreenTopGeneric.hpp"

void ScreenTopGeneric::render(SpritePrinter& spritePrinter)
{
	// Top bar

	C2D_DrawRectSolid(0.0f, 0.0f, 0.0f, TOP_SCREEN_WIDTH, FONT_HEIGHT_STD, C2D_Color32(COLOR_GRAY));

	// Date

	Date date;

	string dateText,
	hour = (date.getHour() < 10) ? "0" + to_string(date.getHour()) : to_string(date.getHour()),
	minute = (date.getMinute() < 10) ? "0" + to_string(date.getMinute()) : to_string(date.getMinute()),
	second = (date.getSecond() < 10) ? "0" + to_string(date.getSecond()) : to_string(date.getSecond());

	string de = " de ";
	if (date.getMonth() == 3 || date.getMonth() == 7 || date.getMonth() == 9)
		de = " d'";

	dateText =
		hour + ":" + minute + ":" + second +
		" - " + date.getWeekDayName() + " " + to_string(date.getDay()) + de + date.getMonthName() + " de " + to_string(date.getYear());

	TextPrinter::print(dateText.c_str(), C2D_AlignCenter, TOP_SCREEN_WIDTH / 2, 0, 1, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_YELLOW));

	
	// Battery

	u8 batteryLevel;
	bool adapter;
	MCUHWC_GetBatteryLevel(&batteryLevel);
	PTMU_GetAdapterState(&adapter);

	u32 batteryColor = C2D_Color32(COLOR_GREEN);

	if (batteryLevel < 25)
		batteryColor = C2D_Color32(COLOR_RED);
	else if (batteryLevel < 60)
		batteryColor = C2D_Color32(COLOR_BLUE);

	string batteryCharge;
	if (adapter)
		batteryCharge = CHAR_BUTTON_PLUS_WIIMOTE;
	else
		batteryCharge = " ";

	string battery = batteryCharge + " " + to_string(batteryLevel) + "%";
	TextPrinter::print(battery.c_str(), C2D_AlignRight, TOP_SCREEN_WIDTH - FONT_MARGIN - 0.1f, 0.0f, 1, FONT_SIZE_STD, FONT_SIZE_STD, batteryColor);

	
	// Wi-fi

	u8 wifiStrength = osGetWifiStrength();

	size_t wifi = 0;

	switch (wifiStrength)
	{
	case 0:
		wifi = SPRITE_WIFI0;
		break;

	case 1:
		wifi = SPRITE_WIFI1;
		break;

	case 2:
		wifi = SPRITE_WIFI2;
		break;

	case 3:
		wifi = SPRITE_WIFI3;
		break;
	}

	spritePrinter.print(wifi, FONT_MARGIN, 0.0f);

	
	// PCT Logo
	// On the center of the screen
	// Size: 150x150

	spritePrinter.print(SPRITE_PCT_LOGO_150, (TOP_SCREEN_WIDTH / 2) - 75, (TOP_SCREEN_HEIGHT / 2) - 75);


	// Bottom bar

	C2D_DrawRectSolid(0.0f, TOP_SCREEN_HEIGHT - FONT_HEIGHT_STD, 0.0f, TOP_SCREEN_WIDTH, FONT_HEIGHT_STD, C2D_Color32(COLOR_GRAY));

	
	// PCT

	TextPrinter::print(m_texts[SCREEN_TOP_GENERIC_TEXT_PCT].c_str(), C2D_AlignLeft, FONT_MARGIN, TOP_SCREEN_HEIGHT - FONT_HEIGHT_STD, 1, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_YELLOW));

	
	// Version

	TextPrinter::print(Version::getCurrentVersionData().getVersionWithNote().c_str(), C2D_AlignRight, TOP_SCREEN_WIDTH - FONT_MARGIN, TOP_SCREEN_HEIGHT - FONT_HEIGHT_STD, 1, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_YELLOW));
}
