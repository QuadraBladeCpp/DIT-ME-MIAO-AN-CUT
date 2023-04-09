#pragma once
#include <Windows.h>//GetAsyncKeyState.
#include <chrono>//We will use chrono for the elapsed time.

//Discord SDK Stuff.
#include "DiscordSDK/include/discord_rpc.h"
#include "DiscordSDK/include/discord_register.h"
static bool gInit, gRPC = true;

void SetupDiscord()
{
	if (gRPC)
	{
		DiscordEventHandlers handlers;
		memset(&handlers, 0, sizeof(handlers));
		Discord_Initialize("1094246703714029588", &handlers, 1, "0");
	}
	else
	{
		Discord_Shutdown();
	}
}


static void UpdateDiscord()
{
	static int64_t StartTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	if (gRPC)
	{
		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.state = u8"SÚC VẬT 20MS BYPASS";
		discordPresence.details = u8"ĐỊT CON MẸ MÀY MIAO Ạ";
		std::time_t CurrentTime = std::time(0); discordPresence.startTimestamp = CurrentTime;    /*discordPresence.endTimestamp = 1507665886;*/
		discordPresence.largeImageKey = "ccmiao";
		discordPresence.largeImageText = "ccmiao";
		Discord_UpdatePresence(&discordPresence);
	}
	else
	{
		Discord_ClearPresence();
	}
}


	




	
