#include <iostream>
#include <iomanip> // For std::setw
#include <list>    // For std::list
#include <string>  // For std::to_string
#include <algorithm> // For std::min, std::max, std::sort
#include <iterator> // For std::distance, std::prev
#include <vector>  // For std::vector (useful for storing requests if you want to reuse them)

using namespace std;

// Cấu trúc đại diện cho một khối bộ nhớ (Memory Block)
struct MemoryBlock {
    int id;             // ID của khối (ví dụ: 1, 2, 3...)
    int startAddress;   // Địa chỉ bắt đầu của khối
    int size;           // Kích thước của khối
    bool isFree;        // true nếu là khối tự do, false nếu đã được cấp phát
    int processId;      // ID của tiến trình đang sử dụng khối này (nếu isFree = false)
                        // hoặc 0 nếu tự do

    // Constructor
    MemoryBlock(int _id = 0, int _startAddr = 0, int _size = 0, bool _isFree = true, int _pId = 0)
        : id(_id), startAddress(_startAddr), size(_size), isFree(_isFree), processId(_pId) {}

    // Toán tử so sánh để sắp xếp theo địa chỉ bắt đầu
    bool operator<(const MemoryBlock& other) const {
        return startAddress < other.startAddress;
    }
    
    // Khai báo static member cho nextBlockId
    static int nextBlockId;
};

// Khởi tạo static member bên ngoài struct definition
int MemoryBlock::nextBlockId = 100; // Bắt đầu từ 100 để tránh trùng với ID ban đầu

// Hàm để in trạng thái của các khối bộ nhớ (chi tiết)
void printDetailedMemoryBlocks(const list<MemoryBlock>& blocks, const string& title) {
    cout << "\n" << title << "\n";
    cout << "----------------------------------------------------------------------------------------\n";
    cout << left << setw(10) << "Block ID"
              << setw(15) << "Start Addr"
              << setw(10) << "Size"
              << setw(15) << "Status"
              << setw(15) << "Process ID" << endl;
    cout << "----------------------------------------------------------------------------------------\n";
    for (const auto& block : blocks) {
        cout << left << setw(10) << block.id
                  << setw(15) << block.startAddress
                  << setw(10) << block.size
                  << setw(15) << (block.isFree ? "FREE" : "ALLOCATED")
                  << setw(15) << (block.isFree ? "-" : to_string(block.processId)) << endl;
    }
    cout << "----------------------------------------------------------------------------------------\n";
}

// Cấu trúc để lưu thông tin yêu cầu cấp phát
struct Request {
    int processId;
    int size;
};

// Biến toàn cục để theo dõi vị trí tìm kiếm cho Next Fit
list<MemoryBlock>::iterator nextFit_lastAllocatedIter;


// --- Hàm chung để giải phóng bộ nhớ và hợp nhất khối tự do ---
void deallocate(list<MemoryBlock>& memoryBlocks, int processId) {
    // 1. Tìm và giải phóng tất cả các khối được cấp phát cho processId này
    for (auto it = memoryBlocks.begin(); it != memoryBlocks.end(); ++it) {
        if (!it->isFree && it->processId == processId) {
            it->isFree = true;
            it->processId = 0;
        }
    }

    // 2. Sắp xếp lại danh sách theo địa chỉ để hợp nhất dễ dàng
    memoryBlocks.sort(); 

    // 3. Hợp nhất các khối tự do liền kề
    if (memoryBlocks.empty()) return;

    auto it = memoryBlocks.begin();
    while (it != memoryBlocks.end()) {
        if (it->isFree) {
            auto next_it = next(it); 
            while (next_it != memoryBlocks.end() && next_it->isFree &&
                   (it->startAddress + it->size == next_it->startAddress)) {
                it->size += next_it->size;
                next_it = memoryBlocks.erase(next_it); 
            }
            ++it; 
        } else {
            ++it;
        }
    }
    nextFit_lastAllocatedIter = memoryBlocks.begin(); 
}

// --- Hàm cấp phát chung cho các thuật toán ---
bool allocateBlock(list<MemoryBlock>& memoryBlocks, list<MemoryBlock>::iterator& it, int processId, int requestedSize) {
    MemoryBlock oldBlock = *it; 

    it->isFree = false;
    it->processId = processId;
    it->size = requestedSize; 

    if (oldBlock.size > requestedSize) {
        MemoryBlock newFreeBlock(
            MemoryBlock::nextBlockId++, 
            oldBlock.startAddress + requestedSize,
            oldBlock.size - requestedSize, 
            true, 
            0 
        );
        memoryBlocks.insert(++it, newFreeBlock);
        --it; 
    }
    return true;
}

