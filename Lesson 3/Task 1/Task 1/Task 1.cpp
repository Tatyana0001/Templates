#include <iostream>
#include <fstream>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class PrintInConsol : public LogCommand {
public:

    void print(const std::string& message) override {
        std::cout << message << "\n";
    }
};

class PrintInFile : public LogCommand {
public:
    explicit PrintInFile(const std::string& filename_) : filename(filename_) {}
    void print(const std::string& message) override {
        std::ofstream fout(filename);
        if (fout.is_open()) {
            fout << message << "\n";
            fout.close();
        }
        else { std::cerr << "File not found " << "\n"; }
    }
private:
    std::string filename;
};

void print(LogCommand& command, const std::string& message) {
    command.print(message);
}

int main()
{
    PrintInConsol cons;
    PrintInFile fil("out.txt");
    print(cons, "Hello, World!");
    print(fil, "Hello, my friend!");
    return 0;
}

