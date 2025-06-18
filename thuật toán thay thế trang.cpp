#include <iostream>
#include <climits>
using namespace std;

const int MAX = 100;

void printFrames(int frames[], int frameCount) {
    for (int i = 0; i < frameCount; i++) {
        if (frames[i] == -1) cout << "- ";
        else cout << frames[i] << " ";
    }
    cout << endl;
}

// FIFO
int simulateFIFO(int pages[], int n, int frameCount) {
    int frames[MAX];
    for (int i = 0; i < frameCount; i++) frames[i] = -1;
    int pointer = 0, faults = 0;

    cout << "\n== FIFO Execution ==\n";
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        bool found = false;

        // Kiểm tra trang đã có trong khung chưa
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == page) {
                found = true;
                break;
            }
        }

        if (!found) {
            // Thay thế trang theo FIFO
            frames[pointer] = page;
            pointer = (pointer + 1) % frameCount;
            faults++;
        }

        cout << "Page " << page << " => ";
        printFrames(frames, frameCount);
    }

    return faults;
}

// LRU
int simulateLRU(int pages[], int n, int frameCount) {
    int frames[MAX], recent[MAX];
    for (int i = 0; i < frameCount; i++) {
        frames[i] = -1;
        recent[i] = -1;
    }

    int faults = 0;

    cout << "\n== LRU Execution ==\n";
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        bool found = false;

        // Kiểm tra trang đã có trong khung chưa
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == page) {
                found = true;
                recent[j] = i;
                break;
            }
        }

        if (!found) {
            // Tìm khung ít được sử dụng gần đây nhất
            int replaceIndex = 0;
            int minRecent = INT_MAX;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == -1) {
                    replaceIndex = j;
                    break;
                }
                if (recent[j] < minRecent) {
                    minRecent = recent[j];
                    replaceIndex = j;
                }
            }

            frames[replaceIndex] = page;
            recent[replaceIndex] = i;
            faults++;
        }

        cout << "Page " << page << " => ";
        printFrames(frames, frameCount);
    }

    return faults;
}

// OPT (Optimal)
int simulateOPT(int pages[], int n, int frameCount) {
    int frames[MAX];
    for (int i = 0; i < frameCount; i++) frames[i] = -1;

    int faults = 0;

    cout << "\n== OPT Execution ==\n";
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        bool found = false;

        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == page) {
                found = true;
                break;
            }
        }

        if (!found) {
            int replaceIndex = -1;
            int farthest = -1;

            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == -1) {
                    replaceIndex = j;
                    break;
                }

                int nextUse = -1;
                for (int k = i + 1; k < n; k++) {
                    if (pages[k] == frames[j]) {
                        nextUse = k;
                        break;
                    }
                }

                if (nextUse == -1) {
                    replaceIndex = j;
                    break;
                } else if (nextUse > farthest) {
                    farthest = nextUse;
                    replaceIndex = j;
                }
            }

            frames[replaceIndex] = page;
            faults++;
        }

        cout << "Page " << page << " => ";
        printFrames(frames, frameCount);
    }

    return faults;
}

int main() {
    int pages[MAX], n, frameCount, choice;

    cout << "So luong trang tham chieu: ";
    cin >> n;

    cout << "Nhap chuoi trang (co dau cach): ";
    for (int i = 0; i < n; i++) cin >> pages[i];

    cout << "Nhap so khung trang: ";
    cin >> frameCount;
    cout << "=> So khung trang: " << frameCount << endl;

    cout << "\nChon thuat toan thay the trangtrang:\n";
    cout << "1. FIFO\n";
    cout << "2. LRU\n";
    cout << "3. OPT (Toi uuuu)\n";
    cout << "Lua chonchon: ";
    cin >> choice;

    int faults = 0;
    switch (choice) {
        case 1:
            faults = simulateFIFO(pages, n, frameCount);
            break;
        case 2:
            faults = simulateLRU(pages, n, frameCount);
            break;
        case 3:
            faults = simulateOPT(pages, n, frameCount);
            break;
        default:
            cout << "khong hop lele.\n";
            return 0;
    }

    cout << "\n>> Tong loi trangtrang: " << faults << endl;
    return 0;
}
