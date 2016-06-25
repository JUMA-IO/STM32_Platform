HTS221 micropython I2C drive for CANNON board

The HTS221 is an ultra compact sensor for relative humidity and temperature. It includes a sensing element and a mixed signal ASIC to provide the measurement information through digital serial interfaces.

The sensing element consists of a polymer dielectric planar capacitor structure capable of detecting relative humidity variations and is manufactured using a dedicated ST process.

The HTS221 is available in a small top-holed cap land grid array (HLGA) package guaranteed to operate over a temperature range from -40 C to +120 C.

http://www.st.com/content/st_com/en/products/mems-and-sensors/humidity-sensors/hts221.html


Paramters special
=================

Device I2C address: 0x5F


usage
=====

from HTS221 import HTS221
ht = HTS221(1)
ht.get()    # get Temp and Humi

# get Temperature
ht.getTemp()

# get Humidity
ht.getHumi()

# power control
ht.poweroff()
ht.poweron()

# average configuration
ht.av()
ht.av(0x11)

