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
		Discord_Initialize("1036246503498457148", &handlers, 1, "0");
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
		discordPresence.state = "Playing PUBGM 2.4 | Gameloop";
		discordPresence.details = "WINDY BYPASS | https://discord.gg/ajXkw52f9W";
		std::time_t CurrentTime = std::time(0); discordPresence.startTimestamp = CurrentTime;    /*discordPresence.endTimestamp = 1507665886;*/
		discordPresence.largeImageKey = "tich-xanh ";
		discordPresence.largeImageKey = "fckingwindy";
		discordPresence.largeImageText = "WINDY BYPASS";
		discordPresence.smallImageText = "tich-xanh";
		discordPresence.partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
		discordPresence.partySize = 1;
		discordPresence.partyMax = 4;
		discordPresence.joinSecret = "MTI4NzM0OjFpMmhuZToxMjMxMjM= ";
		Discord_UpdatePresence(&discordPresence);
	}
	else
	{
		Discord_ClearPresence();
	}
}


	




	
