#include <stdlib.h>
#include <malloc.h>

#include "discord_rpc.h"
#include "FlashRuntimeExtensions.h"

FREContext dllContext;

const char* joinGameQueue[15]; //Malloc wtf
int joinGameQueueIndex = 0;

const DiscordUser* joinRequestQueue[15]; //Malloc wtf
int joinRequestQueueIndex = 0;

void joinGame(const char* secret)
{
	return; //Stfu

	if (joinGameQueueIndex == 15)
	{
		return;
	}

	joinGameQueue[joinGameQueueIndex++] = secret;

	FREDispatchStatusEventAsync(dllContext, (const uint8_t*) "joinGame", (const uint8_t*) "INFO");
}

void joinRequest(const DiscordUser* request)
{
	return; //Stfu

	if (joinRequestQueueIndex == 15)
	{
		return;
	}

	joinRequestQueue[joinRequestQueueIndex++] = request;

	FREDispatchStatusEventAsync(dllContext, (const uint8_t*) "joinRequest", (const uint8_t*) "INFO");
}

FREObject init(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
	uint32_t length;
	const uint8_t* applicationId;

	FREGetObjectAsUTF8(argv[0], &length, &applicationId);

	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	handlers.joinGame = &joinGame;
	handlers.joinRequest = &joinRequest;

	Discord_Initialize((const char*)applicationId, &handlers, 1, "");

	return NULL;
}

FREObject updatePresence(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
	uint32_t length;
	const uint8_t* state;
	const uint8_t* details;

	uint32_t startTimestamp;
	uint32_t endTimestamp;

	const uint8_t* largeImageKey;
	const uint8_t* largeImageText;

	const uint8_t* smallImageKey;
	const uint8_t* smallImageText;

	const uint8_t* partyId;
	int32_t partSize;
	int32_t partyMax;

	const uint8_t* joinSecret;
	const uint8_t* specSecret;

	FREGetObjectAsUTF8(argv[0], &length, &state);
	FREGetObjectAsUTF8(argv[1], &length, &details);

	FREGetObjectAsUint32(argv[2], &startTimestamp);
	FREGetObjectAsUint32(argv[3], &endTimestamp);

	FREGetObjectAsUTF8(argv[4], &length, &largeImageKey);
	FREGetObjectAsUTF8(argv[5], &length, &largeImageText);

	FREGetObjectAsUTF8(argv[6], &length, &smallImageKey);
	FREGetObjectAsUTF8(argv[7], &length, &smallImageText);

	FREGetObjectAsUTF8(argv[8], &length, &partyId);
	FREGetObjectAsInt32(argv[9], &partSize);
	FREGetObjectAsInt32(argv[10], &partyMax);

	FREGetObjectAsUTF8(argv[11], &length, &joinSecret);
	FREGetObjectAsUTF8(argv[12], &length, &specSecret);

	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = (char*)state;
	discordPresence.details = (char*)details;

	discordPresence.startTimestamp = startTimestamp;
	discordPresence.endTimestamp = endTimestamp;

	discordPresence.largeImageKey = (char*)largeImageKey;
	discordPresence.largeImageText = (char*)largeImageText;

	discordPresence.smallImageKey = (char*)smallImageKey;
	discordPresence.smallImageText = (char*)smallImageText;

	discordPresence.partyId = (char*)partyId;
	discordPresence.partySize = partSize;
	discordPresence.partyMax = partyMax;

	discordPresence.joinSecret = (char*)joinSecret;
	discordPresence.spectateSecret = (char*)specSecret;
	Discord_UpdatePresence(&discordPresence);

	return NULL;
}

FREObject runCallbacks(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
	Discord_RunCallbacks();

	return NULL;
}

FREObject respond(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
	uint32_t length;
	const uint8_t* userId;

	int32_t reply;

	FREGetObjectAsUTF8(argv[0], &length, &userId);
	FREGetObjectAsInt32(argv[1], &reply);

	Discord_Respond((char*)userId, reply);

	return NULL;
}

FREObject popJoinSecret(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
	FREObject result;

	if (joinGameQueueIndex > 0)
	{
		const char* secret = joinGameQueue[0];

		for (int i = 1; i < joinGameQueueIndex; i++)
		{
			joinGameQueue[i - 1] = joinGameQueue[i];
		}

		joinGameQueueIndex--;

		FRENewObjectFromUTF8(strlen(secret) + 1, (const uint8_t*)secret, &result);
	}
	else
	{
		result = NULL;
	}

	return result;
}

FREObject popJoinRequest(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
	FREObject result;

	if (joinRequestQueueIndex > 0)
	{
		const DiscordUser* user = joinRequestQueue[0];

		for (int i = 1; i < joinRequestQueueIndex; i++)
		{
			joinRequestQueue[i - 1] = joinRequestQueue[i];
		}

		joinRequestQueueIndex--;

		FRENewObjectFromUTF8(strlen(user->userId) + 1, (const uint8_t*)user->userId, &result);
	}
	else
	{
		result = NULL;
	}

	return result;
}

void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctionsToSet, const FRENamedFunction** functionsToSet)
{
	dllContext = ctx;

	// Create mapping between function names and pointers in an array of FRENamedFunction.
	// These are the functions that you will call from ActionScript -
	// effectively the interface of your native library.
	// Each member of the array contains the following information:
	// { function name as it will be called from ActionScript,
	//   any data that should be passed to the function,
	//   a pointer to the implementation of the function in the native library }
	static FRENamedFunction extensionFunctions[] =
	{
		//Publish use
		{ (const uint8_t*) "init", NULL, &init},
		{ (const uint8_t*) "updatePresence", NULL, &updatePresence},
		{ (const uint8_t*) "runCallbacks", NULL, &runCallbacks},
		{ (const uint8_t*) "respond", NULL, &respond},

		//Internal use
		{ (const uint8_t*) "popJoinSecret", NULL, &popJoinSecret},
		{ (const uint8_t*) "popJoinRequest", NULL, &popJoinRequest},
	};

	// Tell AIR how many functions there are in the array:
	*numFunctionsToSet = sizeof(extensionFunctions) / sizeof(FRENamedFunction);

	// Set the output parameter to point to the array we filled in:
	*functionsToSet = extensionFunctions;
}

void contextFinalizer(FREContext ctx)
{
	Discord_Shutdown();
}

__declspec(dllexport) void ExtensionInitializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer)
{
	*ctxInitializer = &contextInitializer; // The name of function that will intialize the extension context
	*ctxFinalizer = &contextFinalizer; // The name of function that will finalize the extension context
}

__declspec(dllexport) void ExtensionFinalizer(void* extData)
{
}