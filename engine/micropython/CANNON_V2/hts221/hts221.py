"""
File:     hts221.py
Desciipt: HTS221 Humidity and temperature micropython's I2C drive for
          CANNON board.
BOARD:    CANNON V2 board, JUMA inc.
Author:   Shaoziyang, http://www.micro-python.com/
Version:  1.2
DATA:     2016.6

usage:

from HTS221 import HTS221
ht = HTS221(1)
ht.get()

ht.getTemp()
ht.getHumi()
ht.poweroff()
ht.poweron()
"""

from pyb import I2C

# HTS221 I2C address
HTS_I2C_ADDR = const(0x5F)

# HTS221 register mapping
HTS221_WHO_AM_I     = const(0x0F)
HTS221_AV_CONF      = const(0x10)
HTS221_CTRL_REG1    = const(0x20)
HTS221_CTRL_REG2    = const(0x21)
HTS221_CTRL_REG3    = const(0x22)
HTS221_STATUS_REG   = const(0x27)
HTS221_HUMIDITY_OUT_L = const(0x28)
HTS221_HUMIDITY_OUT_H = const(0x29)
HTS221_TEMP_OUT_L   = const(0x2A)
HTS221_TEMP_OUT_H   = const(0x2B)
HTS221_H0_rH_x2     = const(0x30)
HTS221_H1_rH_x2     = const(0x31)
HTS221_T0_degC_x8   = const(0x32)
HTS221_T1_degC_x8   = const(0x33)
HTS221_T1T0_msb     = const(0x35)
HTS221_H0_T0_OUT_L  = const(0x36)
HTS221_H0_T0_OUT_H  = const(0x37)
HTS221_H1_T0_OUT_L  = const(0x3A)
HTS221_H1_T0_OUT_H  = const(0x3B)
HTS221_T0_OUT_L     = const(0x3C)
HTS221_T0_OUT_H     = const(0x3D)
HTS221_T1_OUT_L     = const(0x3E)
HTS221_T1_OUT_H     = const(0x3F)

class HTS221(object):
    def __init__(self, i2c):
        self.i2c = I2C(i2c, I2C.MASTER, baudrate = 100000)
        # HTS221 Temp Calibration registers
        self.T0_OUT = self.get2Reg(HTS221_T0_OUT_L)
        self.T1_OUT = self.get2Reg(HTS221_T1_OUT_L)
        if self.T0_OUT>=0x8000 :
            self.T0_OUT -= 65536
        if self.T1_OUT>=0x8000 :
            self.T1_OUT -= 65536
        t1 = self.getReg(HTS221_T1T0_msb) 
        self.T0_degC = (self.getReg(HTS221_T0_degC_x8) + (t1%4)*256)/8
        self.T1_degC = (self.getReg(HTS221_T1_degC_x8)+ ((t1%16)/4)*256)/8
        # HTS221 Humi Calibration registers
        self.H0_OUT = self.get2Reg(HTS221_H0_T0_OUT_L)
        self.H1_OUT = self.get2Reg(HTS221_H1_T0_OUT_L)
        self.H0_rH = self.getReg(HTS221_H0_rH_x2)/2
        self.H1_rH = self.getReg(HTS221_H1_rH_x2)/2
        # set av conf: T=4 H=8
        self.setReg(0x81, HTS221_AV_CONF)
        # set CTRL_REG1: PD=1 BDU=1 ODR=1
        self.setReg(0x85, HTS221_CTRL_REG1)
    
    def setReg(self, dat, reg):
        buf = bytearray(2)
        buf[0] = reg
        buf[1] = dat
        self.i2c.send(buf, HTS_I2C_ADDR)
        
    def getReg(self, reg):
        self.i2c.send(reg, HTS_I2C_ADDR)
        t = self.i2c.recv(1, HTS_I2C_ADDR)
        return t[0]
    
    def get2Reg(self, reg):
        a = self.getReg(reg)
        b = self.getReg(reg + 1)
        return a + b * 256

    # Device identification
    def WhoAmI(self):
        return self.getReg(HTS221_WHO_AM_I)
        
    # get STATUS regster
    def STATUS(self):
        return self.getReg(HTS221_STATUS_REG)
        
    # power control
    def poweroff(self):
        t = self.getReg(HTS221_CTRL_REG1) & 0x7F
        self.setReg(t, HTS221_CTRL_REG1)

    def poweron(self):
        t = self.getReg(HTS221_CTRL_REG1) | 0x80
        self.setReg(t, HTS221_CTRL_REG1)

    # get/set Output data rate
    def ODR(self, ord=''):
        if ord != '':
            t = self.getReg(HTS221_CTRL_REG1) & 0xFC
            self.setReg(t | ord, HTS221_CTRL_REG1)
        else:
            return self.getReg(HTS221_CTRL_REG1) & 0x03
        
    # get/set Humidity and temperature average configuratio
    def av(self, av=''):
        if av != '':
            self.setReg(av, HTS221_AV_CONF)
        else:
            return self.getReg(HTS221_AV_CONF)

    # calculate Temperature
    def getTemp(self):
        t = self.get2Reg(HTS221_TEMP_OUT_L)
        return self.T0_degC + (self.T1_degC - self.T0_degC) * (t - self.T0_OUT) / (self.T1_OUT - self.T0_OUT)

    # calculate Humidity
    def getHumi(self):
        t = self.get2Reg(HTS221_HUMIDITY_OUT_L)
        return self.H0_rH + (self.H1_rH - self.H0_rH) * (t - self.H0_OUT) / (self.H1_OUT - self.H0_OUT)
        
    # get Humidity and Temperature
    def get(self):
        return [self.getHumi(), self.getTemp()]
