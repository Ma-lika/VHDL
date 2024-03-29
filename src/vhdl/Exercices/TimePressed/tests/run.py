import cocotb
from cocotb.triggers import Timer
import tkinter as tk
from datetime import timedelta
from cocotb_gui import *

led_count  = 16
led_radius = 10
led_spacing = 5

class TimePressedGui(TbGui):
    def setup(self):
        self.add_canvas(led_count * (2 * led_radius + led_spacing) + led_spacing, 2 * (led_radius + led_spacing), tk.TOP)
        self.add_leds(led_count, led_radius, 0, 0, led_spacing)
        self.add_button("btn_center_i", "Press me")
        self.add_clock(Timer(500, "us"), timedelta(milliseconds=10), 10)

@cocotb.test()
def run(dut):
    gui = TimePressedGui(dut)

    while not gui.closed:
        yield Timer(10, "ms")
        gui.update()
