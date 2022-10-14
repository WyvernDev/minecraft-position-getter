#include <iostream>
#include "utils/mem.hpp"

struct vec3_t { float x, y, z; }; /*data structure for a vec3*/

int main()
{
    SetConsoleTitle("minecraft coords");
    auto mc = process("Minecraft.Windows"); /*initialize a new process class so we can read MC's memory*/
    auto base = mc.get_module_base("Minecraft.Windows"); /*get the base address of MC*/
    auto player_obj = mc.follow_ptr_chain(base.value() + 0x4926308, {0x20, 0, 0x20, 0xc8, 0x180, 0, 0}); /*I got this multi-level pointer chain in CE, using Rake's method*/

    auto pos = mc.read<vec3_t>(player_obj + 0x3f0); /*0x3f0 is the offset from the beginning of the player object to the players position*/

    std::printf(
        "X coord: %f\n"
        "Y coord: %f\n"
        "Z coord: %f\n",
        pos.x,
        pos.y,
        pos.z
    );

    std::cin.get();
}