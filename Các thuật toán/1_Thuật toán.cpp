#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits.h>
#include <queue>

using namespace std;

const int MAX = 100;

void FCFS(int n, int pid[], int arrival[], int burst[]);
void SJF(int n, int pid[], int arrival[], int burst[]);
void SRTF(int n, int pid[], int arrival[], int burst[]);
void RoundRobin(int n, int pid[], int arrival[], int burst[], int quantum);

int main() {
    int n, choice, quantum;
    int pid[MAX], arrival[MAX], burst[MAX];

    cout << "Nhap so tien trinh: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        pid[i] = i + 1;
        cout << "Tien trinh " << pid[i] << "\n";
        cout << "  Thoi gian den: ";
        cin >> arrival[i];
        cout << "  Thoi gian xu ly: ";
        cin >> burst[i];
    }

    cout << "\nChon thuat toan lap lich:\n";
    cout << "1. FCFS\n2. SJF\n3. SRTF\n4. Round Robin\nLua chon: ";
    cin >> choice;

    switch (choice) {
        case 1:
            FCFS(n, pid, arrival, burst);
            break;
        case 2:
            SJF(n, pid, arrival, burst);
            break;
        case 3:
            SRTF(n, pid, arrival, burst);
            break;
        case 4:
            cout << "Nhap quantum: ";
            cin >> quantum;
            RoundRobin(n, pid, arrival, burst, quantum);
            break;
        default:
            cout << "Lua chon khong hop le!\n";
    }

    return 0;
}

void FCFS(int n, int pid[], int arrival[], int burst[]) {
    int wt[MAX], tat[MAX], ct[MAX];
    int currentTime = 0;
    float avgWT = 0, avgTAT = 0;

    // Sap xep theo thoi gian den
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arrival[j] > arrival[j + 1]) {
                swap(arrival[j], arrival[j + 1]);
                swap(burst[j], burst[j + 1]);
                swap(pid[j], pid[j + 1]);
            }

    for (int i = 0; i < n; i++) {
        currentTime = max(currentTime, arrival[i]);
        ct[i] = currentTime + burst[i];
        tat[i] = ct[i] - arrival[i];
        wt[i] = tat[i] - burst[i];
        currentTime = ct[i];
    }

    cout << "\nFCFS Scheduling\n";
    cout << left << setw(5) << "ID" << setw(15) << "Arrival" << setw(15)
         << "Burst" << setw(15) << "Complete" << setw(15) << "Turnaround"
         << setw(15) << "Waiting" << endl;

    for (int i = 0; i < n; i++) {
        cout << left << setw(5) << pid[i] << setw(15) << arrival[i]
             << setw(15) << burst[i] << setw(15) << ct[i] << setw(15)
             << tat[i] << setw(15) << wt[i] << endl;
        avgWT += wt[i];
        avgTAT += tat[i];
    }
    cout << "\nTrung binh cho: " << avgWT / n;
    cout << "\nTrung binh hoan tat: " << avgTAT / n << endl;
}

void SJF(int n, int pid[], int arrival[], int burst[]) {
    int wt[MAX], tat[MAX], ct[MAX];
    int completed = 0, currentTime = 0;
    bool done[MAX] = {};
    float avgWT = 0, avgTAT = 0;

    cout << "\nSJF Scheduling\n";

    while (completed < n) {
        int idx = -1, minBurst = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!done[i] && arrival[i] <= currentTime && burst[i] < minBurst) {
                minBurst = burst[i];
                idx = i;
            }
        }

        if (idx == -1) {
            currentTime++;
            continue;
        }

        ct[idx] = currentTime + burst[idx];
        tat[idx] = ct[idx] - arrival[idx];
        wt[idx] = tat[idx] - burst[idx];
        currentTime = ct[idx];
        done[idx] = true;
        completed++;
    }

    cout << left << setw(5) << "ID" << setw(15) << "Arrival" << setw(15)
         << "Burst" << setw(15) << "Complete" << setw(15) << "Turnaround"
         << setw(15) << "Waiting" << endl;

    for (int i = 0; i < n; i++) {
        cout << left << setw(5) << pid[i] << setw(15) << arrival[i]
             << setw(15) << burst[i] << setw(15) << ct[i] << setw(15)
             << tat[i] << setw(15) << wt[i] << endl;
        avgWT += wt[i];
        avgTAT += tat[i];
    }
    cout << "\nTrung binh cho: " << avgWT / n;
    cout << "\nTrung binh hoan tat: " << avgTAT / n << endl;
}

