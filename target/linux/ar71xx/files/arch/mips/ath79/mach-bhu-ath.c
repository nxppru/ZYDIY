/*
 *  BHU board support
 *
 *  Copyright (C) 2013-2014 Terry Yang <yangbo@bhunetworks.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/gpio.h>
#include <linux/platform_device.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"

static const char *bhu_ap123_part_probes[] = {
	"cmdlinepart",
	NULL,
};

static struct flash_platform_data bhu_ap123_flash_data = {
	.part_probes	= bhu_ap123_part_probes,
};

static void __init bhu_ap123_setup(u8 *mac, u8 *ee)
{

	ath79_register_m25p80(&bhu_ap123_flash_data);

	ath79_register_mdio(1, 0x0);

	ath79_init_mac(ath79_eth0_data.mac_addr, mac, 0);
	ath79_init_mac(ath79_eth1_data.mac_addr, mac, 1);

	/* GMAC0 is connected to the PHY4 of the internal switch */
	ath79_switch_data.phy4_mii_en = 1;
	ath79_switch_data.phy_poll_mask = BIT(4);
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
	ath79_eth0_data.phy_mask = BIT(4);
	ath79_eth0_data.mii_bus_dev = &ath79_mdio1_device.dev;
	ath79_register_eth(0);

	/* GMAC1 is connected to the internal switch. Only use PHY3 */
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_eth1_data.phy_mask = BIT(3);
	ath79_register_eth(1);

	ath79_register_wmac(ee, ee+2);

    ath79_register_usb();
}

/*
 * BHU BXU2000n-2 A1 board
 */

#define BHU_BXU2000N2_A1_GPIO_LED_WLAN		13
#define BHU_BXU2000N2_A1_GPIO_LED_WAN		19
#define BHU_BXU2000N2_A1_GPIO_LED_LAN		21
#define BHU_BXU2000N2_A1_GPIO_LED_SYSTEM	14

#define BHU_BXU2000N2_A1_GPIO_BTN_RESET		17

#define BHU_BXU2000N2_KEYS_POLL_INTERVAL	20	/* msecs */
#define BHU_BXU2000N2_KEYS_DEBOUNCE_INTERVAL	\
	(3 * BHU_BXU2000N2_KEYS_POLL_INTERVAL)

static struct gpio_led bhu_bxu2000n2_a1_leds_gpio[] __initdata = {
	{
		.name		= "bhu:green:status",
		.gpio		= BHU_BXU2000N2_A1_GPIO_LED_SYSTEM,
		.active_low	= 1,
	}, {
		.name		= "bhu:green:lan",
		.gpio		= BHU_BXU2000N2_A1_GPIO_LED_LAN,
		.active_low	= 1,
	}, {
		.name		= "bhu:green:wan",
		.gpio		= BHU_BXU2000N2_A1_GPIO_LED_WAN,
		.active_low	= 1,
	}, {
		.name		= "bhu:green:wlan",
		.gpio		= BHU_BXU2000N2_A1_GPIO_LED_WLAN,
		.active_low	= 1,
	},
};

static struct gpio_keys_button bhu_bxu2000n2_a1_gpio_keys[] __initdata = {
	{
		.desc		= "Reset button",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = BHU_BXU2000N2_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= BHU_BXU2000N2_A1_GPIO_BTN_RESET,
		.active_low	= 1,
	}
};

static void __init bhu_bxu2000n2_a1_setup(void)
{
	bhu_ap123_setup((u8 *) KSEG1ADDR(0x1fff0000), (u8 *) KSEG1ADDR(0x1fff1000));

	ath79_register_leds_gpio(-1, ARRAY_SIZE(bhu_bxu2000n2_a1_leds_gpio),
				 bhu_bxu2000n2_a1_leds_gpio);

	ath79_register_gpio_keys_polled(1, BHU_BXU2000N2_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(bhu_bxu2000n2_a1_gpio_keys),
					bhu_bxu2000n2_a1_gpio_keys);
}

MIPS_MACHINE(ATH79_MACH_BHU_BXU2000N2_A1, "BXU2000n-2-A1",
	     "BHU BXU2000n-2 rev. A1",
	     bhu_bxu2000n2_a1_setup);

