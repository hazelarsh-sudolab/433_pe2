// Reads the resistor data file produced by Program 1, sorts by nominal
// resistance (ascending), displays the sorted list, and reports
// minimum, mean, median, and maximum nominal resistance values.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iomanip>

// Mirrors the Resistor record written by Program 1.
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

// Trim leading/trailing whitespace.
std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) ++start;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) --end;
    return s.substr(start, end - start);
}

// Extract the value after the last ':' on a line, trimmed.
std::string valueAfterColon(const std::string& line) {
    size_t pos = line.rfind(':');
    if (pos == std::string::npos) return "";
    return trim(line.substr(pos + 1));
}

// Parse a numeric token like "270000" or "270000 ohm" or "1 %" -> double.
double parseNumber(const std::string& text) {
    size_t i = 0;
    while (i < text.size() && (std::isdigit(static_cast<unsigned char>(text[i])) ||
                               text[i] == '.' || text[i] == '-' || text[i] == '+')) {
        ++i;
    }
    if (i == 0) return 0.0;
    return std::stod(text.substr(0, i));
}

int main() {
    // --- Ask which file to analyse ---------------------------------------
    std::string filename;
    std::cout << "Enter the data file name to analyse (e.g. 12345678.txt): ";
    std::getline(std::cin, filename);
    filename = trim(filename);

    // If the user didn't include an extension, append .txt automatically.
    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt") {
        filename += ".txt";
    }

    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Error: could not open file " << filename << "\n";
        return 1;
    }

    // --- Skip the header line --------------------------------------------
    std::string headerLine;
    std::getline(inputFile, headerLine);
    std::cout << "\n" << headerLine << "\n";

    // --- Parse the resistor blocks ---------------------------------------
    std::vector<Resistor> resistors;
    Resistor current;
    bool haveCurrent = false;
    std::string line;

    while (std::getline(inputFile, line)) {
        std::string t = trim(line);
        if (t.empty()) continue;

        if (t.rfind("Resistor", 0) == 0) {
            if (haveCurrent) resistors.push_back(current);
            current = Resistor();
            haveCurrent = true;
            continue;
        }
        if (!haveCurrent) continue;

        if (t.rfind("Color Band Code", 0) == 0) {
            current.CB_Code = valueAfterColon(t);
        } else if (t.rfind("Nominal Resistance", 0) == 0) {
            current.NOM_RV = parseNumber(valueAfterColon(t));
        } else if (t.rfind("Tolerance", 0) == 0) {
            current.tolerance_100 = parseNumber(valueAfterColon(t));
        } else if (t.rfind("Minimum Resistance", 0) == 0) {
            current.min_RV = parseNumber(valueAfterColon(t));
        } else if (t.rfind("Maximum Resistance", 0) == 0) {
            current.max_RV = parseNumber(valueAfterColon(t));
        }
    }
    if (haveCurrent) resistors.push_back(current);
    inputFile.close();

    if (resistors.empty()) {
        std::cerr << "No resistor records found in " << filename << "\n";
        return 1;
    }

    // --- Sort ascending by nominal resistance ----------------------------
    std::sort(resistors.begin(), resistors.end(),
              [](const Resistor& a, const Resistor& b) {
                  return a.NOM_RV < b.NOM_RV;
              });

    // --- Display sorted list ---------------------------------------------
    std::cout << "\n";
    std::cout << std::left
              << std::setw(5)  << "No."
              << std::setw(30) << "Color Band Code"
              << std::right
              << std::setw(14) << "Nominal (ohm)"
              << std::setw(10) << "Tol (%)"
              << std::setw(14) << "Min (ohm)"
              << std::setw(14) << "Max (ohm)"
              << "\n";
    std::cout << std::string(87, '-') << "\n";

    for (size_t i = 0; i < resistors.size(); ++i) {
        std::cout << std::left
                  << std::setw(5)  << (i + 1)
                  << std::setw(30) << resistors[i].CB_Code
                  << std::right
                  << std::setw(14) << resistors[i].NOM_RV
                  << std::setw(10) << resistors[i].tolerance_100
                  << std::setw(14) << resistors[i].min_RV
                  << std::setw(14) << resistors[i].max_RV
                  << "\n";
    }

    // --- Statistics -------------------------------------------------------
    const size_t n = resistors.size();
    double minVal = resistors.front().NOM_RV;
    double maxVal = resistors.back().NOM_RV;

    double sum = 0.0;
    for (const auto& r : resistors) sum += r.NOM_RV;
    double mean = sum / static_cast<double>(n);

    double median;
    if (n % 2 == 1) {
        median = resistors[n / 2].NOM_RV;
    } else {
        median = (resistors[n / 2 - 1].NOM_RV + resistors[n / 2].NOM_RV) / 2.0;
    }

    std::cout << "\n";
    std::cout << std::left << std::setw(20) << "Sample size" << " : " << n << "\n";
    std::cout << std::left << std::setw(20) << "Minimum"     << " : " << minVal << " ohm\n";
    std::cout << std::left << std::setw(20) << "Mean"        << " : " << mean   << " ohm\n";
    std::cout << std::left << std::setw(20) << "Median"      << " : " << median << " ohm\n";
    std::cout << std::left << std::setw(20) << "Maximum"     << " : " << maxVal << " ohm\n";

    return 0;
}