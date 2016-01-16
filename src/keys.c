#include "keys.h"
#include "terminal.h"
#include "keycode.h"

#include <string.h>

static GPIO_InitTypeDef  GPIO_InitStruct;


typedef struct
{
    GPIO_TypeDef* port;
    uint16_t      pin;
    uint8_t       behavior;
    uint8_t       code;
    bool          state;
    uint8_t       count;
}keys_t;

typedef struct
{
    uint8_t       behavior;
    uint8_t       code;
    bool          state;
}sub_keys_t;

enum {
    KEY_INACTIVE,
    KEY_ACTIVE
};

enum {
    BEHAVIOR_MOD,
    BEHAVIOR_FN,
    BEHAVIOR_NORM,
};

enum {
    OUT_MOD,
    OUT_RES,
    OUT_KC1,
    OUT_KC2,
    OUT_KC3,
    OUT_KC4,
    OUT_KC5,
    OUT_KC6,
};

enum {
    MOD_LCTL,
    MOD_LSH,
    MOD_LALT,
    MOD_LGUI,
    MOD_RCTL,
    MOD_RSH,
    MOD_RALT,
    MOD_RGUI,
};

enum {
    KEY_A1,
    KEY_A2,
    KEY_A3,

    KEY_B1,
    KEY_B2,
    KEY_B3,
    KEY_B4,


    KEY_C1,
    KEY_C2,
    KEY_C3,
    KEY_C4,

    //D
    KEY_D1,
    KEY_D2,
    KEY_D3,
    KEY_D4,
    KEY_D5,

    //E
    KEY_E1,
    KEY_E2,
    KEY_E3,
    KEY_E4,
    KEY_E5,

    //F
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,

    //G
    KEY_G1,
    KEY_G2,
    KEY_G3,
    KEY_G4,
    KEY_G5,

    //H
    KEY_H1,
    KEY_H2,
    KEY_H3,
    KEY_H4,

    //I
    KEY_I1,

    //J
    KEY_J1,

    NUM_KEYS,
};

enum {
    KEY_A1S,
    KEY_A2S,
    KEY_A3S,
    KEY_B1S,
    KEY_B2S,
    KEY_B3S,
    KEY_B4S,
    KEY_C1S,
    KEY_C2S,
    KEY_C3S,
    KEY_C4S,
    KEY_D1S,
    KEY_D2S,
    KEY_D3S,
    KEY_D4S,
    KEY_D5S,
    KEY_E1S,
    KEY_E2S,
    KEY_E3S,
    KEY_E4S,
    KEY_E5S,
    KEY_F1S,
    KEY_F2S,
    KEY_F3S,
    KEY_F4S,
    KEY_F5S,
    KEY_G1S,
    KEY_G2S,
    KEY_G3S,
    KEY_G4S,
    KEY_G5S,
    KEY_H1S,
    KEY_H2S,
    KEY_H3S,
    KEY_H4S,
    KEY_I1S,
    KEY_J1S,

    NUM_SUB_KEYS,
};