/*
 * BHU BXO2000n-2S board
 */

#define BHU_BXO2000N2S_GPIO_LED_SYSTEM   14
#define BHU_BXO2000N2S_GPIO_LED_S0       14
#define BHU_BXO2000N2S_GPIO_LED_S1       19
#define BHU_BXO2000N2S_GPIO_LED_S2       15
#define BHU_BXO2000N2S_GPIO_LED_S3       21
#define BHU_BXO2000N2S_GPIO_LED_WLAN     13
#define BHU_BXO2000N2S_GPIO_LED_WAN      20
#define BHU_BXO2000N2S_GPIO_LED_LAN      22

#define BHU_BXO2000N2S_GPIO_BTN_RESET        17

#define BHU_BXO2000N2S_KEYS_POLL_INTERVAL   20  /* msecs */
#define BHU_BXO2000N2S_KEYS_DEBOUNCE_INTERVAL   \
    (3 * BHU_BXO2000N2S_KEYS_POLL_INTERVAL)

static struct gpio_led bhu_bxo2000n2s_leds_gpio[] __initdata = {
    {
        .name       = "bhu:green:status",
        .gpio       = BHU_BXO2000N2S_GPIO_LED_SYSTEM,
        .active_low = 1,
    }, {
        .name       = "bhu:green:lan",
        .gpio       = BHU_BXO2000N2S_GPIO_LED_LAN,
        .active_low = 1,
    }, {
        .name       = "bhu:green:wan",
        .gpio       = BHU_BXO2000N2S_GPIO_LED_WAN,
        .active_low = 1,
    }, {
        .name       = "bhu:green:wlan",
        .gpio       = BHU_BXO2000N2S_GPIO_LED_WLAN,
        .active_low = 1,
    }, {
        .name       = "bhu:green:rssilow",
        .gpio       = BHU_BXO2000N2S_GPIO_LED_S1,
        .active_low = 1,
    }, {
        .name       = "bhu:green:rssimedium",
        .gpio       = BHU_BXO2000N2S_GPIO_LED_S2,
        .active_low = 1,
    }, {
        .name       = "bhu:green:rssihigh",
        .gpio       = BHU_BXO2000N2S_GPIO_LED_S3,
        .active_low = 1,
    },
};

static struct gpio_keys_button bhu_bxo2000n2s_gpio_keys[] __initdata = {
    {
        .desc       = "Reset button",
        .type       = EV_KEY,
        .code       = KEY_RESTART,
        .debounce_interval = BHU_BXO2000N2S_KEYS_DEBOUNCE_INTERVAL,
        .gpio       = BHU_BXO2000N2S_GPIO_BTN_RESET,
        .active_low = 1,
    }
};

static void __init bhu_bxo2000n2s_setup(void)
{
	bhu_ap123_setup((u8 *) KSEG1ADDR(0x1f7f0000), (u8 *) KSEG1ADDR(0x1f7f1000));

    ath79_register_leds_gpio(-1, ARRAY_SIZE(bhu_bxo2000n2s_leds_gpio),
                 bhu_bxo2000n2s_leds_gpio);

    ath79_register_gpio_keys_polled(1, BHU_BXO2000N2S_KEYS_POLL_INTERVAL,
                    ARRAY_SIZE(bhu_bxo2000n2s_gpio_keys),
                    bhu_bxo2000n2s_gpio_keys);
}

MIPS_MACHINE(ATH79_MACH_BHU_BXO2000N2S, "BXO2000n-2S",
         "BHU BXO2000n-2S",
         bhu_bxo2000n2s_setup);


/*
 * BHU BXO2000n-2S-U board
 */

static void __init bhu_bxo2000n2s_u_setup(void)
{
	bhu_ap123_setup((u8 *) KSEG1ADDR(0x1fff0000), (u8 *) KSEG1ADDR(0x1fff1000));

    ath79_register_leds_gpio(-1, ARRAY_SIZE(bhu_bxo2000n2s_leds_gpio),
                 bhu_bxo2000n2s_leds_gpio);

    ath79_register_gpio_keys_polled(1, BHU_BXO2000N2S_KEYS_POLL_INTERVAL,
                    ARRAY_SIZE(bhu_bxo2000n2s_gpio_keys),
                    bhu_bxo2000n2s_gpio_keys);
}

