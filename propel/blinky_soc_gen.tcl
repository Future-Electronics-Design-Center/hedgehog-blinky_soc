
# $propelRegenLsccBase: Propel installation path, e.g. C:/lscc/propel/2023.2
# If there is any file configuration in sbp_config_ip command, need to update the file path to the correct path on local setup, such as INIT_FILE in IP "System Memory".

set targetDir "."
set projectName "blinky_soc"
set propelRegenLsccBase "C:/lscc/propel/2023.2"

if {[file exists "$targetDir"] != 1} {
    puts "$targetDir doesn't exist. Please update this setting in tcl file first."
    return
}

if {[file exists "$propelRegenLsccBase"] != 1} {
    puts "$propelRegenLsccBase doesn't exist. Please update this setting in tcl file first."
    return
}

sbp_create_project  -name "$projectName" -path "$targetDir" -device {LFMXO5-25-7BBG256C}  -speed {7_High-Performance_1.0V} -language {verilog} -board {NA()}  -psc "$propelRegenLsccBase/templates/MachXO3D_Template01/MachXO3D_Template01.psc"

sbp_design save

sbp_delete "$projectName/clk_i" -type port
sbp_delete "$projectName/rstn_i" -type port

sbp_config_ip -vlnv {latticesemi.com:ip:cpu0:2.5.0}  -meta_vlnv {latticesemi.com:ip:riscv_mc:2.5.0} -cfg_value {SIMULATION:false,DEBUG_ENABLE:true,M_STANDALONE:true,IRQ_NUM:4}
sbp_config_ip -vlnv {latticesemi.com:ip:gpio0:1.6.1}  -meta_vlnv {latticesemi.com:ip:gpio:1.6.1} -cfg_value {IO_LINES_COUNT:8,EXTERNAL_BUF:true,DIRECTION_DEF_VAL_INPUT:FF}
sbp_config_ip -vlnv {latticesemi.com:ip:i2c_ctrl:2.0.0}  -meta_vlnv {latticesemi.com:ip:i2c_controller:2.0.0} -cfg_value {SYS_CLOCK_FREQ:60}
sbp_config_ip -vlnv {latticesemi.com:ip:spi_ctrl:2.1.0}  -meta_vlnv {latticesemi.com:ip:spi_controller:2.1.0} -cfg_value {INTERFACE:APB,SLAVE_COUNT:2,ONLY_WRITE:1,DATA_WIDTH:8,SSNP:0,SYS_CLOCK_FREQ:60.0,DESIRED_CLOCK_FREQ:15.0}
sbp_config_ip -vlnv {latticesemi.com:ip:sysmem0:2.1.0}  -meta_vlnv {latticesemi.com:ip:system_memory:2.1.0} -cfg_value {ADDR_DEPTH:8192,PORT_COUNT:2,INIT_MEM:false,REGMODE_S0:true,REGMODE_S1:true}
sbp_config_ip -vlnv {latticesemi.com:ip:uart0:1.3.0}  -meta_vlnv {latticesemi.com:ip:uart:1.3.0} -cfg_value {SYS_CLOCK_FREQ:60.0}
sbp_config_ip -vlnv {latticesemi.com:module:ahbl0:1.3.0}  -meta_vlnv {latticesemi.com:module:ahb_lite_interconnect:1.3.0} -cfg_value {TOTAL_MASTER_CNT:1}
sbp_config_ip -vlnv {latticesemi.com:module:ahbl2apb0:1.1.0}  -meta_vlnv {latticesemi.com:module:ahb_lite_to_apb_bridge:1.1.0} -cfg_value {APB_CLK_EN:true}
sbp_config_ip -vlnv {latticesemi.com:module:apb0:1.2.0}  -meta_vlnv {latticesemi.com:module:apb_interconnect:1.2.0} -cfg_value {TOTAL_MASTER_CNT:1,TOTAL_SLAVE_CNT:4}
sbp_config_ip -vlnv {latticesemi.com:module:pll0:1.8.0}  -meta_vlnv {latticesemi.com:module:pll:1.8.0} -cfg_value {gui_en_frac_n:true,gui_en_ssc:false,gui_en_int_fbkdel_sel:false,gui_refclk_freq:125.0,gui_clk_op_freq:90.0,gui_clk_op_tol:0.0,gui_clk_os_en:true,gui_clk_os_freq:60.0,gui_en_pll_reset:true,gui_en_pll_lock:true}



