#include "raylib.h"
#include "BannerMessageSource.hpp"

const char* messages[] = {
    "I hate Unity!!!",
    "F*ck that sh*t!!!",
    "That s*cked!!!",
    "Something like that...",
    "My sleep schedule...",
    "I slept 14hrs wtf is going on",
    "Holy fk",
    "GIVEAWAY!",
    "Haus!!!",
    "Liscensce",
    "Hagen Studios",
    "ItsOIK",
    "Bomb Around",
    "LINIX!!!",
    "Bananen!!!",
    "Motivation Liquid"
};

const char *BannerMessageSource::GetRandom()
{
    int messagesLength = sizeof(messages) / sizeof(messages[0]);
    int i = GetRandomValue(0, messagesLength - 1);

    return messages[i];
}