// --- Các Thuật toán Cấp phát Bộ nhớ ---
bool firstFit(list<MemoryBlock>& memoryBlocks, int processId, int requestedSize) {
    for (auto it = memoryBlocks.begin(); it != memoryBlocks.end(); ++it) {
        if (it->isFree && it->size >= requestedSize) {
            cout << "-> Process " << processId << " (Size: " << requestedSize
                 << ") allocated to Block " << it->id << " (Addr: " << it->startAddress
                 << ", Old Size: " << it->size << ")" << endl;
            return allocateBlock(memoryBlocks, it, processId, requestedSize);
        }
    }
    cout << "-> Process " << processId << " (Size: " << requestedSize
         << ") FAILED to allocate. No suitable block found." << endl;
    return false;
}

bool bestFit(list<MemoryBlock>& memoryBlocks, int processId, int requestedSize) {
    list<MemoryBlock>::iterator bestBlockIt = memoryBlocks.end();
    int minRemainingSize = 999999;

    for (auto it = memoryBlocks.begin(); it != memoryBlocks.end(); ++it) {
        if (it->isFree && it->size >= requestedSize) {
            if (it->size - requestedSize < minRemainingSize) {
                minRemainingSize = it->size - requestedSize;
                bestBlockIt = it;
            }
        }
    }

    if (bestBlockIt != memoryBlocks.end()) {
        cout << "-> Process " << processId << " (Size: " << requestedSize
             << ") allocated to Block " << bestBlockIt->id << " (Addr: " << bestBlockIt->startAddress
             << ", Old Size: " << bestBlockIt->size << ")" << endl;
        return allocateBlock(memoryBlocks, bestBlockIt, processId, requestedSize);
    } else {
        cout << "-> Process " << processId << " (Size: " << requestedSize
             << ") FAILED to allocate. No suitable block found." << endl;
        return false;
    }
}

bool worstFit(list<MemoryBlock>& memoryBlocks, int processId, int requestedSize) {
    list<MemoryBlock>::iterator worstBlockIt = memoryBlocks.end();
    int maxBlockSize = -1;

    for (auto it = memoryBlocks.begin(); it != memoryBlocks.end(); ++it) {
        if (it->isFree && it->size >= requestedSize) {
            if (it->size > maxBlockSize) {
                maxBlockSize = it->size;
                worstBlockIt = it;
            }
        }
    }

    if (worstBlockIt != memoryBlocks.end()) {
        cout << "-> Process " << processId << " (Size: " << requestedSize
             << ") allocated to Block " << worstBlockIt->id << " (Addr: " << worstBlockIt->startAddress
             << ", Old Size: " << worstBlockIt->size << ")" << endl;
        return allocateBlock(memoryBlocks, worstBlockIt, processId, requestedSize);
    } else {
        cout << "-> Process " << processId << " (Size: " << requestedSize
             << ") FAILED to allocate. No suitable block found." << endl;
        return false;
    }
}

bool nextFit(list<MemoryBlock>& memoryBlocks, int processId, int requestedSize) {
    bool iter_valid = false;
    if (nextFit_lastAllocatedIter != memoryBlocks.end()) {
        for (auto it = memoryBlocks.begin(); it != memoryBlocks.end(); ++it) {
            if (it == nextFit_lastAllocatedIter) {
                iter_valid = true;
                break;
            }
        }
    }
    if (!iter_valid || memoryBlocks.empty()) { 
        nextFit_lastAllocatedIter = memoryBlocks.begin();
    }

    auto startSearchIt = nextFit_lastAllocatedIter;
    auto currentIt = startSearchIt;
    bool wrappedAround = false;
    
    do {
        if (currentIt == memoryBlocks.end()) { 
            if (!wrappedAround) {
                currentIt = memoryBlocks.begin();
                wrappedAround = true;
            } else { 
                break; 
            }
        }

        if (currentIt->isFree && currentIt->size >= requestedSize) {
            cout << "-> Process " << processId << " (Size: " << requestedSize
                 << ") allocated to Block " << currentIt->id << " (Addr: " << currentIt->startAddress
                 << ", Old Size: " << currentIt->size << ") (starting from Block ID: " << startSearchIt->id << ")" << endl;
            bool allocated = allocateBlock(memoryBlocks, currentIt, processId, requestedSize);
            nextFit_lastAllocatedIter = currentIt; 
            return allocated;
        }
        
        ++currentIt; 
        
        if (wrappedAround && currentIt == startSearchIt) {
            break;
        }
    } while (true); 

    cout << "-> Process " << processId << " (Size: " << requestedSize
         << ") FAILED to allocate. No suitable block found." << endl;
    return false;
}

bool lastFit(list<MemoryBlock>& memoryBlocks, int processId, int requestedSize) {
    for (auto it = memoryBlocks.rbegin(); it != memoryBlocks.rend(); ++it) {
        if (it->isFree && it->size >= requestedSize) {
            auto forward_it = prev(it.base()); 
            cout << "-> Process " << processId << " (Size: " << requestedSize
                 << ") allocated to Block " << forward_it->id << " (Addr: " << forward_it->startAddress
                 << ", Old Size: " << forward_it->size << ")" << endl;
            return allocateBlock(memoryBlocks, forward_it, processId, requestedSize);
        }
    }
    cout << "-> Process " << processId << " (Size: " << requestedSize
         << ") FAILED to allocate. No suitable block found." << endl;
    return false;
}


