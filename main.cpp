#include <iostream>
#include <map>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <cstdio>  // для удаления файлов

struct ProductInfo {
    int quantity;
    std::string lastUpdate;
};

// Получение текущей даты и времени
std::string currentDateTime() {
    std::time_t now = std::time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return buf;
}

// Сохранение инвентаря в файл
void saveInventory(const std::map<std::string, ProductInfo>& inventory, const std::string& filename) {
    std::ofstream outFile(filename);
    for (const auto& pair : inventory) {
        outFile << pair.first << "," << pair.second.quantity << "," << pair.second.lastUpdate << "\n";
    }
}

// Загрузка инвентаря из файла
void loadInventory(std::map<std::string, ProductInfo>& inventory, const std::string& filename) {
    std::ifstream inFile(filename);
    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string name, quantityStr, date;
        if (std::getline(iss, name, ',') && std::getline(iss, quantityStr, ',') && std::getline(iss, date)) {
            ProductInfo info;
            info.quantity = std::stoi(quantityStr);
            info.lastUpdate = date;
            inventory[name] = info;
        }
    }
}

int main() {
    std::map<std::string, ProductInfo> inventory;
    std::string filename = "inventory.txt";

    loadInventory(inventory, filename);

    std::string input;

    std::cout << "\033[1;32mStockKeeper Inventory System v1.0\033[0m\n";
    std::cout << "Type 'help' for list of commands.\n\n";

    while (true) {
        std::cout << "\nEnter command or product name (type 'help' for options): ";
        std::getline(std::cin, input);

        if (input == "exit") {
            std::cout << "Saving and exiting...\n";
            break;
        }
        else if (input == "help") {
            std::cout << "\n\033[1;34mAvailable commands:\033[0m\n";
            std::cout << "  \033[1;33mlist\033[0m         - Show all products\n";
            std::cout << "  \033[1;33mexit\033[0m         - Save and exit program\n";
            std::cout << "  \033[1;33mdelete <file>\033[0m - Delete a file with confirmation\n";
            std::cout << "  \033[1;33mhelp\033[0m         - Show this help message\n";
            std::cout << "  Or type a product name to add/update it.\n";
        }
        else if (input == "list") {
            if (inventory.empty()) {
                std::cout << "Inventory is empty.\n";
            } else {
                std::cout << "\n\033[1;36mCurrent inventory:\033[0m\n";
                for (const auto& pair : inventory) {
                    std::cout << " - " << pair.first
                              << ": " << pair.second.quantity
                              << " pcs (last updated: " << pair.second.lastUpdate << ")\n";
                }
            }
        }
        else if (input.rfind("delete ", 0) == 0) {
            std::string fileToDelete = input.substr(7); // убираем "delete "
            std::cout << "Are you sure you want to delete file \"" << fileToDelete << "\"? (Y/N): ";
            std::string confirm;
            std::getline(std::cin, confirm);

            if (confirm == "Y" || confirm == "y") {
                if (std::remove(fileToDelete.c_str()) == 0) {
                    std::cout << "\033[1;31mFile \"" << fileToDelete << "\" deleted successfully.\033[0m\n";
                } else {
                    std::cerr << "\033[1;31mFailed to delete \"" << fileToDelete << "\". File may not exist.\033[0m\n";
                }
            } else {
                std::cout << "File deletion cancelled.\n";
            }
        }
        else if (!input.empty()) {
            int quantity;
            while (true) {
                std::cout << "Enter quantity: ";
                std::string quantityStr;
                std::getline(std::cin, quantityStr);

                std::stringstream ss(quantityStr);
                if (ss >> quantity && ss.eof() && quantity >= 0) {
                    break;  // корректное число, выходим из цикла
                } else {
                    std::cout << "Invalid input. Please enter a non-negative integer.\n";
                }
            }

            inventory[input].quantity += quantity;
            inventory[input].lastUpdate = currentDateTime();

            std::cout << "\033[1;32mAdded \"" << input << "\" in quantity of " << quantity << ".\033[0m\n";
        }
    }

    saveInventory(inventory, filename);
    return 0;
}
