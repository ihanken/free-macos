#include <cstdio>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <cmath>
#include <iostream>

using namespace std;

void getMem(long long &tMem, long long &fMem, long long &aMem, long long &iMem, long long &wMem) {
    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;

    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);

    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                                            (host_info64_t) &vm_stats, &count)) {

        long long pageSize = (int64_t) page_size;

        fMem = vm_stats.free_count * pageSize;
        aMem = vm_stats.active_count * pageSize;
        iMem = vm_stats.inactive_count * pageSize;
        wMem = vm_stats.wire_count * pageSize;
        tMem = (fMem + aMem + iMem + wMem);
    }
}

void getSwap(long long &totalSwapMemory, long long &freeSwapMemory, long long &usedSwapMemory) {
    xsw_usage vmusage = {0};
    size_t size = sizeof(vmusage);

    if (sysctlbyname("vm.swapusage", &vmusage, &size, NULL, 0) !=0)
        perror("unable to get swap usage");
    else {
        totalSwapMemory = vmusage.xsu_total;
        usedSwapMemory = vmusage.xsu_used;
        freeSwapMemory = vmusage.xsu_total - vmusage.xsu_used;
    }
}

double getKB(long long bytes) { return (double) bytes / 1024.0; }
double getMB(long long bytes) { return (double) bytes / pow(1024.0, 2); }
double getGB(long long bytes) { return (double) bytes / pow(1024.0, 3); }
double getTB(long long bytes) { return (double) bytes / pow(1024.0, 4); }

// Print the column headings.
void printHeadings() {
    printf("%17s      ", "Total"); // Total Memory
    printf("%11s      ", "Free"); // Free Memory
    printf("%11s      ", "Active"); // Active Memory
    printf("%11s      ", "Inactive"); // Inactive Memory
    printf("%11s   \n", "Wired"); // Wired Memory
}

/* MEM Functions */
// Print the memory in bytes.
void printBytes(long long tMem, long long fMem, long long aMem, long long iMem, long long wMem) {
    printHeadings();
    printf("Mem:  %11lld Bytes", tMem); // Total Mempory
    printf("%11lld Bytes", fMem); // Free Memory
    printf("%11lld Bytes", aMem); // Active Memory
    printf("%11lld Bytes", iMem); // Inactive Memory
    printf("%11lld Bytes\n", wMem); // Wired Memory
}

// Print the memory in kilobytes.
void printKB(long long tMem, long long fMem, long long aMem, long long iMem, long long wMem) {
    printHeadings();
    printf("Mem:  %11.2f KB   ", getKB(tMem)); // Total Mempory
    printf("%11.2f KB   ", getKB(fMem)); // Free Mempory
    printf("%11.2f KB   ", getKB(aMem)); // Active Mempory
    printf("%11.2f KB   ", getKB(iMem)); // Inactive Mempory
    printf("%11.2f KB   \n", getKB(wMem)); // Wired Mempory
}

// Print the memory in megabytes.
void printMB(long long tMem, long long fMem, long long aMem, long long iMem, long long wMem) {
    printHeadings();
    printf("Mem:  %11.3f MB   ", getMB(tMem)); // Total Mempory
    printf("%11.3f MB   ", getMB(fMem)); // Free Mempory
    printf("%11.3f MB   ", getMB(aMem)); // Active Mempory
    printf("%11.3f MB   ", getMB(iMem)); // Inactive Mempory
    printf("%11.3f MB   \n", getMB(wMem)); // Wired Mempory
}

// Print the memory in gigabytes.
void printGB(long long tMem, long long fMem, long long aMem, long long iMem, long long wMem) {
    printHeadings();
    printf("Mem:  %11.4f GB   ", getGB(tMem)); // Total Mempory
    printf("%11.4f GB   ", getGB(fMem)); // Free Mempory
    printf("%11.4f GB   ", getGB(aMem)); // Active Mempory
    printf("%11.4f GB   ", getGB(iMem)); // Inactive Mempory
    printf("%11.4f GB   \n", getGB(wMem)); // Wired Mempory
}