// --- Hàm chính (main) ---
int main() {
    MemoryBlock::nextBlockId = 100;

    // --- Nhập thông tin các phân vùng bộ nhớ ban đầu ---
    list<MemoryBlock> initialMemoryBlocks;
    int numMemoryBlocks;
    cout << "Nhap so luong phan vung bo nho ban dau: ";
    cin >> numMemoryBlocks;

    int currentAddress = 0;
    cout << "Nhap kich thuoc (KB) cua tung phan vung:\n";
    for (int i = 0; i < numMemoryBlocks; ++i) {
        int size;
        cout << "Kich thuoc phan vung " << (i + 1) << ": ";
        cin >> size;
        initialMemoryBlocks.push_back(MemoryBlock(i + 1, currentAddress, size, true));
        currentAddress += size;
    }

    printDetailedMemoryBlocks(initialMemoryBlocks, "--- Trang thai bo nho ban dau ---");

    // --- Nhập thông tin các yêu cầu cấp phát của tiến trình ---
    vector<Request> requests; // Sử dụng vector để lưu trữ các yêu cầu
    int numRequests;
    cout << "\nNhap so luong yeu cau cap phat (tien trinh): ";
    cin >> numRequests;

    cout << "Nhap kich thuoc (KB) cua tung yeu cau cap phat:\n";
    for (int i = 0; i < numRequests; ++i) {
        int size;
        cout << "Kich thuoc yeu cau P" << (i + 1) << ": ";
        cin >> size;
        // Gán ID tiến trình bắt đầu từ 101 để dễ phân biệt
        requests.push_back({100 + (i + 1), size}); 
    }

    cout << "\n\n=== CHAY CAC THUAT TOAN CAP PHAT BO NHO ===" << endl;

    // --- Chạy First Fit ---
    list<MemoryBlock> blocksForFirstFit = initialMemoryBlocks; 
    cout << "\n------------------------------------------------------------\n";
    cout << "              THUAT TOAN: FIRST FIT                       \n";
    cout << "------------------------------------------------------------\n";
    for (const auto& req : requests) {
        firstFit(blocksForFirstFit, req.processId, req.size);
    }
    printDetailedMemoryBlocks(blocksForFirstFit, "--- Trang thai bo nho sau First Fit ---");
    // Giải phóng bộ nhớ sau mỗi thuật toán để chuẩn bị cho thuật toán tiếp theo
    for(const auto& req : requests) deallocate(blocksForFirstFit, req.processId);


    // --- Chạy Best Fit ---
    list<MemoryBlock> blocksForBestFit = initialMemoryBlocks; 
    cout << "\n------------------------------------------------------------\n";
    cout << "              THUAT TOAN: BEST FIT                        \n";
    cout << "------------------------------------------------------------\n";
    for (const auto& req : requests) {
        bestFit(blocksForBestFit, req.processId, req.size);
    }
    printDetailedMemoryBlocks(blocksForBestFit, "--- Trang thai bo nho sau Best Fit ---");
    for(const auto& req : requests) deallocate(blocksForBestFit, req.processId);

    // --- Chạy Worst Fit ---
    list<MemoryBlock> blocksForWorstFit = initialMemoryBlocks; 
    cout << "\n------------------------------------------------------------\n";
    cout << "              THUAT TOAN: WORST FIT                       \n";
    cout << "------------------------------------------------------------\n";
    for (const auto& req : requests) {
        worstFit(blocksForWorstFit, req.processId, req.size);
    }
    printDetailedMemoryBlocks(blocksForWorstFit, "--- Trang thai bo nho sau Worst Fit ---");
    for(const auto& req : requests) deallocate(blocksForWorstFit, req.processId);

    // --- Chạy Next Fit ---
    list<MemoryBlock> blocksForNextFit = initialMemoryBlocks; 
    nextFit_lastAllocatedIter = blocksForNextFit.end(); // Reset vị trí cho Next Fit
    cout << "\n------------------------------------------------------------\n";
    cout << "              THUAT TOAN: NEXT FIT                        \n";
    cout << "------------------------------------------------------------\n";
    for (const auto& req : requests) {
        nextFit(blocksForNextFit, req.processId, req.size);
    }
    printDetailedMemoryBlocks(blocksForNextFit, "--- Trang thai bo nho sau Next Fit ---");
    for(const auto& req : requests) deallocate(blocksForNextFit, req.processId);

    // --- Chạy Last Fit ---
    list<MemoryBlock> blocksForLastFit = initialMemoryBlocks; 
    cout << "\n------------------------------------------------------------\n";
    cout << "              THUAT TOAN: LAST FIT                        \n";
    cout << "------------------------------------------------------------\n";
    for (const auto& req : requests) {
        lastFit(blocksForLastFit, req.processId, req.size);
    }
    printDetailedMemoryBlocks(blocksForLastFit, "--- Trang thai bo nho sau Last Fit ---");
    // Không cần deallocate sau thuật toán cuối cùng trong main

    return 0;
}
