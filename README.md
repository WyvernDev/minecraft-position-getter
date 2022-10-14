# PREFACE
I've recently decided to get into MCBE hacking, whipped this up after I played around with MCBE in CE. Consider this a POC.

# What is does?
This program will get the current position of the local player and outputs it to the console.

# How is works?
This works by first getting the address of the player object, then offsetting it by the position offset, and reading that address as a vec3.


