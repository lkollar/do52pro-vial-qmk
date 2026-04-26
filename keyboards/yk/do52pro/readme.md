# yk/do52pro

QMK target for the YK DO52 Pro split keyboard.

Upstream hardware / reference repo:
- https://github.com/ilfmoussa/do52pro

## Hardware summary

This keyboard target is configured for:
- RP2040 Pro Micro-compatible controllers
- split serial transport
- USB on the right half in normal use
- PS/2 trackpoint on the right half
- two 5-way switch clusters near the thumbs
- `EE_HANDS` handedness detection

## Layout summary

The board has:
- 56 main keys
- 2 thumb keys per half on the bottom row
- 2 integrated 5-way clusters, one per half

In QMK's split matrix model, the left half occupies rows `0..5` and the right
half occupies rows `6..11`.

### Main alpha block

```text
Left:                           Right:
`  1  2  3  4  5                6  7  8  9  0  BSPC
Tab Q  W  E  R  T                Y  U  I  O  P  \
Ctl A  S  D  F  G                H  J  K  L  ;  '
Sft Z  X  C  V  B                N  M  ,  .  /  Sft
Gui Alt Thumb Spc                Ent Thumb Alt Gui
```

The default and VIA keymaps currently use both thumb layer keys as:
- tap: `Esc`
- hold: `Fn`

That is implemented with `LT(_FN, KC_ESC)`.

### Current keymap diagram

![Current VIA keymap](../../../docs/keymaps/via_keymap.svg)

The SVG above is generated from the checked-in VIA keymap using
[keymap-drawer](https://github.com/caksoylar/keymap-drawer).

### 5-way clusters

The cluster mapping used by this target is:
- left cluster: `row 5` at columns `5,1,2,3,4`
- right cluster: `row 11` at columns `4,2,1,3,5`

In the default keymaps they are assigned as:
- left cluster: `Home`, `PgDn`, `Del`, `PgUp`, `End`
- right cluster: wheel left, wheel down, mouse button 1, wheel up, wheel right

## Important implementation details

### Handedness

This keyboard uses:

```c
#define EE_HANDS
```

For RP2040 halves, flash each side with the proper split target:

```sh
qmk flash -kb yk/do52pro -km via -bl uf2-split-left
qmk flash -kb yk/do52pro -km via -bl uf2-split-right
```

This writes handedness so Bootmagic and split orientation work correctly on both
halves.

### Bootmagic

Bootmagic is configured on both halves:
- left: `row 0 col 0` (top-left key)
- right: `row 6 col 5` (top-left key of the right half in split matrix space)

### Trackpoint

The right half hosts the PS/2 pointing device with these settings:
- `PS2_CLOCK_PIN GP23`
- `PS2_DATA_PIN GP22`
- `PS2_MOUSE_ROTATE 90`

## VIA

This target includes a VIA keymap and a draft definition JSON:
- `keyboards/yk/do52pro/keymaps/via/keymap.c`
- `keyboards/yk/do52pro/keymaps/via/keymap.json`
- `keyboards/yk/do52pro/do52pro_via.json`

USB identifiers used for VIA:
- VID: `0x1209`
- PID: `0xD052`

If the keyboard is not auto-detected in VIA, load `do52pro_via.json` manually in
usevia.app.

## Build

Default keymap:

```sh
qmk compile -kb yk/do52pro -km default
```

VIA keymap:

```sh
qmk compile -kb yk/do52pro -km via
```

## Generating the keymap diagram

From the QMK repo root:

```sh
mkdir -p docs/keymaps

qmk c2json keyboards/yk/do52pro/keymaps/via/keymap.c \
  -kb yk/do52pro \
  -km via \
  -o docs/keymaps/via_keymap.json

uvx --from keymap-drawer keymap parse \
  -q docs/keymaps/via_keymap.json \
  -l Base Fn \
  -o docs/keymaps/via_keymap.yaml

uvx --from keymap-drawer keymap draw \
  docs/keymaps/via_keymap.yaml \
  -j keyboards/yk/do52pro/keyboard.json \
  -l LAYOUT \
  -o docs/keymaps/via_keymap.svg
```

This requires:
- QMK CLI
- `uvx`
- `keymap-drawer` via `uvx --from keymap-drawer keymap ...`

## Notes

- The right thumb key immediately to the right of `Enter` is mapped at matrix
  position `10,4`.
- The split layout metadata has been corrected for the real 5-way cluster wiring.
- `keyboards/**/keymaps/via/*` is ignored by QMK's default `.gitignore`, so if
  you move this target between repos, remember to force-add those files.
