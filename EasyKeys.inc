/*
Makes using keys easier

Defines:
KEY_HOLD_DETECTION_TIME
    - Time in milliseconds to detect if a key is considered held down (default: 100ms)

Callbacks:
forward OnPlayerKeyPressed(playerid, KEY:key, bool:isHeld)
    - Called when a new key is pressed (isHeld = false)
    - Called when a key is held down for KEY_HOLD_DETECTION_TIME milliseconds (isHeld = true)
    - If isHeld is true, player pressed the key KEY_HOLD_DETECTION_TIME ms ago. If false, the key was just released by the player (in under KEY_HOLD_DETECTION_TIME ms)

forward OnPlayerKeyReleased(playerid, KEY:key, bool:isHeld)
    - Called when a key is released
    - isHeld holds if the key was held down(same value as in OnPlayerKeyPressed callback)

Functions:
stock IsKeyBeingPressedByPlayer(playerid, KEY:key)
    - Returns 1 if the specified key is currently being pressed by the player, 0 otherwise
    - Since keys are bit masks, you can combine multiple using bitwise OR (|)

stock KEY:GetPlayerPressedKeys(playerid)
    - Returns the current keys being pressed by the player as a bit mask

By Leamir
*/

#if defined _inc_EasyKeys
    #undef _inc_EasyKeys
#endif

#if defined _EasyKeys_included
    #endinput
#endif
#define _EasyKeys_included

#if !defined KEY_HOLD_DETECTION_TIME
    #define KEY_HOLD_DETECTION_TIME 100 // Time in milliseconds to detect if a key is considered held down
#endif

static
        KEY:s_PlayerPressedKeys[MAX_PLAYERS],
        s_playerKeyPressTimers[MAX_PLAYERS][32]; // TODO: Check actual amount of detectable keys

forward _EK_OnPlayerKeyPressTimer(playerid, KEY:key, currentIndex);
public _EK_OnPlayerKeyPressTimer(playerid, KEY:key, currentIndex)
{
    s_playerKeyPressTimers[playerid][currentIndex] = -1;
    
    if (s_PlayerPressedKeys[playerid] & key)
    {
        if (funcidx("OnPlayerKeyPressed") >= 0)
            CallLocalFunction("OnPlayerKeyPressed", "iii", playerid, key, 1);
    }
    else // Prob never gonna be reached (timer is killed before), but just in case
    {
        if (funcidx("OnPlayerKeyPressed") >= 0)
            CallLocalFunction("OnPlayerKeyPressed", "iii", playerid, key, 0);
    }

    return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys) 
{
    new KEY:justPressedKeys = (newkeys & ~oldkeys);
    new KEY:justReleasedKeys = (~newkeys & oldkeys);
    new KEY:changedKeys = justPressedKeys | justReleasedKeys;

    new KEY:currentKey = KEY:1, currentIndex = 0;
    do
    {
        new bool:keyWasHeld = ((changedKeys & currentKey) && s_playerKeyPressTimers[playerid][currentIndex] == -1);
        if ((changedKeys & currentKey) && s_playerKeyPressTimers[playerid][currentIndex] != -1)
        {
            KillTimer(s_playerKeyPressTimers[playerid][currentIndex]);
            s_playerKeyPressTimers[playerid][currentIndex] = -1;

            if (funcidx("OnPlayerKeyPressed") >= 0)
                CallLocalFunction("OnPlayerKeyPressed", "iii", playerid, currentKey, 0);
        }
        if (justReleasedKeys & currentKey)
        {
            if (funcidx("OnPlayerKeyReleased") >= 0)
                CallLocalFunction("OnPlayerKeyReleased", "iii", playerid, currentKey, keyWasHeld);
        }
        else if (justPressedKeys & currentKey)
        {
            new timerId = SetTimerEx("_EK_OnPlayerKeyPressTimer", KEY_HOLD_DETECTION_TIME, false, "iii", playerid, currentKey, currentIndex);
            s_playerKeyPressTimers[playerid][currentIndex] = timerId;
        }

        currentKey = currentKey << KEY:1; // shift to the next key
        currentIndex++;
    }
    while (currentKey <= justPressedKeys || currentKey <= justReleasedKeys);

    s_PlayerPressedKeys[playerid] |= justPressedKeys; // add new keys to the variable
    s_PlayerPressedKeys[playerid] ^= justReleasedKeys; // remove released keys from the variable

    #if defined ALS_EK_OnPlayerKeyStateChange
        return ALS_EK_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange ALS_EK_OnPlayerKeyStateChange
#if defined ALS_EK_OnPlayerKeyStateChange
    forward ALS_EK_OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys);
#endif

