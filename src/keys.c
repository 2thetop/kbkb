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

keys_t keys[NUM_KEYS] = {
    [KEY_A1] = {GPIOD, GPIO_PIN_3,  BEHAVIOR_NORM, KC_EQUAL},
    [KEY_A2] = {GPIOD, GPIO_PIN_2,  BEHAVIOR_NORM, KC_RBRACKET},
    [KEY_A3] = {GPIOD, GPIO_PIN_15, BEHAVIOR_NORM, KC_ENTER},
    [KEY_B1] = {GPIOD, GPIO_PIN_14,  BEHAVIOR_NORM, KC_MINUS},
    [KEY_B2] = {GPIOD, GPIO_PIN_13,  BEHAVIOR_NORM, KC_LBRACKET},
    [KEY_B3] = {GPIOD, GPIO_PIN_12,  BEHAVIOR_NORM, KC_QUOTE},
    [KEY_B4] = {GPIOD, GPIO_PIN_11,  BEHAVIOR_NORM, KC_BSLASH},
    [KEY_C1] = {GPIOD, GPIO_PIN_10, BEHAVIOR_NORM, KC_0},
    [KEY_C2] = {GPIOD, GPIO_PIN_9, BEHAVIOR_NORM, KC_P},
    [KEY_C3] = {GPIOD, GPIO_PIN_8, BEHAVIOR_NORM, KC_SCOLON},
    [KEY_C4] = {GPIOE, GPIO_PIN_15,  BEHAVIOR_NORM, KC_SLASH},
    [KEY_D1] = {GPIOE, GPIO_PIN_14,  BEHAVIOR_NORM, KC_9},
    [KEY_D2] = {GPIOE, GPIO_PIN_13,  BEHAVIOR_NORM, KC_O},
    [KEY_D3] = {GPIOE, GPIO_PIN_12,  BEHAVIOR_NORM, KC_L},
    [KEY_D4] = {GPIOE, GPIO_PIN_11,  BEHAVIOR_NORM, KC_DOT},
    [KEY_D5] = {GPIOE, GPIO_PIN_10,  BEHAVIOR_NORM, KC_BSPACE},
    [KEY_E1] = {GPIOE, GPIO_PIN_9,  BEHAVIOR_NORM, KC_8},
    [KEY_E2] = {GPIOE, GPIO_PIN_8,  BEHAVIOR_NORM, KC_I},
    [KEY_E3] = {GPIOE, GPIO_PIN_7,  BEHAVIOR_NORM, KC_K},
    [KEY_E4] = {GPIOB, GPIO_PIN_1,  BEHAVIOR_NORM, KC_COMMA},
    [KEY_E5] = {GPIOB, GPIO_PIN_0,  BEHAVIOR_NORM, KC_DELETE},
    [KEY_F1] = {GPIOC, GPIO_PIN_5, BEHAVIOR_NORM, KC_7},
    [KEY_F2] = {GPIOC, GPIO_PIN_4, BEHAVIOR_NORM, KC_U},
    [KEY_F3] = {GPIOC, GPIO_PIN_3, BEHAVIOR_NORM, KC_J},
    [KEY_F4] = {GPIOC, GPIO_PIN_2, BEHAVIOR_NORM, KC_M},
    [KEY_F5] = {GPIOC, GPIO_PIN_1, BEHAVIOR_NORM, KC_SPACE},
    [KEY_G1] = {GPIOC, GPIO_PIN_0, BEHAVIOR_NORM, KC_6},
    [KEY_G2] = {GPIOC, GPIO_PIN_15,  BEHAVIOR_NORM, KC_Y},
    [KEY_G3] = {GPIOC, GPIO_PIN_14,  BEHAVIOR_NORM, KC_H},
    [KEY_G4] = {GPIOC, GPIO_PIN_13, BEHAVIOR_NORM, KC_N},
    [KEY_G5] = {GPIOE, GPIO_PIN_6, BEHAVIOR_NORM, KC_NO},
    [KEY_H1] = {GPIOE, GPIO_PIN_5, BEHAVIOR_NORM, KC_NO},
    [KEY_H2] = {GPIOE, GPIO_PIN_4, BEHAVIOR_NORM, KC_NO},
    [KEY_H3] = {GPIOE, GPIO_PIN_3, BEHAVIOR_NORM, KC_NO},
    [KEY_H4] = {GPIOE, GPIO_PIN_2, BEHAVIOR_MOD,  MOD_RSH},
    [KEY_I1] = {GPIOE, GPIO_PIN_1,  BEHAVIOR_MOD,  MOD_RCTL},
    [KEY_J1] = {GPIOE, GPIO_PIN_0,  BEHAVIOR_NORM, KC_NO},
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


void keys_translate(uint8_t* buf) {
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
}
