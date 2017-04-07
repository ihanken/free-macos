#include <cstdio>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <cmath>
#include <iostream>

using namespace std; // I really just need this for string types.

typedef long long ll; // A typedef to make some lines shorter.

/* This function grabs the normal memory values from vm_stat */
void getMem(ll &tMem, ll &fMem, ll &aMem, ll &iMem, ll &wMem) {
    vm_size_t page_size;                // Page size
    mach_port_t mach_port;              // Mach port
    mach_msg_type_number_t count;       // Count
    vm_statistics64_data_t vm_stats;    // vm_sytat variable.

    mach_port = mach_host_self(); // Set up the mach port.
    count = sizeof(vm_stats) / sizeof(natural_t); // Set count to size of
                                                  // vm_stat over size of
                                                  // natural_t.

    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics64(mach_port,
                                            HOST_VM_INFO,
                                            (host_info64_t) &vm_stats,
                                            &count)) { // Ensure the system
                                                       // calls succeeded.
        ll pageSize = (int64_t) page_size;          // Page size long long

        fMem = vm_stats.free_count * pageSize;      // Free memory
        aMem = vm_stats.active_count * pageSize;    // Active memory
        iMem = vm_stats.inactive_count * pageSize;  // Inactive memory
        wMem = vm_stats.wire_count * pageSize;      // Wired memory
        tMem = (fMem + aMem + iMem + wMem);         // Total memory
    }
}

/* This function grabs the swap memory values from sysctl. */
void getSwap(ll &tSwapMem, ll &fSwapMem, ll &aSwapMem) {
    xsw_usage vmusage = {0}; // Our value for vm memory.
    size_t size = sizeof(vmusage); // This is necessary for the sysctl call.

    if (sysctlbyname("vm.swapusage", &vmusage, &size, NULL, 0) !=0) {
        perror("unable to get swap usage"); // Handle an error.
    }
    else { // Grab our values and put them in the values passed.
        tSwapMem = vmusage.xsu_total;                       // Total memory
        aSwapMem = vmusage.xsu_used;                        // Used Memory
        fSwapMem = vmusage.xsu_total - vmusage.xsu_used;    // Free memory
    }
}

/* Functions for converting memory values. */
double getKB(ll bytes) { return (double) bytes / 1024.0; }
double getMB(ll bytes) { return (double) bytes / pow(1024.0, 2); }
double getGB(ll bytes) { return (double) bytes / pow(1024.0, 3); }
double getTB(ll bytes) { return (double) bytes / pow(1024.0, 4); }

/* This is the human conversion function. It's a bit more complicated. */
pair<double, string> getHumanReadable(ll bytes) {
    int exponent = 0; // 0 = bytes, 8 = KB, etc.
    double dBytes = (double) bytes; // Cast to a double for floating point stuff.

    while ((int) log10(dBytes) + 1 > 3) { // Repeatedly divide by 1024.
        dBytes /= 1024.0;   // Divide.
        exponent += 8;      // Update the exponent.
    }

    string suffix = "";

    switch (exponent) {
    case 0:     suffix = "Bytes";       break;
    case 8:     suffix = "KB   ";       break;
    case 16:    suffix = "MB   ";       break;
    case 24:    suffix = "GB   ";       break;
    case 32:    suffix = "TB   ";       break;
    case 40:    suffix = "PB   ";       break;
    default:                            break;
    }

    return make_pair(dBytes, suffix);
}

// Print the column headings.
void printHeadings() {
    printf("%17s      ", "Total");      // Total Memory
    printf("%11s      ", "Free");       // Free Memory
    printf("%11s      ", "Active");     // Active Memory
    printf("%11s      ", "Inactive");   // Inactive Memory
    printf("%11s   \n", "Wired");       // Wired Memory
}

/* MEM Functions */
// Print the memory in bytes.
void printBytes(ll tMem, ll fMem, ll aMem, ll iMem, ll wMem) {
    printHeadings();                    // Print our headings.
    printf("Mem:  %11lld Bytes", tMem); // Total Memory
    printf("%11lld Bytes", fMem);       // Free Memory
    printf("%11lld Bytes", aMem);       // Active Memory
    printf("%11lld Bytes", iMem);       // Inactive Memory
    printf("%11lld Bytes\n", wMem);     // Wired Memory
}

