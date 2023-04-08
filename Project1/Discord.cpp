#include "Discord.h"

void Discord::Initialize()
{
	DiscordEventHandlers Hanle;
	memset(&Hanle, 0, sizeof(Hanle));
	Discord_Initialize("1036246503498457148", &Hanle, 1, NULL);

}
void Discord::Update()
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Playing Solo";
    discordPresence.details = "Competitive";
    discordPresence.startTimestamp = 1507665886;
    discordPresence.endTimestamp = 1507665886;
    discordPresence.largeImageText = "WINDY BYPASS";
    Discord_UpdatePresence(&discordPresence);
}

