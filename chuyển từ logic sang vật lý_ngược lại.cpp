#include <iostream>
using namespace std;

#define PAGE_SIZE 1024 // Kích thước 1 trang = 1024 byte

// Hàm chuyển địa chỉ logic -> vật lý
int logicalToPhysical(int logical_address, int page_table[], int num_pages) {
    int page_number = logical_address / PAGE_SIZE;
    int offset = logical_address % PAGE_SIZE;

    if (page_number >= num_pages) {
        cout << "Lỗi: Số trang vượt quá bảng trang!" << endl;
        return -1;
    }

    int frame_number = page_table[page_number];
    int physical_address = frame_number * PAGE_SIZE + offset;
    return physical_address;
}

// Hàm chuyển địa chỉ vật lý -> logic
int physicalToLogical(int physical_address, int page_table[], int num_pages) {
    int frame_number = physical_address / PAGE_SIZE;
    int offset = physical_address % PAGE_SIZE;

    int page_number = -1;
    for (int i = 0; i < num_pages; i++) {
        if (page_table[i] == frame_number) {
            page_number = i;
            break;
        }
    }

    if (page_number == -1) {
        cout << "Lỗi: Không tìm thấy frame trong bảng trang!" << endl;
        return -1;
    }

    int logical_address = page_number * PAGE_SIZE + offset;
    return logical_address;
}

int main() {
    int num_pages;
    cout << "Nhap so luong trang: ";
    cin >> num_pages;

    if (num_pages <= 0) {
        cout << "So luong trang khong hop le." << endl;
        return 1;
    }

    int* page_table = new int[num_pages];
    cout << "Nhap bang trang (Page Table):" << endl;
    for (int i = 0; i < num_pages; i++) {
        cout << "Trang " << i << " -> Khung: ";
        cin >> page_table[i];
    }

    int choice;
    cout << "\nChon chuc nang:\n";
    cout << "1. Chuyen dia chi logic sang dia chi vat ly\n";
    cout << "2. Chuyen dia chi vat ly sang dia chi logic\n";
    cout << "Lua chon (1/2): ";
    cin >> choice;

    if (choice == 1) {
        int logical_address;
        cout << "Nhap dia chi logic: ";
        cin >> logical_address;

        int physical_address = logicalToPhysical(logical_address, page_table, num_pages);
        if (physical_address != -1) {
            cout << "Dia chi vat ly tuong ung: " << physical_address << endl;
        }

    } else if (choice == 2) {
        int physical_address;
        cout << "Nhap dia chi vat ly: ";
        cin >> physical_address;

        int logical_address = physicalToLogical(physical_address, page_table, num_pages);
        if (logical_address != -1) {
            cout << "Dia chi logic tuong ung: " << logical_address << endl;
        }

    } else {
        cout << "Lua chon khong hop le!" << endl;
    }

    delete[] page_table; // Giải phóng bộ nhớ cấp phát động
    return 0;
}