MIPS_MACHINE(ATH79_MACH_BHU_BXO2000N2SU, "BXO2000n-2S-U",
         "BHU BXO2000n-2S-U",
         bhu_bxo2000n2s_u_setup);

/*
 * BHU BXI2000n-2 board
 */

#define BHU_BXI2000N2_GPIO_LED_WLAN		13
#define BHU_BXI2000N2_GPIO_LED_LAN		14
#define BHU_BXI2000N2_GPIO_LED_SYSTEM	15
#define BHU_BXI2000N2_GPIO_LED_RES  	19
#define BHU_BXI2000N2_GPIO_LED_5G     	21

#define BHU_BXI2000N2_GPIO_BTN_RESET		17

#define BHU_BXI2000N2_KEYS_POLL_INTERVAL	20	/* msecs */
#define BHU_BXI2000N2_KEYS_DEBOUNCE_INTERVAL	\
	(3 * BHU_BXI2000N2_KEYS_POLL_INTERVAL)

static struct gpio_led bhu_bxi2000n2_leds_gpio[] __initdata = {
	{
		.name		= "bhu:green:status",
		.gpio		= BHU_BXI2000N2_GPIO_LED_SYSTEM,
		.active_low	= 1,
	}, {
		.name		= "bhu:green:lan",
		.gpio		= BHU_BXI2000N2_GPIO_LED_LAN,
		.active_low	= 1,
	}, {
		.name		= "bhu:green:wlan",
		.gpio		= BHU_BXI2000N2_GPIO_LED_WLAN,
		.active_low	= 1,
	}, {
		.name		= "bhu:green:res",
		.gpio		= BHU_BXI2000N2_GPIO_LED_RES,
		.active_low	= 1,
	}, {
		.name		= "bhu:green:5g",
		.gpio		= BHU_BXI2000N2_GPIO_LED_5G,
		.active_low	= 1,
	},
};

static struct gpio_keys_button bhu_bxi2000n2_gpio_keys[] __initdata = {
	{
		.desc		= "Reset button",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = BHU_BXI2000N2_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= BHU_BXI2000N2_GPIO_BTN_RESET,
		.active_low	= 1,
	}
};

static void __init bhu_bxi2000n2_setup(void)
{
	bhu_ap123_setup((u8 *) KSEG1ADDR(0x1f7f0000), (u8 *) KSEG1ADDR(0x1f7f1000));

	ath79_register_leds_gpio(-1, ARRAY_SIZE(bhu_bxi2000n2_leds_gpio),
				 bhu_bxi2000n2_leds_gpio);

	ath79_register_gpio_keys_polled(1, BHU_BXI2000N2_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(bhu_bxi2000n2_gpio_keys),
					bhu_bxi2000n2_gpio_keys);
}

MIPS_MACHINE(ATH79_MACH_BHU_BXI2000N2, "BXI2000n-2",
	     "BHU BXI2000n-2",
	     bhu_bxi2000n2_setup);

/*
 * BHU BXI2000n-2-V2 board
 */

static void __init bhu_bxi2000n2v2_setup(void)
{
	bhu_ap123_setup((u8 *) KSEG1ADDR(0x1fff0000), (u8 *) KSEG1ADDR(0x1fff1000));

	ath79_register_leds_gpio(-1, ARRAY_SIZE(bhu_bxi2000n2_leds_gpio),
				 bhu_bxi2000n2_leds_gpio);

	ath79_register_gpio_keys_polled(1, BHU_BXI2000N2_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(bhu_bxi2000n2_gpio_keys),
					bhu_bxi2000n2_gpio_keys);
}

MIPS_MACHINE(ATH79_MACH_BHU_BXI2000N2V2, "BXI2000n-2-V2",
	     "BHU BXI2000n-2-V2",
	     bhu_bxi2000n2v2_setup);

/*
 * BHU BXO5000n-2S board
 */

