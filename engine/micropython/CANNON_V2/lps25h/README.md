LPS25H micropython I2C drive for CANNON board

The LPS25H is an ultra compact absolute piezoresistive pressure sensor. It includes a monolithic sensing element and an IC interface able to take the information from the sensing element and to provide a digital signal to the external world.

http://www.st.com/content/st_com/en/products/mems-and-sensors/pressure-sensors/lps25h.html?icmp=pf255230_pron_pr_feb2014


Device I2C address

0x5D (SA0 = 1)


usage
=====

from LPS25H import LPS25H
lps25=LPS25H(1)

lps25=LPS25H(1, 1)

lps25.TEMP()
lps25.PRESS()

