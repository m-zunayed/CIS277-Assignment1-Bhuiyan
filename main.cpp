#include <iostream>
#include <cstring>
#include <iomanip>

#include "MemoryPool.h"

int main()
{
    std::cout << "Network Packet Buffer Pool\n";
    std::cout << "==========================\n\n";

    MemoryPool pool(512, 8);

    std::cout << "Block Size: "
              << pool.blockSize() << " bytes\n";

    std::cout << "Blocks: "
              << pool.availableBlocks() << "\n";

    std::cout << "Total Capacity: "
              << pool.capacity() << " bytes\n\n";

    void* packet1 = pool.allocate();
    void* packet2 = pool.allocate();
    void* packet3 = pool.allocate();

    std::cout << "Packet 1 allocated: " << packet1 << "\n";
    std::cout << "Packet 2 allocated: " << packet2 << "\n";
    std::cout << "Packet 3 allocated: " << packet3 << "\n\n";

    std::cout << "Available blocks: "
              << pool.availableBlocks() << "\n";

    std::cout << "Allocated blocks: "
              << pool.allocatedBlocks() << "\n\n";

    unsigned char packetData[] =
    {
        0x45, 0x00, 0x00, 0x3C,
        0xAB, 0xCD, 0x12, 0x34
    };

    if (packet1 != nullptr &&
        sizeof(packetData) <= pool.blockSize())
    {
        std::memcpy(packet1, packetData, sizeof(packetData));

        std::cout << "Binary packet written to Packet 1.\n";

        unsigned char* storedData =
            static_cast<unsigned char*>(packet1);

        std::cout << "Binary packet read back: ";

        for (size_t i = 0; i < sizeof(packetData); i++)
        {
            std::cout << "0x"
                      << std::hex
                      << std::uppercase
                      << std::setw(2)
                      << std::setfill('0')
                      << static_cast<int>(storedData[i])
                      << " ";
        }

        std::cout << std::dec << "\n\n";
    }

    void* releasedAddress = packet2;

    if (pool.deallocate(packet2))
    {
        std::cout << "Packet 2 released.\n";
    }

    std::cout << "Available blocks: "
              << pool.availableBlocks() << "\n";

    std::cout << "Allocated blocks: "
              << pool.allocatedBlocks() << "\n\n";

    void* packet4 = pool.allocate();

    std::cout << "Packet 4 allocated: "
              << packet4 << "\n";

    if (packet4 == releasedAddress)
    {
        std::cout
            << "Packet 4 reused the previously released block.\n";
    }
    else
    {
        std::cout
            << "Packet 4 did not reuse the expected block.\n";
    }

    std::cout << "\n";

    std::cout << "Attempting to exhaust pool...\n";

    void* extraBlocks[8];
    size_t extraCount = 0;

    while (pool.availableBlocks() > 0)
    {
        void* block = pool.allocate();

        if (block != nullptr)
        {
            extraBlocks[extraCount] = block;
            extraCount++;

            std::cout << "Additional block allocated: "
                      << block << "\n";
        }
    }

    void* failedAllocation = pool.allocate();

    if (failedAllocation == nullptr)
    {
        std::cout << "No blocks available.\n";
        std::cout << "allocate() returned nullptr.\n";
    }

    std::cout << "\n";

    std::cout << "Available blocks: "
              << pool.availableBlocks() << "\n";

    std::cout << "Allocated blocks: "
              << pool.allocatedBlocks() << "\n\n";

    std::cout << "Attempting double deallocation...\n";

    bool firstDeallocation = pool.deallocate(packet4);
    bool secondDeallocation = pool.deallocate(packet4);

    std::cout << "First deallocation: "
              << (firstDeallocation ? "accepted" : "rejected")
              << "\n";

    std::cout << "Second deallocation: "
              << (secondDeallocation ? "accepted" : "rejected")
              << "\n";

    if (!secondDeallocation)
    {
        std::cout << "Double deallocation rejected.\n";
    }

    pool.deallocate(packet1);
    pool.deallocate(packet3);

    for (size_t i = 0; i < extraCount; i++)
    {
        pool.deallocate(extraBlocks[i]);
    }

    std::cout << "\nFinal available blocks: "
              << pool.availableBlocks() << "\n";

    std::cout << "Final allocated blocks: "
              << pool.allocatedBlocks() << "\n";

    return 0;
}