sbp_add_component -vlnv {latticesemi.com:ip:cpu0:2.5.0} -name cpu0_inst
sbp_add_component -vlnv {latticesemi.com:ip:gpio0:1.6.1} -name gpio0_inst
sbp_add_component -vlnv {latticesemi.com:ip:i2c_ctrl:2.0.0} -name i2c_ctrl_inst
sbp_add_component -vlnv {latticesemi.com:ip:spi_ctrl:2.1.0} -name spi_ctrl_inst
sbp_add_component -vlnv {latticesemi.com:ip:sysmem0:2.1.0} -name sysmem0_inst
sbp_add_component -vlnv {latticesemi.com:ip:uart0:1.3.0} -name uart0_inst
sbp_add_component -vlnv {latticesemi.com:module:ahbl0:1.3.0} -name ahbl0_inst
sbp_add_component -vlnv {latticesemi.com:module:ahbl2apb0:1.1.0} -name ahbl2apb0_inst
sbp_add_component -vlnv {latticesemi.com:module:apb0:1.2.0} -name apb0_inst
sbp_add_component -vlnv {latticesemi.com:module:pll0:1.8.0} -name pll0_inst

sbp_add_gluelogic -name equation_module0_inst -logicinfo [sbp_create_glue_logic equation equation_module0 {} { {
 "expr": "A & B",
 "module_name": "equation_module0"
}
}]

sbp_add_port -direction in clk_i
sbp_add_port -direction in miso_i
sbp_add_port -direction out mosi_o
sbp_add_port -direction in rstn_i
sbp_add_port -direction in rxd_i
sbp_add_port -direction out s_clk
sbp_add_port -direction inout scl
sbp_add_port -direction inout sda
sbp_add_port -direction out txd_o
sbp_add_port -from 7 -to 0 -direction out led_o
sbp_add_port -from 1 -to 0 -direction out ssn_o

sbp_connect_net "$projectName/pll0_inst/clki_i" "$projectName/clk_i" 
sbp_connect_net "$projectName/spi_ctrl_inst/miso_i" "$projectName/miso_i" 
sbp_connect_net "$projectName/equation_module0_inst/A" "$projectName/pll0_inst/rstn_i" "$projectName/rstn_i" 
sbp_connect_net "$projectName/uart0_inst/rxd_i" "$projectName/rxd_i" 
sbp_connect_net "$projectName/i2c_ctrl_inst/scl_io" "$projectName/scl" 
sbp_connect_net "$projectName/i2c_ctrl_inst/sda_io" "$projectName/sda" 
sbp_connect_net "$projectName/ahbl2apb0_inst/presetn_i" "$projectName/ahbl2apb0_inst/rst_n_i" "$projectName/ahbl0_inst/ahbl_hresetn_i" "$projectName/sysmem0_inst/ahbl_hresetn_i" "$projectName/apb0_inst/apb_presetn_i" "$projectName/cpu0_inst/system_resetn_o" "$projectName/gpio0_inst/resetn_i" "$projectName/uart0_inst/rst_n_i" "$projectName/i2c_ctrl_inst/rst_n_i" "$projectName/spi_ctrl_inst/rst_n_i" 
sbp_connect_net "$projectName/equation_module0_inst/O" "$projectName/cpu0_inst/rst_n_i" 
sbp_connect_net "$projectName/ahbl2apb0_inst/clk_i" "$projectName/ahbl0_inst/ahbl_hclk_i" "$projectName/pll0_inst/clkop_o" "$projectName/sysmem0_inst/ahbl_hclk_i" "$projectName/cpu0_inst/clk_i" 
sbp_connect_net "$projectName/ahbl2apb0_inst/pclk_i" "$projectName/pll0_inst/clkos_o" "$projectName/apb0_inst/apb_pclk_i" "$projectName/gpio0_inst/clk_i" "$projectName/uart0_inst/clk_i" "$projectName/i2c_ctrl_inst/clk_i" "$projectName/spi_ctrl_inst/clk_i" 
sbp_connect_net "$projectName/equation_module0_inst/B" "$projectName/pll0_inst/lock_o" 
sbp_connect_net "$projectName/spi_ctrl_inst/mosi_o" "$projectName/mosi_o" 
sbp_connect_net "$projectName/spi_ctrl_inst/sclk_o" "$projectName/s_clk" 
sbp_connect_net "$projectName/uart0_inst/txd_o" "$projectName/txd_o" 
sbp_connect_net "$projectName/gpio0_inst/gpio_o" "$projectName/led_o" 
sbp_connect_net "$projectName/spi_ctrl_inst/ssn_o" "$projectName/ssn_o" 

