
#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>
#include <linux/ar8216_platform.h>

#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"
#include "pci.h"

#define KOS_QCA953X_B_GPIO_LED_WLAN0		12
#define KOS_QCA953X_B_GPIO_LED_WLAN1		1
#define KOS_QCA953X_B_GPIO_LED_USB		3
#define KOS_QCA953X_B_GPIO_LED_STATUS		13

#define KOS_QCA953X_B_GPIO_LED_WAN		11
#define KOS_QCA953X_B_GPIO_LED_LAN1		4
#define KOS_QCA953X_B_GPIO_LED_LAN2		14
#define KOS_QCA953X_B_GPIO_LED_LAN3		15
#define KOS_QCA953X_B_GPIO_LED_LAN4		16

#define KOS_QCA953X_B_GPIO_BTN_RESET		17

#define KOS_QCA953X_B_KEYS_POLL_INTERVAL	20	/* msecs */
#define KOS_QCA953X_B_KEYS_DEBOUNCE_INTERVAL	(3 * KOS_QCA953X_B_KEYS_POLL_INTERVAL)

#define KOS_QCA953X_B_MAC0_OFFSET		0
#define KOS_QCA953X_B_MAC1_OFFSET		6
#define KOS_QCA953X_B_WMAC_CALDATA_OFFSET	0x1000

static struct gpio_led kos_qca953x_b_leds_gpio[] __initdata = {
	{
		.name		= "kos-qca953x-b:green:wlan0",
		.gpio		= KOS_QCA953X_B_GPIO_LED_WLAN0,
		.active_low	= 1,
	},
	{
		.name		= "kos-qca953x-b:green:wlan1",
		.gpio		= KOS_QCA953X_B_GPIO_LED_WLAN1,
		.active_low	= 1,
	},
	{
		.name		= "kos-qca953x-b:green:usb",
		.gpio		= KOS_QCA953X_B_GPIO_LED_USB,
		.active_low	= 1,
	},
	{
		.name		= "kos-qca953x-b:green:status",
		.gpio		= KOS_QCA953X_B_GPIO_LED_STATUS,
		.active_low	= 1,
	}
};

static struct gpio_keys_button kos_qca953x_b_gpio_keys[] __initdata = {
	 {
        .desc       = "reset",
        .type       = EV_KEY,
        .code       = KEY_RESTART,
        .debounce_interval = KOS_QCA953X_B_KEYS_DEBOUNCE_INTERVAL,
        .gpio       = KOS_QCA953X_B_GPIO_BTN_RESET,
        .active_low = 1,
    }, 
};

static void __init kos_qca953x_b_gpio_led_setup(void)
{
	ath79_gpio_direction_select(KOS_QCA953X_B_GPIO_LED_WAN, true);
	ath79_gpio_direction_select(KOS_QCA953X_B_GPIO_LED_LAN1, true);
	ath79_gpio_direction_select(KOS_QCA953X_B_GPIO_LED_LAN2, true);
	ath79_gpio_direction_select(KOS_QCA953X_B_GPIO_LED_LAN3, true);
	ath79_gpio_direction_select(KOS_QCA953X_B_GPIO_LED_LAN4, true);

	ath79_gpio_output_select(KOS_QCA953X_B_GPIO_LED_WAN,
			QCA953X_GPIO_OUT_MUX_LED_LINK5);
	ath79_gpio_output_select(KOS_QCA953X_B_GPIO_LED_LAN1,
			QCA953X_GPIO_OUT_MUX_LED_LINK1);
	ath79_gpio_output_select(KOS_QCA953X_B_GPIO_LED_LAN2,
			QCA953X_GPIO_OUT_MUX_LED_LINK2);
	ath79_gpio_output_select(KOS_QCA953X_B_GPIO_LED_LAN3,
			QCA953X_GPIO_OUT_MUX_LED_LINK3);
	ath79_gpio_output_select(KOS_QCA953X_B_GPIO_LED_LAN4,
			QCA953X_GPIO_OUT_MUX_LED_LINK4);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(kos_qca953x_b_leds_gpio),
			kos_qca953x_b_leds_gpio);
	ath79_register_gpio_keys_polled(-1, KOS_QCA953X_B_KEYS_POLL_INTERVAL,
			ARRAY_SIZE(kos_qca953x_b_gpio_keys),
			kos_qca953x_b_gpio_keys);
}

static void __init kos_qca953x_b_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(NULL);

	kos_qca953x_b_gpio_led_setup();

	ath79_register_usb();
	ath79_register_pci();

	ath79_register_wmac(art + KOS_QCA953X_B_WMAC_CALDATA_OFFSET, NULL);

	ath79_register_mdio(0, 0x0);
	ath79_register_mdio(1, 0x0);

	ath79_init_mac(ath79_eth0_data.mac_addr, art + KOS_QCA953X_B_MAC0_OFFSET, 0);
	ath79_init_mac(ath79_eth1_data.mac_addr, art + KOS_QCA953X_B_MAC1_OFFSET, 0);
	/* add by zcc */
	ath79_setup_ar934x_eth_cfg(AR934X_ETH_CFG_SW_PHY_SWAP);
	/* WAN port */
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
	ath79_eth0_data.speed = SPEED_100;
	ath79_eth0_data.duplex = DUPLEX_FULL;
//	ath79_eth0_data.phy_mask = BIT(4);
	ath79_eth0_data.phy_mask = BIT(0);
	ath79_register_eth(0);

	/* LAN ports */
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_eth1_data.speed = SPEED_1000;
	ath79_eth1_data.duplex = DUPLEX_FULL;
//	ath79_switch_data.phy_poll_mask |= BIT(4);
	ath79_switch_data.phy_poll_mask |= BIT(0);
	ath79_switch_data.phy4_mii_en = 1;
	ath79_register_eth(1);
}

MIPS_MACHINE(ATH79_MACH_KOS_QCA953X_B, "KOS-QCA953X-B", "UOSBOX KOS-QCA953X-B",
	    kos_qca953x_b_setup);
