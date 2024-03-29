import cocotb
from cocotb.triggers import Timer
import tkinter as tk
from datetime import timedelta
from cocotb_gui import *

led_count  = 16
led_radius = 10
led_spacing = 5

sw_count         = led_count
sw_width         = 2 * led_radius
sw_height        = 30
sw_cursor_height = 16
sw_spacing       = led_spacing

class BargraphGui(TbGui):
    def setup(self):
        self.add_canvas(sw_count * (sw_width + sw_spacing) + sw_spacing, 2 * (led_radius + led_spacing) + sw_height + 2 * sw_spacing, tk.BOTTOM)
        self.add_leds(led_count, led_radius, 0, 0, led_spacing)
        self.add_switches(sw_count, sw_width, sw_height, sw_cursor_height, 0, (led_spacing + led_radius) * 2, sw_spacing)

@cocotb.test()
def run(dut):
    gui = BargraphGui(dut)

    while not gui.closed:
        yield Timer(10, "ms")
        gui.update()