#define BHU_BXO5000N2S_GPIO_LED_SYSTEM   19
#define BHU_BXO5000N2S_GPIO_LED_S0       19
#define BHU_BXO5000N2S_GPIO_LED_S1       20
#define BHU_BXO5000N2S_GPIO_LED_S2       21
#define BHU_BXO5000N2S_GPIO_LED_S3       15
#define BHU_BXO5000N2S_GPIO_LED_WLAN     12
#define BHU_BXO5000N2S_GPIO_LED_WAN      18
#define BHU_BXO5000N2S_GPIO_LED_LAN      22

#define BHU_BXO5000N2S_GPIO_BTN_RESET        17

#define BHU_BXO5000N2S_KEYS_POLL_INTERVAL   20  /* msecs */
#define BHU_BXO5000N2S_KEYS_DEBOUNCE_INTERVAL   \
    (3 * BHU_BXO5000N2S_KEYS_POLL_INTERVAL)

static struct gpio_led bhu_bxo5000n2s_leds_gpio[] __initdata = {
    {
        .name       = "bhu:green:status",
        .gpio       = BHU_BXO5000N2S_GPIO_LED_SYSTEM,
        .active_low = 1,
    }, {
        .name       = "bhu:green:lan",
        .gpio       = BHU_BXO5000N2S_GPIO_LED_LAN,
        .active_low = 1,
    }, {
        .name       = "bhu:green:wan",
        .gpio       = BHU_BXO5000N2S_GPIO_LED_WAN,
        .active_low = 1,
    }, {
        .name       = "bhu:green:wlan",
        .gpio       = BHU_BXO5000N2S_GPIO_LED_WLAN,
        .active_low = 1,
    }, {
        .name       = "bhu:green:rssilow",
        .gpio       = BHU_BXO5000N2S_GPIO_LED_S1,
        .active_low = 1,
    }, {
        .name       = "bhu:green:rssimedium",
        .gpio       = BHU_BXO5000N2S_GPIO_LED_S2,
        .active_low = 1,
    }, {
        .name       = "bhu:green:rssihigh",
        .gpio       = BHU_BXO5000N2S_GPIO_LED_S3,
        .active_low = 1,
    },
};

static struct gpio_keys_button bhu_bxo5000n2s_gpio_keys[] __initdata = {
    {
        .desc       = "Reset button",
        .type       = EV_KEY,
        .code       = KEY_RESTART,
        .debounce_interval = BHU_BXO5000N2S_KEYS_DEBOUNCE_INTERVAL,
        .gpio       = BHU_BXO5000N2S_GPIO_BTN_RESET,
        .active_low = 1,
    }
};


static void __init bhu_bxo5000n2s_setup(void)
{
	bhu_ap123_setup((u8 *) KSEG1ADDR(0x1f7f0000), (u8 *) KSEG1ADDR(0x1f7f1000));

    ath79_register_leds_gpio(-1, ARRAY_SIZE(bhu_bxo5000n2s_leds_gpio),
                 bhu_bxo5000n2s_leds_gpio);

    ath79_register_gpio_keys_polled(1, BHU_BXO5000N2S_KEYS_POLL_INTERVAL,
                    ARRAY_SIZE(bhu_bxo5000n2s_gpio_keys),
                    bhu_bxo5000n2s_gpio_keys);
}

MIPS_MACHINE(ATH79_MACH_BHU_BXO5000N2S, "BXO5000n-2S",
         "BHU BXO5000n-2S",
         bhu_bxo5000n2s_setup);


/*
 * BHU BXO5000n-2s-v2 board
 */

static void __init bhu_bxo5000n2sv2_setup(void)
{
	bhu_ap123_setup((u8 *) KSEG1ADDR(0x1fff0000), (u8 *) KSEG1ADDR(0x1fff1000));

    ath79_register_leds_gpio(-1, ARRAY_SIZE(bhu_bxo5000n2s_leds_gpio),
                 bhu_bxo5000n2s_leds_gpio);

    ath79_register_gpio_keys_polled(1, BHU_BXO5000N2S_KEYS_POLL_INTERVAL,
                    ARRAY_SIZE(bhu_bxo5000n2s_gpio_keys),
                    bhu_bxo5000n2s_gpio_keys);
}

MIPS_MACHINE(ATH79_MACH_BHU_BXO5000N2SV2, "BXO5000n-2S-V2",
         "BHU BXO5000n-2S-V2",
         bhu_bxo5000n2sv2_setup);