keys_t keys[NUM_KEYS] = {
    [KEY_A1] = {GPIOD, GPIO_PIN_3,  BEHAVIOR_NORM, KC_EQUAL},
    [KEY_A2] = {GPIOD, GPIO_PIN_2,  BEHAVIOR_NORM, KC_RBRACKET},
    [KEY_A3] = {GPIOD, GPIO_PIN_15, BEHAVIOR_NORM, KC_ENTER},
    [KEY_B1] = {GPIOD, GPIO_PIN_14, BEHAVIOR_NORM, KC_MINUS},
    [KEY_B2] = {GPIOD, GPIO_PIN_13, BEHAVIOR_NORM, KC_LBRACKET},
    [KEY_B3] = {GPIOD, GPIO_PIN_12, BEHAVIOR_NORM, KC_QUOTE},
    [KEY_B4] = {GPIOD, GPIO_PIN_11, BEHAVIOR_NORM, KC_BSLASH},
    [KEY_C1] = {GPIOD, GPIO_PIN_10, BEHAVIOR_NORM, KC_0},
    [KEY_C2] = {GPIOD, GPIO_PIN_9,  BEHAVIOR_NORM, KC_P},
    [KEY_C3] = {GPIOD, GPIO_PIN_8,  BEHAVIOR_NORM, KC_SCOLON},
    [KEY_C4] = {GPIOE, GPIO_PIN_15, BEHAVIOR_NORM, KC_SLASH},
    [KEY_D1] = {GPIOE, GPIO_PIN_14, BEHAVIOR_NORM, KC_9},
    [KEY_D2] = {GPIOE, GPIO_PIN_13, BEHAVIOR_NORM, KC_O},
    [KEY_D3] = {GPIOE, GPIO_PIN_12, BEHAVIOR_NORM, KC_L},
    [KEY_D4] = {GPIOE, GPIO_PIN_11, BEHAVIOR_NORM, KC_DOT},
    [KEY_D5] = {GPIOE, GPIO_PIN_10, BEHAVIOR_NORM, KC_BSPACE},
    [KEY_E1] = {GPIOE, GPIO_PIN_9,  BEHAVIOR_NORM, KC_8},
    [KEY_E2] = {GPIOE, GPIO_PIN_8,  BEHAVIOR_NORM, KC_I},
    [KEY_E3] = {GPIOE, GPIO_PIN_7,  BEHAVIOR_NORM, KC_K},
    [KEY_E4] = {GPIOB, GPIO_PIN_1,  BEHAVIOR_NORM, KC_COMMA},
    [KEY_E5] = {GPIOB, GPIO_PIN_0,  BEHAVIOR_NORM, KC_DELETE},
    [KEY_F1] = {GPIOC, GPIO_PIN_5,  BEHAVIOR_NORM, KC_7},
    [KEY_F2] = {GPIOC, GPIO_PIN_4,  BEHAVIOR_NORM, KC_U},
    [KEY_F3] = {GPIOC, GPIO_PIN_3,  BEHAVIOR_NORM, KC_J},
    [KEY_F4] = {GPIOC, GPIO_PIN_2,  BEHAVIOR_NORM, KC_M},
    [KEY_F5] = {GPIOC, GPIO_PIN_1,  BEHAVIOR_NORM, KC_NO},
    [KEY_G1] = {GPIOC, GPIO_PIN_0,  BEHAVIOR_NORM, KC_6},
    [KEY_G2] = {GPIOC, GPIO_PIN_15, BEHAVIOR_NORM, KC_Y},
    [KEY_G3] = {GPIOC, GPIO_PIN_14, BEHAVIOR_NORM, KC_H},
    [KEY_G4] = {GPIOC, GPIO_PIN_13, BEHAVIOR_NORM, KC_N},
    [KEY_G5] = {GPIOE, GPIO_PIN_6,  BEHAVIOR_MOD,  MOD_RGUI},
    [KEY_H1] = {GPIOE, GPIO_PIN_5,  BEHAVIOR_NORM, KC_NO},
    [KEY_H2] = {GPIOE, GPIO_PIN_4,  BEHAVIOR_NORM, KC_NO},
    [KEY_H3] = {GPIOE, GPIO_PIN_3,  BEHAVIOR_NORM, KC_NO},
    [KEY_H4] = {GPIOE, GPIO_PIN_2,  BEHAVIOR_NORM, KC_SPACE},
    [KEY_I1] = {GPIOE, GPIO_PIN_1,  BEHAVIOR_MOD,  MOD_RSH},
    [KEY_J1] = {GPIOE, GPIO_PIN_0,  BEHAVIOR_MOD,  MOD_RCTL},
};

sub_keys_t sub_keys[NUM_SUB_KEYS] = {
    [KEY_A1S] = {BEHAVIOR_NORM, KC_NO},
    [KEY_A2S] = {BEHAVIOR_NORM, KC_NO},
    [KEY_A3S] = {BEHAVIOR_NORM, KC_NO},
    [KEY_B1S] = {BEHAVIOR_NORM, KC_GRAVE},
    [KEY_B2S] = {BEHAVIOR_NORM, KC_TAB},
    [KEY_B3S] = {BEHAVIOR_NORM, KC_NO},
    [KEY_B4S] = {BEHAVIOR_NORM, KC_ESCAPE},
    [KEY_C1S] = {BEHAVIOR_NORM, KC_1},
    [KEY_C2S] = {BEHAVIOR_NORM, KC_Q},
    [KEY_C3S] = {BEHAVIOR_NORM, KC_A},
    [KEY_C4S] = {BEHAVIOR_NORM, KC_Z},
    [KEY_D1S] = {BEHAVIOR_NORM, KC_2},
    [KEY_D2S] = {BEHAVIOR_NORM, KC_W},
    [KEY_D3S] = {BEHAVIOR_NORM, KC_S},
    [KEY_D4S] = {BEHAVIOR_NORM, KC_X},
    [KEY_D5S] = {BEHAVIOR_NORM, KC_NO},
    [KEY_E1S] = {BEHAVIOR_NORM, KC_3},
    [KEY_E2S] = {BEHAVIOR_NORM, KC_E},
    [KEY_E3S] = {BEHAVIOR_NORM, KC_D},
    [KEY_E4S] = {BEHAVIOR_NORM, KC_C},
    [KEY_E5S] = {BEHAVIOR_NORM, KC_NO},
    [KEY_F1S] = {BEHAVIOR_NORM, KC_4},
    [KEY_F2S] = {BEHAVIOR_NORM, KC_R},
    [KEY_F3S] = {BEHAVIOR_NORM, KC_F},
    [KEY_F4S] = {BEHAVIOR_NORM, KC_V},
    [KEY_F5S] = {BEHAVIOR_NORM, KC_NO},
    [KEY_G1S] = {BEHAVIOR_NORM, KC_5},
    [KEY_G2S] = {BEHAVIOR_NORM, KC_T},
    [KEY_G3S] = {BEHAVIOR_NORM, KC_G},
    [KEY_G4S] = {BEHAVIOR_NORM, KC_B},
    [KEY_G5S] = {BEHAVIOR_NORM, KC_NO},
    [KEY_H1S] = {BEHAVIOR_NORM, KC_NO},
    [KEY_H2S] = {BEHAVIOR_NORM, KC_NO},
    [KEY_H3S] = {BEHAVIOR_NORM, KC_NO},
    [KEY_H4S] = {BEHAVIOR_NORM, KC_SPACE},
    [KEY_I1S] = {BEHAVIOR_NORM, KC_NO},
    [KEY_J1S] = {BEHAVIOR_MOD,  MOD_LALT},
};