void SRTF(int n, int pid[], int arrival[], int burst[]) {
    int rt[MAX], wt[MAX] = {}, tat[MAX], ct[MAX];
    for (int i = 0; i < n; i++) rt[i] = burst[i];
    int completed = 0, currentTime = 0, minIdx;
    float avgWT = 0, avgTAT = 0;

    cout << "\nSRTF Scheduling\n";

    while (completed < n) {
        int minRT = INT_MAX;
        minIdx = -1;
        for (int i = 0; i < n; i++) {
            if (arrival[i] <= currentTime && rt[i] > 0 && rt[i] < minRT) {
                minRT = rt[i];
                minIdx = i;
            }
        }
        if (minIdx == -1) {
            currentTime++;
            continue;
        }
        rt[minIdx]--;
        currentTime++;

        if (rt[minIdx] == 0) {
            completed++;
            ct[minIdx] = currentTime;
            tat[minIdx] = ct[minIdx] - arrival[minIdx];
            wt[minIdx] = tat[minIdx] - burst[minIdx];
        }
    }

    cout << left << setw(5) << "ID" << setw(15) << "Arrival" << setw(15)
         << "Burst" << setw(15) << "Complete" << setw(15) << "Turnaround"
         << setw(15) << "Waiting" << endl;

    for (int i = 0; i < n; i++) {
        cout << left << setw(5) << pid[i] << setw(15) << arrival[i]
             << setw(15) << burst[i] << setw(15) << ct[i] << setw(15)
             << tat[i] << setw(15) << wt[i] << endl;
        avgWT += wt[i];
        avgTAT += tat[i];
    }
    cout << "\nTrung binh cho: " << avgWT / n;
    cout << "\nTrung binh hoan tat: " << avgTAT / n << endl;
}

void RoundRobin(int n, int pid[], int arrival[], int burst[], int quantum) {
    int rt[MAX], wt[MAX] = {}, tat[MAX], ct[MAX];
    for (int i = 0; i < n; i++) rt[i] = burst[i];
    int time = 0, done = 0;
    queue<int> q;
    bool visited[MAX] = {};

    cout << "\nRound Robin Scheduling\n";

    while (done < n) {
        for (int i = 0; i < n; i++)
            if (arrival[i] <= time && !visited[i]) {
                q.push(i);
                visited[i] = true;
            }

        if (q.empty()) {
            time++;
            continue;
        }

        int i = q.front();
        q.pop();

        int slice = min(rt[i], quantum);
        rt[i] -= slice;
        time += slice;

        for (int j = 0; j < n; j++)
            if (arrival[j] <= time && !visited[j]) {
                q.push(j);
                visited[j] = true;
            }

        if (rt[i] > 0)
            q.push(i);
        else {
            ct[i] = time;
            tat[i] = ct[i] - arrival[i];
            wt[i] = tat[i] - burst[i];
            done++;
        }
    }

    float avgWT = 0, avgTAT = 0;
    cout << left << setw(5) << "ID" << setw(15) << "Arrival" << setw(15)
         << "Burst" << setw(15) << "Complete" << setw(15) << "Turnaround"
         << setw(15) << "Waiting" << endl;

    for (int i = 0; i < n; i++) {
        cout << left << setw(5) << pid[i] << setw(15) << arrival[i]
             << setw(15) << burst[i] << setw(15) << ct[i] << setw(15)
             << tat[i] << setw(15) << wt[i] << endl;
        avgWT += wt[i];
        avgTAT += tat[i];
    }
    cout << "\nTrung binh cho: " << avgWT / n;
    cout << "\nTrung binh hoan tat: " << avgTAT / n << endl;
}
