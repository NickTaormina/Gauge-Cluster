# Gauge-Cluster
A QT-based gauge cluster for 15+ WRXs with a Cobb tuned ECU.

Only tested on a 2018 usdm using cobb v361-based map.

![cluster](https://user-images.githubusercontent.com/13877614/193724010-3b9d3e27-4816-4ce6-b1d8-dbe20b11d257.jpg)


------Hardware-----

-Requires a direct can connection (see: https://github.com/NickTaormina/ESP32-CAN). 18+ wrxs cannot use the obd2 port

-Lg la123 or similar. Its a 12.3" 1920x720 display used in oem digital clusters. I got a vdisplay version from amazon that came with a controller board.

------Planned features------

-linux compatitbility

-logging

-real time fuel economy

-shift light

-multiple gauge layouts

-navigation

-music 

-dtc 

-on the fly map switching with connection to tgv

-much more


*i've never used c++ or qt before, so everything is very sloppy*
