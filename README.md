![alt text](https://cdn.upload.systems/uploads/SP5Mhc5t.png)

# PREFACE
I've recently decided to get into MCBE hacking, whipped this up after I played around with MCBE in CE. Consider this a POC.

# What is does?
This program will get the current position of the local player and outputs it to the console.

# How to compile?
#### MAKE SURE YOU'RE COMPILING ON X64
![alt text](https://cdn.upload.systems/uploads/v6ujwvab.png)
#### MAKE SURE YOU'RE COMPILING ON C++23 ATLEAST
![alt text](https://cdn.upload.systems/uploads/BiDz8rZm.png)
#### MAKE SURE YOU HAVE MULTI-BYTE CHARACTER SET SELECTED
![alt text](https://cdn.upload.systems/uploads/jisHVTja.png)

# How is works?
This works by first getting the address of the player object, then offsetting it by the position offset, and reading that address as a vec3.
### How you is figure this out?
It was simple really, first I created a new world and turned on "Show coordinates", then scanned for my X coordinate, moved and re-scanned, repeated until I was left with the correct value.
Then I clicked the value clicked "Find out what accesses this address" and got this:

![alt text](https://cdn.upload.systems/uploads/4hz67aBb.png)

From my intuition I knew that the offset was `0x3F0` and that the registers that were being offsetted using this and then de-referenced were probably the player object address.

So I then got the value of these registers using CE, and used Rake's pointer scanning method.
Which left me with about 1k pointers still, so I just added a couple of them to my cheat table and restarted Minecraft.
I actually had a shit ton of working pointer chains, so I just picked the first one and put it into my source.