// Print the memory in terabytes.
void printTB(long long tMem, long long fMem, long long aMem, long long iMem, long long wMem) {
    printHeadings();
    printf("Mem:  %11.6f TB   ", getTB(tMem)); // Total Mempory
    printf("%11.6f TB   ", getTB(fMem)); // Free Mempory
    printf("%11.6f TB   ", getTB(aMem)); // Active Mempory
    printf("%11.6f TB   ", getTB(iMem)); // Inactive Mempory
    printf("%11.6f TB   \n", getTB(wMem)); // Wired Mempory
}

/* SWAP functions */
// Print the memory in bytes.
void printBytes(long long tMem, long long fMem, long long aMem) {
    printf("Swap: %11lld Bytes", tMem); // Total Mempory
    printf("%11lld Bytes", fMem); // Free Memory
    printf("%11lld Bytes\n", aMem); // Active Memory
}

// Print the memory in kilobytes.
void printKB(long long tMem, long long fMem, long long aMem) {
    printf("Swap: %11.2f KB   ", getKB(tMem)); // Total Mempory
    printf("%11.2f KB   ", getKB(fMem)); // Free Mempory
    printf("%11.2f KB   \n", getKB(aMem)); // Active Mempory
}

// Print the memory in megabytes.
void printMB(long long tMem, long long fMem, long long aMem) {
    printf("Swap: %11.3f MB   ", getMB(tMem)); // Total Mempory
    printf("%11.3f MB   ", getMB(fMem)); // Free Mempory
    printf("%11.3f MB   \n", getMB(aMem)); // Active Mempory
}

// Print the memory in gigabytes.
void printGB(long long tMem, long long fMem, long long aMem) {
    printf("Swap: %11.4f GB   ", getGB(tMem)); // Total Mempory
    printf("%11.4f GB   ", getGB(fMem)); // Free Mempory
    printf("%11.4f GB   \n", getGB(aMem)); // Active Mempory
}

// Print the memory in terabytes.
void printTB(long long tMem, long long fMem, long long aMem) {
    printf("Swap: %11.6f TB   ", getTB(tMem)); // Total Mempory
    printf("%11.6f TB   ", getTB(fMem)); // Free Mempory
    printf("%11.6f TB   \n", getTB(aMem)); // Active Mempory
}

int main(int argc, const char *argv[]) {

    long long freeMemory, activeMemory, inactiveMemory, wiredMemory,
                totalMemory, totalSwapMemory, freeSwapMemory, usedSwapMemory;

    getMem(totalMemory, activeMemory, inactiveMemory, freeMemory, wiredMemory);
    getSwap(totalSwapMemory, freeSwapMemory, usedSwapMemory);



    string lastArg = argv[argc - 1];

    if (argc == 1 || lastArg == "-b" || lastArg == "--bytes") {
        printBytes(totalMemory, activeMemory, inactiveMemory, freeMemory, wiredMemory);
        printBytes(totalSwapMemory, freeSwapMemory, usedSwapMemory);
    }
    else if (lastArg == "-k" || lastArg == "--kilo") {
        printKB(totalMemory, activeMemory, inactiveMemory, freeMemory, wiredMemory);
        printKB(totalSwapMemory, freeSwapMemory, usedSwapMemory);
    }
    else if (lastArg == "-m" || lastArg == "--mega") {
        printMB(totalMemory, activeMemory, inactiveMemory, freeMemory, wiredMemory);
        printMB(totalSwapMemory, freeSwapMemory, usedSwapMemory);
    }
    else if (lastArg == "-g" || lastArg == "--giga") {
        printGB(totalMemory, activeMemory, inactiveMemory, freeMemory, wiredMemory);
        printGB(totalSwapMemory, freeSwapMemory, usedSwapMemory);
    }
    else if (lastArg == "-t" || lastArg == "--tera") {
        printTB(totalMemory, activeMemory, inactiveMemory, freeMemory, wiredMemory);
        printTB(totalSwapMemory, freeSwapMemory, usedSwapMemory);
    }
    else {
        printf("Invalid final argument supplied.\n");
        exit(0);
    }

    return 0;
}
