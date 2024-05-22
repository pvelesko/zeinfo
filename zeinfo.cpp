#include <iostream>
#include <vector>
#include <level_zero/ze_api.h>

void checkZeResult(ze_result_t result, const char *function) {
    if (result != ZE_RESULT_SUCCESS) {
        std::cerr << "Error in " << function << ": " << result << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void printDeviceMemoryProperties(const ze_device_handle_t& device) {
    uint32_t memoryCount = 0;
    checkZeResult(zeDeviceGetMemoryProperties(device, &memoryCount, nullptr), "zeDeviceGetMemoryProperties (count)");

    std::vector<ze_device_memory_properties_t> memoryProperties(memoryCount);
    checkZeResult(zeDeviceGetMemoryProperties(device, &memoryCount, memoryProperties.data()), "zeDeviceGetMemoryProperties");

    for (const auto& memory : memoryProperties) {
        std::cout << "Memory Name: " << memory.name << std::endl;
        std::cout << "Memory Max Clock Rate: " << memory.maxClockRate << " MHz" << std::endl;
        std::cout << "Memory Max Bus Width: " << memory.maxBusWidth << " bits" << std::endl;
        std::cout << "Memory Total Size: " << memory.totalSize << " bytes" << std::endl;
    }
}

void printDeviceComputeProperties(const ze_device_handle_t& device) {
    ze_device_compute_properties_t computeProperties = {};
    checkZeResult(zeDeviceGetComputeProperties(device, &computeProperties), "zeDeviceGetComputeProperties");

    std::cout << "Max Total Group Size: " << computeProperties.maxTotalGroupSize << std::endl;
    std::cout << "Max Group Size X: " << computeProperties.maxGroupSizeX << std::endl;
    std::cout << "Max Group Size Y: " << computeProperties.maxGroupSizeY << std::endl;
    std::cout << "Max Group Size Z: " << computeProperties.maxGroupSizeZ << std::endl;
    std::cout << "Max Group Count X: " << computeProperties.maxGroupCountX << std::endl;
    std::cout << "Max Group Count Y: " << computeProperties.maxGroupCountY << std::endl;
    std::cout << "Max Group Count Z: " << computeProperties.maxGroupCountZ << std::endl;
    std::cout << "Max Shared Local Memory: " << computeProperties.maxSharedLocalMemory << " bytes" << std::endl;
    std::cout << "Number of Subgroup Sizes: " << computeProperties.numSubGroupSizes << std::endl;
    for (uint32_t i = 0; i < computeProperties.numSubGroupSizes; ++i) {
        std::cout << "Subgroup Size " << i << ": " << computeProperties.subGroupSizes[i] << std::endl;
    }
}

void printDriverInfo(const ze_driver_handle_t& driver) {
    ze_driver_properties_t driverProperties = {};
    checkZeResult(zeDriverGetProperties(driver, &driverProperties), "zeDriverGetProperties");

    std::cout << "Driver Version: " << driverProperties.driverVersion << std::endl;
    std::cout << "API Version: " << driverProperties.driverVersion << std::endl;

    uint32_t deviceCount = 0;
    checkZeResult(zeDeviceGet(driver, &deviceCount, nullptr), "zeDeviceGet (count)");

    std::vector<ze_device_handle_t> devices(deviceCount);
    checkZeResult(zeDeviceGet(driver, &deviceCount, devices.data()), "zeDeviceGet");

    for (const auto& device : devices) {
        ze_device_properties_t deviceProperties = {};
        checkZeResult(zeDeviceGetProperties(device, &deviceProperties), "zeDeviceGetProperties");

        std::cout << "Device Name: " << deviceProperties.name << std::endl;
        std::cout << "Device Type: " << static_cast<int>(deviceProperties.type) << std::endl;
        std::cout << "Device Vendor ID: " << deviceProperties.vendorId << std::endl;
        std::cout << "Device ID: " << deviceProperties.deviceId << std::endl;
        std::cout << "Device UUID: ";
        for (auto id : deviceProperties.uuid.id) {
            std::cout << std::hex << static_cast<int>(id);
        }
        std::cout << std::dec << std::endl;
        std::cout << "Device Core Clock Rate: " << deviceProperties.coreClockRate << " MHz" << std::endl;
        std::cout << "Max Memory Allocation Size: " << deviceProperties.maxMemAllocSize << " bytes" << std::endl;
        std::cout << "Max Hardware Contexts: " << deviceProperties.maxHardwareContexts << std::endl;
        std::cout << "Max Command Queue Priority: " << deviceProperties.maxCommandQueuePriority << std::endl;
        std::cout << "Number of Threads per EU: " << deviceProperties.numThreadsPerEU << std::endl;
        std::cout << "Physical EU SIMD Width: " << deviceProperties.physicalEUSimdWidth << std::endl;
        std::cout << "Number of EUs per Subslice: " << deviceProperties.numEUsPerSubslice << std::endl;
        std::cout << "Number of Subslices per Slice: " << deviceProperties.numSubslicesPerSlice << std::endl;
        std::cout << "Number of Slices: " << deviceProperties.numSlices << std::endl;
        std::cout << "Timer Resolution: " << deviceProperties.timerResolution << " ns" << std::endl;
        std::cout << "Timestamp Valid Bits: " << deviceProperties.timestampValidBits << std::endl;
        std::cout << "Kernel Timestamp Valid Bits: " << deviceProperties.kernelTimestampValidBits << std::endl;
        std::cout << "\n\n";

        printDeviceMemoryProperties(device);
        printDeviceComputeProperties(device);
    }
}

int main() {
    checkZeResult(zeInit(ZE_INIT_FLAG_GPU_ONLY), "zeInit");

    uint32_t driverCount = 0;
    checkZeResult(zeDriverGet(&driverCount, nullptr), "zeDriverGet (count)");

    std::vector<ze_driver_handle_t> drivers(driverCount);
    checkZeResult(zeDriverGet(&driverCount, drivers.data()), "zeDriverGet");

    for (const auto& driver : drivers) {
        printDriverInfo(driver);
    }

    return 0;
}