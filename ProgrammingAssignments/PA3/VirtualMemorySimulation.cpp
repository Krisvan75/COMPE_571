#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <set>
#include <random>
#include <algorithm>
#include <climits>


const int OFFSET_BITS = 9;
const int VIRTUAL_ADDR_BITS = 16;
const int VIRTUAL_PAGES = 1 << (VIRTUAL_ADDR_BITS - OFFSET_BITS); // 128
const int PHYSICAL_PAGES = 32;
const int RESET_INTERVAL = 200;

struct PageTableEntry {
    bool valid = false;
    bool dirty = false;
    bool referenced = false;
    int frameNumber = -1;
    int lastUsedTime = -1;
};

struct Frame {
    int processId = -1;
    int virtualPage = -1;
};

struct Statistics {
    int pageFaults = 0;
    int diskAccesses = 0;
    int dirtyPageWrites = 0;
};

std::unordered_map<int, std::vector<PageTableEntry>> pageTables;
std::vector<Frame> physicalMemory(PHYSICAL_PAGES);
std::queue<int> fifoQueue;
std::set<int> freeFrames;
Statistics stats;
int currentTime = 0;
int referenceCount = 0;

int getPageNumber(int virtualAddress) {
    return virtualAddress >> OFFSET_BITS;
}

int selectVictim_RAND() {
    static std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, PHYSICAL_PAGES - 1);
    return dist(rng);
}

int selectVictim_FIFO() {
    int victim = fifoQueue.front();
    fifoQueue.pop();
    return victim;
}

int selectVictim_LRU() {
    int minTime = INT_MAX, victim = -1;
    for (int i = 0; i < PHYSICAL_PAGES; ++i) {
        int pid = physicalMemory[i].processId;
        int vpn = physicalMemory[i].virtualPage;
        auto &pte = pageTables[pid][vpn];
        if (pte.lastUsedTime < minTime) {
            minTime = pte.lastUsedTime;
            victim = i;
        }
    }
    return victim;
}

int selectVictim_PER() {
    struct Candidate {
        int frameIndex;
        int category;
    };
    std::vector<Candidate> candidates;

    for (int i = 0; i < PHYSICAL_PAGES; ++i) {
        int pid = physicalMemory[i].processId;
        int vpn = physicalMemory[i].virtualPage;
        auto &pte = pageTables[pid][vpn];

        int cat = 5;
        if (!pte.referenced && !pte.dirty) cat = 2;
        else if (!pte.referenced && pte.dirty) cat = 3;
        else if (pte.referenced && !pte.dirty) cat = 4;
        else if (pte.referenced && pte.dirty) cat = 5;

        candidates.push_back({i, cat});
    }

    std::sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) {
        return (a.category != b.category) ? (a.category < b.category) : (a.frameIndex < b.frameIndex);
    });

    return candidates.front().frameIndex;
}


int evictPage(int policy) {
    int victimFrame;
    if (policy == 0) victimFrame = selectVictim_RAND();
    else if (policy == 1) victimFrame = selectVictim_FIFO();
    else if (policy == 2) victimFrame = selectVictim_LRU();
    else if (policy == 3) victimFrame = selectVictim_PER();
    else {
        std::cerr << "Invalid policy!\n";
        exit(1);
    }

    auto &victim = physicalMemory[victimFrame];
    auto &pte = pageTables[victim.processId][victim.virtualPage];

    if (pte.dirty) {
        stats.dirtyPageWrites++;
        stats.diskAccesses++;
    }
    pte.valid = false;

    return victimFrame;
}

void loadPage(int pid, int vpn, char op, int policy) {
    int frame;
    if (!freeFrames.empty()) {
        frame = *freeFrames.begin();
        freeFrames.erase(frame);
    } else {
        frame = evictPage(policy);
    }

    physicalMemory[frame] = {pid, vpn};
    auto &pte = pageTables[pid][vpn];
    pte.valid = true;
    pte.frameNumber = frame;
    pte.dirty = (op == 'W');
    pte.referenced = true;
    pte.lastUsedTime = currentTime;

    if (policy == 1) 
    fifoQueue.push(frame);

    stats.pageFaults++;
    stats.diskAccesses++;
}

void handleMemoryAccess(int pid, int virtualAddress, char op, int policy) {
    int vpn = getPageNumber(virtualAddress);
    auto &pt = pageTables[pid];
    if (pt.empty()) pt.resize(VIRTUAL_PAGES);
    auto &pte = pt[vpn];

    referenceCount++;
    currentTime++;

    if (referenceCount % RESET_INTERVAL == 0) {
        for (auto &[_, table] : pageTables)
            for (auto &entry : table)
                entry.referenced = false;
    }

    if (pte.valid) {
        pte.referenced = true;
        if (op == 'W') pte.dirty = true;
        pte.lastUsedTime = currentTime;
    } else {
        loadPage(pid, vpn, op, policy);
    }
}

int main(int argc, char* argv[]) {
    // argv : [0] = program name, [1] = input file, [2] = policy
    
    std::ifstream infile(argv[1]);
    int policy = std::stoi(argv[2]);
    std::string line;

    for (int i = 0; i < PHYSICAL_PAGES; ++i) freeFrames.insert(i);

    while (getline(infile, line)) {
        std::istringstream iss(line);
        int pid, vaddr;
        char op;
        if (!(iss >> pid >> vaddr >> op)) continue;
        handleMemoryAccess(pid, vaddr, op, policy);
    }

    std::cout << "Policy: ";
    switch (policy) {
        case 0: std::cout << "RAND\n"; break;
        case 1: std::cout << "FIFO\n"; break;
        case 2: std::cout << "LRU\n"; break;
        case 3: std::cout << "PER\n"; break;
        break;
        default: std::cout << "Unknown\n";
    }

    std::cout << "Page Faults: " << stats.pageFaults << "\n";
    std::cout << "Disk Accesses: " << stats.diskAccesses << "\n";
    std::cout << "Dirty Page Writes: " << stats.dirtyPageWrites << "\n";

    return 0;
}