sbp_connect_interface_net "$projectName/ahbl0_inst/AHBL_M00" "$projectName/sysmem0_inst/AHBL_S1" 
sbp_connect_interface_net "$projectName/ahbl0_inst/AHBL_M01" "$projectName/ahbl2apb0_inst/AHBL_S0" 
sbp_connect_interface_net "$projectName/ahbl2apb0_inst/APB_M0" "$projectName/apb0_inst/APB_S00" 
sbp_connect_interface_net "$projectName/apb0_inst/APB_M00" "$projectName/uart0_inst/APB_S0" 
sbp_connect_interface_net "$projectName/apb0_inst/APB_M01" "$projectName/gpio0_inst/APB_S0" 
sbp_connect_interface_net "$projectName/apb0_inst/APB_M02" "$projectName/spi_ctrl_inst/APB_S0" 
sbp_connect_interface_net "$projectName/apb0_inst/APB_M03" "$projectName/i2c_ctrl_inst/APB_S0" 
sbp_connect_interface_net "$projectName/cpu0_inst/AHBL_M0_INSTR" "$projectName/sysmem0_inst/AHBL_S0" 
sbp_connect_interface_net "$projectName/cpu0_inst/AHBL_M1_DATA" "$projectName/ahbl0_inst/AHBL_S00" 
sbp_connect_interface_net "$projectName/gpio0_inst/INTR" "$projectName/cpu0_inst/IRQ_S1" 
sbp_connect_interface_net "$projectName/i2c_ctrl_inst/INTR" "$projectName/cpu0_inst/IRQ_S3" 
sbp_connect_interface_net "$projectName/spi_ctrl_inst/INTR" "$projectName/cpu0_inst/IRQ_S2" 
sbp_connect_interface_net "$projectName/uart0_inst/INT_M0" "$projectName/cpu0_inst/IRQ_S0" 



sbp_assign_addr_seg -offset 'h00008000 "$projectName/apb0_inst/APB_M00" "$projectName/uart0_inst/APB_S0"
sbp_assign_addr_seg -offset 'h00008400 "$projectName/apb0_inst/APB_M01" "$projectName/gpio0_inst/APB_S0"
sbp_assign_addr_seg -offset 'h00008800 "$projectName/apb0_inst/APB_M02" "$projectName/spi_ctrl_inst/APB_S0"
sbp_assign_addr_seg -offset 'h00008c00 "$projectName/apb0_inst/APB_M03" "$projectName/i2c_ctrl_inst/APB_S0"
sbp_assign_addr_seg -offset 'h00000000 "$projectName/cpu0_inst/AHBL_M0_INSTR" "$projectName/sysmem0_inst/AHBL_S0"

 
sbp_design save
sbp_design generate


