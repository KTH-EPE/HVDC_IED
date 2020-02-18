# IED building manual

This manual shows how to build a laboratory protection intelligent electronic device (IED) compatible with common real-time digital simulators and studies for high-voltage direct-current (HVDC) grids. The device was built as part of the PROMOTioN research project, which aims to develop meshed HVDC offshore grids on the basis of cost-effective and reliable technological innovation. In the picture below you can see the finalized look of the relay. 
<div style="text-align:center"><img src="./IED_images/final_look.png" width="300x">  
</div>

## Initial set up 
The building process starts with assembling the physical box, which is going to contain the prototype. 
<div style="text-align:center"><img src="./IED_images/box_parts.png" width="300x">  
</div>
Before assembling the parts together, it is important to position the components on the bottom panel and drill the holes for mounting the Zedboard, FMC and a power supply. 
<div style="text-align:center"><img src="./IED_images/placement.png" width="300x">  
</div>
Drill the holes in respective locations. For mounting the board and the power supply, holes need to be 3mm in diameter. 2 holes for FMC should be 2mm. 
<div style="text-align:center"><img src="./IED_images/holes2.png" width="300x">  
</div>
After the holes are done, screw the boards to the bottom panel. 
Take the top screws out of the 4 couplings of the Zedboard and screw them into the panel from the bottom. 

<div style="text-align:center"><img src="./IED_images/zed_screws.png" width="300x">  
</div>
On top of the screws attach the coupling nuts.
<div style="text-align:center"><img src="./IED_images/couplings.png" width="300x">  
</div>
Put long 2 mm screws into the 2 smaller holes and fixate ones with nuts to attach the FMC board in the future.
<!--Put an image here-->
Proceed to mounting the zedboard using 3mm machine screws and nuts.

<div style="text-align:center"><img src="./IED_images/board_mount.png" width="300x">  
</div>
## Power supply 
<div style="text-align:center"><img src="./IED_images/power_supply_white.png" width="300x">  
</div>
To connect the switched-mode power supply we will need to install wire-to-board terminal blocks. First, remove the white terminal blocks in place with some help of pliers and a soldering machine. 
<div style="text-align:center"><img src="./IED_images/remove_white_pins.png" width="300x">  
</div>
Solder the terminal blocks in the same position as the removed terminal blocks.
<div style="text-align:center"><img src="./IED_images/power_supply2.png" width="300x">  
</div>
The power supply board is ready. Use 3mm machine screws to connect female-to-male space bolts to the bottom panel.
<div style="text-align:center"><img src="./IED_images/screws_power.png" width="300x">  
</div>
<div style="text-align:center"><img src="./IED_images/spacers_power.png" width="300x">  
</div>
The power supply is plugged in the socket with a power cable. Cut the cable and remove the insulation, there has to be 3 wires: positive, negative and ground.
<div style="text-align:center"><img src="./IED_images/power_cable.png" width="300x">  
</div>
<div style="text-align:center"><img src="./IED_images/power_cable_final.png" width="300x">  
</div>
The switched-mode supply feeds power at 5 V voltage to the board. We connect the zedboard using the standard power adapter provided in the board kit. We need to connect the power cable directly to the wires in the power adapter. To do that cut a window in the plastic shell of the adapter.
<div style="text-align:center"><img src="./IED_images/plastic_window.png" width="300x">  
</div>
The default charger wire would be too long, so it is also advisable to shorten it, i.e. cut the middle, remove the insulation on the ends and solder them together.
<div style="text-align:center"><img src="./IED_images/shortened.png" width="300x">  
</div>
Before soldering the wires make sure there are shrink tubes of two sizes for both conductor and drain wires. The shrink tubes will provide insulation between core, drain wire and the environment. 
<div style="text-align:center"><img src="./IED_images/shortened_cable.png" width="300x">  
</div>
<div style="text-align:center"><img src="./IED_images/shortened_shrinked.png" width="300x">  
</div>
The charger is shortened.
<div style="text-align:center"><img src="./IED_images/shortened_adaptor.png" width="300x">  
</div>
The 230V side of the adaptor will connect these cables, which we have prepared bare ends of.
<div style="text-align:center"><img src="./IED_images/power_wires.png" width="300x">  
</div>
Connect them to the feed supply terminals in the following manner. 
<div style="text-align:center"><img src="./IED_images/power_supply_feed.png" width="300x">  
</div>
We add an extra earth cable bind connected to the metal case of an appliance. Here, we used a crimper to connect the ring to the cable.

