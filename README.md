### load firmware using edbg
```
/Users/jcobb/dev/edbg/edbg -bpv -t atmel_cm4 -f ~/dev/projects/sam4s_quad/sam4s_quad.bin
```

### cli commands:

####
```
mdoem:
navigate to sam4s_telit
exec make

command line:
sudo -cu -l /dev/device_name -s 115200
Issue ATE1 to turn echo on
Use AT commands to communicate with modem
~. to escape cu

```