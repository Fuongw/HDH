#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

const int MAX = 100; // giới hạn tối đa cho số block/process

int main() {
    int blocks[MAX], processes[MAX];
    int n_blocks, n_processes;

    // Nhập số block và tiến trình
    cout << "Nhap so luong block bo nho: ";
    cin >> n_blocks;
    cout << "Nhap kich thuoc tung block (KB):\n";
    for (int i = 0; i < n_blocks; i++) {
        cout << "Block " << i + 1 << ": ";
        cin >> blocks[i];
    }

    cout << "\nNhap so luong tien trinh: ";
    cin >> n_processes;
    cout << "Nhap kich thuoc tung tien trinh (KB):\n";
    for (int i = 0; i < n_processes; i++) {
        cout << "Tien trinh " << i + 1 << ": ";
        cin >> processes[i];
    }

    // ======= Tạo hàm in kết quả =======
    auto printResult = [&](const char* name, int allocation[], int allocBlock[]) {
        cout << "\n=== " << name << " ===\n";
        for (int i = 0; i < n_processes; i++) {
            if (allocation[i] != -1)
                cout << "Tien trinh " << processes[i] << " KB => Block " << allocBlock[i] << " KB\n";
            else
                cout << "Tien trinh " << processes[i] << " KB => Khong duoc cap phat\n";
        }
    };

    // ======= First Fit =======
    {
        int b[MAX], allocation[MAX], allocBlock[MAX];
        memcpy(b, blocks, sizeof(int) * n_blocks);
        fill_n(allocation, n_processes, -1);

        for (int i = 0; i < n_processes; i++) {
            for (int j = 0; j < n_blocks; j++) {
                if (b[j] >= processes[i]) {
                    allocation[i] = j;
                    allocBlock[i] = b[j];
                    b[j] -= processes[i];
                    break;
                }
            }
        }

        for (int i = 0; i < n_processes; i++)
            if (allocation[i] != -1)
                allocBlock[i] = blocks[allocation[i]];
        printResult("First Fit", allocation, allocBlock);
    }

    // ======= Best Fit =======
    {
        int b[MAX], allocation[MAX], allocBlock[MAX];
        memcpy(b, blocks, sizeof(int) * n_blocks);
        fill_n(allocation, n_processes, -1);

        for (int i = 0; i < n_processes; i++) {
            int bestIdx = -1;
            for (int j = 0; j < n_blocks; j++) {
                if (b[j] >= processes[i]) {
                    if (bestIdx == -1 || b[j] < b[bestIdx])
                        bestIdx = j;
                }
            }
            if (bestIdx != -1) {
                allocation[i] = bestIdx;
                allocBlock[i] = b[bestIdx];
                b[bestIdx] -= processes[i];
            }
        }

        for (int i = 0; i < n_processes; i++)
            if (allocation[i] != -1)
                allocBlock[i] = blocks[allocation[i]];
        printResult("Best Fit", allocation, allocBlock);
    }

    // ======= Worst Fit =======
    {
        int b[MAX], allocation[MAX], allocBlock[MAX];
        memcpy(b, blocks, sizeof(int) * n_blocks);
        fill_n(allocation, n_processes, -1);

        for (int i = 0; i < n_processes; i++) {
            int worstIdx = -1;
            for (int j = 0; j < n_blocks; j++) {
                if (b[j] >= processes[i]) {
                    if (worstIdx == -1 || b[j] > b[worstIdx])
                        worstIdx = j;
                }
            }
            if (worstIdx != -1) {
                allocation[i] = worstIdx;
                allocBlock[i] = b[worstIdx];
                b[worstIdx] -= processes[i];
            }
        }

        for (int i = 0; i < n_processes; i++)
            if (allocation[i] != -1)
                allocBlock[i] = blocks[allocation[i]];
        printResult("Worst Fit", allocation, allocBlock);
    }

    // ======= Next Fit =======
    {
        int b[MAX], allocation[MAX], allocBlock[MAX];
        memcpy(b, blocks, sizeof(int) * n_blocks);
        fill_n(allocation, n_processes, -1);

        int pos = 0;
        for (int i = 0; i < n_processes; i++) {
            int j = pos, count = 0;
            while (count < n_blocks) {
                if (b[j] >= processes[i]) {
                    allocation[i] = j;
                    allocBlock[i] = b[j];
                    b[j] -= processes[i];
                    pos = j;
                    break;
                }
                j = (j + 1) % n_blocks;
                count++;
            }
        }

        for (int i = 0; i < n_processes; i++)
            if (allocation[i] != -1)
                allocBlock[i] = blocks[allocation[i]];
        printResult("Next Fit", allocation, allocBlock);
    }

    // ======= Last Fit =======
    {
        int b[MAX], allocation[MAX], allocBlock[MAX];
        memcpy(b, blocks, sizeof(int) * n_blocks);
        fill_n(allocation, n_processes, -1);

        for (int i = 0; i < n_processes; i++) {
            for (int j = n_blocks - 1; j >= 0; j--) {
                if (b[j] >= processes[i]) {
                    allocation[i] = j;
                    allocBlock[i] = b[j];
                    b[j] -= processes[i];
                    break;
                }
            }
        }

        for (int i = 0; i < n_processes; i++)
            if (allocation[i] != -1)
                allocBlock[i] = blocks[allocation[i]];
        printResult("Last Fit", allocation, allocBlock);
    }

    return 0;
}