<div style="text-align:center"><img src="./IED_images/earth_ring_cable.jpg" width="500x"> 
</div>

 The ring is kept in place with a bolt and screw to the bottom edge of the box. 

<div style="text-align:center"><img src="./IED_images/earth_ring.jpg" width="500x"> 
</div>

The power supply block is covered with a shield cut from plexiglas. The shield is attached with 4 machine screws into female spacers. 

<div style="text-align:center"><img src="./IED_images/power_supply_shield.jpg" width="500x"> 
</div>



## Box
Put the sticker on the front panel. The best way to do it is fixating the sticker with a duct tape and removing the backing paper gradually. 6
<div style="text-align:center"><img src="./IED_images/sticker.png" width="300x">  
</div>
Construct the rack enclosure, putting the parts together with screws included in the kit
<div style="text-align:center"><img src="./IED_images/box_assemble.png" width="300x">  
</div>
Put the power cable through a cable gland on the outside the box and a lock nut from the inside. Fixate the power cable at the back panel.
<div style="text-align:center"><img src="./IED_images/power_backside.png" width="300x">  
</div>
Mount the USB and ethernet connectors, 8 and 10 pin connectors on the back panel. Put the labels to the corresponding ports. 
<div style="text-align:center"><img src="./IED_images/labels_back.png" width="500x">  
</div>

Attach the USB connector and LED lights to the front panel.

<div style="text-align:center"><img src="./IED_images/front_panel.png" width="500x">  
</div>

A large LED is screwed to the panel with a nut.

<div style="text-align:center"><img src="./IED_images/LED_main_nut.jpg" width="500x">  
</div>



## Led Connector

We need to make an LED connector board to link the Zedboard with the frontpanel LEDs. See the schematic and breadboard view.

<div style="text-align:center"><img src="./IED_images/LED_connector_schem.png" width="700x">  
</div>

<div style="text-align:center"><img src="./IED_images/LED_connector_bb.png" width="300x">  
</div>



The connector incorporates a blank pcb board, eight 50 Ohm resistors, two sets of pin headers and two sets of pin sockets. 



<div style="text-align:center"><img src="./IED_images/LED_connector.png" width="300x">  
</div>
The rightmost pins and sockets on the picture are connected with bits of metallic wire. 
<div style="text-align:center"><img src="./IED_images/LED_resistors.png" width="300x">  
</div>
It is convenient to solder the resistor legs to the sockets on one side and to the ground on the other side 
<div style="text-align:center"><img src="./IED_images/LED_resistors_after.png" width="300x">  
</div>
Once connector is finalized, we can start connecting the LEDs.
<div style="text-align:center"><img src="./IED_images/LED_connect_start.png" width="300x">  
</div>
<div style="text-align:center"><img src="./IED_images/LED_connect.png" width="300x">  
</div>

Connect the 8 long pins to the connector on the board. The left-most wire (red) connect to JC1_N. Next one links to JC1_P. Third pin links to JC2_N. Fourth pin links to JC2_P. Next four reiterate in the same arrangement. 
<div style="text-align:center"><img src="./IED_images/LED_here.png" width="300x">  
</div>
<div style="text-align:center"><img src="./IED_images/LED_here_wires.png" width="300x">  
</div>
We will only need two of the ground pins, which we connect to the GND terminal pins at the PMOD port.

<div style="text-align:center"><img src="./IED_images/LED_GND2.jpg" width="500x">  
</div>

Some of the pins may be left idle. Here we connect some idle wires for safety. 

<div style="text-align:center"><img src="./IED_images/LED_GND_idle_edited.jpg" width="500x">  
</div>

## Switches

Solder pin headers (6 mm - 3.1 mm) to the switches. Bend the shorter end pins to fit into the terminals of the switches. 

<div style="text-align:center"><img src="./IED_images/switch_1.jpg" width="300x">  
</div>

<div style="text-align:center"><img src="./IED_images/switch_2.jpg" width="300x">  
</div>

Plug the switches onto the front panel and attach them with screws. 

<div style="text-align:center"><img src="./IED_images/switches_front.png" width="300x">  
</div>

The right terminals are connected to the UCC port in one of the PMOD ports.

<div style="text-align:center"><img src="./IED_images/switches_ucc2.jpg" width="300x">  
</div>