void keys_init(void) {
    uint8_t i;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    for(i = 0; i < NUM_KEYS; i++) {
        GPIO_InitStruct.Pin = keys[i].pin;
        HAL_GPIO_Init(keys[i].port, &GPIO_InitStruct);
    }

    for(i = 0; i < NUM_KEYS; i++) {
        keys[i].state = KEY_INACTIVE;
        keys[i].count = 0;
    }

    for(i = 0; i < NUM_SUB_KEYS; i++) {
        keys[i].state = KEY_INACTIVE;
    }
}

static bool keys_read_pin(uint8_t key_num) {
    return HAL_GPIO_ReadPin(keys[key_num].port, keys[key_num].pin) ==
                                                                GPIO_PIN_RESET;
}

void keys_scan(void) {
    uint8_t i;
    bool read;

    for(i = 0; i < NUM_KEYS; i++) {
        read = keys_read_pin(i);
        if(keys[i].state != read) {
            keys[i].count++;
            if(keys[i].count > 10) {
                keys[i].state = read;
                keys[i].count = 0;
            }
        }
        else
            keys[i].count = 0;
    }
}

void keys_received(uint8_t* buf, uint8_t len)
{
    uint8_t i;
    uint8_t byte;
    uint8_t bit;

    for(i = 0; i < NUM_SUB_KEYS; i++) {
        byte = i / 8;
        bit = i % 8;
        if(buf[byte] & (1 << bit))
            sub_keys[i].state = KEY_ACTIVE;
        else
            sub_keys[i].state = KEY_INACTIVE;
    }
}


void keys_get_keys(uint8_t* buf)
{
    uint8_t i;
    uint8_t byte;
    uint8_t bit;

    memset(buf, 0x00, NUM_KEY_BYTES);

    for(i = 0; i < NUM_KEYS; i++) {
        byte = i / 8;
        bit = i % 8;
        if(keys[i].state == (bool)KEY_ACTIVE)
            buf[byte] |= (1 << bit);;
    }
}


void keys_translate(uint8_t* buf)
{
    uint8_t i;
    uint8_t j = OUT_KC1;

    memset(buf, 0x00, 8);

    for(i = 0; i < NUM_KEYS; i++) {
        if(keys[i].state == (bool)KEY_ACTIVE) {
            if(keys[i].behavior == BEHAVIOR_MOD) {
                //set modifier bit
                buf[OUT_MOD] |= (1 << keys[i].code);
            }
            else if(keys[i].behavior == BEHAVIOR_NORM) {
                //add to key buffer
                buf[j++] = keys[i].code;
                if(j > OUT_KC6)
                    j = OUT_KC6;
            }
        }
    }

    for(i = 0; i < NUM_SUB_KEYS; i++) {
        if(sub_keys[i].state == (bool)KEY_ACTIVE) {
            if(sub_keys[i].behavior == BEHAVIOR_MOD) {
                //set modifier bit
                buf[OUT_MOD] |= (1 << sub_keys[i].code);
            }
            else if(sub_keys[i].behavior == BEHAVIOR_NORM) {
                //add to key buffer
                buf[j++] = sub_keys[i].code;
                if(j > OUT_KC6)
                    j = OUT_KC6;
            }
        }
    }
}