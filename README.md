# Linux---Android-driver-call-LDO-power
This is a driver that we can use it to call power (regulator) when we want


**Release Note**
Version: 1.0.0
Author: Nguyen Van Huynh Y
Date: 2022-05-01

**How to use this driver for android**

1. Defconfig file: CONFIG_POWERCONFIG=y

2. Device tree:
	myldo_powerconfig {
		compatible = "power-config";
		status = "okay";
		my_ldo-supply = <&pm89xx_lx>;
	};
	
	- compatible = "power-config": need to link driver with device-tree
	- pm89xx_lx: LDO power that we need to use
