#include <fstream>

class Printable
{
public:
    virtual ~Printable() = default;
    virtual std::string Print() const = 0;
};

class PrintAsHTML : public Printable
{
public:
    PrintAsHTML(std::string data) : data_(std::move(data)) {}
    ~PrintAsHTML() override = default;
    std::string Print() const override {
        return "<html>" + data_+ "<html/>";
    }
private:
    std::string data_;
};

class PrintAsText : public Printable
{
public:
    PrintAsText(std::string data) : data_(std::move(data)) {}
    ~PrintAsText() override = default;
    std::string Print() const override {
        return data_;
    }
private:
    std::string data_;
};

class PrintAsJSON : public Printable
{
public:
    PrintAsJSON(std::string data) : data_(std::move(data)) {}
    ~PrintAsJSON() override = default;
    std::string Print() const override {
        return "{ \"data\": \"" + data_ + "\"}";
    }
private:
    std::string data_;
};

void SaveTo(std::ofstream& file, const Printable& printable) {
    file << printable.Print();
}

int main() {
    PrintAsHTML printHTML("HTML");
    PrintAsText printText("Text");
    PrintAsJSON printJSON("JSON");
    std::ofstream out("out.txt");
    if (out.is_open()) {
        SaveTo(out, printHTML);
        out << "\n";
        SaveTo(out, printText);
        out << "\n";
        SaveTo(out, printJSON);
        out << "\n";
        out.close();
    }
    return 0;
}