// Snippet standard crucial preprocessor directive; "#include and #define"
#include <iostream> // << STD input/output stream
#include <fstream>  // << read/write data to/from file
#include <string>   // << specialized container optimized for human-readable text
#include <vector>   // << generic data storage
#include <cctype>   // << for std::isdigit
#include <cmath>    // << for std::pow
#include <limits>   // << for std::numeric_limits

#ifndef File_Ext
#define File_Ext ".txt"
#endif

struct Resistor {
    std::string Color1;
    std::string Color2;
    std::string Color3;
    std::string Color4;
    std::string CB_Code;
    double NOM_RV;
    double tolerance_100;
    double min_RV;
    double max_RV;
};

struct Student {
    std::string StudentName;
    std::string MatricNumber;
};

// Check if the matriculation number is valid (8 digits)
bool Valid_MN(const std::string& s) {
    if (s.size() != 8) return false;
    for (char c : s) {
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

// "mn" remarks the matric number to check, and "existing" is a vector of already used matric numbers.
bool ExistValue(const std::string& mn, const std::vector<std::string>& existing) {
    for (const auto& e : existing) {
        if (e == mn) return true;
    }
    return false;
}

// Asks the user for their full name and returns it.
std::string AskName() {
    std::string name;
    while (true) {
        std::cout << "Enter your full name: ";
        std::getline(std::cin, name);

        if (!name.empty()) {
            break;
        }
        std::cout << "Name cannot be empty. Please enter your full name.\n";
    }
    return name;
}

// References function for colour decoding. Returns -1 if the colour is invalid.
int get_D(const std::string& c) {
    if (c == "Black" || c == "0") return 0;
    if (c == "Brown" || c == "1") return 1;
    if (c == "Red" || c == "2") return 2;
    if (c == "Orange" || c == "3") return 3;
    if (c == "Yellow" || c == "4") return 4;
    if (c == "Green" || c == "5") return 5;
    if (c == "Blue" || c == "6") return 6;
    if (c == "Violet" || c == "7") return 7;
    if (c == "Grey" || c == "Gray" || c == "8") return 8;
    if (c == "White" || c == "9") return 9;
    return -1; // Invalid color
}

// Returns the multiplier for a colour. Returns -1 if the colour is not a valid multiplier.
double get_M(const std::string& c) {
    if (c == "Gold" || c == "G") return 0.1;
    if (c == "Silver" || c == "S") return 0.01;
    int d = get_D(c);
    if (d == -1) return -1; // Invalid colour
    return std::pow(10.0, d);
}

// Returns the tolerance for a colour. Returns -1 if the colour is not a valid tolerance.
double get_T(const std::string& c) {
    if (c == "Brown" || c == "1") return 1.0;
    if (c == "Red" || c == "2") return 2.0;
    if (c == "Green" || c == "5") return 0.5;
    if (c == "Blue" || c == "6") return 0.25;
    if (c == "Violet" || c == "7") return 0.1;
    if (c == "Grey" || c == "Gray" || c == "8") return 0.05;
    if (c == "Gold" || c == "G") return 5.0;
    if (c == "Silver" || c == "S") return 10.0;
    return -1; // Invalid color
}

// Reads one line from std::cin and splits it into whitespace-delimited tokens.
void readTokens(std::vector<std::string>& out) {
    out.clear();
    std::string line;
    std::getline(std::cin, line);
    std::string tok;
    for (char ch : line) {
        if (std::isspace(static_cast<unsigned char>(ch))) {
            if (!tok.empty()) { out.push_back(tok); tok.clear(); }
        } else {
            tok += ch;
        }
    }
    if (!tok.empty()) out.push_back(tok);
}

int main() {
    std::string USER_Name;
    std::string USER_MatricNumber;

    // Ask the user for their matriculation number and validate it (8 digits)
    while (true) {
        std::cout << "Enter your matriculation number (8 digits): ";
        std::getline(std::cin, USER_MatricNumber);

        if (Valid_MN(USER_MatricNumber)) {
            break;
        }
        std::cout << "Invalid matriculation number. Please enter an 8-digit number.\n";
    }

    // Ask for the user name.
    USER_Name = AskName();

    // The first line of the output file must be:
    // Name: [Your Name] | Matric No: [Your Matric Number]
    std::string filename = USER_MatricNumber + File_Ext;
    std::ofstream outputFile(filename);
    if (!outputFile) {
        std::cerr << "Error: File not created " << filename << std::endl;
        return 1;
    }

    outputFile << "Name: " << USER_Name << " | Matric No: " << USER_MatricNumber << std::endl;

    // 15 resistors to be identified, each with a unique FOUR (4) color band.
    const int NO_Resistors = 15;
    std::vector<Resistor> resistors(NO_Resistors);

    // Ask for all 15 resistors' information (color bands) and store them.
    std::cout << "\n==================================================\n";
    std::cout << "    Enter Color Bands for 15 Resistors (4 Bands)  \n";
    std::cout << "==================================================\n";

    for (int i = 0; i < NO_Resistors; ++i) {
        std::cout << "\nResistor " << (i + 1) << ":\n";

        // --- Band 1 & 2 (exactly 2 tokens) ---
        while (true) {
            std::cout << "Band 1 , Band 2 : ";
            std::vector<std::string> t;
            readTokens(t);

            if (t.size() != 2) {
                std::cout << "Please enter exactly 2 colors (Band 1 and Band 2).\n";
                continue;
            }
            if (get_D(t[0]) == -1) {
                std::cout << "Invalid color: " << t[0] << "\n";
                continue;
            }
            if (get_D(t[1]) == -1) {
                std::cout << "Invalid color: " << t[1] << "\n";
                continue;
            }

            resistors[i].Color1 = t[0];
            resistors[i].Color2 = t[1];
            break;
        }

        // --- Multiplier (exactly 1 token) ---
        while (true) {
            std::cout << "Multiplier      : ";
            std::vector<std::string> t;
            readTokens(t);

            if (t.size() != 1) {
                std::cout << "Please enter exactly 1 color for the multiplier.\n";
                continue;
            }
            if (get_M(t[0]) == -1) {
                std::cout << "Invalid multiplier color: " << t[0] << "\n";
                continue;
            }

            resistors[i].Color3 = t[0];
            break;
        }

        // --- Tolerance (exactly 1 token) ---
        while (true) {
            std::cout << "Tolerance       : ";
            std::vector<std::string> t;
            readTokens(t);

            if (t.size() != 1) {
                std::cout << "Please enter exactly 1 color for the tolerance.\n";
                continue;
            }
            if (get_T(t[0]) == -1) {
                std::cout << "Invalid tolerance color: " << t[0] << "\n";
                continue;
            }

            resistors[i].Color4 = t[0];
            break;
        }

        resistors[i].CB_Code = resistors[i].Color1 + " " + resistors[i].Color2 + " " +
                               resistors[i].Color3 + " " + resistors[i].Color4;

        double digitValue = static_cast<double>(get_D(resistors[i].Color1)) * 10 +
                            static_cast<double>(get_D(resistors[i].Color2));
        resistors[i].NOM_RV = digitValue * get_M(resistors[i].Color3);
        resistors[i].tolerance_100 = get_T(resistors[i].Color4);

        double toleranceValue = (resistors[i].NOM_RV * resistors[i].tolerance_100) / 100.0;
        resistors[i].min_RV = resistors[i].NOM_RV - toleranceValue;
        resistors[i].max_RV = resistors[i].NOM_RV + toleranceValue;

        // Display: Resistor Number >> Color-band codes >> Nominal Resistance Value
        //          >> Tolerance Percentage >> Minimum and Maximum Resistance Values.
        std::cout << "\nResistor " << (i + 1) << ":\n";
        std::cout << "  Color Band Code    : " << resistors[i].CB_Code << "\n";
        std::cout << "  Nominal Resistance : " << resistors[i].NOM_RV << " ohm\n";
        std::cout << "  Tolerance          : " << resistors[i].tolerance_100 << " %\n";
        std::cout << "  Minimum Resistance : " << resistors[i].min_RV << " ohm\n";
        std::cout << "  Maximum Resistance : " << resistors[i].max_RV << " ohm\n";

        // Write the same data to the output file.
        outputFile << "\nResistor " << (i + 1) << ":\n";
        outputFile << "  Color Band Code    : " << resistors[i].CB_Code << "\n";
        outputFile << "  Nominal Resistance : " << resistors[i].NOM_RV << " ohm\n";
        outputFile << "  Tolerance          : " << resistors[i].tolerance_100 << " %\n";
        outputFile << "  Minimum Resistance : " << resistors[i].min_RV << " ohm\n";
        outputFile << "  Maximum Resistance : " << resistors[i].max_RV << " ohm\n";
    }

    outputFile.close();
    std::cout << "\nAll resistor data has been written to " << filename << "\n";

    return 0;
}