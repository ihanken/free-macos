#include <cstdio>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <cmath>

double getKB(long long bytes) { return (double) bytes / 1024.0; }
double getMB(long long bytes) { return (double) bytes / pow(1024.0, 2); }
double getGB(long long bytes) { return (double) bytes / pow(1024.0, 3); }
double getTB(long long bytes) { return (double) bytes / pow(1024.0, 4); }

// Print the column headings.
void printHeadings() {
    printf("%14s   ", "Total"); // Total Memory
    printf("%14s   ", "Free"); // Free Memory
    printf("%14s   ", "Active"); // Active Memory
    printf("%14s   ", "Inactive"); // Inactive Memory
    printf("%14s   \n", "Wired"); // Wired Memory
}

// Print the memory in bytes.
void printBytes(long long tMem, long long fMem, long long aMem, long long iMem, long long wMem) {
    printf("%11lld Bytes", tMem); // Total Mempory
    printf("%11lld Bytes", fMem); // Free Memory
    printf("%11lld Bytes", aMem); // Active Memory
    printf("%11lld Bytes", iMem); // Inactive Memory
    printf("%11lld Bytes\n", wMem); // Wired Memory
}

// Print the memory in kilobytes.
void printKB(long long tMem, long long fMem, long long aMem, long long iMem, long long wMem) {
    printf("%11.2f KB   ", getKB(tMem)); // Total Mempory
    printf("%11.2f KB   ", getKB(fMem)); // Free Mempory
    printf("%11.2f KB   ", getKB(aMem)); // Active Mempory
    printf("%11.2f KB   ", getKB(iMem)); // Inactive Mempory
    printf("%11.2f KB   \n", getKB(wMem)); // Wired Mempory
}

// Print the memory in megabytes.
void printMB(long long tMem, long long fMem, long long aMem, long long iMem, long long wMem) {
    printf("%11.3f MB   ", getMB(tMem)); // Total Mempory
    printf("%11.3f MB   ", getMB(fMem)); // Free Mempory
    printf("%11.3f MB   ", getMB(aMem)); // Active Mempory
    printf("%11.3f MB   ", getMB(iMem)); // Inactive Mempory
    printf("%11.3f MB   \n", getMB(wMem)); // Wired Mempory
}

// Print the memory in gigabytes.
void printGB(long long tMem, long long fMem, long long aMem, long long iMem, long long wMem) {
    printf("%11.4f GB   ", getGB(tMem)); // Total Mempory
    printf("%11.4f GB   ", getGB(fMem)); // Free Mempory
    printf("%11.4f GB   ", getGB(aMem)); // Active Mempory
    printf("%11.4f GB   ", getGB(iMem)); // Inactive Mempory
    printf("%11.4f GB   \n", getGB(wMem)); // Wired Mempory
}

// Print the memory in terabytes.
void printTB(long long tMem, long long fMem, long long aMem, long long iMem, long long wMem) {
    printf("%11.6f TB   ", getTB(tMem)); // Total Mempory
    printf("%11.6f TB   ", getTB(fMem)); // Free Mempory
    printf("%11.6f TB   ", getTB(aMem)); // Active Mempory
    printf("%11.6f TB   ", getTB(iMem)); // Inactive Mempory
    printf("%11.6f TB   \n", getTB(wMem)); // Wired Mempory
}

int main(int argc, const char * argv[]) {
    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;

    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);

    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO, (host_info64_t) &vm_stats, &count)) {

        long long pageSize = (int64_t) page_size;

        long long freeMemory = vm_stats.free_count * pageSize;
        long long activeMemory = vm_stats.active_count * pageSize;
        long long inactiveMemory = vm_stats.inactive_count * pageSize;
        long long wiredMemory = vm_stats.wire_count * pageSize;
        long long totalMemory = (freeMemory + activeMemory +
                                inactiveMemory + wiredMemory);
        printHeadings();
        printBytes(totalMemory, activeMemory, inactiveMemory, freeMemory, wiredMemory);
        printKB(totalMemory, activeMemory, inactiveMemory, freeMemory, wiredMemory);
        printMB(totalMemory, activeMemory, inactiveMemory, freeMemory, wiredMemory);
        printGB(totalMemory, activeMemory, inactiveMemory, freeMemory, wiredMemory);
        printTB(totalMemory, activeMemory, inactiveMemory, freeMemory, wiredMemory);
    }

    return 0;
}