## Power supply wiring 

To connect the devices on the 5V side we will solder multiple wires jointly in the following arrangement. Strip the MTE cables on the "input" and "output" bundles. Before soldering you can intertwine the stranded wires or wrap the bundle with a separate strand. 

<div style="text-align:center"><img src="./IED_images/power_5V_wires.png" width="500x">  
</div>

Keep in mind to keep the shrink tubes or some other type of insulation over the joints. Proceed to soldering.  

<div style="text-align:center"><img src="./IED_images/power_5V_wires_tube2.jpg" width="300x">  
</div>

For setting the power supply you will need the new-made distribution joint plus an additional wire for the positive terminal. Here we used an MTE cable stripped on one end.

<div style="text-align:center"><img src="./IED_images/power_5V_wires_red3.jpg" width="500x">  
</div>

Connect the wires to the power supply side.

<div style="text-align:center"><img src="./IED_images/power_5V_wires_input2.jpg" width="300x">  
</div>

Connect the other end to the pins on the top switch.

<div style="text-align:center"><img src="./IED_images/power_5V_wires_output.jpg" width="500x">  
</div>

Connect the power supply to strands on the big LED. Spare the terminals with some insulating tape. 

<div style="text-align:center"><img src="./IED_images/LED_power.jpg" width="500x">  
</div>

## USB peripherals

Use micro USB to USB cables to connect the outside ports to nests on the board. The USB connector at the front is used for data output and setting configuration. It is connected via the UART terminal at the board. The USB peripheral at the back is used for programming the device and connects at the terminal next to the terminal for power supply. 

<div style="text-align:center"><img src="./IED_images/usb1_fixed_edited(1).jpg" width="500x">  
</div>

<div style="text-align:center"><img src="./IED_images/usb2_fixed_edited.jpg" width="500x">  
</div>

  

## PMOD ADC 

The set up is extended with a peripheral module (PMOD) Analog to Digital Converters (ADCs) connected via  headers on the zedboard. 

<div style="text-align:center"><img src="./IED_images/ADC_placement(2).jpg" width="500x"> 
</div>

The module is supplied power from the power supply module with an MTE 2-pin cable.

<div style="text-align:center"><img src="./IED_images/ADC_power.jpg" width="500x">  
</div>

Prepare 8 ground wires together in a pin header. 

<div style="text-align:center"><img src="./IED_images/ground_pins.jpg" width="500x">  
</div>

Solder the pins to a metal wire in order to route them into IED's GND interface pin.

<div style="text-align:center"><img src="./IED_images/ground_pins_wire.jpg" width="500x"> 
</div>

<div style="text-align:center"><img src="./IED_images/ground_pins_wire_port.jpg" width="500x"> 
</div>

 The ADC module interfaces with IED's 3.3 V logical pins on the back of the box. 

<div style="text-align:center"><img src="./IED_images/ADC1_ports_wire.jpg" width="500x"> 
</div>

The wires are plugged onto metal wire pieces

<div style="text-align:center"><img src="./IED_images/ADC1_pins_closer2.jpg" width="500x"> 
</div>

In case the board is placed far enough from the back panel, we might need to use jointed wires extended by a pin header. Here we manage to reach the ports with one wire length and metal wire pieces 4-5 cm in length. 

<div style="text-align:center"><img src="./IED_images/ADC_1_wire.jpg" width="500x"> 
</div>

In case there is no enough length, an alternative would be stacking two wires together with a pin header.

<div style="text-align:center"><img src="./IED_images/ADC_2_wires.jpg" width="500x"> 
</div>

The wires have the following arrangement at the connector periphery side

<div style="text-align:center"><img src="./IED_images/ADC_wires1.png" width="300x">  
                          </div>

The order from left to right repeats at the ADC side from bottom to top. The signal inputs are intertwined with ground wires.

<div style="text-align:center"><img src="./IED_images/ADC_pins_order1.jpg" width="500x"> 
</div>

## PMOD ADC 2

The PMOD 2x6 headers are located too close together, so that the second ADC module cannot be placed vertically. We solve this by using an extender.

<div style="text-align:center"><img src="./IED_images/ADC_extender.jpg" width="500x"> 
</div>

Plug in the extender and connect the power supply with an MTE cable in the same arrangement as the first module (red wire closer to the middle of the module). The module connects to the board via an extender plugged in JA headers. 