// Print the memory in kilobytes.
void printKB(ll tMem, ll fMem, ll aMem, ll iMem, ll wMem) {
    printHeadings();                            // Print our headings.
    printf("Mem:  %11.2f KB   ", getKB(tMem));  // Total Memory
    printf("%11.3f KB   ", getKB(fMem));        // Free Memory
    printf("%11.3f KB   ", getKB(aMem));        // Active Memory
    printf("%11.3f KB   ", getKB(iMem));        // Inactive Memory
    printf("%11.3f KB   \n", getKB(wMem));      // Wired Memory
}

// Print the memory in megabytes.
void printMB(ll tMem, ll fMem, ll aMem, ll iMem, ll wMem) {
    printHeadings();                            // Print our headings.
    printf("Mem:  %11.3f MB   ", getMB(tMem));  // Total Memory
    printf("%11.3f MB   ", getMB(fMem));        // Free Memory
    printf("%11.3f MB   ", getMB(aMem));        // Active Memory
    printf("%11.3f MB   ", getMB(iMem));        // Inactive Memory
    printf("%11.3f MB   \n", getMB(wMem));      // Wired Memory
}

// Print the memory in gigabytes.
void printGB(ll tMem, ll fMem, ll aMem, ll iMem, ll wMem) {
    printHeadings();                            // Print our headings.
    printf("Mem:  %11.4f GB   ", getGB(tMem));  // Total Memory
    printf("%11.3f GB   ", getGB(fMem));        // Free Memory
    printf("%11.3f GB   ", getGB(aMem));        // Active Memory
    printf("%11.3f GB   ", getGB(iMem));        // Inactive Memory
    printf("%11.3f GB   \n", getGB(wMem));      // Wired Memory
}

// Print the memory in terabytes.
void printTB(ll tMem, ll fMem, ll aMem, ll iMem, ll wMem) {
    printHeadings();                            // Print our headings.
    printf("Mem:  %11.6f TB   ", getTB(tMem));  // Total Memory
    printf("%11.3f TB   ", getTB(fMem));        // Free Memory
    printf("%11.3f TB   ", getTB(aMem));        // Active Memory
    printf("%11.3f TB   ", getTB(iMem));        // Inactive Memory
    printf("%11.3f TB   \n", getTB(wMem));      // Wired Memory
}

// Print human readable format.
void printHuman(ll tMem, ll fMem, ll aMem, ll iMem, ll wMem) {
    printHeadings(); // Print our headings.

    // Get Human readable pairs.
    pair<double, string> hTMem = getHumanReadable(tMem);    // Total Memory
    pair<double, string> hFMem = getHumanReadable(fMem);    // Free Memory
    pair<double, string> hAMem = getHumanReadable(aMem);    // Active Memory
    pair<double, string> hIMem = getHumanReadable(iMem);    // Inactive Memory
    pair<double, string> hWMem = getHumanReadable(wMem);    // Wired Memory

    printf("Mem:  %11.3g %s", hTMem.first, hTMem.second.c_str());// Total Mem
    printf("%11.3g %s", hFMem.first, hFMem.second.c_str());      // Free Mem
    printf("%11.3g %s", hAMem.first, hAMem.second.c_str());      // Act. Mem
    printf("%11.3g %s", hIMem.first, hIMem.second.c_str());      // Inac. Mem
    printf("%11.3g %s\n", hWMem.first, hWMem.second.c_str());    // Wir. Mem
}

/* SWAP functions */
// Print the memory in bytes.
void printBytes(ll tMem, ll fMem, ll aMem) {
    printf("Swap: %11lld Bytes", tMem);         // Total Memory
    printf("%11lld Bytes", fMem);               // Free Memory
    printf("%11lld Bytes\n", aMem);             // Active Memory
}

// Print the memory in kilobytes.
void printKB(ll tMem, ll fMem, ll aMem) {
    printf("Swap: %11.3f KB   ", getKB(tMem));  // Total Memory
    printf("%11.3f KB   ", getKB(fMem));        // Free Memory
    printf("%11.3f KB   \n", getKB(aMem));      // Active Memory
}

