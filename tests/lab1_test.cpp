#include <gtest/gtest.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

extern "C" {
    #include <parent.h>
}

void testingProgram(const std::vector<std::string>& input, const std::vector<int>& expectedOutput) {
    const char * fileName = "input.txt";

    // Записываем входные данные в файл
    std::ofstream outFile(fileName);
    outFile << fileName << std:: endl;
    for (const std::string& line : input) {
        outFile << line << std::endl;
    }
    outFile.close();
    
    // Сохраняем старые буферы
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::streambuf* oldCinBuf = std::cin.rdbuf();

    // Подключаем файл ввода
    std::ifstream inputFile(fileName);
    std::cin.rdbuf(inputFile.rdbuf());

    // Захватываем вывод программы
    std::stringstream capturedOutput;
    std::cout.rdbuf(capturedOutput.rdbuf());

    ParentRoutine(getenv("PATH_TO_CHILD"));

    // Восстанавливаем буферы
    std::cin.rdbuf(oldCinBuf);
    std::cout.rdbuf(oldCoutBuf);

    // Проверяем вывод программы
    std::istringstream outputStream(capturedOutput.str());
    for (const int &i : expectedOutput) {
        int result;
        outputStream >> result;
        EXPECT_EQ(result, i);
    }
    auto removeIfExists = [](const char* path) {
        if(fs::exists(path)) {
            fs::remove(path);
        }
    };

    removeIfExists(fileName);
    
}

TEST(firstLabTests, simpleTest) {
    std::vector<std::string> input = {
        "4 2",
        "3 1 1 1 1 1",
        "-10 1 1 2",
        "1337 1"
    };
    std::vector<int> expectedOutput = {
        2,
        3,
        -5,
        1337
    };

    testingProgram(input, expectedOutput);
}

TEST(firstLabTests, emptyTest) {
    std::vector<std::string> input = {};
    std::vector<int> expectedOutput = {};
    
    testingProgram(input, expectedOutput);
}

TEST(firstLabTests, withZeroTest) {
    std::vector<std::string> input = {
        "12 -4 1 1 1",
        "33 2 5",
        "789 0 6",
        "22 4"
    };
    std::vector<int> expectedOutput = {
        -3,
        3
    };

    testingProgram(input, expectedOutput);
}

TEST(firstLabTests, withLetterTest) {
    std::vector<std::string> input = {
        "32 -4 1 1 1",
        "33 a 5",
        "789 3 6",
        "22 4"
    };

    std::vector<int> expectedOutput = {
        -8
    };

    testingProgram(input, expectedOutput);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}