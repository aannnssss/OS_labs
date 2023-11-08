#include <gtest/gtest.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <iostream>

namespace fs = std::filesystem;

extern "C" {
    #include <parent.h>
}

void testingProgram(const std::vector<std::string>& input, const std::vector<std::string>& expectedOutput, const std::vector<std::string>& expectedFile) {
    const char * fileName = "input.txt";

    // Записываем входные данные в файл
    std::ofstream outFile(fileName);
    //outFile << fileName << std::endl;
    for (const std::string& line : input) {
        outFile << line << std::endl;
    }
    outFile.close();

    auto deleter = [](FILE* file) {
        fclose(file);
    };

    // Создаем умный указатель
    std::unique_ptr<FILE, decltype(deleter)> inFile(fopen(fileName, "r"), deleter);
    if (inFile == nullptr) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    // Сохраняем старые буферы
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::streambuf* oldCinBuf = std::cin.rdbuf();

    // Подключаем файл ввода
    std::ifstream inputFile(fileName);
    std::cin.rdbuf(inputFile.rdbuf());

    // Захватываем вывод программы
    std::stringstream capturedOutput;
    std::cout.rdbuf(capturedOutput.rdbuf());

    ParentRoutine(getenv("PATH_TO_CHILD"), inFile.get());

    // Восстанавливаем стандартные буферы
    std::cin.rdbuf(oldCinBuf);
    std::cout.rdbuf(oldCoutBuf);

    // Проверяем вывод программы
    std::istringstream outputStream(capturedOutput.str());
    for (const std::string &i : expectedOutput) {
        std::string result;
        std::getline(outputStream, result);
        EXPECT_EQ(result, i);
    }
    
    // Проверяем содержимое файла
    std::ifstream fileStream(fileName);
    for (const std::string &expectation : expectedFile) {
        std::string result;
        std::getline(fileStream, result);
        EXPECT_EQ(result, expectation);
    }
    
    fileStream.close();
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
        "3 3 1 1 1 1",
        "-10 1 1 2",
        "1337 1"
    };
    std::vector<std::string> expectedOutput = {
        "2",
        "3",
        "-5",
        "1337"
    };
    std::vector<std::string> expectedFile = {
        "4 2",
        "3 3 1 1 1 1",
        "-10 1 1 2",
        "1337 1"
    };

    testingProgram(input, expectedOutput, expectedFile);
}

TEST(firstLabTests, emptyTest) {
    std::vector<std::string> input = {};
    std::vector<std::string> expectedOutput = {};
    std::vector<std::string> expectedFile = {};
    
    testingProgram(input, expectedOutput, expectedFile);
}

TEST(firstLabTests, withZeroTest) {
    std::vector<std::string> input = {
        "12 -4 1 1 1",
        "33 2 5",
        "789 0 6",
        "22 4"
    };
    std::vector<std::string> expectedOutput = {
        "-3",
        "3"
    };
    std::vector<std::string> expectedFile = {
        "12 -4 1 1 1",
        "33 2 5",
        "789 0 6",
        "22 4"
    };

    testingProgram(input, expectedOutput, expectedFile);
}

TEST(firstLabTests, withLetterTest) {
    std::vector<std::string> input = {
        "32 -4 1 1 1",
        "33 a 5",
        "789 3 6",
        "22 4"
    };

    std::vector<std::string> expectedOutput = {
        "-8"
    };
    std::vector<std::string> expectedFile = {
        "32 -4 1 1 1",
        "33 a 5",
        "789 3 6",
        "22 4"
    };

    testingProgram(input, expectedOutput, expectedFile);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}