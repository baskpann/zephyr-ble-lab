/*
 * 00-hci-bringup — Host application
 *
 * Purpose: prove the STM32F4 Host <-> nRF52840 Controller HCI UART link
 * works, driven manually from the Zephyr shell (CONFIG_BT_SHELL=y).
 *
 * Deliberately does NOT call bt_enable() itself — with CONFIG_BT_SHELL,
 * the "bt init" shell command calls bt_enable() for you. Leaving init to
 * the shell means you drive each step by hand (bt init -> bt adv on)
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(hci_bringup, LOG_LEVEL_INF);

int main(void)
{
	LOG_INF("Host app booted. Use shell: 'bt init' then 'bt scan on'.");
	return 0;
}
