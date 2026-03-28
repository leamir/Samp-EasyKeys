# EasyKeys
Uma include para facilitar a detecção de teclas no SA-MP.
###### [English](./README.md) | Português

---

### Defines:
```pawn
#define KEY_HOLD_DETECTION_TIME 100
```
- Tempo em milisegundos para detectar se uma tecla é considerada sendo segurada (padrão: 100ms)

### Callbacks:
```pawn
forward OnPlayerKeyPressed(playerid, KEY:key, bool:isHeld)
```
- Chamado quando uma tecla nova é apertada (isHeld = false)
- Chamado quando a tecla está sendo segurada por KEY_HOLD_DETECTION_TIME milisegundos (isHeld = true)
- Se isHeld for true, o jogador apertou a tecla KEY_HOLD_DETECTION_TIME ms atrás. Se for false, a tecla acabou de ser solta pelo player (em menos de KEY_HOLD_DETECTION_TIME ms)

```pawn
forward OnPlayerKeyReleased(playerid, KEY:key, bool:isHeld)
```
- Chamado quando a tecla é solta.
- isHeld identifica se a tecla foi segurada (mesmo valor do callback OnPlayerKeyPressed)

### Funções:
```pawn
stock IsKeyBeingPressedByPlayer(playerid, KEY:key)
```
- Retorna 1 se a tecla especificada está sendo apertada pelo player, 0 se não estiver
- Como teclas são bit masks, é possível combinar multiplas teclas usando bitwise OR (|)

```pawn
stock KEY:GetPlayerPressedKeys(playerid)
```
- Returna as teclas sendo apertadas atualmente pelo player como um bit mask
