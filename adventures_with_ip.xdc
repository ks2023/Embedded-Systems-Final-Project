## LED Constraints
set_property PACKAGE_PIN M14 [get_ports {LEDs_out_0[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {LEDs_out_0[0]}]

set_property PACKAGE_PIN M15 [get_ports {LEDs_out_0[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {LEDs_out_0[1]}]

set_property PACKAGE_PIN G14 [get_ports {LEDs_out_0[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {LEDs_out_0[2]}]

set_property PACKAGE_PIN D18 [get_ports {LEDs_out_0[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {LEDs_out_0[3]}]

## Zybo Audio Codec Constraints
set_property PACKAGE_PIN K18 [get_ports BCLK_0]
set_property IOSTANDARD LVCMOS33 [get_ports BCLK_0]

set_property PACKAGE_PIN L17 [get_ports PBLRCLK_0]
set_property IOSTANDARD LVCMOS33 [get_ports PBLRCLK_0]

set_property PACKAGE_PIN M18 [get_ports RECLRCLK_0]
set_property IOSTANDARD LVCMOS33 [get_ports RECLRCLK_0]

set_property PACKAGE_PIN K17 [get_ports RECDAT_0]
set_property IOSTANDARD LVCMOS33 [get_ports RECDAT_0]

set_property PACKAGE_PIN M17 [get_ports PBDATA_0]
set_property IOSTANDARD LVCMOS33 [get_ports PBDATA_0]

# MCLK
set_property PACKAGE_PIN T19 [get_ports FCLK_CLK1_0]
set_property IOSTANDARD LVCMOS33 [get_ports FCLK_CLK1_0]

#I2C 0 Interface
set_property PACKAGE_PIN N18 [get_ports IIC_0_0_scl_io]
set_property IOSTANDARD LVCMOS33 [get_ports IIC_0_0_scl_io]

set_property PACKAGE_PIN N17 [get_ports IIC_0_0_sda_io]
set_property IOSTANDARD LVCMOS33 [get_ports IIC_0_0_sda_io]

#GPIO_0[0] Digital Mute
set_property PACKAGE_PIN P18 [get_ports {GPIO_0_tri_io[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {GPIO_0_tri_io[0]}]