public OnPlayerDisconnect(playerid, reason) 
{
    ClearUserKeyData(playerid);

    #if defined ALS_EK_OnPlayerDisconnect
        return ALS_EK_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect ALS_EK_OnPlayerDisconnect
#if defined ALS_EK_OnPlayerDisconnect
    forward ALS_EK_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerConnect(playerid) 
{
    ClearUserKeyData(playerid);

    #if defined ALS_EK_OnPlayerConnect
        return ALS_EK_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect ALS_EK_OnPlayerConnect
#if defined ALS_EK_OnPlayerConnect
    forward ALS_EK_OnPlayerConnect(playerid);
#endif

static ClearUserKeyData(playerid)
{
    s_PlayerPressedKeys[playerid] = KEY:0;

    for (new i; i < sizeof(s_playerKeyPressTimers[]); i++)
    {
        if (s_playerKeyPressTimers[playerid][i] != -1)
        {
            KillTimer(s_playerKeyPressTimers[playerid][i]);
            s_playerKeyPressTimers[playerid][i] = -1;
        }
    }

    return 1;
}

stock IsKeyBeingPressedByPlayer(playerid, KEY:key)
{
    return (s_PlayerPressedKeys[playerid] & key) == key;
}

stock KEY:GetPlayerPressedKeys(playerid)
{
    return s_PlayerPressedKeys[playerid];
}

forward OnPlayerKeyPressed(playerid, KEY:key, bool:isHeld);
forward OnPlayerKeyReleased(playerid, KEY:key, bool:isHeld);

#if defined EASY_KEYS_DEBUG_MODE
#define MAX_KEY_MAPPINGS 24
#define KEY_EMBEDDING_LENGTH 32

static stock KEY:keyValues[MAX_KEY_MAPPINGS] = {
    KEY_ACTION,
    KEY_CROUCH,
    KEY_FIRE,
    KEY_SPRINT,
    KEY_SECONDARY_ATTACK,
    KEY_JUMP,
    KEY_LOOK_RIGHT,
    KEY_HANDBRAKE,
    KEY_AIM,
    KEY_LOOK_LEFT,
    KEY_LOOK_BEHIND,
    KEY_SUBMISSION,
    KEY_WALK,
    KEY_ANALOG_UP,
    KEY_ANALOG_DOWN,
    KEY_ANALOG_LEFT,
    KEY_ANALOG_RIGHT,
    KEY_YES,
    KEY_NO,
    KEY_CTRL_BACK,
    KEY:KEY_UP,
    KEY:KEY_DOWN,
    KEY:KEY_LEFT,
    KEY:KEY_RIGHT
};

static stock keyFootCodes[MAX_KEY_MAPPINGS][KEY_EMBEDDING_LENGTH] = {
    "PED_ANSWER_PHONE",
    "PED_DUCK",
    "PED_FIREWEAPON",
    "PED_SPRINT",
    "VEHICLE_ENTER_EXIT",
    "PED_JUMP",
    "KEY_LOOK_RIGHT",
    "PED_LOCK_TARGET",
    "PED_LOCK_TARGET",
    "KEY_LOOK_LEFT",
    "PED_LOOKBEHIND",
    "KEY_SUBMISSION",
    "SNEAK_ABOUT",
    "KEY_ANALOG_UP",
    "KEY_ANALOG_DOWN",
    "VEHICLE_LOOKLEFT",
    "VEHICLE_LOOKRIGHT",
    "CONVERSATION_YES",
    "CONVERSATION_NO",
    "GROUP_CONTROL_BWD",
    "GO_FORWARD",
    "GO_BACKWARD",
    "GO_LEFT",
    "GO_RIGHT"
};

static _GetKeyIndex(KEY:key)
{
    for (new i = 0; i < MAX_KEY_MAPPINGS; i++)
    {
        if (keyValues[i] == key)
            return i;
    }
    return -1; // Not found
}

public OnPlayerKeyPressed(playerid, KEY:key, bool:isHeld)
{
    new keyIndex = _GetKeyIndex(key);
    if (keyIndex != -1)
    {
        SendClientMessage(playerid, -1, "Player %d %s key: %s (isHeld: %d)", playerid, isHeld ? "held" : "pressed", keyFootCodes[keyIndex], isHeld);
        printf("Player %d %s key: %s (isHeld: %d)", playerid, isHeld ? "held" : "pressed", keyFootCodes[keyIndex], isHeld);
    }
    else
    {
        SendClientMessage(playerid, -1, "Player %d %s unknown key: %d (isHeld: %d)", playerid, isHeld ? "held" : "pressed", key, isHeld);
        printf("Player %d %s unknown key: %d (isHeld: %d)", playerid, isHeld ? "held" : "pressed", key, isHeld);
    }
}
public OnPlayerKeyReleased(playerid, KEY:key, bool:isHeld)
{
    new keyIndex = _GetKeyIndex(key);
    if (keyIndex != -1)
    {
        SendClientMessage(playerid, -1, "Player %d released key: %s (isHeld: %d)", playerid, keyFootCodes[keyIndex], isHeld);
        printf("Player %d released key: %s (isHeld: %d)", playerid, keyFootCodes[keyIndex], isHeld);
    }
    else
    {
        SendClientMessage(playerid, -1, "Player %d released unknown key: %d (isHeld: %d)", playerid, key, isHeld);
        printf("Player %d released unknown key: %d (isHeld: %d)", playerid, key, isHeld);
    }
}
#warning EasyKeys debug mode is enabled. Remember to disable it before releasing your gamemode.
#endif