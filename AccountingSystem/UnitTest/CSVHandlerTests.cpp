#include <experimental/filesystem>
#include "gtest/gtest.h"
#include "CSVHandler.h"

class CSVHandlerTest : public ::testing::Test {
protected:
    struct Data
    {
        int FirstField;
        std::string SecondField;
        int ThirdField;
        static int GetID(const std::string& line);
        static std::string GetCSVFormat(const Data& data);
        static Data GetData(const std::string& line);
    };

    void SetUp();
    void TearDown();
    std::unique_ptr<IDataHandler<Data>> _handler;
    std::string _pathToFile;
};

int CSVHandlerTest::Data::GetID(const std::string& line)
{
    std::stringstream ss(line);
    std::string id;
    std::getline(ss, id, ',');
    return stoi(id);
}

std::string CSVHandlerTest::Data::GetCSVFormat(const Data& data)
{
    std::stringstream ss;
    const auto delim = ',';
    ss << data.FirstField << delim
        << data.SecondField << delim
        << data.ThirdField;
    return ss.str();
}

CSVHandlerTest::Data CSVHandlerTest::Data::GetData(const std::string& line)
{
    Data data;
    std::stringstream ss(line);
    std::string tmp;
    std::getline(ss, tmp, ',');
    data.FirstField = stoi(tmp);
    std::getline(ss, data.SecondField, ',');
    std::getline(ss, tmp, ',');
    data.ThirdField = stoi(tmp);
    return data;
}

void CSVHandlerTest::SetUp()
{
    std::vector<std::string> vec{ "FirstField","SecondField","ThirdField" };
    _pathToFile = "../Test.txt";
    _handler = CSVHandler<Data>::CreateHandler(_pathToFile, vec, Data::GetCSVFormat, Data::GetData);
}

void CSVHandlerTest::TearDown()
{
    remove(_pathToFile.c_str());
}
TEST_F(CSVHandlerTest, Create)
{
   EXPECT_EQ(std::experimental::filesystem::exists(_pathToFile), true);
}

TEST_F(CSVHandlerTest, Add)
{
    _handler->Add({ 1,"vvv",1 });
    ASSERT_EQ(_handler->TableSize(), 1);
    _handler->Add({ 2,"vvv",2 });
    ASSERT_EQ(_handler->TableSize(), 2);
    _handler->Add({ 3,"vvv",3 });
    ASSERT_EQ(_handler->TableSize(), 3);
}

TEST_F(CSVHandlerTest, Get) 
{
    ASSERT_THROW(_handler->Get(1), std::exception);
    _handler->Add({ 1,"vvv",11 });
    ASSERT_THROW(_handler->Get(2), std::exception);
    auto data = _handler->Get(1);
    EXPECT_EQ(data.FirstField, 1);
    EXPECT_EQ(data.SecondField, "vvv");
    EXPECT_EQ(data.ThirdField, 11);
}

TEST_F(CSVHandlerTest, Delete)
{
    ASSERT_THROW(_handler->Delete(1), std::exception);

    _handler->Add({ 1,"vvv",11 });
    _handler->Add({ 2,"bbb",12 });
    _handler->Add({ 3,"ccc",13 });

    _handler->Delete(4);
    ASSERT_EQ(_handler->TableSize(), 3);

    _handler->Delete(2);
    ASSERT_EQ(_handler->TableSize(), 2);
    ASSERT_THROW(_handler->Get(2), std::exception);

    _handler->Delete(1);
    ASSERT_EQ(_handler->TableSize(), 1);
    ASSERT_THROW(_handler->Get(1), std::exception);

    _handler->Delete(3);
    ASSERT_EQ(_handler->TableSize(), 0);
    ASSERT_THROW(_handler->Get(3), std::exception);
}

TEST_F(CSVHandlerTest, Update)
{
     ASSERT_THROW(_handler->Update(2, { 2,"ccc",13 }), std::exception);

    _handler->Add({ 1,"vvv",11 });
    _handler->Add({ 2,"bbb",12 });
    _handler->Update(3, { 3,"ccc",13 });

    auto data = _handler->Get(1);
    EXPECT_EQ(data.FirstField, 1);
    EXPECT_EQ(data.SecondField, "vvv");
    EXPECT_EQ(data.ThirdField, 11);

    data = _handler->Get(2);
    EXPECT_EQ(data.FirstField, 2);
    EXPECT_EQ(data.SecondField, "bbb");
    EXPECT_EQ(data.ThirdField, 12);

    _handler->Update(2, { 2,"ccc",13 });
    data = _handler->Get(2);
    EXPECT_EQ(data.FirstField, 2);
    EXPECT_EQ(data.SecondField, "ccc");
    EXPECT_EQ(data.ThirdField, 13);
}
