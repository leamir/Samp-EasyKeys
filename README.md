# EasyKeys
A include to facilitate the detection of keys in SA-MP.
###### English | **[Português](./README-pt-Br.md)**

---

### Defines:
```pawn
#define KEY_HOLD_DETECTION_TIME 100
```
- Time in milliseconds to detect if a key is considered held down (default: 100ms)

### Callbacks:
```pawn
forward OnPlayerKeyPressed(playerid, KEY:key, bool:isHeld)
```
- Called when a new key is pressed (isHeld = false)
- Called when a key is held down for KEY_HOLD_DETECTION_TIME milliseconds (isHeld = true)
- If isHeld is true, player pressed the key KEY_HOLD_DETECTION_TIME ms ago. If false, the key was just released by the player (in under KEY_HOLD_DETECTION_TIME ms)

```pawn
forward OnPlayerKeyReleased(playerid, KEY:key, bool:isHeld)
```
- Called when a key is released
- isHeld holds if the key was held down(same value as in OnPlayerKeyPressed callback)


### Functions:
```pawn
stock IsKeyBeingPressedByPlayer(playerid, KEY:key)
```
- Returns 1 if the specified key is currently being pressed by the player, 0 otherwise
- Since keys are bit masks, you can combine multiple using bitwise OR (|)

```pawn
stock KEY:GetPlayerPressedKeys(playerid)
```
- Returns the current keys being pressed by the player as a bit mask