<div style="text-align:center"><img src="./IED_images/ADC2_power(3).jpg" width="500x"> 
</div>

Connect the input pins on the periphery to the 1x16 pin header on the ADC module in the order identical to the first module. The signal pins correspond with the analog input connectors 9 -16. 

<div style="text-align:center"><img src="./IED_images/ADC2_pins_order(1).jpg" width="500x"> 
</div>

Similarly to ADC 1, all ground wires are intertwined with the signal wires when connected to the ADC pin header. The other ends of ground wires are soldered together to a piece of wire, which is plugged to a GND connector on the periphery. 

<div style="text-align:center"><img src="./IED_images/ADC2_pins_order_gnd.jpg" width="500x"> 
</div>

## PMOD DIGITAL INPUT
The digital input module enables us to incorporate more input signals and transmit them to the board. It is used to receive digital values from a simulator (RTDS). The module looks like this

<div style="text-align:center"><img src="./IED_images/PMOD_DIN.jpg" width="500x"> 
</div>

We plug in the module to the top row of a JD header on the Zedboard.

<div style="text-align:center"><img src="./IED_images/DIN_placement.jpg" width="500x"> 
</div>

We start connecting the module input pins to the digital input ports on the periphery. The bottom pin corresponds to port 11 and the top pin corresponds to port 24.

<div style="text-align:center"><img src="./IED_images/DIN_pins11_24.jpg" width="500x"> 
</div>

The sliders on the front panel are also connected via the Digital Input (DIGIN) PMOD module. Connect the middle pin of the bottom two pitches to the two bottom left pins of the module. The lowermost pin on the module is connected to the middle switch. Third pin in the row is connected to the bottom switch.

 The inputs 1-10 are connected to the pins on the left row of the DIGIN module. The rightmost digital input port 1 on the back panel links to the third pin from the bottom.  

<div style="text-align:center"><img src="./IED_images/DIN_pins(2)1_10.jpg" width="500x"> 
</div>

The top two pins are kept idle. Here we still attach two wires yet keep ones insulated.

<div style="text-align:center"><img src="./IED_images/DIN_2idle(2).jpg" width="500x"> 
</div>

## PMOD DIGITAL OUT

The second PMOD is used to send digital 5V output signals to RTDS. It converts Zedboard's 3.3 V logical pin to 5 V specification. The digital out (DIGOUT) module layout is slightly different from the DIN. 

<div style="text-align:center"><img src="./IED_images/DOUT_module.jpg" width="500x"> 
</div>

Both input and output PMODs cannot be plugged into 2x6 header simultaneously. The DIGOUT module is connected via a self-made extender. 

<div style="text-align:center"><img src="./IED_images/DOUT_extender.jpg" width="500x"> 
</div>

The module is connected in the following arrangement.

<div style="text-align:center"><img src="./IED_images/DOUT_supply(1).jpg" width="500x"> 
</div>

On the other end it is connected via the bottom row of the JD 2x6 header on the board. 

<div style="text-align:center"><img src="./IED_images/DOUT_boardside.jpg" width="500x"> 
</div>

The power is supply via the pins in the middle.

<div style="text-align:center"><img src="./IED_images/DOUT_power.jpg" width="500x"> 
</div>

Digital output ports 1-10 are wired to the respective pins on the bottom row of the DIGOUT PMOD module. 

<div style="text-align:center"><img src="./IED_images/DOUT_pins1-10.jpg" width="500x"> 
</div>

Complete the row on he module with the connections to digital output ports 11-14

<div style="text-align:center"><img src="./IED_images/DOUT_pins11-14.jpg" width="500x"> 
</div>

Pins 15-28 of digital output ports connect to PMOD from right to left of the second row.

<div style="text-align:center"><img src="./IED_images/DOUT_pins15-28(1).jpg" width="500x"> 
</div>

The ground wires are connected from both sides and lead to GND output ports. 

<div style="text-align:center"><img src="./IED_images/DOUT_ground.jpg" width="500x"> 
</div>

## USB EARTHING CABLE 

Tests of the devise have indicated that the data/configuration cable creates disruptions with signal readings  when connected to the IED. It is recommended to add additional ground wire to the USB port at the front. The wire is tucked between the box walls and the bottom sheet. 

<div style="text-align:center"><img src="./IED_images/USB_earthing.jpg" width="500x"> 
</div>