// Print the memory in megabytes.
void printMB(ll tMem, ll fMem, ll aMem) {
    printf("Swap: %11.3f MB   ", getMB(tMem));  // Total Memory
    printf("%11.3f MB   ", getMB(fMem));        // Free Memory
    printf("%11.3f MB   \n", getMB(aMem));      // Active Memory
}

// Print the memory in gigabytes.
void printGB(ll tMem, ll fMem, ll aMem) {
    printf("Swap: %11.3f GB   ", getGB(tMem));  // Total Memory
    printf("%11.3f GB   ", getGB(fMem));        // Free Memory
    printf("%11.3f GB   \n", getGB(aMem));      // Active Memory
}

// Print the memory in terabytes.
void printTB(ll tMem, ll fMem, ll aMem) {
    printf("Swap: %11.3f TB   ", getTB(tMem));  // Total Memory
    printf("%11.3f TB   ", getTB(fMem));        // Free Memory
    printf("%11.3f TB   \n", getTB(aMem));      // Active Memory
}

// Print human readable format.
void printHuman(ll tMem, ll fMem, ll aMem) {
    // Get Human readable pairs.
    pair<double, string> hTMem = getHumanReadable(tMem);    // Total Memory
    pair<double, string> hFMem = getHumanReadable(fMem);    // Free Memory
    pair<double, string> hAMem = getHumanReadable(aMem);    // Active Memory

    printf("Swap: %11.3g %s", hTMem.first, hTMem.second.c_str());   // Total Mem
    printf("%11.3g %s", hFMem.first, hFMem.second.c_str());         // Free Mem
    printf("%11.3g %s\n", hAMem.first, hAMem.second.c_str());       // Act. Mem
}

int main(int argc, const char *argv[]) {
    ll tMem,        // Total memory
       fMem,        // Free memory
       aMem,        // Active memory
       iMem,        // inactive memory
       wMem,        // Wired memory
       tSwapMem,    // Total swap memory
       fSwapMem,    // Free swap memory
       aSwapMem;    // Used swap memory

    getMem(tMem, aMem, iMem, fMem, wMem);   // Fill the mem values.
    getSwap(tSwapMem, fSwapMem, aSwapMem);  // Fill the swap mem values.

    // Only one argument can be used, so just grab the last one.
    string lastArg = argv[argc - 1];

    if (argc == 1 || lastArg == "-b" || lastArg == "--bytes") { // Use bytes
        printBytes(tMem, fMem, aMem, iMem, wMem); // Print mem.
        printBytes(tSwapMem, fSwapMem, aSwapMem); // Print swap mem.
    }
    else if (lastArg == "-k" || lastArg == "--kilo") { // Use kilobytes
        printKB(tMem, fMem, aMem, iMem, wMem); // Print mem.
        printKB(tSwapMem, fSwapMem, aSwapMem); // Print swap mem.
    }
    else if (lastArg == "-m" || lastArg == "--mega") { // Use megabytes
        printMB(tMem, fMem, aMem, iMem, wMem); // Print mem.
        printMB(tSwapMem, fSwapMem, aSwapMem); // Print swap mem.
    }
    else if (lastArg == "-g" || lastArg == "--giga") { // Use gigabytes
        printGB(tMem, fMem, aMem, iMem, wMem); // Print mem.
        printGB(tSwapMem, fSwapMem, aSwapMem); // Print swap mem.
    }
    else if (lastArg == "-t" || lastArg == "--tera") { // Use terabytes
        printTB(tMem, fMem, aMem, iMem, wMem); // Print mem.
        printTB(tSwapMem, fSwapMem, aSwapMem); // Print swap mem.
    }
    else if (lastArg == "-h" || lastArg == "--human") { // Use human readable
        printHuman(tMem, fMem, aMem, iMem, wMem); // Print mem.
        printHuman(tSwapMem, fSwapMem, aSwapMem); // Print swap mem.
    }
    else { // Invalid argument.
        printf("Invalid final argument supplied.\n"); // Give an error.
        exit(0); // Kill the program.
    }

    return 0; // Main is complete.
}
