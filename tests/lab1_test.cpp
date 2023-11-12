#include <gtest/gtest.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;

extern "C" {
    #include <parent.h>
}

void testingProgram(const std::vector<std::string>& input, const std::vector<int>& expectedOutput) {
    
    // Создаем файл и записываем имя входящего файла
    std::ofstream nameFile("name.txt");
    nameFile << "input.txt";
    nameFile.close();

    const char* fileWithOutput = "output.txt";
    const char * fileWithInput = "input.txt";

    // Записываем входные данные в файл
    std::ofstream inFile(fileWithInput);
 
    for (const std::string& line : input) {
        inFile << line << std::endl;
    }
    inFile.close();

    // Открываем файл как stdin
    freopen("name.txt", "r", stdin);
    
    ParentRoutine(getenv("PATH_TO_CHILD"));
    
    // Проверяем что файл с выводом был создан
    ASSERT_TRUE(fs::exists(fileWithOutput));
    std::ifstream outFile(fileWithOutput);

    // Проверяем содержимое файла вывода
    int result;
    for (int i : expectedOutput) {
        if (!(outFile >> result)) {
            FAIL() << "Ошибка чтения из файла вывода";
        }
        EXPECT_EQ(result, i);
    }

    outFile.close();

    
    auto removeIfExists = [](const char* path) {
        if(fs::exists(path)) {
            fs::remove(path);
        }
    };

    removeIfExists(fileWithInput);
    removeIfExists(fileWithOutput);
    
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